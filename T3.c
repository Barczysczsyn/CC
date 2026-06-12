#include <stdio.h>
#include <string.h>

#define MAX_PROC 50
#define MAX_ARQV 100
struct Processo
{
    int prioridade, sub;
    int cpu[MAX_PROC];
    int es[MAX_PROC];
};
//

int instante = 0;
int main(int argc, char **argv)
{

    char entrada[50];

    int quantum, seq = 0;

    strcpy(entrada, argv[1]);
    quantum = argv[2];

    FILE *arqv = fopen(entrada, "r");

    if (argc > 2)
    {
        if (strcmp(argv[3], "-seq"))
        {
            seq = 1;
        }
        else
        {
            printf("\nArgumento desconhecido");
        }
    }
    char texto[MAX_ARQV];

    // Codigo do strtok
    //TODO imagina q tenha q colocar um strtok dentro do outro, um para processos com \n e outro pra cada argumento do processo com ' '
    fgets(texto, MAX_ARQV, arqv);
    char *token = strtok(texto, " \t");

    int args = 0;
    while (token != NULL)
    {
        // ver se funciona ne
        numeros[args] = atoi(token);
        ++args;
        token = strtok(NULL, " \t");
    }

    // Codigo do strtok

    strcat(entrada, ".out");
    FILE *saida = fopen(entrada, "w");

    fclose(arqv);

    return 0;
}