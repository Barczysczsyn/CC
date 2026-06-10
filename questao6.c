#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

unsigned val =1;

int main(){
    if (fork() > 0){
        val--;
        
    }
    else if(fork() == 0){
        val++;
    }


    printf("pid=%i,val=%u\n",getpid(),val);
    return 0;
}