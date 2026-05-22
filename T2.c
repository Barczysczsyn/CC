#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

struct s_no
{
    int num;
    struct s_no *prox;
};
// #define s_no struct s_no

int primo(int numero)
{
    if (numero <= 1)
    {
        return 0;
    }
    if (numero <= 3)
    {
        return 1;
    }
    if ((numero % 2 == 0) || (numero % 3 == 0))
    {
        return 0;
    }

    for (int i = 5; i * i <= numero; i += 6)
    {
        if (numero % i == 0 || numero % (i + 2) == 0)
        {
            return 0;
        }
    }
    return 1;
}
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

                printf("\nremovido %d", p1->num);
                fflush(stdout);
                //   teoricamente funciona
                ant->prox = p1->prox;

                struct s_no *temp = p1;
                p1 = p1->prox;
                free(temp);
            }

            // ve se ja nao esta no final da fila
            if (p1 != NULL)
            {
                ant = p1;
                p1 = p1->prox;
            }
        }

        // se o no for achado
    }
}

void removerPrimos(struct s_no **pont)
{
    if (*pont == NULL)
    {

        return;
    }
    else if (!primo((*pont)->num))
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
            if (!primo(p1->num))
            {

                printf("\nremovido %d", p1->num);
                fflush(stdout);
                //   teoricamente funciona
                ant->prox = p1->prox;

                struct s_no *temp = p1;
                p1 = p1->prox;
                free(temp);
            }

            // ve se ja nao esta no final da fila
            if (p1 != NULL)
            {
                ant = p1;
                p1 = p1->prox;
            }
        }

        // se o no for achado
    }
}

int main()
{
    struct s_no *L = NULL;
    // L->prox = NULL;

    /* teste
    inserir(&L, 1);
    inserir(&L, 2);
    inserir(&L, 3);
    inserir(&L, 4);
    inserir(&L, 5);
    inserir(&L, 11);
    printaLista(L);
    removerPares(&L);
    printaLista(L);
    removerPrimos(&L);
    printaLista(L);

    //*/
    // teste

    FILE *arqv = fopen("100.txt", "r");

    // mede o tamanho
    fseek(arqv, 0, SEEK_END); // Seek to the end
    long tam = ftell(arqv);   // Get current position
    fseek(arqv, 0, SEEK_SET); // volta pro comeco (VITAL)

    int tamanho = tam / sizeof(int);

    printf("\ntamanho %d", tamanho);
    // int *numeros = malloc( * 2);
    int numeros[100];
    // char *texto = malloc((tam / sizeof(int)) * 4);
    char texto[300];
    // fread(numeros, sizeof(int), 1, arqv);

    fgets(texto, 300, arqv);

    printf("\ntexto %s", texto);
    printf("\ntam %li", tam);

    // strtok

    char *token = strtok(texto, " \t");

    int nms = 0;
    while (token != NULL)
    {
        // ver se funciona ne
        numeros[nms] = atoi(token);
        ++nms;
        token = strtok(NULL, " \t");
    }
    // tem um warning entao eu tirei
    // FIXME sera q precisa disso?
    numeros[nms] = '\0';

    fflush(stdout);
    // strtok

    // FIXME teoricamente tá certo, mas da um erro desgraçado
    // free(texto);

    for (int i = 0; i < sizeof(numeros) / sizeof(int); i++)
    {
        printf("\nnumeros %d", numeros[i]);
        inserir(&L, numeros[i]);
    }
    // TODO só coloca na lista até o 69

    printaLista(L);
    // 1 thread

    // aparentemente funciona até aqui

    // FIXME corrupted size vs. prev_size
    // causado pelo fclose de alguma forma???
    //

    fclose(arqv);
    return 0;
}