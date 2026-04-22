// servidor usando multiplexacao

#include <stdio.h>
#include <sys/ioctl.h> //funcoes de entrada e saida de alto nivel
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define true 1
int main(int argc, char **argv)
{

    // este programa usa a funcao select()
    // a funcao select() e uma funcao do sistema operacional que permite especificar um conjunto de descritores

    struct sockaddr_in servidor;
    // file descriptor set
    fd_set readset, testeset;
    int listensock;
    int novosocket;
    char buffer[25];
    int resposta, nlidos, x, val;

    // ouvir chamadas de conexao dos clientes
    listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    val = 1;
    resposta = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    // O linux costuma reservar a porta e o endereco durante o seu uso, o SO_REUSEADDR evita isso.
    // Ao recuperar uma opcao do socket ou defini-la, você especifica o nome da opcao, bem como o nivel.
    // Quando o level == SOL_SOCKET, o item será procurado no próprio socket.
    // Por exemplo, suponha que queremos definir a opção de socket para reutilizar o endereço com o valor 1 (on/true),
    // passamos no "nível" SOL_SOCKET e o valor que queremos definir.

    if (resposta < 0)
    {
        perror("servidor_resposta");
        return 0;
    }

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(1972);        // a porta usada no outro programa
    servidor.sin_addr.s_addr = INADDR_ANY; // aceita qualquer maquina
    // se quisesse que so sua maquina pudesse conectar, colocava seu endereço ali

    resposta = bind(listensock, (struct sockaddr *)&servidor, sizeof(servidor));

    if (resposta < 0)
    {

        perror("bind");
        return 0;
    }
    // ouvir conexão
    resposta = listen(listensock, 5);
    if (resposta < 0)
    {

        perror("listen");
        return 0;
    }
    // manipulador de conexoes

    // zero o nlidos
    FD_ZERO(&readset);
    FD_SET(listensock, &readset);
    // São disponiveis 4 macros para ajudar com o conjunto de descritores
    // FD_CLR reseta o conjunto de flags do descritor;
    // FD_ISSET determina quando um descritor está com a flag ativa ou não
    // FD_SET ativa uma flag para observar um descritor
    // FD_ZERO limpa/zera o conjunto de descritores que estão sendo observador

    while (true)
    {
        testeset = readset;
        // resposta = select(FD_SETSIZE,&testeset,NULL,NULL,NULL);
        resposta = select(__FD_SETSIZE, &testeset, NULL, NULL, NULL);
        if (resposta < 1)
        {
            perror("select");
            return 0;
        }

        for (x = 0; x < __FD_SETSIZE; ++x)
        {
            if (FD_ISSET(x, &testeset))
            {

                if (x == listensock)
                {
                    novosocket = accept(listensock, NULL, NULL);

                    FD_SET(novosocket, &readset);
                }
                else
                {
                    nlidos = recv(x, buffer, 25, 0);
                    if (nlidos <= 0)
                    {
                        close(x);
                        FD_CLR(x,&readset);
                        printf("\nCliente do manipulador #%i desconectado",x);
                    }else{
                        buffer[nlidos] = '\0';
                        printf("\n%s",buffer);
                        send(x,buffer,nlidos,0);
                    }
                }
            }
        }
    }
}