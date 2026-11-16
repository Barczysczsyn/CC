#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

struct s_no
{
    int num;
    struct s_no *prox;
    pthread_mutex_t mutex;
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

// insere tudo de uma só vez
void inserirTotal(struct s_no **pont, int *num, int tam)
{
    // coloca o primeiro elemento na lista
    struct s_no *novo = malloc(sizeof(struct s_no));
    // basicamente não vai usar, mas vou colocar pra ficar mais consistente
    pthread_mutex_init(&(novo->mutex), NULL);
    pthread_mutex_lock(&(novo->mutex));
    novo->num = num[0];
    novo->prox = NULL;

    pthread_mutex_unlock(&(novo->mutex));
    (*pont) = novo;

    struct s_no *atual = (*pont);
    for (int i = 1; i < tam; i++)
    {

        struct s_no *novo = malloc(sizeof(struct s_no));
        pthread_mutex_init(&(novo->mutex), NULL);
        // TODO testar
        pthread_mutex_lock(&(novo->mutex));
        novo->num = num[i];
        novo->prox = NULL;

        atual->prox = novo;

        // ir pra frente
        atual = atual->prox;
        // c1++;
        // incrementa 1
        pthread_mutex_unlock(&(novo->mutex));
        // printf("\ncheguei %i", i);
        fflush(stdout);
    }
}

static void *removerPares(void *args)
{
    struct s_no **pont = (struct s_no **)args;

    // remove o primeiro nó

    if (*pont == NULL)
    {

        // so pra ele nao reclamar
        return args;
    }
    else if (((*pont)->num % 2 == 0) && ((*pont)->num != 2))
    {
        // TODO nao sei nem se precisa
        pthread_mutex_lock(&(*pont)->mutex);
        struct s_no *temp = *pont;
        pthread_mutex_unlock(&(*pont)->mutex);
        *pont = (*pont)->prox;
        free(temp);
    }

    struct s_no *ant = *pont, *p1 = (*pont)->prox;
    // achar o no
    while (p1 != NULL)
    {
        // TODO testar
        pthread_mutex_lock(&p1->mutex);

        if ((p1->num % 2 == 0) && (p1->num != 2))
        {
            // printf("\nremovido %d", p1->num);
            // fflush(stdout);
            //    teoricamente funciona
            ant->prox = p1->prox;

            struct s_no *temp = p1;
            p1 = p1->prox;

            // aparentemente precisa destruir o mutex
            pthread_mutex_unlock(&temp->mutex);
            pthread_mutex_destroy(&temp->mutex);
            free(temp);
        }
        else
        {
            pthread_mutex_unlock(&ant->mutex);
            ant = p1;
            p1 = p1->prox;
        }
        // c2++;
    }

    // se o no for achado

    pthread_mutex_unlock(&ant->mutex);
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
        pthread_mutex_lock(&p1->mutex);
        // TODO testar
        if (!primo(p1->num))
        {

            // printf("\nremovido %d", p1->num);
            // fflush(stdout);
            //    teoricamente funciona
            ant->prox = p1->prox;

            struct s_no *temp = p1;
            p1 = p1->prox;
            // aparentemente precisa destruir o mutex
            pthread_mutex_unlock(&temp->mutex);
            pthread_mutex_destroy(&temp->mutex);
            free(temp);
        }
        else
        {
            // destrava o anterior
            pthread_mutex_unlock(&ant->mutex);
            ant = p1;
            p1 = p1->prox;
        }
        // TODO testar
    }

    pthread_mutex_unlock(&ant->mutex);
    // so pra ele nao reclamar
    return args;
}

static void *printaLista(void *args)
{
    // BUG
    struct s_no *pont = (struct s_no *)args;
    if (pont != NULL)
    {
        printf("\n");

        pthread_mutex_lock(&pont->mutex);
        printf("%d ", pont->num);
        pthread_mutex_unlock(&pont->mutex);
        struct s_no *p1 = pont->prox;
        while ((p1 != NULL))
        {
            // TODO testar
            // ele nao precisa bloquear na verdade, mas essa funcao é necessaria so pra ele testar
            pthread_mutex_lock(&p1->mutex);
            printf("%d ", p1->num);
            pthread_mutex_unlock(&p1->mutex);
            p1 = p1->prox;
            // TODO bloquear o prox tambem?
        }
    }
    fflush(stdout);
    //  so pra ele nao reclamar

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

    fflush(stdout);
    //  strtok
    //
    //  free(texto);

    // s4 = sem_open("/sem4", O_CREAT, 0660, 4);
    //  thread principal
    inserirTotal(&L, numeros, nms);
    // thread principal

    fflush(stdout);
    //printaLista(L);

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

    //

    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
    pthread_join(thread_id[2], NULL);
    // free(texto);
    fclose(arqv);
    return 0;
}