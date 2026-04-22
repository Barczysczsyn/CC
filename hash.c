#include <stdio.h>

struct hash{
    struct hash *prox;
    int valor;
    int estado;
};

typedef struct hash Hash;


int h(int x){
    return x % 23;

}

int main(){


    Hash tabela[50];
    return 0;
}