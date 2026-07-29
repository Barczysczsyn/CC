#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>

#define MAX_NOME 50
#define MAX_SENHA 20
#define MAX_BUF 25
#define MAX_NUMERO 10
#define MAX_STRING 500
#define true 1

typedef struct
{
    int sock;
    char fila_local[MAX_STRING];
    char response_buffer[MAX_STRING];
    int response_ready;
    int running;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} ClienteState;

ClienteState cliente_state;

void funcao_filhos(int num_filhos);

void *reader_thread(void *arg)
{
    ClienteState *state = (ClienteState *)arg;
    char buffer[MAX_STRING];

    while (state->running)
    {
        int leitor = recv(state->sock, buffer, sizeof(buffer) - 1, 0);
        if (leitor <= 0)
        {
            break;
        }

        buffer[leitor] = '\0';
        if (strncmp(buffer, "BROADCAST", 9) == 0)
        {
            char *pos1 = strchr(buffer, '|');
            char *pos2 = NULL;
            if (pos1 != NULL)
            {
                pos2 = strchr(pos1 + 1, '|');
            }
            if (pos2 != NULL)
            {
                pthread_mutex_lock(&state->mutex);
                strcpy(state->fila_local, pos2 + 1);
                pthread_mutex_unlock(&state->mutex);
                printf("\n[ATUALIZACAO] Fila atualizada:\n%s", state->fila_local);
                fflush(stdout);
                continue;
            }
        }

        pthread_mutex_lock(&state->mutex);
        strcpy(state->response_buffer, buffer);
        state->response_ready = 1;
        pthread_cond_signal(&state->cond);
        pthread_mutex_unlock(&state->mutex);
    }

    pthread_mutex_lock(&state->mutex);
    state->response_ready = 1;
    pthread_cond_broadcast(&state->cond);
    pthread_mutex_unlock(&state->mutex);
    return NULL;
}

void receber(int socket, char *buffer, int tam)
{
    (void)socket;
    pthread_mutex_lock(&cliente_state.mutex);

    while (!cliente_state.response_ready && cliente_state.running)
    {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;

        int rc = pthread_cond_timedwait(&cliente_state.cond, &cliente_state.mutex, &ts);
        if (rc == ETIMEDOUT)
        {
            break;
        }
    }

    if (cliente_state.response_ready)
    {
        strncpy(buffer, cliente_state.response_buffer, tam - 1);
        buffer[tam - 1] = '\0';
        cliente_state.response_ready = 0;
    }
    else
    {
        buffer[0] = '\0';
    }

    pthread_mutex_unlock(&cliente_state.mutex);
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    funcao_filhos(0);
    return 0;
}

//[ ] devia existir?
void funcao_filhos(int num_filhos)
{
    int sock;
    struct sockaddr_in sAddr;
    char buffer[25] = "opa";
    // socket cliente e binda da porta local

    // zera o ngc
    memset((void *)&sAddr, 0, sizeof(struct sockaddr_in));
    // familia de enderecos
    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = INADDR_ANY;
    // no futuro pode ser outra porta
    sAddr.sin_port = 0;
    // sock stream e tcp
    // sock dgram e udp
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // bind(sock, (const struct sockaddr *)&sAddr, sizeof(sAddr));

    // conectar ao servidor que estiver rodando localmente
    sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // chutou qualquer valor pra htons, qualquer porta acima de 1000 vai dar
    sAddr.sin_port = htons(1972);

    if (connect(sock, (const struct sockaddr *)&sAddr, sizeof(sAddr)) != 0)
    {

        perror("\nCliente");
        return;
    }

    memset(&cliente_state, 0, sizeof(cliente_state));
    cliente_state.sock = sock;
    cliente_state.running = 1;
    strcpy(cliente_state.fila_local, "fila vazia");
    pthread_mutex_init(&cliente_state.mutex, NULL);
    pthread_cond_init(&cliente_state.cond, NULL);

    pthread_t reader;
    pthread_create(&reader, NULL, reader_thread, &cliente_state);

    // deu tudo certo
    send(sock, buffer, MAX_BUF, 0);

    // recebe o q ele mandou
    receber(sock, buffer, MAX_BUF);

    printf("\nConectado ao servidor");
    fflush(stdout);

    char nome[MAX_NOME], senha[MAX_NOME];

    // autenticacao
    //[x] o cliente precisa de autenticacao, ou é só o servidor?
    char aut[MAX_STRING];
    // do
    // {
    // printf("\nInsira seu Nome: ");
    // scanf("%s", nome);
    strcpy(nome, "joao");
    send(sock, nome, MAX_NOME, 0);
    // printf("\nInsira sua senha: ");
    // scanf("%s", senha);
    // fflush(stdin);
    sleep(1);

    strcpy(senha, "123");
    send(sock, senha, MAX_NOME, 0);
    receber(sock, aut, MAX_STRING);

    // printf("aut %s", aut);
    fflush(stdout);
    if (strcmp(aut, "encontrado") != 0)
    {
        printf("\nNome ou senha incorretos");
        fflush(stdout);
    }
    // } while (strcmp(aut, "encontrado") != 0);

    printf("\nTela principal: ");
    fflush(stdout);

    //so pra nao ser 0 
    int resposta = 5;

    // quando tudo estiver pronto
    while (resposta != 0)
    {
        printf("\n1 - Adicionar paciente \n2 - Ver fila\n3 - Heartbeat\n0 - Sair\n");
        fflush(stdout);
        scanf("%d", &resposta);

        switch (resposta)
        {
            char resp[4];
            //nao pode declarar variaveis locais nos cases
            char string[MAX_STRING];
        case 0:
            strcpy(resp, "0");
            send(sock, resp, 4, 0);
            printf("\nSaindo...");
            sleep(1);
            break;
        case 1:
            // envia pro servidor pra ele preparar
            // ao colocar isso antes dos scanfs o problema parece ter sido resolvido
            strcpy(resp, "1");
            //a resposta ser 4 aumenta a chance de ser enviada corretamente
            send(sock, resp, 4, 0);

            char id[MAX_NUMERO];
            char nome[MAX_NOME];
            printf("\nID:");
            // TODO nao tem verificacao pra ver se o novo id é um numero
            // se nao for ele coloca 0
            scanf("%s", id);
            printf("\nNome:");
            // scanf("%s", nome);

            // le o \n para que o fgets nao seja pulado,
            // TODO má pratica
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            fgets(nome, MAX_NOME, stdin);
            // tira o \n do final
            nome[strlen(nome) - 1] = '\0';

            // sleep(1);
            send(sock, nome, MAX_NOME, 0);
            send(sock, id, MAX_NUMERO, 0);

            char status[MAX_STRING];
            receber(sock, status, MAX_STRING);
            if (strcmp(status, "sucesso") == 0)
            {
                printf("\npaciente %s inserido com sucesso", nome);
            }
            else
            {
                printf("\npaciente %s não pôde ser inserido:%s", nome, status);
            }

            break;

        case 2:

            strcpy(resp, "2");
            send(sock, resp, 4, 0);
            receber(sock, string, MAX_STRING);
            if (strcmp(string, "fila vazia") != 0 && string[0] != '\0')
            {
                strcpy(cliente_state.fila_local, string);
            }
            printf("\n%s", cliente_state.fila_local);
            break;
        case 3:

            strcpy(resp, "3");
            send(sock, resp, 4, 0);

            // esperar
            char alive[MAX_STRING];
            sleep(1);
            receber(sock, alive, MAX_STRING);
            // printf("\n%s", alive);
            sleep(1);

            if (strcmp(alive, "ALIVE") == 0)
            {
                printf("\nALIVE");
            }
            else
            {
                //precisa enviar alguma coisa
                strcpy(alive,"DEAD");
                send(sock,alive,10,0);
                //char string[MAX_STRING];
                receber(sock, string, MAX_STRING);
                printf("\n%s", string);
            }
            break;
        default:
            printf("\nEntrada desconhecida.");
            break;
        }
    }
    // printf("acabou");
    // fflush(stdout);

    cliente_state.running = 0;
    shutdown(sock, SHUT_RDWR);
    close(sock);
    pthread_cond_broadcast(&cliente_state.cond);
    pthread_join(reader, NULL);
    pthread_mutex_destroy(&cliente_state.mutex);
    pthread_cond_destroy(&cliente_state.cond);
}

//HACK o heartbeat envia o codigo 3 pro servidor, que manda quantos pacientes há em sua fila
//se o numero for igual, ele assume que a fila esta atualizada, senão, ele
//faz o broadcast com a fila inteira de pacientes
//os clientes estão todos sempre ouvindo o broadcast pra isso funcionar
//so que para isso funcionar, a fila tem que estar salva no cliente
//pois no meu codigo atual, ela so esta salva no socket do servidor de tal cliente