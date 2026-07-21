// servidor usando multiplexacao

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h> //funcoes de entrada e saida de alto nivel
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define true 1
#define MAX_NOME 25
int main(int argc, char **argv)
{

    // este programa usa a funcao select()
    // a funcao select() e uma funcao do sistema operacional que permite especificar um conjunto de descritores

    struct sockaddr_in servidor;
    // file descriptor set
    // BUG o vscode dá erro, mas o codigo funciona
    fd_set readset, testeset;
    int listensock;
    int novosocket;
    char buffer[25];
    int resposta, nlidos, x, val;

    FILE *arqv;
    arqv = fopen("banco_de_dados", "rw");

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
    servidor.sin_port = htons(1972);       // a porta usada no outro programa
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

        char nome[25] = "joao";
        char senha[25] = "123";
        // x é o manipulador
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
                    // TODO ele recebe 1 ngc antes de tudo
                    nlidos = recv(x, buffer, 25, 0);
                    if (nlidos <= 0)
                    {
                        close(x);
                        FD_CLR(x, &readset);
                        printf("\nCliente do manipulador #%i desconectado", x);
                    }
                    else
                    {

                        // para cada cliente conectado
                        printf("\nNovo cliente conectado %i",x);
                        buffer[nlidos] = '\0';
                        printf("\n%s", buffer);
                        // fflush(stdout);
                        send(x, buffer, nlidos, 0);
                        // printf(" x = %d", x);

                        char sen[MAX_NOME], nom[MAX_NOME];
                        // autenticacao
                        // nome
                        do
                        {
                            nlidos = recv(x, nom, 25, 0);
                            nom[nlidos] = '\0';

                            nlidos = recv(x, sen, 25, 0);
                            sen[nlidos] = '\0';
                            printf("nome: %s senha: %s",nom,sen);
                            if ((strcmp(nom, nome) != 0) || (strcmp(sen, senha) != 0))
                            {
                                // eu pensei em fazer ele só enviar flags de um numero, mas nao sei se o send funciona assim

                                // nao encontrado
                                char msg[25] = "nada";
                                printf("\nnada");
                                send(x, msg, 5, 0);
                            }
                            else
                            {
                                // encontrado
                                char msg[25] = "encontrado";
                                printf("\nencontrado");
                                send(x, msg, 11, 0);
                            }
                            fflush(stdout);

                        } while ((strcmp(nom, nome) != 0) || (strcmp(sen, senha) != 0));
                        // autenticacao
                        // senha

                                printf("\ntela principal");
                        // tela principal
                        while (true)
                        {
                            nlidos = recv(x, buffer, 1, 0);
                            switch (atoi(buffer))
                            {
                            case 1:
                                /* code */
                                break;

                            default:
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    fclose(arqv);
}