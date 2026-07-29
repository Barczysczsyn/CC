// servidor de processo
// este servidor utiliza a funcao fork para gerenciar multiplos clientes
// Ao se fazer a chamada de sistema fork(), cria-se uma duplicata exata do programa, e um novo processo filho é iniciado para essa cópia

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
// a biblioteca sys/wait e signal sao necessarias para evitar a criação de zombies
// zombies são processos filhos que aparecem quando processos pais deixam de existir sem ser feita a chamada wait ou waitpid dos filhos
#include <sys/wait.h>
#include <signal.h>
#include <sys/select.h>

#define true 1

#define MAX_NOME 50
#define MAX_STRING 500
#define MAX_NUMERO 10

#define ARQV_PACIENTE "bd_pacientes"
// #define ARQV_USUARIOS "bd_usuario"

#define ARQV_HISTORICO "bd_historico"
#define ARQV_SESSAO "bd_sessao"
#define ARQV_LOGS "bd_logs"
#define MAX_CLIENTES 100

//[x] como diabos se cadastra usuario?
//[x] como consulta o historico, logs e sessoes? isso é feito pelo servidor ou cliente?
//[x] como funciona o heartbeat?
//[ ] o ver fila faz broadcast? isso é a retransmissão de mensagens?
// TODO ele consegue receber 10000 clientes, so nao sei se é ao mesmo tempo
// lista encadeada de pacientes

int pipefd[2];
int clientes[MAX_CLIENTES];
int qtd_clientes = 0;

/*
//BUG se 2 clientes inserirem num arquivo ao mesmo tempo, ele sera corrompido
solucao flock()
*/
struct Paciente
{
    char nome[MAX_NOME];
    int id;
    struct Paciente *prox;
};
// struct Usuario
// {
//     char nome[MAX_NOME];
//     char senha[MAX_NOME];
// };

// armazena a data e hora atual numa string formatada
void getTempo(char *string)
{

    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    sprintf(string, "%s", asctime(timeinfo));
    // tira o \n do final
    string[strlen(string) - 1] = '\0';
}

// guarda o historico no arquivo
void historico(int comando, char string1[MAX_NOME], char string2[MAX_NOME])
{
    // a é modo append
    FILE *arqv = fopen(ARQV_HISTORICO, "a");
    char tempo[25];
    getTempo(tempo);
    switch (comando)
    {
    case 0:

        fprintf(arqv, "\n%s:usuario %s saiu do sistema", tempo, string1);
        break;
    case 1:
        fprintf(arqv, "\n%s:usuario %s inseriu o paciente %s na fila", tempo, string1, string2);
        break;
    case 2:
        fprintf(arqv, "\n%s:usuario %s abriu a fila", tempo, string1);

        break;
    case 3:
        fprintf(arqv, "\n%s:usuario %s executou heartbeat", tempo, string1);
        break;
    // usuario entrou
    case 4:
        fprintf(arqv, "\n%s:usuario %s entrou no sistema", tempo, string1);

        break;

    default:
        break;
    }
    fclose(arqv);
}

// guarda a sessao no arquivo
void sessao(char nome[MAX_NOME], time_t duracao)
{
    // printf("duracao %lf",((double)duracao / CLOCKS_PER_SEC));
    char tempo[25];
    getTempo(tempo);

    // por enquanto so salva o tempo de uso
    FILE *arqv = fopen(ARQV_SESSAO, "a");
    fprintf(arqv, "\n%s:usuario %s usou o sistema por %ld segundos", tempo, nome, duracao);
    fclose(arqv);
}

// guarda os logs no arquivo
void logs(int comando, char string1[MAX_NOME], char string2[MAX_NOME])
{
    // a é modo append
    FILE *arqv = fopen(ARQV_LOGS, "a");
    char tempo[25];
    getTempo(tempo);
    switch (comando)
    {
    case 0:

        fprintf(arqv, "\n%s:usuario %s nao pôde enviar a entrada e foi desconectado à força", tempo, string1);

        break;
    case 1:

        fprintf(arqv, "\n%s:usuario %s nao conseguiu inserir o paciente %s", tempo, string1, string2);

        break;
    case 52:

        fprintf(arqv, "\n%s:usuario %s causou o erro desconhecido", tempo, string1);

        break;

    default:
        break;
    }
    fclose(arqv);
}

// funcao pra encapsular recebimento de mensagens
// FIXME possivel overflow
int receber(int socket, char *buffer, int tam)
{
    // printf("tam %lu",strlen(buffer));
    int leitor = recv(socket, buffer, tam, 0);
    if (leitor <= 0)
    {
        perror("receber");
        //  ele ficava recebendo eternamente de qualquer jeito
        // precisava dar um jeito de setar o resp para 0 daqui
        //[ ] porque eu comentei?
        close(socket);
        printf("\nsocket %d será fechado à força", socket);
        fflush(stdout);
        return 1;
    }
    else
    {
        buffer[leitor] = '\0';
        return 0;
    }
}

// grava todos os pacientes em uma fila vazia
void gravarPacientes(struct Paciente **inicio)
{
    if (*inicio == NULL)
    {
        return;
    }
    FILE *arqv = fopen(ARQV_PACIENTE, "w");
    struct Paciente *p1 = *inicio;
    // vai até o final da fila
    while (p1 != NULL)
    {
        fwrite(p1->nome, sizeof(p1->nome), 1, arqv);
        fwrite(&(p1->id), sizeof(p1->id), 1, arqv);
        // nao vou fazer assim porque ele escreveria o ponteiro tambem, que é inutil nessa ocasiao
        // fwrite(p1,sizeof(struct Paciente),1,arqv);
        p1 = p1->prox;
    }

    fclose(arqv);
}

// grava um paciente no final da fila
void gravarPaciente(struct Paciente **inicio)
{
    if (*inicio == NULL)
    {
        return;
    }
    FILE *arqv = fopen(ARQV_PACIENTE, "a");
    struct Paciente *p1 = *inicio;
    // vai até o final da fila
    while (p1->prox != NULL)
    {
        p1 = p1->prox;
    }
    // é o ultimo da fila
    fwrite(p1->nome, MAX_NOME, 1, arqv);
    fwrite(&(p1->id), sizeof(p1->id), 1, arqv);

    fclose(arqv);
}
// limpa a fila e libera a memória
void destroiPaciente(struct Paciente **inicio)
{
    if (inicio == NULL || *inicio == NULL)
    {
        return;
    }

    struct Paciente *atual = *inicio;
    struct Paciente *proximo;

    while (atual != NULL)
    {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }

    *inicio = NULL;
}

// le o arquivo e coloca os pacientes na lista encadeada
void lerPaciente(struct Paciente **inicio)
{
    FILE *arqv = fopen(ARQV_PACIENTE, "r");
    if (arqv == NULL)
        return;
    // isso conta que o inicio esta vazio
    // faz o primeiro ponteiro
    *inicio = malloc(sizeof(struct Paciente));
    fread((*inicio)->nome, MAX_NOME, 1, arqv);
    fread(&((*inicio)->id), sizeof(int), 1, arqv);

    printf("\nlido nome: %s id %d", (*inicio)->nome, (*inicio)->id);

    struct Paciente *p1 = *inicio;
    char nome[MAX_NOME];
    int id;
    while (fread(nome, MAX_NOME, 1, arqv) == 1)
    {
        fread(&id, sizeof(int), 1, arqv);

        struct Paciente *novo = malloc(sizeof(struct Paciente));
        strcpy(novo->nome, nome);
        novo->id = id;
        printf("\nlido nome: %s id %d", novo->nome, novo->id);
        fflush(stdout);
        // sleep(1);

        p1->prox = novo;
        p1 = p1->prox;
    }
    fclose(arqv);
}
// insere um paciente no final da fila
int inserirPaciente(int id, char nome[MAX_NOME], struct Paciente **inicio)
{
    // retorna 1 se funcionou
    struct Paciente *novo = malloc(sizeof(struct Paciente));
    // erro no malloc
    if (novo == NULL)
        return 0;

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
    return 1;
}

// coloca a fila toda numa string
int verFila(struct Paciente *inicio, char string[MAX_STRING])
{
    if (inicio == NULL)
    {
        strcpy(string, "fila vazia");
        return 0;
    }
    else
    {
        int cont = 0;
        strcpy(string, "\n===== FILA =====");

        struct Paciente *p1 = inicio;
        while (p1 != NULL)
        {
            // strcat(string, "\n%d - %s", p1->id, p1->nome);
            char id[MAX_NUMERO];
            snprintf(id, MAX_NUMERO, "%d", p1->id);
            strcat(string, "\n");
            strcat(string, id);
            strcat(string, " - ");
            strcat(string, p1->nome);
            p1 = p1->prox;
            ++cont;
        }
        strcat(string, "\n================");
        return cont;
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

void adicionarCliente(int socket)
{
    if (qtd_clientes < MAX_CLIENTES)
    {
        clientes[qtd_clientes++] = socket;
    }
}

void removerCliente(int socket)
{
    for (int i = 0; i < qtd_clientes; ++i)
    {
        if (clientes[i] == socket)
        {
            for (int j = i; j < qtd_clientes - 1; ++j)
            {
                clientes[j] = clientes[j + 1];
            }
            --qtd_clientes;
            break;
        }
    }
}

void enviarBroadcast(int origem, const char *mensagem)
{
    for (int i = 0; i < qtd_clientes; ++i)
    {
        if (clientes[i] != origem && clientes[i] >= 0)
        {
            if (send(clientes[i], mensagem, strlen(mensagem) + 1, 0) < 0)
            {
                perror("send broadcast");
                removerCliente(clientes[i]);
                close(clientes[i]);
            }
        }
    }
}

void processarPipe(int fd)
{
    char msg[512];
    while (1)
    {
        ssize_t n = read(fd, msg, sizeof(msg) - 1);
        if (n <= 0)
        {
            break;
        }

        msg[n] = '\0';
        char *acao = strtok(msg, "|");
        if (acao == NULL)
        {
            continue;
        }

        if (strcmp(acao, "BROADCAST") == 0)
        {
            char *origem_s = strtok(NULL, "|");
            char *corpo = strtok(NULL, "");
            if (origem_s != NULL && corpo != NULL)
            {
                enviarBroadcast(atoi(origem_s), corpo);
            }
        }
        else if (strcmp(acao, "LEAVE") == 0)
        {
            char *socket_s = strtok(NULL, "|");
            if (socket_s != NULL)
            {
                int socket = atoi(socket_s);
                removerCliente(socket);
                close(socket);
            }
        }
    }
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
    if (meu_socket < 0)
    {
        perror("socket");
        return 0;
    }

    valor = 1;
    // SO_REUSEADDR significa que as regras utilizadas para a validação de endereço feita pelo bind permite a reutilizacao de endereços locais.
    resultado = setsockopt(meu_socket, SOL_SOCKET, SO_REUSEADDR, &valor, sizeof(valor));
    if (resultado < 0)
    {
        perror("setsockopt");
        close(meu_socket);
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
        close(meu_socket);
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

    if (pipe(pipefd) < 0)
    {
        perror("pipe");
        return 0;
    }

    struct Paciente *pacientes;

    // struct Usuario usuarios[10000];

    // int quantUsuarios = lerUsuario(usuarios);

    // //insercao é feita assim por enquanto
    // strcpy(usuarios[1].nome, "artur");
    // strcpy(usuarios[1].senha, "123456");
    // gravarUsuario(usuarios, 2);

    // aparentemente aqui já está pronto o servidor
    // printf("\nServidor aberto na porta %i as %li horas", servidor.sin_port,(time(NULL))/3600);
    printf("\nServidor aberto na porta %i", servidor.sin_port);
    // esse fflush é essencial, se nao tiver ele fica mandando essa mensagem pra cada cliente que entra
    fflush(stdout);
    while (true)
    {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(meu_socket, &readfds);
        FD_SET(pipefd[0], &readfds);

        int maxfd = (meu_socket > pipefd[0]) ? meu_socket : pipefd[0];
        select(maxfd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(pipefd[0], &readfds))
        {
            processarPipe(pipefd[0]);
        }

        if (!FD_ISSET(meu_socket, &readfds))
        {
            continue;
        }

        // antes da chamada ser aceita ou retornada, chama-se o fork para a criação de novos processos
        novo_socket = accept(meu_socket, NULL, NULL);
        adicionarCliente(novo_socket);
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

            time_t inicio = time(NULL);

            // recebe e envia a confirmacao de conexao
            receber(novo_socket, buffer, 25);
            send(novo_socket, buffer, 25, 0);

            // autenticacao
            char nom[MAX_NOME], sen[MAX_NOME];
            int encontrado = 0;
            // do
            // {
            receber(novo_socket, nom, MAX_NOME);
            receber(novo_socket, sen, MAX_NOME);

            // checa
            // essa versao tem um nome e senha especifico
            if ((strcmp(nom, "joao") != 0) || (strcmp(sen, "123") != 0))
            {
                strcpy(buffer, "nao encontrado");
                send(novo_socket, buffer, 25, 0);
                // finalizar a sessao

                char msg_saida[32];
                snprintf(msg_saida, sizeof(msg_saida), "LEAVE|%d", novo_socket);
                write(pipefd[1], msg_saida, strlen(msg_saida) + 1);
                close(novo_socket);
                exit(0);
            }
            // } while (encontrado == 0);

            strcpy(buffer, "encontrado");
            send(novo_socket, buffer, 25, 0);

            printf("\nusuario %s entrou no sistema", nom);

            // atualiza a fila de pacientes antes de tudo
            lerPaciente(&pacientes);

            // sessao
            // while principal
            do
            {
                // recebe uma string q é só um numero
                if (receber(novo_socket, buffer, 4))
                {
                    // se esse receber der erro ele sai do laço
                    // TODO essa verificação é boa, mas talvez deveria colocar em mais lugares
                    logs(0, nom, NULL);
                    break;
                }
                // printf("\nentrada %s", buffer);
                //  converte essa string pra um numero int
                int resp = buffer[0] - '0';

                switch (resp)
                {
                case 0:
                    // sem o case 0 ele buga forte
                    printf("\n filho #%i desconectou.", getpid());
                    historico(0, nom, NULL);
                    sleep(1);
                    break;

                case 1:
                    char novoNome[MAX_NOME], novoID[MAX_NUMERO];
                    receber(novo_socket, novoNome, MAX_NOME);
                    receber(novo_socket, novoID, MAX_NUMERO);
                    char status[10];
                    if (inserirPaciente(atoi(novoID), novoNome, &pacientes))
                    {
                        printf("\npaciente %s inserido com sucesso", novoNome);
                        strcpy(status, "sucesso");
                        send(novo_socket, status, 25, 0);

                        char mensagem[MAX_STRING];
                        snprintf(mensagem, sizeof(mensagem), "BROADCAST|%d|NOVO_PACIENTE:%s:%d", novo_socket, novoNome, atoi(novoID));
                        write(pipefd[1], mensagem, strlen(mensagem) + 1);
                    }
                    else
                    {

                        printf("\npaciente %s não pôde ser inserido", novoNome);
                        strcpy(status, "fracasso");
                        send(novo_socket, status, 25, 0);

                        logs(1, nom, novoNome);
                    }
                    // nom é o nome do usuario atual
                    historico(1, nom, novoNome);
                    gravarPaciente(&pacientes);
                    fflush(stdout);
                    break;
                case 2:
                    char string[MAX_STRING];
                    verFila(pacientes, string);

                    send(novo_socket, string, MAX_STRING, 0);
                    // printf("string: %s", string);
                    historico(2, nom, NULL);
                    break;

                case 3:
                    // pega a fila atual
                    char string1[MAX_STRING];
                    verFila(pacientes, string1);

                    // pega a fila do arquivo, que esta sempre atualizada
                    destroiPaciente(&pacientes);
                    lerPaciente(&pacientes);
                    char string2[MAX_STRING];
                    int tam = verFila(pacientes, string2);

                    // aparentemente ele tem acesso à variavel do case 1
                    // char status[10];
                    if (strcmp(string1, string2) == 0)
                    {
                        strcpy(status, "ALIVE");
                        send(novo_socket, status, 10, 0);
                    }
                    else
                    {
                        // qual o contrario de alive?
                        strcpy(status, "DEAD");
                        send(novo_socket, status, 10, 0);
                        //precisa receber alguma coisa
                        receber(novo_socket,status,10);
                        /*
                        sprintf(status, "%d", tam);
                        send(novo_socket, status, 10, 0);

                        for (int i = 0; i < tam; i++)
                        {
                            send(novo_socket, status, 10, 0);
                        }
                            */
                        // nem precisa disso, o lado do cliente nunca vai ter nada, so precisa atualizar no socket dele

                        send(novo_socket, string2, MAX_STRING, 0);
                    }

                    historico(3, nom, NULL);
                    break;

                default:
                    // isso basicamente não vai acontecer, pq já tem proteção contra isso do lado do cliente
                    printf("erro desconhecido, entrada %d.", resp);
                    perror("erro desconhecido");
                    logs(52, nom, NULL);
                    break;
                }
                sleep(1);
            } while (buffer[0] != '0');
            // if (strcmp(buffer, "exit") == 0)
            time_t fim = time(NULL);

            sessao(nom, (fim - inicio));
            // acaba o filho

            char msg_saida[32];
            snprintf(msg_saida, sizeof(msg_saida), "LEAVE|%d", novo_socket);
            write(pipefd[1], msg_saida, strlen(msg_saida) + 1);
            close(novo_socket);
            // essa ultima linha só é alcançada no processo pai, uma vez que o processo filho tem uma cópia do socket cliente, o processo pai
            // faz a sua referência e decrementa o contador no kernel
            // printf("\nprocesso filho #%i terminado.", getpid());

            exit(0);
            // fclose(sess);
        }
    }

    return 0;
}