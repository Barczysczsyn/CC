#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>

int main(int argc ,char** argv){

    int meu_socket;
    struct addrinfo hints, *res;

    memset(&hints,0,sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;


    getaddrinfo("www.example.com","http",&hints,&res);
    //meu_socket = socket(AF_INET, SOCK_STREAM,0);
    //sock stream = tcp
    meu_socket = socket(res->ai.family,res->ai_socktype,res->ai_protocol);
    //sock dgram = udp


    if(meu_socket == -1){
        printf("\nErro ao criar socket\n");
    }else{

        printf("\nSocket criado com sucesso\n");
    }

    return 0;
}