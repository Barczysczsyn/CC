#include <stdio.h>
#include <sys/socket.h>

int main(int argc ,char** argv){

    int meu_socket;
    //meu_socket = socket(AF_INET, SOCK_STREAM,0);
    //sock stream = tcp
    meu_socket = socket(AF_INET, SOCK_DGRAM,0);
    //sock dgram = udp


    if(meu_socket == -1){
        printf("\nErro ao criar socket\n");
    }else{

        printf("\nSocket criado com sucesso\n");
    }

    return 0;
}