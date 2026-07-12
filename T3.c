#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// XXX TESTE
#include <unistd.h>

#define MAX_CPU 50
#define MAX_PROC 50
#define MAX_ARQV 100
#define true 1
#define false 0
struct Processo
{
    // o "nome" do processo
    int indice;
    int prioridade, submissao;
    int cpu[MAX_CPU];
    int es[MAX_CPU];
    int marcador;
};

// lista encadeada
struct s_no
{
    struct Processo *proc;
    struct s_no *prox;
};
// typedef struct s_no No;

void inserirLista(struct s_no **inicio, struct Processo *processo)
{
    if (*inicio != NULL)
    {
        struct s_no *p1 = *inicio;
        // vai ate o ultimo no
        while (p1->prox != NULL)
        {
            p1 = p1->prox;
        }

        struct s_no *novo = malloc(sizeof(struct s_no));
        // o negocio do endereco é duvidoso
        //[ ] testar
        novo->proc = processo;
        novo->prox = NULL;
        p1->prox = novo;
    }
    else
    {
        struct s_no *novo = malloc(sizeof(struct s_no));
        novo->proc = processo;
        novo->prox = NULL;
        *inicio = novo;
    }
}

void removerListaIni(struct s_no **inicio)
{

    // retira o primeiro elemento da lista
    if (*inicio != NULL)
    {
        struct s_no *temp = *inicio;
        *inicio = (*inicio)->prox;
        free(temp);
    }
    else
    {
        perror("remocao do no inicial");
    }
}

void removerLista(struct s_no **inicio, int x)
{
    if (*inicio == NULL)
    {
        // nada
    }
    else if ((*inicio)->proc->indice == x)
    {
        struct s_no *temp = *inicio;
        *inicio = (*inicio)->prox;
        free(temp);
    }
    else
    {
        // retira o elemento comparando os ponteiros, deve ser um mau habito
        struct s_no *p1 = *inicio, *p2;
        while (p1->proc->indice != x && p1->prox != NULL)
        {
            p2 = p1;
            p1 = p1->prox;
        }

        if (p1->proc->indice == x)
        {
            struct s_no *temp = p1;
            // costura o resto
            p1 = p1->prox;
            p2->prox = p1;
            // remove o no
            free(temp);
        }
        else
        {
            perror("remocao do no");
        }
    }
}

//[x] o vetor de proximos tem que ser uma lista encadeada preferencialmente
//[x] tem que ver o uso de CPU tambem:
// uso de CPU = tempo de execucao - tempo ocioso
//[x] nao precisa de fila de ES
// ES e CPU são executados separadamente

struct Processo *procurarSJF(struct s_no **inicio)
{
    if (*inicio == NULL)
    {
        return NULL;
    }
    else
    {
        struct Processo *menor;
        menor = (*inicio)->proc;
        struct s_no *p1 = *inicio;
        while (p1->prox != NULL)
        {
            p1 = p1->prox;
            // printf("pont P%d ->",p1->proc->indice);
            //  ve qual tem o menor pico de cpu
            if (menor->cpu[menor->marcador] > p1->proc->cpu[p1->proc->marcador])
            {
                menor = p1->proc;
            }
        }
        return menor;
    }
}
struct Processo *procurarPrioridade(struct s_no **inicio)
{
    if (*inicio == NULL)
    {
        return NULL;
    }
    else
    {
        struct Processo *maior;
        maior = (*inicio)->proc;
        struct s_no *p1 = *inicio;
        //[ ] nao vai mudar a referencia pro primeiro como aquele outro codigo?
        while (p1->prox != NULL)
        {
            p1 = p1->prox;
            // printf("pont P%d ->",p1->proc->indice);
            // ve qual tem o prioridade maior
            if (maior->prioridade < p1->proc->prioridade)
            {
                maior = p1->proc;
            }
            else if (maior->prioridade == p1->proc->prioridade)
            {
                // se forem iguais entao é FCFS
                if (maior->submissao > p1->proc->submissao)
                {
                    maior = p1->proc;
                }
            }
        }
        // printf("retorna P%d",maior->indice);
        return maior;
    }
}

// TODO -seq no FCFS nao serve pra nada
void FCFS(struct Processo *processos, int procCont, int seq)
{
    printf("FCFS: ");
    // honestamente, instante local faz mais sentido da forma como estou fazendo
    // instante é 1 né?
    int instante = 1;
    int ocioso = 0;
    int clock = 0;
    // assim nao vamos precisar sobrescrever nada no processos original
    struct Processo *proximos = processos;
    // cria inicio da lista de prontos
    struct s_no *prontos = NULL;

    //[ ] EU ACHO que o ES não vai mais precisar
    //[ ] na verdade nao entendi direito o papel do es nesse programa

    // salva qual processo esta executando
    struct Processo *executando = NULL;

    int exec = 0;
    while (exec < procCont)
    {
        printf("\ninstante %d", instante);
        //  para marcar no diagrama de gantt
        for (int i = 0; i < procCont; i++)
        {
            // procura se um processo ficou pronto agora
            if (proximos[i].submissao == instante)
            {
                // coloca na fila de prontos
                inserirLista(&prontos, &proximos[i]);
            }
        }

        // nao esta executando nada
        // nao tem preempção
        if (executando == NULL)
        {
            // pega o processo com menor tempo de submissao, que obviamente é o primeiro da fila
            if (prontos != NULL)
            {
                executando = prontos->proc;
            }

            if (executando == NULL)
            {
                // se nao tem nenhum processo pronto, o programa fica ocioso
                // TODO talvez colocar esse if la no final?
                // [ ] executar ES tambem conta como cpu?
                ++ocioso;
                ++clock;
                // printf("ocioso");
            }
            else
            {
                // escrever no diagrama de gantt quanto tempo ficou ocioso
                // TODO no primeiro é diferente
                // se for 0 nao tem porque printar
                if (clock > 0)
                {
                    printf("*** %d|", instante);
                }
                else
                {

                    // executa o pico de cpu
                    // so precisa disso se o ngc nao durou nada
                    executando->cpu[executando->marcador]--;
                    //++clock;
                }
                clock = 0;

                // printf("executando P%d", executando->indice);
            }
        }
        else
        {
            // executa o pico de cpu
            // printf("executando P%d", executando->indice);
            // fflush(stdout);
            executando->cpu[executando->marcador]--;
            ++clock;
            // printf("+1");

            // ve se já terminou
            if (executando->cpu[executando->marcador] == 0)
            {
                // pula pro proximo pico de cpu
                executando->marcador++;
                // se o proximo pico tambem for zero
                if (executando->cpu[executando->marcador] == 0)
                {
                    // o processo já foi encerrado
                    exec++;
                    // marca com -1 pra ele não executar de novo
                    executando->submissao = -1;
                }
                else
                {
                    // ainda tem um proximo pico
                    // [ ] sera que dá bom fazer assim? instante de submissao modificado
                    // ele so fica pronto de novo quando acabar a execucao da ES dele
                    executando->submissao = instante + executando->es[(executando->marcador) - 1];
                    // obviamente, isso só faz sentido se o executando conseguir mudar os proximos por referencia
                }
                // escrever no diagrama de gantt
                printf("P%d %d|", executando->indice, instante);
                clock = 0;
                // [ ] remover o processo da lista de prontos?
                removerListaIni(&prontos);
                // vai ver qual é o proximo a executar
                executando = NULL;
            }
        }
        ++instante;
        // sleep(1);
    }
}

// TODO -seq nao serve pra nada
void SJF(struct Processo *processos, int procCont, int seq)
{
    printf("SJF: ");
    // honestamente, instante local faz mais sentido da forma como estou fazendo
    int instante = 0;
    int ocioso = 0;
    int clock = 0;
    // assim nao vamos precisar sobrescrever nada no processos original
    struct Processo *proximos = processos;
    // cria inicio da lista de prontos
    struct s_no *prontos = NULL;

    //[ ] EU ACHO que o ES não vai mais precisar
    //[ ] na verdade nao entendi direito o papel do es nesse programa

    // salva qual processo esta executando
    struct Processo *executando = NULL;

    int exec = 0;
    while (exec < procCont)
    {
        // printf("\ninstante %d", instante);
        for (int i = 0; i < procCont; i++)
        {
            // printf("submissao %d",proximos[i].submissao);
            fflush(stdout);
            // procura se um processo ficou pronto agora
            if (proximos[i].submissao == instante)
            {
                // printf("inseriu");
                //  coloca na fila de prontos
                inserirLista(&prontos, &proximos[i]);
            }
        }

        // nao esta executando nada
        // nao tem preempção
        if (executando == NULL)
        {
            // pega o processo com menor pico de CPU
            //[ ] pode simplesmente pegar o menor? ou tem que fazer aquela fórmula pra descobrir qual é o menor?
            executando = procurarSJF(&prontos);

            if (executando == NULL)
            {
                // se nao tem nenhum processo pronto, o programa fica ocioso
                // TODO talvez colocar esse if la no final?
                //[ ] executar ES tambem conta como cpu?
                ++ocioso;
                ++clock;
                // printf("ocioso");
            }
            else
            {

                // se for 0 nao tem porque printars
                if (clock > 0)
                {
                    printf("*** %d|", instante);
                }
                else
                {

                    // executa o pico de cpu
                    executando->cpu[executando->marcador]--;
                    //++clock;
                }
                clock = 0;
                // printf("executando P%d", executando->indice);
            }
        }
        else
        {
            // executa o pico de cpu
            // printf("executando P%d", executando->indice);
            executando->cpu[executando->marcador]--;
            ++clock;

            // ve se já terminou
            if (executando->cpu[executando->marcador] == 0)
            {
                // pula pro proximo pico de cpu
                executando->marcador++;
                // se o proximo pico tambem for zero
                if (executando->cpu[executando->marcador] == 0)
                {
                    // o processo já foi encerrado
                    exec++;
                    // marca com -1 pra ele não executar de novo
                    executando->submissao = -1;
                }
                else
                {
                    // ainda tem um proximo pico
                    // [ ] sera que dá bom fazer assim? instante de submissao modificado
                    // ele so fica pronto de novo quando acabar a execucao da ES dele
                    executando->submissao = instante + executando->es[(executando->marcador) - 1];
                    // obviamente, isso só faz sentido se o executando conseguir mudar os proximos por referencia
                }

                // escrever no diagrama de gantt
                printf("P%d %d|", executando->indice, instante);
                clock = 0;
                // [ ] remover o processo da lista de prontos?
                removerLista(&prontos, executando->indice);
                // vai ver qual é o proximo a executar
                executando = NULL;
            }
        }
        ++instante;
        // sleep(1);
    }
}

// TODO -seq nao serve pra nada
void SRTF(struct Processo *processos, int procCont, int seq)
{
    printf("SRTF: ");
    // honestamente, instante local faz mais sentido da forma como estou fazendo
    int instante = 0;
    int ocioso = 0;
    int clock = 0;
    // assim nao vamos precisar sobrescrever nada no processos original
    struct Processo *proximos = processos;
    // cria inicio da lista de prontos
    struct s_no *prontos = NULL;

    //[ ] EU ACHO que o ES não vai mais precisar
    //[ ] na verdade nao entendi direito o papel do es nesse programa

    // salva qual processo esta executando
    struct Processo *executando = NULL;

    int exec = 0;
    while (exec < procCont)
    {
        // jeito bem anta de se arrumar
        int flag = 0;
        // executando = NULL;
        printf("\ninstante %d", instante);
        for (int i = 0; i < procCont; i++)
        {
            // procura se um processo ficou pronto agora
            if (proximos[i].submissao == instante)
            {
                // printf("inseriu P%d",proximos[i].indice);
                //  coloca na fila de prontos
                inserirLista(&prontos, &proximos[i]);
            }
        }
        // pelo jeito que o programa é formatado, precisa disso daqui

        struct Processo *novo = NULL;

        // tem preempção
        // pega o processo com menor pico de CPU
        novo = procurarSJF(&prontos);
        if (novo == NULL)
        {

            // se nao tem nenhum processo pronto, o programa fica ocioso
            ++ocioso;
            ++clock;
        }
        else
        {
            if (novo != executando)
            {
                // se o processo terminou ou foi interrompido
                if (executando != NULL)
                // se um processo foi interrompido
                {
                    printf("P%d %d| interrompido", executando->indice, instante);
                    flag = 1;
                    // executa o pico de cpu
                    executando->cpu[executando->marcador]--;
                    printf("executando P%d", executando->indice);
                }
                executando = novo;
            }
            if (clock > 0)
            {
                // se ficou um tempo ocioso
                //  esse clock é apenas para ocioso

                printf("*** %d|", instante);
                clock = 0;
            }
            else if (!flag)
            {
                // executa o pico de cpu
                executando->cpu[executando->marcador]--;
                printf("executando P%d", executando->indice);

                // ve se já terminou
                if (executando->cpu[executando->marcador] == 0)
                {
                    // pula pro proximo pico de cpu
                    executando->marcador++;
                    // se o proximo pico tambem for zero
                    if (executando->cpu[executando->marcador] == 0)
                    {
                        // o processo já foi encerrado
                        exec++;
                        // marca com -1 pra ele não executar de novo
                        executando->submissao = -1;
                    }
                    else
                    {
                        // ainda tem um proximo pico
                        // [ ] sera que dá bom fazer assim? instante de submissao modificado
                        // ele so fica pronto de novo quando acabar a execucao da ES dele
                        executando->submissao = instante + executando->es[(executando->marcador) - 1];
                        // obviamente, isso só faz sentido se o executando conseguir mudar os proximos por referencia
                    }

                    // escrever no diagrama de gantt
                    printf("P%d %d|", executando->indice, instante);
                    // clock = 0;
                    //  [ ] remover o processo da lista de prontos?
                    removerLista(&prontos, executando->indice);
                    // vai ver qual é o proximo a executar
                    executando = NULL;
                }
            }
        }
        ++instante;
    }
}

// TODO -seq nao serve pra nada
void PrioridadePreemptivo(struct Processo *processos, int procCont, int seq)
{
    // honestamente, instante local faz mais sentido da forma como estou fazendo
    int instante = 0;
    int ocioso = 0;
    int clock = 0;
    // assim nao vamos precisar sobrescrever nada no processos original
    struct Processo *proximos = processos;
    // cria inicio da lista de prontos
    struct s_no *prontos = NULL;

    //[ ] EU ACHO que o ES não vai mais precisar
    //[ ] na verdade nao entendi direito o papel do es nesse programa

    // salva qual processo esta executando
    struct Processo *executando = NULL;

    int exec = 0;
    while (exec < procCont)
    {
        // jeito bem anta de se arrumar
        int flag = 0;
        printf("\ninstante %d", instante);
        for (int i = 0; i < procCont; i++)
        {
            // procura se um processo ficou pronto agora
            if (proximos[i].submissao == instante)
            {
                // printf("inseriu P%d",proximos[i].indice);
                //  coloca na fila de prontos
                inserirLista(&prontos, &proximos[i]);
            }
        }

        struct Processo *novo = NULL;
        // tem preempção
        // pega o processo com menor pico de CPU
        novo = procurarPrioridade(&prontos);
        if (novo == NULL)
        {

            // se nao tem nenhum processo pronto, o programa fica ocioso
            ++ocioso;
            ++clock;
        }
        else
        {
            if (novo != executando)
            {
                // se o processo terminou ou foi interrompido
                if (executando != NULL)
                // se um processo foi interrompido
                {
                    printf("P%d %d| interrompido", executando->indice, instante);
                    flag = 1;
                    // executa o pico de cpu
                    executando->cpu[executando->marcador]--;
                    printf("executando P%d", executando->indice);
                    // ve se já terminou
                    if (executando->cpu[executando->marcador] == 0)
                    {
                        // pula pro proximo pico de cpu
                        executando->marcador++;
                        // se o proximo pico tambem for zero
                        if (executando->cpu[executando->marcador] == 0)
                        {
                            // o processo já foi encerrado
                            exec++;
                            // marca com -1 pra ele não executar de novo
                            executando->submissao = -1;
                        }
                        else
                        {
                            // ainda tem um proximo pico
                            // [ ] sera que dá bom fazer assim? instante de submissao modificado
                            // ele so fica pronto de novo quando acabar a execucao da ES dele
                            executando->submissao = instante + executando->es[(executando->marcador) - 1];
                            // obviamente, isso só faz sentido se o executando conseguir mudar os proximos por referencia
                        }
                        // escrever no diagrama de gantt
                        printf("P%d %d|", executando->indice, instante);
                        // [ ] remover o processo da lista de prontos?
                        removerLista(&prontos, executando->indice);
                        // vai ver qual é o proximo a executar
                        executando = NULL;
                    }
                }
                executando = novo;
            }
            if (clock > 0)
            {
                // se ficou um tempo ocioso
                //  esse clock é apenas para ocioso

                printf("*** %d|", instante);
                clock = 0;
            }
            else if (!flag)
            {
                // executa o pico de cpu
                executando->cpu[executando->marcador]--;
                printf("executando P%d", executando->indice);

                // ve se já terminou
                if (executando->cpu[executando->marcador] == 0)
                {
                    // pula pro proximo pico de cpu
                    executando->marcador++;
                    // se o proximo pico tambem for zero
                    if (executando->cpu[executando->marcador] == 0)
                    {
                        // o processo já foi encerrado
                        exec++;
                        // marca com -1 pra ele não executar de novo
                        executando->submissao = -1;
                    }
                    else
                    {
                        // ainda tem um proximo pico
                        // [ ] sera que dá bom fazer assim? instante de submissao modificado
                        // ele so fica pronto de novo quando acabar a execucao da ES dele
                        executando->submissao = instante + executando->es[(executando->marcador) - 1];
                        // obviamente, isso só faz sentido se o executando conseguir mudar os proximos por referencia
                    }
                    // escrever no diagrama de gantt
                    printf("P%d %d|", executando->indice, instante);
                    // [ ] remover o processo da lista de prontos?
                    removerLista(&prontos, executando->indice);
                    // vai ver qual é o proximo a executar
                    executando = NULL;
                }
            }
        }
        ++instante;
    }
}
// HACK agora que penso, dá pra fazer -seq com apenas um numero:
// vc vai adicionando os tempos de ES nele, e sempre q for recolocar o processo em espera vc adiciona esse numero no tempo de submissao
//  TODO -seq no FCFS nao serve pra nada
void RoundRobin(struct Processo *processos, int procCont, int seq, int quantum)
{
    // honestamente, instante local faz mais sentido da forma como estou fazendo
    int instante = 0;
    int ocioso = 0;
    // assim nao vamos precisar sobrescrever nada no processos original
    struct Processo *proximos = processos;
    // cria inicio da lista de prontos
    // no RR especificamente, prontos sempre aponta pro inicio da lista, e atual aponta pro q vai executar
    struct s_no *prontos = NULL, *atual = NULL;

    //[ ] EU ACHO que o ES não vai mais precisar
    //[ ] na verdade nao entendi direito o papel do es nesse programa

    // salva qual processo esta executando
    struct Processo *executando = NULL;

    int exec = 0;
    int q = 0;
    while (exec < procCont)
    {
        for (int i = 0; i < procCont; i++)
        {
            // procura se um processo ficou pronto agora
            if (proximos[i].submissao == instante)
            {
                // coloca na fila de prontos
                inserirLista(&prontos, &proximos[i]);
            }
        }

        // nao esta executando nada
        // nao tem preempção
        if (executando == NULL)
        {
            // atual é nulo quando é a primeira execucao ou ele chegou no final da fila
            if (atual == NULL)
            {
                atual = prontos;
            }
            executando = atual->proc;

            if (executando == NULL)
            {
                // se nao tem nenhum processo pronto, o programa fica ocioso
                // TODO talvez colocar esse if la no final?
                //[ ] executar ES tambem conta como cpu?
                ++ocioso;
            }
            else
            {

                // executa o pico de cpu
                // FIXME essa execução dupla pode dar erro
                executando->cpu[executando->marcador]--;
                q++;
            }
        }
        else
        {
            // executa o pico de cpu
            executando->cpu[executando->marcador]--;
            q++;

            // ve se já terminou
            if (executando->cpu[executando->marcador] == 0)
            {
                // pula pro proximo pico de cpu
                executando->marcador++;
                // se o proximo pico tambem for zero
                if (executando->cpu[executando->marcador] == 0)
                {
                    // o processo já foi encerrado
                    exec++;
                    // marca com -1 pra ele não executar de novo
                    executando->submissao = -1;
                }
                else
                {
                    // ainda tem um proximo pico
                    // [ ] sera que dá bom fazer assim? instante de submissao modificado
                    // ele so fica pronto de novo quando acabar a execucao da ES dele
                    executando->submissao = instante + executando->es[(executando->marcador) - 1];
                    // obviamente, isso só faz sentido se o executando conseguir mudar os proximos por referencia
                }
                // [ ] remover o processo da lista de prontos?
                removerListaIni(&prontos);
                // vai ver qual é o proximo a executar
                executando = NULL;
                atual = atual->prox;
            }

            if (q = quantum)
            {
                // se ja chegou no quantum tem q executar o proximo
                executando = NULL;
                atual = atual->prox;
            }
        }
        ++instante;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("\nquantidade de argumentos inválida");
        printf("\nEscreva: nome do programa - nome do arquivo a ser lido - numero de quantum - sequencial ou nao");
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

    // printf("\ntexto %s ", texto);
    char *proc = strtok(texto, "\n");

    int numProc = 0, numArgs = 0, numCpu = 0, numEs = 0;

    while (proc != NULL)
    {
        strcpy(textoProc[numProc], proc);
        // printf("\n%s", textoProc[numProc]);
        ++numProc;
        proc = strtok(NULL, "\n");
    }

    // printf("\nnumproc %d", numProc);

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
        // coloca 0 no final
        processos[i].cpu[numCpu] = 0;
        processos[i].es[numEs] = 0;
        processos[i].marcador = 0;
        // printf("\ncheegou");
        fflush(stdout);

        // o nome é dado pela ordem de chegada, sem depender da leitura do arquivo
        //+1 pra ficar como no do fabricio
        processos[i].indice = i + 1;
    }

    // Codigo do strtok

    /*
    for (int i = 0; i < numProc; i++)
    {
        printf("\nprocesso %d", i);
        printf("\n %d %d %d %d %d", processos[i].prioridade, processos[i].submissao, processos[i].cpu[0], processos[i].es[0], processos[i].cpu[1]);
        fflush(stdout);
    }
        */

    // FIXME processos é passado por referencia, quer dizer que só uma funcao pode ser usada
    // FCFS(processos, numProc, seq);
    // SJF(processos, numProc, seq);
    // SRTF(processos, numProc, seq);
    PrioridadePreemptivo(processos, numProc, seq);

    strcat(entrada, ".out");
    FILE *saida = fopen(entrada, "w");

    fclose(arqv);
    fclose(saida);
    return 0;
}
