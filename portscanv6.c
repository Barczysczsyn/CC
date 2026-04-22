//NAO SEI SE FUNCIONA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>
#include <ctype.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{

    struct hostent *host;
    int busca, i, sock, inicio, fim;
    char nome_maquina[100];
    struct sockaddr_in servidor;

    // nome da maquina pra escanar
    printf("entre com o nome da máquina ou seu IP: ");
    fgets(nome_maquina, 100, stdin);

    // porta de inicio de escanemaneto
    printf("\nEscreva o número inicial da porta: ");
    scanf("%d", &inicio);

    // porta final de escaneamento
    printf("\nescreva o numero final da porta: ");
    scanf("%d", &fim);

    // inicializando a estrutura sockaddr_in
    strncpy((char *)&servidor, "", sizeof(servidor));

    //AF_INET é pra ipv4, da pra mudar pra ipv6
    servidor.sin_family = AF_INET6;

    // uso do endereco ip direto
    if (isdigit(nome_maquina[0]))
    {
        printf("\nexecutando o inet_addr...");
        servidor.sin_addr.s_addr = inet_addr(nome_maquina);
        printf("\nINET feito!\n");
    }

    // resolvendo o nome da maquina (hostname) para endereco IP
    //aparentemente tem um erro aqui
    else if ((host = gethostbyname(nome_maquina)) != 0)
    {
        printf("\nExecutando o gethostbyname");
        strncpy((char *)&servidor.sin_addr, (char *)host->h_addr, sizeof(servidor.sin_addr));
        printf("\nhostbyname feito!");
    }
    else
    {
        herror(nome_maquina);
        exit(2);
    }
    // começando a escanear as portas. fazer um loop
    printf("\n-------- Inicio do escaneamento de portas --------\n");

    for (i = inicio; i <= fim; i++)
    {
        // preencher o numero da porta
        servidor.sin_port = htons(i);

        // criar uma conexao socket do tipo ip
        sock = socket(AF_INET6, SOCK_STREAM, 0);

        // testar se a conexao funcionou
        if (sock < 0)
        {
            perror("\nfalha na conexao socket");
            exit(1);
        }
        // conectar usando o socket e a estrutura sockaddr_in

        busca = connect(sock, (struct sockaddr *)&servidor, sizeof(servidor));

        if (busca < 0)
        {
            //printf("\nfalha na funcao connect! - Porta %d fechada", i);
            printf("\x1b[31m\nfalha na funcao connect! - Porta %d fechada\x1b[0m\n",i);
            fflush(stdout);
        }
        else
        {
            //printf("\n Porta %d aberta!", i);
            printf("\x1b[32m\n Porta %d aberta!\x1b[0m\n",i);
        }

        close(sock);
    }
    printf("\r");
    fflush(stdout);
    return 0;
}

//170.254.139.144
