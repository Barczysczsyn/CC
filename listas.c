//anteriormente chamado T1
//FIXME Remoção do último elemento lista circular resulta em impressão de nó com valor aleatório;
#include <stdio.h>
#include <stdlib.h>

// aluno: João Marcos Barczysczyn Vilhanueva

struct nos
{
    int dado;
    struct nos *prox;
};
typedef struct nos nos;

struct nosD
{
    int dado;
    struct nosD *prox;
    struct nosD *ant;
};
typedef struct nosD nosD;

// libera a memoria alocada com funcao recursiva
void liberar(struct nos **pont)
{

    if ((*pont) != NULL)
    {
        *pont = (*pont)->prox;
        liberar(pont);

        free(*pont);
        //*pont = (*pont)->prox;
    }
}
// libera a memoria alocada com funcao recursiva em listas duplamente encadeadas
void liberarDupla(struct nosD **pont)
{

    if ((*pont) != NULL)
    {
        *pont = (*pont)->prox;
        liberarDupla(pont);

        free(*pont);
    }
}
// libera a memoria alocada com funcao recursiva em listas circulares
void liberarCirc(struct nos **pont, struct nos *prim)
{
    if ((*pont) != prim)
    {
        *pont = (*pont)->prox;
        liberarCirc(pont, prim);

        free(*pont);
        //*pont = (*pont)->prox;
    }
}

void imprimir(struct nos *pont)
{
    if (pont != NULL)
    {
        printf("\n");
        printf("%i o-> ", pont->dado);
        pont = pont->prox;

        while (pont != NULL)
        {
            printf("%i o-> ", pont->dado);

            pont = pont->prox;
        }
        printf("\n");
    }
    else
    {
        printf("\nlista vazia\n");
    }
}

// imprimir listas circulares
void imprimirCirc(struct nos *pont)
{
    if (pont != NULL)
    {
        nos *primeiro;
        primeiro = pont;

        printf("\n");
        printf("%i o-> ", pont->dado);
        pont = pont->prox;

        while (pont != primeiro)
        {
            printf("%i o-> ", pont->dado);

            pont = pont->prox;
        }
        // mostra o primeiro valor pra ver se deu certo a circularidade
        printf("%i o-> ", pont->dado);
        printf("\n");
    }
    else
    {
        printf("\nlista vazia\n");
    }
}

void imprimirDupla(struct nosD *pont)
{
    printf("\n");

    while (pont != NULL)
    {
        printf("%i <-o-> ", pont->dado);
        pont = pont->prox;
    }
    printf("\n");
}

void buscarPos(struct nos *pont, int valor)
{
    int posicao = 0, enc = 0;
    while (pont != NULL)
    {
        if (valor == pont->dado)
        {

            printf("\nvalor %d encontrado na posicao %d \n", valor, posicao);
            enc = 1;
        }
        pont = pont->prox;
        posicao++;
    }
    if (enc == 0)
    {
        printf("\nvalor nao encontrado\n");
    }
}

void buscar(struct nos *pont, int valor)
{

    // se a lista for circular
    nos *primeiro;
    primeiro = malloc(sizeof(struct nos));
    primeiro = pont;

    int enc = 0;

    if (valor == pont->dado)
    {

        printf("\nvalor %d encontrado \n", valor);
        enc = 1;
    }
    pont = pont->prox;

    while (pont != NULL && pont != primeiro)
    {
        if (valor == pont->dado)
        {

            printf("\nvalor %d encontrado \n", valor);
            enc = 1;
        }
        pont = pont->prox;
    }
    if (enc == 0)
    {
        printf("\nvalor nao encontrado\n");
    }
}

void buscarDupla(struct nosD *pont, int valor)
{
    int enc = 0;
    while (pont != NULL)
    {
        if (valor == pont->dado)
        {

            printf("\nvalor %d encontrado \n", valor);
            enc = 1;
        }
        pont = pont->prox;
    }
    if (enc == 0)
    {
        printf("\nvalor nao encontrado\n");
    }
}

// insere em listas nao circulares
void inserirOrden(nos **pont, int x)
{
    //  encontrar o lugar certo
    nos *p1, *p2;
    p1 = *pont, p2 = *pont;
    int final = 0;

    // checa se esta vazia
    if (*pont == NULL)
    {
        // printf("\nvazia");
        nos *pN;
        pN = malloc(sizeof(struct nos));
        pN->dado = x;
        pN->prox = NULL;
        *pont = pN;
    }
    else if (p1->dado > x)
    {
        // checa se é o primeiro
        // printf("\nprimeiro");
        nos *pN;
        pN = malloc(sizeof(struct nos));
        pN->dado = x;
        pN->prox = p1;
        *pont = pN;
    }
    else
    {

        while (p1 != NULL)
        {
            if (p1->dado > x)
            {

                // printf("%d e maior que %d\n", p1->dado, x);
                final++;
            }
            else
            {
                p2 = p1; // p2 e um anterior
            }

            p1 = p1->prox;
        }

        // inserir no lugar
        nos *pN;
        pN = malloc(sizeof(struct nos));

        pN->dado = x;

        if (!final)
        {
            // esse negocio de final e pra inicializar com um ponteiro vazio se for o ultimo
            // talvez nao seja uma forma muito inteligente

            pN->prox = NULL;
        }
        else
        {

            pN->prox = p2->prox;
        }

        p2->prox = pN;
    }
}

// insere em listas circulares
void inserirOrdenC(nos **pont, int x)
{
    //  encontrar o lugar certo
    nos *p1, *p2, *prim;
    p1 = *pont, p2 = *pont, prim = *pont;
    int nfinal = 0;

    // checa se esta vazia
    if (*pont == NULL)
    {
        // printf("\nvazia");
        nos *pN;
        pN = malloc(sizeof(struct nos));
        pN->dado = x;
        pN->prox = pN; // sera
        *pont = pN;
    }
    else if (p1->dado > x)
    {

        // tem que ir ate o final e pegar o ultimo, pra poder apontar ele pro pN agora
        while (p1->prox != NULL && p1->prox != prim)
        {
            p1 = p1->prox;
        }

        // checa se é o primeiro
        nos *pN;
        pN = malloc(sizeof(struct nos));
        pN->dado = x;
        pN->prox = p2;
        *pont = pN;

        // aponta o ultimo pro primeiro
        p1->prox = pN;
    }
    else
    {

        p1 = p1->prox;

        while (p1 != NULL && p1 != prim)
        {
            if (p1->dado > x)
            {
                nfinal++;
            }
            else
            {
                p2 = p1; // p2 e um anterior
            }

            p1 = p1->prox;
        }

        // inserir no lugar
        nos *pN;
        pN = malloc(sizeof(struct nos));

        pN->dado = x;

        if (!nfinal)
        {
            // pra inicializar com um ponteiro pro comeco se for o ultimo

            pN->prox = *pont;
        }
        else
        {

            pN->prox = p2->prox;
        }

        p2->prox = pN;
    }
}

// insere na ultima posicao em listas duplas
void inserirDupla(nosD **pont, int x)
{
    //  encontrar o lugar certo
    nosD *p1, *p2;
    p1 = *pont, p2 = *pont;

    while (p1 != NULL)
    {
        p2 = p1; // p2 e um anterior

        p1 = p1->prox;
    }

    nosD *pN;
    pN = malloc(sizeof(struct nosD));

    pN->dado = x;
    pN->prox = NULL;
    pN->ant = p2;
    p2->prox = pN;
}

// insere na ultima posicao em pilhas
void inserirPilha(nos **pont, int x)
{
    nos *pN;
    pN = malloc(sizeof(struct nos));

    pN->prox = *pont;
    pN->dado = x;
    *pont = pN;
}

void remover(nos **pont, int x)
{
    // checa se esta vazia
    if (*pont != NULL)
    {
        //  encontrar o lugar certo
        nos *p1, *p2, *prim;
        p1 = *pont, p2 = *pont, prim = *pont;
        if (p1->dado == x)
        {

            // checa se é o primeiro
            *pont = p1->prox;
            free(p1);
        }
        else
        {

            p2 = p1; // p2 e um anterior
            p1 = p1->prox;

            while (p1 != NULL && p1 != prim && p1->dado != x)
            {
                // sla
                p2 = p1; // p2 e um anterior
                p1 = p1->prox;
            }

            if (p1 == NULL)
            {

                printf("\nvalor nao encontrado\n");
            }
            else if (p1->dado == x)
            {

                p2->prox = p1->prox;
                free(p1);
            }
        }
    }
    else
    {
        printf("\nlista vazia");
    }
}

void removerCirc(nos **pont, int x)
{
    // checa se esta vazia
    if (*pont != NULL)
    {
        //  encontrar o lugar certo
        nos *p1, *p2, *prim;
        p1 = *pont, p2 = *pont, prim = *pont;

        if (p1->dado == x)
        {
            // checa se é o primeiro

            // precisa fazer isso antes de mudar o pont
            while (p2->prox != *pont) //& o null?
            {

                // p2 e o ultimo
                p2 = p2->prox;
            }
            *pont = p1->prox;
            free(p1);

            p2->prox = *(pont);
        }
        else
        {
            p2 = p1; // p2 e um anterior
            p1 = p1->prox;

            // passa pela lista procurando o valor
            while (p1 != NULL && p1 != prim && p1->dado != x)
            {
                p2 = p1; // p2 e um anterior
                p1 = p1->prox;
            }

            // se nao encontrou
            if (p1 == NULL || p1 == prim)
            {

                printf("\nvalor nao encontrado\n");
            }
            // se encontrou
            else if (p1->dado == x)
            {

                p2->prox = p1->prox;
                free(p1);
            }
        }
    }
    else
    {
        printf("\nlista vazia");
    }
}

void removerDupla(nosD **pont, int x)
{
    if (*pont != NULL)
    {
        if ((*pont)->prox != NULL)
        {
            //  encontrar o lugar certo
            nosD *p1, *p2;
            p1 = *pont, p2 = *pont;
            // checa se é o primeiro

            p2 = p1; // p2 e um anterior
            p1 = p1->prox;

            // passa pela lista procurando o valor
            while (p1 != NULL && p1->dado != x)
            {
                p2 = p1; // p2 e um anterior
                p1 = p1->prox;
            }

            if (p1 == NULL)
            {

                printf("\nvalor nao encontrado\n");
            }

            else if (p1->dado == x)
            {

                // se encontrou
                p2->prox = p1->prox;
                free(p1);
            }
        }
        else
        {

            printf("\nno cabeca nao pode ser apagado");
        }
    }
    else
    {
        printf("\nlista vazia");
    }
}

// insere na ultima posicao em pilhas
void removerPilha(nos **pont)
{
    if (*pont != NULL)
    {
        *pont = (*pont)->prox;
    }
    else
    {
        printf("\npilha vazia");
    }
}

// insere na ultima posicao em pilhas
void removerFila(nos **pont)
{

    if (*pont != NULL)
    {
        nos *p1, *p2;
        p1 = (*pont);
        p2 = p1->prox;
        if (p2 == NULL)
        {
            *pont = NULL;
            free(p1);
        }
        else
        {
            // vai ate o ultimo elemento
            while (p2->prox != NULL)
            {
                p1 = p1->prox;
                p2 = p1->prox;
            }
            p1->prox = NULL;
            free(p2);
        }
    }
    else
    {
        printf("\nfila vazia");
    }
}

void lista1()
{
    system("clear");
    nos *pont;
    pont = NULL;

    int resp;

    do
    {
        printf("Selecione a operacao desejada: \n   1 - Busca \n   2 - Insercao \n   3 - Remocao. \n   4 - Voltar.\n");

        scanf("%d", &resp);
        int num;
        switch (resp)
        {
        case 1:
            printf("escreva o numero a ser procurado\n");
            scanf("%d", &num);
            buscar(pont, num);
            break;
        case 2:
            printf("escreva o numero a ser inserido\n");
            scanf("%d", &num);
            inserirOrden(&pont, num);
            imprimir(pont);
            break;
        case 3:
            printf("escreva o numero a ser removido\n");
            scanf("%d", &num);
            remover(&pont, num);
            imprimir(pont);
            break;

        case 4:
            liberar(&pont);
            system("clear");
            break;
        default:
            printf("\nentrada invalida \n");
            break;
        }
    } while (resp != 4);
}

void lista2()
{
    system("clear");
    // gerar lista
    nos *pont;
    pont = NULL;

    int resp;

    do
    {
        printf("Selecione a operacao desejada: \n   1 - Busca \n   2 - Insercao \n   3 - Remocao. \n   4 - Voltar.\n");

        scanf("%d", &resp);
        int num;
        switch (resp)
        {
        case 1:
            printf("escreva o numero a ser procurado\n");
            scanf("%d", &num);
            buscar(pont, num);
            break;
        case 2:
            printf("escreva o numero a ser inserido\n");
            scanf("%d", &num);
            inserirOrdenC(&pont, num);
            imprimirCirc(pont);
            break;
        case 3:
            printf("escreva o numero a ser removido\n");
            scanf("%d", &num);
            removerCirc(&pont, num);
            imprimirCirc(pont);
            break;

        case 4:
            liberarCirc(&pont, pont);
            system("clear");
            break;
        default:
            printf("\nentrada invalida \n");
            break;
        }
    } while (resp != 4);
}

void lista3()
{
    system("clear");
    // gerar lista
    nosD *pont;
    pont = malloc(sizeof(nosD));

    pont->prox = NULL;
    pont->ant = NULL;
    pont->dado = 0; // ou nulo

    int resp;

    do
    {
        printf("Selecione a operacao desejada: \n   1 - Busca \n   2 - Insercao \n   3 - Remocao. \n   4 - Voltar.\n");

        scanf("%d", &resp);
        int num;
        switch (resp)
        {
        case 1:
            printf("escreva o numero a ser procurado\n");
            scanf("%d", &num);
            buscarDupla(pont, num);
            break;
        case 2:
            printf("escreva o numero a ser inserido\n");
            scanf("%d", &num);
            inserirDupla(&pont, num);
            imprimirDupla(pont);
            break;
        case 3:
            printf("escreva o numero a ser removido\n");
            scanf("%d", &num);
            removerDupla(&pont, num);
            imprimirDupla(pont);
            break;

        case 4:
            liberarDupla(&pont);
            system("clear");
            break;
        default:
            printf("\nentrada invalida \n");
            break;
        }
    } while (resp != 4);
}

void lista4()
{
    system("clear");
    // gerar pilha
    nos *pont;
    pont = NULL;
    int resp;

    do
    {
        printf("Selecione a operacao desejada: \n   1 - Insercao \n   2 - Remocao. \n   3 - Voltar.\n");

        scanf("%d", &resp);
        int num;
        switch (resp)
        {
        case 1:
            printf("escreva o numero a ser inserido\n");
            scanf("%d", &num);
            inserirPilha(&pont, num);
            imprimir(pont);
            break;
        case 2:
            removerPilha(&pont);
            imprimir(pont);
            break;

        case 3:
            liberar(&pont);
            system("clear");
            break;
        default:
            printf("\nentrada invalida \n");
            break;
        }
    } while (resp != 3);
}

void lista5()
{
    system("clear");
    // gerar fila
    nos *pont;
    pont = NULL;
    int resp;

    do
    {
        printf("Selecione a operacao desejada: \n   1 - Insercao \n   2 - Remocao. \n   3 - Voltar.\n");

        scanf("%d", &resp);
        int num;
        switch (resp)
        {
        case 1:
            printf("escreva o numero a ser inserido\n");
            scanf("%d", &num);
            inserirPilha(&pont, num);
            imprimir(pont);
            break;
        case 2:
            removerFila(&pont);
            imprimir(pont);
            break;

        case 3:
            liberar(&pont);
            system("clear");
            break;
        default:
            printf("\nentrada invalida \n");
            break;
        }
    } while (resp != 3);
}

int main(void)
{

    int resp;

    do
    {
        printf("Selecione a lista desejada: \n   1 - Lista simplesmente encadeada sem no cabeca ordenada \n   2 - Lista circular simplesmente encadeada sem no cabeca ordenada \n   3 - Lista duplamente encadeada com no cabeca \n   4 - Fila (FIFO) \n   5 - Pilha (LIFO) \n   6 - Sair.\n");

        scanf("%d", &resp);
        switch (resp)
        {
        case 1:
            system("clear");
            lista1();
            break;

        case 2:
            system("clear");
            lista2();
            break;

        case 3:
            system("clear");
            lista3();
            break;

        case 4:
            system("clear");
            lista5();
            break;

        case 5:
            system("clear");
            lista4();
            break;

        case 6:
            system("clear");
            break;
        }
    } while (resp != 6);

    return 1;
}