#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

struct s_no
{
    int num;
    struct s_no *prox;
};
// #define s_no struct s_no

void printaLista(struct s_no *no)
{
    if (no != NULL)
    {
        printf("\n");

        printf("%d o-> ", no->num);
        no = no->prox;
        while ((no != NULL))
        {
            printf("%d o-> ", no->num);
            no = no->prox;
        }
    }
}
void inserir(struct s_no **pont, int num)
{
    if (*pont == NULL)
    {

        struct s_no *novo = malloc(sizeof(struct s_no));
        novo->num = num;
        novo->prox = NULL;

        (*pont) = novo;
    }
    else
    {

        // ir ate o final da lista
        while ((*pont)->prox != NULL)
        {
            *pont = (*pont)->prox;
        }

        struct s_no *novo = malloc(sizeof(struct s_no));
        novo->num = num;
        novo->prox = NULL;

        (*pont)->prox = novo;
    }
}
void remover(struct s_no **no, int num)
{
    struct s_no *ant;
    // achar o no
    while (((*no)->num != num) && ((*no)->prox != NULL))
    {
        ant = *no;
        *no = (*no)->prox;
    }

    // teoricamente funciona
    ant->prox = (*no)->prox;
    // free(*no);
}
void removerPares(struct s_no **no)
{
    // remove pares diferentes de 2
    struct s_no *ant;
    // achar o no
    while ((*no)->prox != NULL)
    {
        ant = *no;
        *no = (*no)->prox;

        if (((*no)->num % 2 == 0) && ((*no)->num != 2))
        {

            // teoricamente funciona
            ant->prox = (*no)->prox;
            free(*no);
        }
    }
}

int main()
{
    struct s_no *L;
    // L->prox = NULL;

    // teste
    inserir(&L, 1);
    inserir(&L, 2);
    inserir(&L, 3);
    inserir(&L, 4);
    printaLista(L);
    fflush(stdout);
    remover(&L, 2);
    printaLista(L);

    FILE *arqv = fopen("in.txt", "r");

    int *numeros;
    while (fread(numeros, sizeof(int), 1, arqv))
    {
        inserir(&L, *numeros);
    }

    // 1 thread

    return 0;
}