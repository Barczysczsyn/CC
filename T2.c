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

        printf("%d -o-> ", no->num);
        struct s_no *p1 = no->prox;
        while ((p1 != NULL))
        {
            printf("%d -o-> ", p1->num);
            p1 = p1->prox;
        }
    }
    fflush(stdout);
}
void inserir(struct s_no **pont, int num)
{

    struct s_no *novo = malloc(sizeof(struct s_no));
    novo->num = num;
    novo->prox = NULL;

    if (*pont == NULL)
    {

        (*pont) = novo;
    }
    else
    {

        // ir ate o final da lista
        // aparentemente precisa de um auxiliar
        struct s_no *atual = *pont;
        while (atual->prox != NULL)
        {
            atual = atual->prox;
        }

        atual->prox = novo;
    }
}
void remover(struct s_no **pont, int num)
{
    if (*pont == NULL)
    {

        return;
    }
    else if ((*pont)->num == num)
    {
        struct s_no *temp = *pont;
        *pont = (*pont)->prox;
        free(temp);
        return;
    }
    else
    {
        struct s_no *ant = *pont, *p1 = (*pont)->prox;
        // achar o no
        while ((p1 != NULL) && (p1->num != num))
        {
            ant = p1;
            p1 = p1->prox;
        }

        // se o no for achado
        if (p1->num == num)
        {

            // printf("\nremovido %d", p1->num);
            // fflush(stdout);
            //  teoricamente funciona
            ant->prox = p1->prox;
            free(p1);
        }
    }
}
void removerPares(struct s_no **pont)
{
    if (*pont == NULL)
    {

        return;
    }
    else if (((*pont)->num % 2 == 0) && ((*pont)->num != 2))
    {
        struct s_no *temp = *pont;
        *pont = (*pont)->prox;
        free(temp);
        return;
    }
    else
    {
        struct s_no *ant = *pont, *p1 = (*pont)->prox;
        // achar o no
        while (p1 != NULL)
        {
            if ((p1->num % 2 == 0) && (p1->num != 2))
            {

                // printf("\nremovido %d", p1->num);
                // fflush(stdout);
                //TODO testar
                //  teoricamente funciona
                ant->prox = p1->prox;

                struct s_no *temp = p1;
                p1 = p1->prox;
                free(temp);
            }
            ant = p1;
            p1 = p1->prox;
        }

        // se o no for achado
    }
}

int main()
{
    struct s_no *L = NULL;
    // L->prox = NULL;

    // teste
    inserir(&L, 1);
    inserir(&L, 2);
    inserir(&L, 3);
    inserir(&L, 4);
    printaLista(L);
    removerPares(&L);
    printaLista(L);

    FILE *arqv = fopen("in.txt", "r");

    // mede o tamanho
    fseek(arqv, 0, SEEK_END); // Seek to the end
    long tam = ftell(arqv);   // Get current position

    int *numeros = malloc(tam / sizeof(int));
    fread(numeros, sizeof(int), 1, arqv);
    inserir(&L, *numeros);

    // 1 thread

    return 0;
}