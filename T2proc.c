#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_NODES 1000000
#define SHM_NAME "/lista"
typedef struct
{
    unsigned int value;
    int next;
    volatile int removed;
} s_no;

// volatile: obriga o processo a reler o valor da memoria a cada acesso.
typedef struct
{

    // Contadores que coordenam a execucao dos processos
    volatile int c1, c2, c3, c4;

    volatile int done_reading;
    volatile int done_par;
    volatile int done_prime;

    // Lista por offsets
    int head_offset;
    int tail_offset;
    int next_free;

    unsigned char bitmap[MAX_NODES]; // mapa de alocacao: 1 byte (1=alocado, 0=livre)
    s_no nodes[MAX_NODES];
} Shared;
// #define s_no struct s_no

Shared *map_shared(int create)
{
    // TODO mudar
    int flags = create ? (O_CREAT | O_RDWR) : O_RDWR;
    int fd = shm_open(SHM_NAME, flags, 0600);
    if (fd == -1)
    {
        perror("shm_open");
        exit(1);
    }

    if (create)
    {
        if (ftruncate(fd, sizeof(Shared)) == -1)
        {
            perror("ftruncate");
            exit(1);
        }
    }

    Shared *sh = (Shared *)mmap(0, sizeof(Shared),
                                PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (sh == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    close(fd);
    return sh;
}

int alocar(Shared *sh)
{
    for (int i = sh->next_free; i < MAX_NODES; i++)
    {
        if (sh->bitmap[i] == 0)
        {
            sh->bitmap[i] = 1;
            sh->next_free = i + 1;
            return i;
        }
    }
    return -1;
}

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
 void printaLista(struct s_no *pont)
{
    // printf("\nlista");
    //   contador
    if (c4 >= c3 + 1)
    {
        // TODO 1 milissegundo é mto grande na real, deve ter outra forma
        sleep(0.001);
    }
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
    // so pra ele nao reclamar
    c4++;
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
        c1++;
    }
}

 void removerPares(void *args)
{
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
        // contador
        if (c2 >= c1 + 1)
        {
            // TODO 1 milissegundo é mto grande na real, deve ter outra forma
            sleep(0.001);
        }
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
        c2++;
    }

    // se o no for achado

    // so pra ele nao reclamar
    return args;
}

static void *removerPrimos(void *args)
{
    // TODO nao sei se funciona
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
        // contador
        if (c3 >= c2 + 1)
        {
            // TODO 1 milissegundo é mto grande na real, deve ter outra forma
            sleep(0.001);
        }
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
        ++c3;
    }

    // so pra ele nao reclamar
    return args;
}

int main()
{
    // TODO usar contadores
    struct s_no *L = NULL;
    // TODO inserir com ponteiro pro ultimo
    struct s_no *FL = NULL;
    // L->prox = NULL;

    /*
    //teste
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
    // FIXME sera q precisa disso?
    numeros[nms] = '\0';

    // fflush(stdout);
    //  strtok

    // FIXME teoricamente tá certo, mas da um erro desgraçado
    //  free(texto);

    // thread principal
    inserirTotal(&L, numeros, nms);
    // thread principal

    // printaLista(L);

    pthread_t *thread_id[3] = {NULL, NULL, NULL};

    // TODO 1 milissegundo é mto grande na real, deve ter outra forma
    sleep(0.001);
    // 1 thread
    // removerPares(&L);
    removerPares();
    // printaLista(L);
    //  1 thread

    // TODO 1 milissegundo é mto grande na real, deve ter outra forma
    sleep(0.001);
    // 2 thread
    // removerPrimos(&L);
    pthread_create((&thread_id[1]), NULL, removerPrimos, &L);
    // printaLista(L);
    //  2 thread

    // TODO 1 milissegundo é mto grande na real, deve ter outra forma
    sleep(0.001);
    // 3 thread
    // printaLista(L);
    pthread_create((&thread_id[2]), NULL, printaLista, L);
    // 3 thread

    // aparentemente funciona até aqui

    // FIXME corrupted size vs. prev_size
    // causado pelo fclose de alguma forma???
    //

    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
    pthread_join(thread_id[2], NULL);
    // free(texto);
    fclose(arqv);
    return 0;
}