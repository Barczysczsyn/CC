#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc ,char** argv){

    int meu_socket;

    struct sockaddr_in servidor;
    meu_socket = socket(AF_INET, SOCK_STREAM,0);
    
    if(meu_socket == -1){
        printf("\nErro ao criar socket\n");
    }else{

        printf("\nSocket criado com sucesso\n");
    }

    servidor.sin_addr.s_addr = inet_addr("200.129.208.21");
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(80);


    if(connect (meu_socket, (struct sockaddr *)&servidor,sizeof(servidor)) < 0){
        puts("erro ao conectar");
    }
    puts("conectado");
    return 0;
}