// servidor de processo
// este servidor utiliza a funcao fork para gerenciar multiplos clientes
// Ao se fazer a chamada de sistema fork(), cria-se uma duplicata exata do programa, e um novo processo filho é iniciado para essa cópia

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
// a biblioteca sys/wait e signal sao necessarias para evitar a criação de zombies
// zombies são processos filhos que aparecem quando processos pais deixam de existir sem ser feita a chamada wait ou waitpid dos filhos
#include <sys/wait.h>
#include <signal.h>

#define true 1

// manipulador de sinais. Ele simplesmente faz a chamada waitpid para todo filho que for desconectado
void sigchld_handler(int signo)
{
    // a ideia de se chamar em um laço é que não se tem certeza que há uma corelação 1 para 1
    // entre os filhos desconectados e as chamadas ao manipulador de sinais
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;//ozzydeia
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
    int resultado,leitor,pid,valor;

    meu_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    valor = 1;
    //SO_REUSEADDR significa que as regras utilizadas para a validação de endereço feita pelo bind permite a reutilizacao de endereços locais.
    resultado = setsockopt(meu_socket,SOL_SOCKET,SO_REUSEADDR,&valor,sizeof(valor));
    if(resultado < 0){
        perror("setsockopt");
        return 0;
    }
    //uso do bind para associar a porta com todos os endereços
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(1972);
    //ele colocou INADDRANY
    servidor.sin_addr.s_addr = INADDR_ANY;

    resultado = bind(meu_socket, (struct sockaddr *)&servidor, sizeof(servidor));
    if(resultado < 0){
        perror("bind");
        return 0;
    }

    //colocando o socket para ouvir a chegada de conexões
    resultado = listen(meu_socket,5);
    if(resultado < 0){
        perror("listen");
        return 0;
    }

    //ativando o manipulador de sinais antes de entrar no laço
    signal(SIGCHLD,sigchld_handler);

    while(true){
        //antes da chamada ser aceita ou retornada, chama-se o fork para a criação de novos processos
        novo_socket = accept(meu_socket,NULL,NULL);
        if((pid = fork()) == 0){
            //se retorna 0 e porque estamos no processo inicial
            //caso contrario retorna o PID do novo processo filho
            printf("\nProcesso filho #%i criado.",getpid());
            close(meu_socket);
            //Uma vez com o processo filho, fecha-se o processo listen
            //lembre-se que todos os filhos são copiados do processo pai
            leitor = recv(novo_socket, buffer, 25,0);//0 é só flag
            buffer[leitor] = '\0';
            printf("\n%s",buffer);
            send(novo_socket,buffer,leitor,0);
            close(novo_socket);
            //essa ultima linha só é alcançada no processo pai, uma vez que o processo filho tem uma cópia do socket cliente, o processo pai
            //faz a sua referência e decrementa o contador no kernel
            printf("\nprocesso filho #%i terminado.",getpid());
            exit(0);
        }
        close(novo_socket);
    }

    return 0;
}
