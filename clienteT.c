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

// funcao pra encapsular recebimento de mensagens
void receber(int socket, char *buffer, int tam)
{
    // printf("tam %lu",strlen(buffer));
    // fflush(stdout);
    int leitor = recv(socket, buffer, tam, 0);
    if (leitor <= 0)
    {
        perror("receber");
        close(socket);
        printf("\nsocket %d fechado à força", socket);
    }
    else
    {
        buffer[leitor] = '\0';
    }
}

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
    receber(sock, buffer, 25);

    printf("\nConectado ao servidor");
    fflush(stdout);

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
        fflush(stdin);
        send(sock, nome, strlen(nome), 0);
        sleep(1);
        send(sock, senha, strlen(senha), 0);
        // BUG ao receber aut, o 'en' fica salvo
        receber(sock, aut, 25);

        //printf("aut %s", aut);
        fflush(stdout);
        if (strcmp(aut, "encontrado") != 0)
        {
            printf("\nNome ou senha incorretos");
            fflush(stdout);
        }
    } while (strcmp(aut, "encontrado") != 0);

    printf("\nTela principal: ");
    fflush(stdout);

    int resposta;

    // quando tudo estiver acabado
    while (resposta != 0)
    {
        printf("\n1 - Adicionar paciente \n2 - Ver fila\n3 - Heartbeat\n0 - Sair\n");
        scanf("%d", &resposta);

        switch (resposta)
        {
            char resp[2];
        case 0:
            strcpy(resp, "0");
            send(sock, resp, 2, 0);
            printf("\nSaindo...");
            sleep(1);
            break;
        case 1:
            // envia pro servidor pra ele preparar
            //ao colocar isso antes dos scanfs o problema parece ter sido resolvido
            strcpy(resp, "1");
            send(sock, resp, 2, 0);

            // FIXME 10?
            char id[10];
            char nome[MAX_NOME];
            printf("\nID:");
            scanf("%s", id);
            printf("\nNome:");
            scanf("%s", nome);

            //sleep(1);
            //BUG ele envia um em cima do outro
            send(sock, nome, MAX_NOME, 0);
            send(sock, id, MAX_NOME, 0);
            break;

        case 67:
            system("xdg-open https://www.youtube.com/watch?v=eKCLxt9PLhk");
            break;
        default:
            printf("\nEntrada desconhecida.");
            break;
        }
    }
    printf("acabou");
    fflush(stdout);

    close(sock);
}