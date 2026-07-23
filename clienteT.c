#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_NOME 50
#define MAX_SENHA 20
#define true 1
void funcao_filhos(int num_filhos);

int main(int argc, char **argv)
{
    int nfilhos = 1;
    int pid, x;
    //[ ] quanto disso realmente precisa?
    if (argc > 1)
    {
        nfilhos = atoi(argv[1]);
    }
    for (x = 0; x < nfilhos; x++)
    {
        if ((pid = fork()) == 0)
        {

            funcao_filhos(x + 1);
            exit(0);
        }
    }

    // depois de criar os filhos, o processo pai espera por eles
    wait(NULL);

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
    bind(sock, (const struct sockaddr *)&sAddr, sizeof(sAddr));

    // conectar ao servidor que estiver rodando localmente
    sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // chutou qualquer valor pra htons, qualquer porta acima de 1000 vai dar
    sAddr.sin_port = htons(1972);

    if (connect(sock, (const struct sockaddr *)&sAddr, sizeof(sAddr)) != 0)
    {

        perror("\nCliente");
        return;
    }

    // deu tudo certo
    send(sock, buffer, strlen(buffer), 0);

    // recebe a merda q ele mandou
    recv(sock, buffer, strlen(buffer), 0);

    printf("\nConectado ao servidor");

    // TODO resposta do servidor

    char nome[MAX_NOME], senha[MAX_SENHA];

    // autenticacao
    //[ ] o cliente precisa de autenticacao, ou é só o servidor?
    char aut[25];
    do
    {
        printf("\nInsira seu Nome: ");
        scanf("%s", nome);
        printf("\nInsira sua senha: ");
        scanf("%s", senha);
        send(sock, nome, strlen(nome), 0);
        send(sock, senha, strlen(senha), 0);
        sleep(1);
        // XXX ao receber aut
        recv(sock, aut, 11, 0);

        printf("aut %s", aut);
        fflush(stdout);
        if (strcmp(aut, "encontrado") != 0)
        {
            printf("\nNome ou senha incorretos");
            fflush(stdout);
        }
    } while (strcmp(aut, "encontrado") != 0);

    // retorna 1 se deu certo ou 0 para erro
    printf(" porra");
    fflush(stdout);

    int resposta;

    // quando tudo estiver acabado
    while (resposta != 0)
    {
        printf("\n1 - Adicionar paciente \n2 - Ver fila\n3 - Heartbeat\n0 - Sair\n");
        scanf("%d", &resposta);

        switch (resposta)
        {
        case 0:

            printf("\nSaindo...");
            break;
        case 1:
            // FIXME 10?
            char id[10];
            char nome[MAX_NOME];
            printf("\nID:");
            scanf("%s", id);
            printf("\nNome:");
            scanf("%s", nome);

            // envia pro servidor pra ele preparar
            char resp[9] = "entrada1";
            send(sock, resp, 2, 0);
            send(sock, id, 10, 0);
            send(sock, nome, strlen(nome), 0);
            break;

        case 67:
            system("START pranx.com");
            break;
        default:
            printf("\nEntrada desconhecida.");
            break;
        }
    }

    // enviar alguma coisa ao servidor
    // snprintf(buffer, 128, "dados do cliente #%i.", num_filhos);
    /*
        sleep(1); // 1 segundo
        printf("\nfilho #%i mandou %i caracteres", num_filhos, send(sock, buffer, strlen(buffer), 0));
        sleep(1);
        printf("\nfilho #%i recebeu %i caracteres", funcao_filhos, recv(sock, buffer, 25, 0));
        sleep(1);
        */
    close(sock);
}