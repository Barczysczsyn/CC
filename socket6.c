#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{

    int meu_socket, novo_socket, c;

    struct sockaddr_in servidor, cliente;
    meu_socket = socket(AF_INET, SOCK_STREAM, 0);
    char *mensagem;

    if (meu_socket == -1)
    {
        printf("\nErro ao criar socket\n");
    }
    else
    {

        printf("\nSocket criado com sucesso\n");
    }

    servidor.sin_addr.s_addr = INADDR_ANY;
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(8888);

    // liga uma comnexao socket a uma porta
    if (bind(meu_socket, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
        if (connect(meu_socket, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
        {
            puts("\nerro no bind");
        }

    puts("\nbind executado corretamente");

    // 3 é o limite
    listen(meu_socket, 3);

    puts("\naguardando chegada de conexoes");
    c = sizeof(struct sockaddr_in);
    while (novo_socket = accept(meu_socket, (struct sockaddr *)&cliente, (socklen_t *)&c))
    {
        if (novo_socket < 0)
        {
            perror("\nerro ao aceitar conexao");
        }

        puts("\nconexao aceita");

        mensagem = "E aí meu  chapa, acabei de receber a sua conexao, mas ja vou indo, fallows";
        write(novo_socket, mensagem, strlen(mensagem));
    }
    return 0;
}
// telnet localhost 8888