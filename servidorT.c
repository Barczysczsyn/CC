// servidor de processo
// este servidor utiliza a funcao fork para gerenciar multiplos clientes
// Ao se fazer a chamada de sistema fork(), cria-se uma duplicata exata do programa, e um novo processo filho é iniciado para essa cópia

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
// a biblioteca sys/wait e signal sao necessarias para evitar a criação de zombies
// zombies são processos filhos que aparecem quando processos pais deixam de existir sem ser feita a chamada wait ou waitpid dos filhos
#include <sys/wait.h>
#include <signal.h>

#define true 1

#define MAX_NOME 50
#define MAX_STRING 100

#define ARQV_PACIENTE "bd_pacientes"
#define ARQV_USUARIOS "bd_usuario"

//[ ] como diabos se cadastra usuario?
//[ ] como consulta o historico, logs e sessoes? isso é feito pelo servidor ou cliente?
// lista encadeada de pacientes
struct Paciente
{
    char nome[MAX_NOME];
    int id;
    struct Paciente *prox;
};
struct Usuario
{
    char nome[MAX_NOME];
    char senha[MAX_NOME];
};

// funcao pra encapsular recebimento de mensagens
void receber(int socket, char *buffer, int tam)
{
    // printf("tam %lu",strlen(buffer));
    // fflush(stdout);
    int leitor = recv(socket, buffer, tam, 0);
    if (leitor <= 0)
    {
        perror("receber");
    }
    else
    {
        buffer[leitor] = '\0';
    }
}

void gravarPaciente(struct Paciente **inicio)
{
    FILE *arqv = fopen(ARQV_PACIENTE, "w");
    struct Paciente *p1 = *inicio;
    // vai até o final da fila
    while (p1->prox != NULL)
    {
        fwrite(p1->nome, sizeof(p1->nome), 1, arqv);
        fwrite(&(p1->id), sizeof(p1->id), 1, arqv);
        p1 = p1->prox;
    }

    fclose(arqv);
}

// TODO logs, sessoes, historico
void lerPaciente(struct Paciente **inicio)
{
    FILE *arqv = fopen(ARQV_PACIENTE, "r");
    // isso conta que o inicio esta vazio
    // faz o primeiro ponteiro
    *inicio = malloc(sizeof(struct Paciente));
    fread((*inicio)->nome, sizeof((*inicio)->nome), 1, arqv);
    fread(&((*inicio)->id), sizeof(int), 1, arqv);

    struct Paciente *p1 = *inicio;
    while (arqv != NULL)
    {
        struct Paciente *novo = malloc(sizeof(struct Paciente));
        fread(novo->nome, sizeof(novo->nome), 1, arqv);
        fread(&(novo->id), sizeof(int), 1, arqv);

        p1->prox = novo;
        p1 = p1->prox;
    }
    fclose(arqv);
}
void gravarUsuario(struct Usuario *usuarios)
{
    FILE *arqv = fopen(ARQV_USUARIOS, "w");

    // FIXME sizeof nao deve funcionar
    for (int i = 0; i < sizeof(usuarios); i++)
    {
        // burro?
        fwrite(usuarios[i].nome, sizeof(usuarios[i].nome), 1, arqv);
        fwrite(usuarios[i].senha, sizeof(usuarios[i].senha), 1, arqv);
    }

    fclose(arqv);
}

// TODO logs, sessoes, historico
int lerUsuario(struct Usuario *usuarios)
{
    int i = 0;
    FILE *arqv = fopen(ARQV_USUARIOS, "r");
    while (arqv != NULL)
    {
        // burro?
        fread(usuarios[i].nome, sizeof(usuarios[i].nome), 1, arqv);
        fread(usuarios[i].senha, sizeof(usuarios[i].senha), 1, arqv);
        ++i;
    }

    fclose(arqv);
    return i;
}

int inserirPaciente(int id, char nome[MAX_NOME], struct Paciente **inicio)
{
    struct Paciente *novo = malloc(sizeof(struct Paciente));
    // erro no malloc
    if (novo == NULL)
        return 1;

    strcpy(novo->nome, nome);
    novo->id = id;
    novo->prox = NULL;

    // se é a primeira insercao
    if (*inicio == NULL)
    {
        *inicio = novo;
    }
    else
    {
        struct Paciente *p1 = *inicio;
        // vai até o final da fila
        while (p1->prox != NULL)
        {
            p1 = p1->prox;
        }

        p1->prox = novo;
    }
    return 0;
}

void verFila(struct Paciente *inicio, char string[MAX_STRING])
{
    if (inicio == NULL)
    {
        sprintf(string, "fila vazia");
    }
    else
    {
        sprintf(string, "\n===== FILA =====");

        struct Paciente *p1 = inicio;
        while (p1->prox != NULL)
        {
            sprintf(string, "\n%d - %s", p1->id, p1->nome);
            p1 = p1->prox;
        }
        sprintf(string, "\n================");
    }
}
// manipulador de sinais. Ele simplesmente faz a chamada waitpid para todo filho que for desconectado
void sigchld_handler(int signo)
{
    // a ideia de se chamar em um laço é que não se tem certeza que há uma corelação 1 para 1
    // entre os filhos desconectados e as chamadas ao manipulador de sinais
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ; // ozzydeia
}
// vale lembrar que o posix nao permite que a criação de filas nas chamadas
// de sinal. Ou seja, pode acontecer de chamar o manipulador após vários
// já terem sido desconectados
int main(int argc, char **argv)
{
    struct sockaddr_in servidor;
    int meu_socket;
    int novo_socket;
    char buffer[25];
    int resultado, leitor, pid, valor;

    meu_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    valor = 1;
    // SO_REUSEADDR significa que as regras utilizadas para a validação de endereço feita pelo bind permite a reutilizacao de endereços locais.
    resultado = setsockopt(meu_socket, SOL_SOCKET, SO_REUSEADDR, &valor, sizeof(valor));
    if (resultado < 0)
    {
        perror("setsockopt");
        return 0;
    }
    // uso do bind para associar a porta com todos os endereços
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(1972);
    // ele colocou INADDRANY
    servidor.sin_addr.s_addr = INADDR_ANY;

    resultado = bind(meu_socket, (struct sockaddr *)&servidor, sizeof(servidor));
    if (resultado < 0)
    {
        perror("bind");
        return 0;
    }

    // colocando o socket para ouvir a chegada de conexões
    resultado = listen(meu_socket, 5);
    if (resultado < 0)
    {
        perror("listen");
        return 0;
    }

    // ativando o manipulador de sinais antes de entrar no laço
    signal(SIGCHLD, sigchld_handler);

    // XXX temqter
    char nome[MAX_NOME] = "joao";
    char senha[MAX_NOME] = "123";
    struct Paciente *pacientes;
    // TODO aumentar
    struct Usuario usuarios[10000];
    int quantUsuarios = lerUsuario(usuarios);

    // //insercao é feita assim por enquanto
    // strcpy(usuarios[0].nome,"joao");
    // strcpy(usuarios[0].senha,"123");
    // gravarUsuario(usuarios);

    while (true)
    {
        // antes da chamada ser aceita ou retornada, chama-se o fork para a criação de novos processos
        novo_socket = accept(meu_socket, NULL, NULL);
        if ((pid = fork()) == 0)
        {
            // se retorna 0 e porque estamos no processo inicial
            // caso contrario retorna o PID do novo processo filho
            printf("\nProcesso filho #%i criado.", getpid());
            fflush(stdout);
            close(meu_socket);
            // Uma vez com o processo filho, fecha-se o processo listen
            // lembre-se que todos os filhos são copiados do processo pai

            /// XXX comeca o meu codigo

            // recebe e envia a confirmacao de conexao
            receber(novo_socket, buffer, 25);
            send(novo_socket, buffer, 25, 0);

            // autenticacao
            char nom[25], sen[25];
            do
            {
                receber(novo_socket, nom, 25);
                receber(novo_socket, sen, 25);
            } while ((strcmp(nom, nome) != 0) || (strcmp(sen, senha) != 0));

            strcpy(buffer,"encontrado");
            send(novo_socket,buffer,25,0);

            printf("\nusuario %s entrou no sistema",nom);
            //while principal
            do
            {
                //recebe uma string q é só um numero
                receber(novo_socket,buffer,2);
                //converte essa string pra um numero int 
                int resp = buffer[0] - '0';


                switch (resp)
                {
                case 1:
                char novoNome[MAX_NOME],novaSenha[MAX_NOME];
                receber(novo_socket,novoNome,MAX_NOME);
                receber(novo_socket,novaSenha,MAX_NOME);
                //usuarios[quantUsuarios].nome = novoNome;
                //usuarios[quantUsuarios].senha = novaSenha;
                //gravarUsuario(usuarios);
                //TODO continuar
                    break;
                
                default:
                    break;
                }
            } while (strcmp(buffer, "exit") != 0);
            // if (strcmp(buffer, "exit") == 0)
            close(novo_socket);
            // essa ultima linha só é alcançada no processo pai, uma vez que o processo filho tem uma cópia do socket cliente, o processo pai
            // faz a sua referência e decrementa o contador no kernel
            printf("\nprocesso filho #%i terminado.", getpid());
            exit(0);
        }
        close(novo_socket);
    }

    return 0;
}
