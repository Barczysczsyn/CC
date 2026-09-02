#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>


    pthread_mutex_t *mutexes;
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
static void *printaLista(void *args)
{
    // printf("\nlista");
    //   contador
    sem_wait(s3);
    struct s_no *pont = (struct s_no *)args;
    if (pont != NULL)
    {
        printf("\n");

        printf("%d ", pont->num);
        struct s_no *p1 = pont->prox;
        while ((p1 != NULL))
        {
            printf("%d ", p1->num);
            p1 = p1->prox;
        }
    }
    fflush(stdout);
    // sem_post(s4);
    //  so pra ele nao reclamar

    return args;
}

// insere tudo de uma só vez
void inserirTotal(struct s_no **pont, int *num, int tam)
{
    // coloca o primeiro elemento na lista
    struct s_no *novo = malloc(sizeof(struct s_no));
    novo->num = num[0];
    novo->prox = NULL;
    (*pont) = novo;

    struct s_no *atual = (*pont);
    for (int i = 1; i < tam; i++)
    {

        struct s_no *novo = malloc(sizeof(struct s_no));
        novo->num = num[i];
        novo->prox = NULL;

        atual->prox = novo;

        // ir pra frente
        atual = atual->prox;
        // c1++;
        // incrementa 1
        pthread_mutex_unlock(&m1);
    }
}

static void *removerPares(void *args)
{
    int cont = 0;
    // void *saida;
    struct s_no **pont = (struct s_no **)args;

    if (*pont == NULL)
    {

        // so pra ele nao reclamar
        return args;
    }
    else if (((*pont)->num % 2 == 0) && ((*pont)->num != 2))
    {
        struct s_no *temp = *pont;
        *pont = (*pont)->prox;
        free(temp);
    }
    struct s_no *ant = *pont, *p1 = (*pont)->prox;
    // achar o no
    while (p1 != NULL)
    {
        // while (c2 >= c1 + 1)
        // {
        //     // assim ele vai esperar
        //     ;
        // }
        pthread_mutex_lock(&mutexes[cont]);
        if ((p1->num % 2 == 0) && (p1->num != 2))
        {

            // printf("\nremovido %d", p1->num);
            // fflush(stdout);
            //    teoricamente funciona
            ant->prox = p1->prox;

            struct s_no *temp = p1;
            p1 = p1->prox;
            free(temp);
        }
        else
        {
            ant = p1;
            p1 = p1->prox;
        }
        // c2++;
        pthread_mutex_unlock(&m2);
    }

    // se o no for achado

    // so pra ele nao reclamar
    return args;
}

static void *removerPrimos(void *args)
{
    struct s_no **pont = (struct s_no **)args;

    if (*pont == NULL)
    {

        // so pra ele nao reclamar
        return args;
    }
    else if (!primo((*pont)->num))
    {
        struct s_no *temp = *pont;
        *pont = (*pont)->prox;
        free(temp);
    }

    struct s_no *ant = *pont, *p1 = (*pont)->prox;
    // achar o no
    while (p1 != NULL)
    {
        // while (c3 >= c2 + 1)
        // {
        //     // assim ele vai esperar
        //     ;
        // }
        sem_wait(s2);
        if (!primo(p1->num))
        {

            // printf("\nremovido %d", p1->num);
            // fflush(stdout);
            //    teoricamente funciona
            ant->prox = p1->prox;

            struct s_no *temp = p1;
            p1 = p1->prox;
            free(temp);
        }
        else
        {
            ant = p1;
            p1 = p1->prox;
        }
        //++c3;
        sem_post(s3);
    }

    // so pra ele nao reclamar
    return args;
}

int main()
{
    struct s_no *L = NULL;
    struct s_no *FL = NULL;
    // L->prox = NULL;

    // thread principal

    FILE *arqv = fopen("100.txt", "r");

    // mede o tamanho
    fseek(arqv, 0, SEEK_END); // Seek to the end
    long tam = ftell(arqv);   // Get current position
    fseek(arqv, 0, SEEK_SET); // volta pro comeco (VITAL)

    // tamanho nem vai precisar
    // int tamanho = tam / sizeof(int);
    // printf("\ntamanho %d", tamanho);

    int *numeros = malloc(tam);
    // int numeros[100];
    // *3 é uma aproximacao q costuma funcionar
    char *texto = malloc(tam * 3);
    // char texto[300];
    //  fread(numeros, sizeof(int), 1, arqv);

    fgets(texto, (tam * 3), arqv);

    // printf("\ntexto %s", texto);
    // printf("\ntam %li", tam);

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
    numeros[nms] = '\0';

    // fflush(stdout);
    //  strtok
    //
    //  free(texto);
    mutexes = malloc(nms * sizeof(pthread_mutex_t));
    for (int i = 0; i < nms; i++)
    {
        mutexes[i] = PTHREAD_MUTEX_INITIALIZER;
    }

    // s4 = sem_open("/sem4", O_CREAT, 0660, 4);
    //  thread principal
    inserirTotal(&L, numeros, nms);
    // thread principal

    // printaLista(L);

    pthread_t *thread_id[3] = {NULL, NULL, NULL};

    // 1 thread
    // removerPares(&L);

    pthread_create((&thread_id[0]), NULL, removerPares, &L);
    // printaLista(L);
    //  1 thread

    // 2 thread
    // removerPrimos(&L);
    pthread_create((&thread_id[1]), NULL, removerPrimos, &L);
    // printaLista(L);
    //  2 thread

    // 3 thread
    // printaLista(L);
    pthread_create((&thread_id[2]), NULL, printaLista, L);
    // 3 thread

    // aparentemente funciona até aqui

    // causado pelo fclose de alguma forma???
    //

    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
    pthread_join(thread_id[2], NULL);
    // free(texto);
    fclose(arqv);
    return 0;
}
