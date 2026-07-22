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
void receber(int socket, char *buffer)
{
    int leitor = recv(socket, buffer, strlen(buffer), 0);
    if (leitor <= 0)
    {
        perror("receber");
    }
    else
    {
        buffer[leitor] = '\0';
    }
}

void gravar(FILE *arqv, struct Paciente **inicio, struct Usuario *usuarios)
{
    fprintf(arqv, "PACIENTES");
    struct Paciente *p1 = *inicio;
    // vai até o final da fila
    while (p1->prox != NULL)
    {
        fprintf(arqv, "%s,%d|", p1->nome, p1->id);
        p1 = p1->prox;
    }
    fprintf(arqv, "X");

    fprintf(arqv, "USUARIOS");

    // FIXME sizeof nao deve funcionar
    for (int i = 0; i < sizeof(usuarios); i++)
    {
        fprintf(arqv, "%s,%s|", usuarios[i].nome, usuarios[i].senha);
    }
    fprintf(arqv, "X");

    // TODO logs, sessoes, historico
}

void ler(FILE *arqv, struct Paciente **inicio, struct Usuario *usuarios)
{
    while (arqv != NULL)
    {
        char string[10];
        fscanf(arqv, "%s", string);
        if (strcmp(string, "PACIENTES" == 0))
        {
            char c;
            char nom[MAX_NOME], sen[MAX_NOME];
            fscanf("%c", c);
            while (c != 'X')
            {
                fscanf(arqv, "%s", nom);
                // pega a virgula
                fscanf(arqv, "%c", c);

                fscanf(arqv, "%s", sen);
                // pega a |
                fscanf(arqv, "%c", c);
                
                fscanf("%c", c);
            }
        }
    }
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
    struct Usuario usuarios[500];
    while (true)
    {
        // antes da chamada ser aceita ou retornada, chama-se o fork para a criação de novos processos
        novo_socket = accept(meu_socket, NULL, NULL);
        if ((pid = fork()) == 0)
        {
            // se retorna 0 e porque estamos no processo inicial
            // caso contrario retorna o PID do novo processo filho
            printf("\nProcesso filho #%i criado.", getpid());
            close(meu_socket);
            // Uma vez com o processo filho, fecha-se o processo listen
            // lembre-se que todos os filhos são copiados do processo pai
            leitor = recv(novo_socket, buffer, 25, 0); // 0 é só flag
            buffer[leitor] = '\0';
            printf("\n%s", buffer);
            send(novo_socket, buffer, leitor, 0);

            // XXX comeca o meu codigo
            char sen[MAX_NOME], nom[MAX_NOME];
            // autenticacao
            // nome
            do
            {
                printf("fez");
                fflush(stdout);
                leitor = recv(novo_socket, nom, 25, 0);
                nom[leitor] = '\0';

                leitor = recv(novo_socket, sen, 25, 0);
                sen[leitor] = '\0';
                printf("nome: %s senha: %s", nom, sen);
                if ((strcmp(nom, nome) != 0) || (strcmp(sen, senha) != 0))
                {
                    // HACK eu pensei em fazer ele só enviar flags de um numero, mas nao sei se o send funciona assim

                    // nao encontrado
                    char msg[25] = "nada";
                    printf("\nnada");
                    send(novo_socket, msg, 5, 0);
                }
                else
                {
                    // encontrado
                    char msg[25] = "encontrado";
                    printf("\nencontrado");
                    send(novo_socket, msg, 11, 0);
                }
                fflush(stdout);

            } while ((strcmp(nom, nome) != 0) || (strcmp(sen, senha) != 0));
            // autenticacao
            // senha

            printf("\ntela principal");
            // tela principal
            char entrada[10];
            while (true)
            {
                //[x] porque o servidor faz isso mas o cliente não? porque no codigo do professor ele nao fazia nada com a string
                receber(novo_socket, entrada);
                printf("\nentrada %s", entrada);
                int ent = entrada[7] - '0';
                switch (ent)
                {
                case 0:

                    close(novo_socket);
                    // nao precisa do readset parece
                    printf("\nCliente do manipulador #%i desconectou", novo_socket);
                    // BUG ele fica recebendo 0 e entrando aqui
                    // HACK talvez o 0 nao faz nada, e sera feito com ifs,
                    sleep(1);
                    break;
                case 1:

                    char id[10];
                    // reutilizando o nom
                    receber(novo_socket, nom);
                    receber(novo_socket, id);

                    if (inserirPaciente(atoi(id), nom, &pacientes))
                    {
                        printf("paciente não pôde ser inserido");
                    }
                    else
                    {

                        printf("paciente inserido com sucesso");
                    }
                    // TODO salvar em arquivo binario

                    break;

                case 2:
                    // send
                    char string[MAX_STRING];
                    verFila(pacientes, string);
                    break;
                default:
                    printf("\ninsira um valor válido");
                    break;
                }
            }

            // fecha o socket e termina o programa
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
