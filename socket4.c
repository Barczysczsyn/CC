#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h> //funcao close

int main(int argc, char **argv)
{

    int meu_socket;

    struct sockaddr_in servidor;

    char *mensagem, resposta_servidor[2000];
    meu_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (meu_socket == -1)
    {
        printf("\nErro ao criar socket\n");
    }
    else
    {

        printf("\nSocket criado com sucesso\n");
    }

    servidor.sin_addr.s_addr = inet_addr("172.237.146.8");
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(80);

    if (connect(meu_socket, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    {
        puts("erro ao conectar");
    }
    puts("conectado");

    mensagem = "GET / HTTP/1.1\r\n\r\n";

    if (send(meu_socket, mensagem, strlen(mensagem), 0) < 0)
    {
        puts("falha ao enviar mensagem");
        return 1;
    }
    puts("mensagem enviada com sucesso!");



    if(recv(meu_socket,resposta_servidor,2000,0)<0){
        puts("falha ao receber resposta");
    }

    puts("Resposta recebida");
    puts(resposta_servidor);

    return 0;
}