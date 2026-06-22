#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CPU 50
#define MAX_PROC 50
#define MAX_ARQV 100
struct Processo
{
    int prioridade, submissao;
    int cpu[MAX_CPU];
    int es[MAX_CPU];
};
//

int instante = 0;
int main(int argc, char *argv[])
{

    char entrada[50];

    int quantum, seq = 0;

    strcpy(entrada, argv[1]);
    quantum = atoi(argv[2]);

    FILE *arqv = fopen(entrada, "r");

    if (argc > 3)
    {
        if (strcmp(argv[3], "-seq") == 0)
        {
            seq = 1;
        }
        else
        {
            printf("\nArgumento desconhecido");
        }
    }
    char texto[MAX_ARQV];
    char textoProc[MAX_PROC][MAX_PROC];
    // onde os processos serão guardados
    struct Processo processos[MAX_PROC];

    // Codigo do strtok
    fgets(texto, MAX_ARQV, arqv);
    char *proc = strtok(texto, "\n");

    int numProc = 0, numArgs = 0, numCpu = 0, numEs = 0;

    while (proc != NULL)
    {
        strcpy(textoProc[numProc], proc);
        ++numProc;
        proc = strtok(NULL, "\n");
        printf("\n%s", textoProc[numProc-1]);
    }

    printf("\nnumproc %d", numProc);

    fflush(stdout);

    // TODO ver se funciona

    for (int i = 0; i < numProc; i++)
    {
        char *args = strtok(textoProc[i], "\n");
        // pega a prioridade
        processos[i].prioridade = atoi(args);
        //++numProc;
        args = strtok(NULL, " ");

        // pega o momento de submissao
        processos[i].submissao = atoi(args);
        //++numProc;
        args = strtok(NULL, " ");

        while (args != NULL)
        {

            // pega a cpu
            processos[i].cpu[numCpu] = atoi(args);
            ++numCpu;
            args = strtok(NULL, " ");

            // pega o E/S
            processos[i].es[numEs] = atoi(args);
            ++numEs;
            args = strtok(NULL, " ");
        }
    }

    // Codigo do strtok

    for (int i = 0; i < numProc; i++)
    {
        printf("\nprocesso %i", i);
        printf("\n %i %i %i %i", processos[i].prioridade, processos[i].submissao, processos[i].cpu[0], processos[i].es[0]);
    }

    strcat(entrada, ".out");
    FILE *saida = fopen(entrada, "w");

    fclose(arqv);

    return 0;
}