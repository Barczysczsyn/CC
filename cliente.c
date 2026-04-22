#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

void funcao_filhos(int num_filhos);

int main(int argc, char **argv)
{
    int nfilhos = 1;
    int pid, x;
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

void funcao_filhos(int num_filhos)
{
    int sock;
    struct sockaddr_in sAddr;
    char buffer[25];
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

    // enviar alguma coisa ao servidor
    snprintf(buffer, 128, "dados do cliente #%i.", num_filhos);

    sleep(1); // 1 segundo
    printf("\nfilho #%i mandou %i caracteres", num_filhos, send(sock, buffer, strlen(buffer), 0));
    sleep(1);
    printf("\nfilho #%i recebeu %i caracteres", funcao_filhos, recv(sock, buffer, 25, 0));
    sleep(1);
    close(sock);
}