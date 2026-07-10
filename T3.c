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
    int marcador;
};

//lista encadeada
struct s_no{
    struct Processo *ptr;
    struct s_no *prox;
};
//typedef struct s_no No;

//[x] o vetor de proximos tem que ser uma lista encadeada preferencialmente
//[x] tem que ver o uso de CPU tambem:
//uso de CPU = tempo de execucao - tempo ocioso
//[x] nao precisa de fila de ES
//ES e CPU são executados separadamente
int instante = 0;

void FCFS(struct Processo *processos, int procCont)
{
    // assim nao vamos precisar sobrescrever nada no processos original
    struct Processo *proximos = processos;
    for (int k = 0; k < procCont; k++)
    {
        int j = 0;
        for (int i = 0; i < procCont; i++)
        {

            // escolhe o processo submetido mais cedo
            if (proximos[i].submissao < proximos[j].submissao)
            {
                // ve se o processo ja nao foi executado
                // fiz em dois ifs pra ficar mais facil de entender
                // estava em ==, mas era pra ser != né?
                if ((proximos[i].cpu[0] != 0) && (proximos[j].es[0] != 0))
                    j = i;
            }
        }

        //"executa" o processo
        // tira ele da lista de proximos
        int i = 0;
        while (proximos[j].cpu[i] != 0)
        {
            instante += proximos[j].cpu[i];
            proximos[j].cpu[i] = 0;
            i++;
        }
        i = 0;
        while (proximos[j].es[i] != 0)
        {
            instante += proximos[j].es[i];
            proximos[j].es[i] = 0;
            i++;
        }
    }
}

// TODO perguntar ao professor se pode colocar uma flag de processo ja executado, e outras coisas na struct processo
// FIXME pode, deve na verdade
void RR(struct Processo *processos, int procCont, int quantum)
{
    // par significa cpu
    // impar significa es
    int marcador[MAX_PROC][2];

    // talvez nao precisava, mas vai ficar bem mais facil
    // serve pra diferenciar se vai ser es ou cpu
    int qual[MAX_PROC];
    // assim nao vamos precisar sobrescrever nada no processos original
    struct Processo *proximos = processos;
    // bubble sort para ordenar o vetor de proximos
    int flag;
    do
    {
        flag = 0;
        for (int i = 1; i < procCont; i++)
        {

            if (proximos[i - 1].submissao > proximos[i].submissao)
            {
                struct Processo aux = proximos[i - 1];
                proximos[i - 1] = proximos[i];
                proximos[i] = aux;
                flag = 1;
            }
        }
    } while (flag);

    // bubble sort para ordenar o vetor de proximos

    int j = 0;
    //TODO condição
    while (j < procCont)
    {
        if (marcador[j][0] >= marcador[j][1])
        {
            if (proximos[j].cpu[marcador[j][0]] > quantum)
            {
                instante += quantum;
                proximos[j].cpu[marcador[j][0]] -= quantum;
                marcador[j][0]++;
            }
            else
            {

                instante += proximos[j].es[marcador[j][0]];
                proximos[j].cpu[marcador[j][0]] = 0;
                marcador[j][0]++;
            }
        }
        else
        {
            if (proximos[j].cpu[marcador[j][1]] > quantum)
            {
                instante += quantum;
                proximos[j].cpu[marcador[j][1]] -= quantum;
                marcador[j][1]++;
            }
            else
            {

                instante += proximos[j].es[marcador[j][1]];
                proximos[j].cpu[marcador[j][1]] = 0;
                marcador[j][1]++;
            }
        }
        j++;
    }
}
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("\nquantidade de argumentos inválida");
        return 0;
    }

    char entrada[50];

    int quantum, seq = 0;

    strcpy(entrada, argv[1]);
    quantum = atoi(argv[2]);

    FILE *arqv = fopen(entrada, "r+");

    if (arqv == NULL)
    {
        printf("\nArquivo não encontrado ou corrompido.");
        return 0;
    }

    if (argc > 3)
    {
        if (strcmp(argv[3], "-seq") == 0)
        {
            seq = 1;
        }
        else
        {
            printf("\nArgumento desconhecido");
            // return 0;
        }
    }
    // char texto[MAX_ARQV];
    char textoProc[MAX_PROC][MAX_PROC];
    // onde os processos serão guardados
    struct Processo processos[MAX_PROC];

    //*
    // como precisamos ler o \n, tera q ser feito desse jeito
    fseek(arqv, 0, SEEK_END);
    long file_size = ftell(arqv);
    rewind(arqv); // volta o ponteiro pro comeco

    // ia burra corrigiu o proprio codigo
    char *texto = malloc(file_size + 1);
    if (texto == NULL)
    {
        perror("\nerro no malloc");
        fclose(arqv);
        return 0;
    }

    size_t bytes_read = fread(texto, 1, file_size, arqv);
    texto[bytes_read] = '\0';
    //*/
    // fgets(texto, MAX_ARQV, arqv);
    // fread(texto,sizeof(arqv)*4,1,arqv);

    // Codigo do strtok

    printf("\ntexto %s ", texto);
    char *proc = strtok(texto, "\n");

    int numProc = 0, numArgs = 0, numCpu = 0, numEs = 0;

    while (proc != NULL)
    {
        strcpy(textoProc[numProc], proc);
        printf("\n%s", textoProc[numProc]);
        ++numProc;
        proc = strtok(NULL, "\n");
    }

    printf("\nnumproc %d", numProc);

    fflush(stdout);

    for (int i = 0; i < numProc; i++)
    {
        char *args = strtok(textoProc[i], " ");
        // pega a prioridade
        processos[i].prioridade = atoi(args);
        //++numProc;
        args = strtok(NULL, " ");

        // pega o momento de submissao
        processos[i].submissao = atoi(args);
        //++numProc;
        args = strtok(NULL, " ");

        numCpu = 0, numEs = 0;
        while (args != NULL)
        {

            // pega a cpu
            processos[i].cpu[numCpu] = atoi(args);
            ++numCpu;
            args = strtok(NULL, " ");

            // pega o E/S
            // so um cheque a mais
            if (args != NULL)
            {
                processos[i].es[numEs] = atoi(args);
                ++numEs;
                args = strtok(NULL, " ");
            }
        }
        printf("\ncheegou");
        fflush(stdout);
    }

    // Codigo do strtok

    for (int i = 0; i < numProc; i++)
    {
        printf("\nprocesso %d", i);
        printf("\n %d %d %d %d %d", processos[i].prioridade, processos[i].submissao, processos[i].cpu[0], processos[i].es[0], processos[i].cpu[1]);
        fflush(stdout);
    }

    strcat(entrada, ".out");
    FILE *saida = fopen(entrada, "w");

    fclose(arqv);
    fclose(saida);
    return 0;
}
