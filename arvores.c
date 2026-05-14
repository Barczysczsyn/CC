#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct s_arq_no
{
    int32_t chave : 30;
    uint32_t esq : 1;
    uint32_t dir : 1;
};

struct s_no
{
    int chave;
    struct s_no *esq;
    struct s_no *dir;
};

typedef struct s_no Arv;

// A função auxiliar imprimeNo imprime o caracter
// c precedido de 3b espaços e seguido de uma mudança
// de linha.
void imprimeNo(int c, int b, int null)
{
    for (int i = 0; i < b; i++)
    {
        printf("   ");
    }
    if (null == 1)
    {
        printf("*\n");
    }
    else
    {
        printf("%d\n", c);
    }
}

// A função mostraArvore faz um desenho esquerda-direita-raiz
// da árvore x. O desenho terá uma margem esquerda de
// 3b espaços.
void mostraArvore(Arv *a, int b)
{
    if (a == NULL)
    {
        imprimeNo('*', b, 1);
        return;
    }
    mostraArvore(a->dir, b + 1);

    imprimeNo(a->chave, b, 0);

    mostraArvore(a->esq, b + 1);
}

void mostraPre(Arv *a, int b)
{
    if (a == NULL)
    {
        imprimeNo('*', b, 1);
        return;
    }
    imprimeNo(a->chave, b, 0);

    mostraPre(a->dir, b + 1);
    mostraPre(a->esq, b + 1);
}
void mostraPos(Arv *a, int b)
{
    if (a == NULL)
    {
        imprimeNo('*', b, 1);
        return;
    }
    mostraPos(a->dir, b + 1);
    mostraPos(a->esq, b + 1);

    imprimeNo(a->chave, b, 0);
}
void destruir(Arv *pt)
{
    if (pt != NULL)
    {

        if (pt->dir != NULL)
        {
            destruir(pt->dir);
            // free(pt->dir);
        }
        if (pt->esq != NULL)
        {
            destruir(pt->esq);
            // free(pt->esq);
        }
        free(pt);
    }
    else
    {
        printf("\nArvore vazia\n");
    }
}
void minimo(Arv *pont)
{
    if (pont == NULL)
    {

        printf("\nArvore vazia\n");
    }
    else
    {

        while (pont->esq != NULL)
        {
            pont = pont->esq;
        }
        printf("\nminimo %d\n", pont->chave);
    }
}
void maximo(Arv *pont)
{
    if (pont == NULL)
    {

        printf("\nArvore vazia\n");
    }
    else
    {

        while (pont->dir != NULL)
        {
            pont = pont->dir;
        }
        printf("\nmaximo %d\n", pont->chave);
    }
}

void gravar(Arv *pont, FILE *stream)
{
    size_t tam = sizeof(struct s_arq_no);

    // converte s_no para s_arq_no
    struct s_arq_no *arq;
    arq = malloc(sizeof(struct s_arq_no));

    arq->chave = pont->chave;
    if (pont->esq != NULL)
    {
        arq->esq = 1;
    }
    if (pont->dir != NULL)
    {
        arq->dir = 1;
    }

    // e assim ou tem que escrever item por item?
    fwrite(arq, tam, 1, stream);
    if (pont->esq != NULL)
    {
        gravar(pont->esq, stream);
    }
    if (pont->dir != NULL)
    {
        gravar(pont->dir, stream);
    }
}
Arv *reconstruir(FILE *stream)
{
    //  faz primeiro so com a raiz pra guardar ela
    size_t tam = sizeof(struct s_arq_no);
    struct s_arq_no *ra;
    ra = malloc(tam);

    //  faz primeiro so com a raiz pra guardar ela
    size_t tam2 = sizeof(Arv);
    Arv *raiz;
    raiz = malloc(tam2);

    fread(ra, tam, 1, stream); // teoricamente e assim

    // coloca a do arquivo na real
    raiz->chave = ra->chave;

    if (ra->esq == 1)
    {

        raiz->esq = reconstruir(stream);
    }
    else
    {
        raiz->esq = NULL;
    }

    if (ra->dir == 1)
    {

        raiz->dir = reconstruir(stream);
    }
    else
    {
        raiz->dir = NULL;
    }
    return raiz;
}

void predecessor(Arv *raiz, int x)
{
    if (raiz == NULL)
    {
        printf("\nArvore vazia\n");
        return;
    }
    else
    {
        Arv *pont, *pAnt;
        pont = raiz;
        pAnt = NULL;

        // encontrar o no
        while (pont != NULL && pont->chave != x)
        {

            if (pont->chave > x)
            {
                pont = pont->esq;
            }
            if (pont->chave < x)
            {
                pAnt = pont;
                pont = pont->dir;
            }
        }

        if (pont->esq != NULL)
        {
            // possui um a esquerda
            Arv *p2;
            p2 = pont->esq;

            // vai ate o ultimo da direita
            while (p2->dir != NULL)
            {
                p2 = p2->dir;
            }

            printf("\nO predecessor e: %d", p2->chave);
        }
        else
        {
            // o sucessor e o anterior
            if (pAnt != NULL)
            {
                printf("\nO predecessor e: %d", pAnt->chave);
            }
            else
            {

                printf("\nO predecessor nao existe");
            }
        }
    }
}
void sucessor(Arv *raiz, int x)
{
    if (raiz == NULL)
    {
        printf("\nArvore vazia\n");
        return;
    }
    else
    {
        Arv *pont, *pAnt;
        pont = raiz;
        pAnt = NULL;
        // int esq;

        // encontrar o no
        while (pont != NULL && pont->chave != x)
        {

            if (pont->chave > x)
            {
                pAnt = pont;
                pont = pont->esq;
            }
            if (pont->chave < x)
            {
                pont = pont->dir;
            }
        }

        if (pont->dir != NULL)
        {
            // possui um a direita
            Arv *p2;
            p2 = pont->dir;

            // vai ate o ultimo da esquerda
            while (p2->esq != NULL)
            {
                p2 = p2->esq;
            }

            printf("\nO sucessor e: %d", p2->chave);
        }
        else
        {
            // o sucessor e o anterior
            if (pAnt != NULL)
            {
                printf("\nO sucessor e: %d", pAnt->chave);
            }
            else
            {

                printf("\nO sucessor nao existe");
            }
        }
    }
}

void inserirOrden(Arv **pont, int x)
{
    Arv *p1;
    p1 = *pont;
    // checa se esta vazia
    if (*pont == NULL)
    {
        // printf("\nArvore vazia\n");
        Arv *pN;
        pN = malloc(sizeof(Arv));
        pN->chave = x;
        pN->dir = NULL;
        pN->esq = NULL;
        *pont = pN;
    }
    else
    {

        while (p1->dir != NULL || p1->esq != NULL)
        {
            if (p1->chave >= x && p1->esq != NULL)
            {
                p1 = p1->esq;
            }
            else if (p1->chave < x && p1->dir != NULL)
            {
                p1 = p1->dir;
            }
            else
            {
                break;
            }
        }

        Arv *pN;
        pN = malloc(sizeof(Arv));
        pN->chave = x;
        pN->dir = NULL;
        pN->esq = NULL;

        if (p1->chave >= x)
        {
            p1->esq = pN;
        }
        else
        {
            p1->dir = pN;
        }
    }
}

void buscar(Arv *raiz, int x)
{
    if (raiz == NULL)
    {
        printf("\nArvore vazia\n");
    }
    else
    {
        Arv *pont = raiz;

        // encontrar o no
        while (pont != NULL && pont->chave != x)
        {
            if (pont->chave > x)
            {
                pont = pont->esq;
            }
            if (pont->chave < x)
            {
                pont = pont->dir;
            }
        }

        if (pont == NULL)
        {

            printf("\nvalor nao encontrado\n");
        }
        else if (pont->chave == x)
        {

            // encontrou
            printf("\nvalor %d encontrado\n", x);
        }
    }
}

int remover(Arv **raiz, int x)
{
    if (*raiz == NULL)
    {
        printf("\nArvore vazia\n");
        return 0;
    }
    // se for o primeiro
    else if ((*raiz)->chave == x)
    {
        // infelizmente o jeito mais facil e repetir todo o codigo mesmo

        // nao tem filhos
        if ((*raiz)->esq == NULL && (*raiz)->dir == NULL)
        {
            *raiz = NULL;
            return 1;
        }
        // tem so um filho
        else if ((*raiz)->esq != NULL && (*raiz)->dir == NULL)
        {
            Arv *p2 = (*raiz)->esq;
            free(*raiz);
            *raiz = p2;
            return 1;
        }
        else if ((*raiz)->dir != NULL && (*raiz)->esq == NULL)
        {
            Arv *p2 = (*raiz)->dir;
            free(*raiz);
            *raiz = p2;
            return 1;
        }
        else
        // tem 2 filhos
        {
            Arv *suc = (*raiz)->dir;
            if (suc->esq == NULL)
            // o sucessor e direto
            {
                suc->esq = (*raiz)->esq;
                free(*raiz);
                *raiz = suc;
                return 1;
            }
            else
            {
                // o sucessor nao e direto
                // encontra o sucessor verdadeiro
                while (suc->esq != NULL)
                {
                    suc = suc->esq;
                }

                Arv *aesq, *adir, *sucdir;

                // guarda as arvores esquerda e direita restantes
                aesq = (*raiz)->esq;
                adir = (*raiz)->dir;
                sucdir = suc->dir;

                // printf("\n aesq %d", aesq->chave);
                // printf("\n adir %d", adir->chave);
                // printf("\n sucdir %d", sucdir->chave);

                // faz as trocas
                suc->dir = adir;
                adir->esq = sucdir;
                suc->esq = aesq;
                // remove o pont

                free(*raiz);

                *raiz = suc;
                return 1;
            }
        }
    }
    // nao e o primeiro
    else
    {
        Arv *p1, *pont;
        pont = *raiz;
        p1 = pont;

        int prox = 0;
        // 1 pra esquerda, 2 pra direita

        // encontrar o no
        while (pont != NULL && pont->chave != x)
        {
            // p1 e um atras
            p1 = pont;
            if (pont->chave > x)
            {
                prox = 1;
                pont = pont->esq;
            }
            if (pont->chave < x)
            {
                prox = 2;
                pont = pont->dir;
            }
        }

        if (pont == NULL || pont->chave != x)
        {
            printf("\nnumero nao encontrado\n");
            return 0;
        }

        // nao tem filhos
        if (pont->esq == NULL && pont->dir == NULL)
        {
            if (prox == 1)
            {
                p1->esq = NULL;
            }
            if (prox == 2)
            {
                p1->dir = NULL;
            }
            else
            {
                // erro
            }
            free(pont);
            return 1;
        }
        // tem so um filho
        else if (pont->esq != NULL && pont->dir == NULL)
        {
            if (prox == 1)
            {
                p1->esq = pont->esq;
            }
            if (prox == 2)
            {
                p1->dir = pont->esq;
            }
            else
            {
                // erro
            }
            free(pont);
            return 1;
        }
        else if (pont->dir != NULL && pont->esq == NULL)
        {
            if (prox == 1)
            {
                p1->esq = pont->dir;
            }
            if (prox == 2)
            {
                p1->dir = pont->dir;
            }
            else
            {
                // erro
            }
            free(pont);
            return 1;
        }
        else
        // tem 2 filhos
        {
            Arv *suc = pont->dir;
            if (suc->esq == NULL)
            // o sucessor e direto
            {
                // muda o proximo do anterior
                if (prox == 1)
                {
                    p1->esq = suc;
                }
                if (prox == 2)
                {
                    p1->dir = suc;
                }
                suc->esq = pont->esq;
                free(pont);
                return 1;
            }
            else
            {

                // o sucessor nao e direto
                // encontra o sucessor verdadeiro
                while (suc->esq != NULL)
                {
                    suc = suc->esq;
                }

                Arv *aesq, *adir, *sucdir;

                // guarda as arvores esquerda e direita restantes
                aesq = pont->esq;
                adir = pont->dir;
                sucdir = suc->dir;

                // faz as trocas
                suc->dir = adir;
                adir->esq = sucdir;
                suc->esq = aesq;

                // muda o proximo do anterior
                if (prox == 1)
                {
                    p1->esq = suc;
                }
                if (prox == 2)
                {
                    p1->dir = suc;
                }

                // remove o pont

                free(pont);
                return 1;
            }
        }
    }
}

int main()
{
    int resp;

    Arv *raiz;
    raiz = NULL;

    char nome[30];

    do
    {
        printf("\nSelecione a operacao desejada: \n\t1 - Busca \n\t2 - Insercao \n\t3 - Remocao. \n\t4 - Minimo.\n\t5 - Maximo.\n\t6 - Imprimir.\n\t7 - Gravar em arquivo binario.\n\t8 - Predecessor.\n\t9 - Sucessor.\n\t10 - Ler arvore em arquivo binario.\n\t11 - Sair.\n");

        scanf("%d", &resp);
        int num;
        switch (resp)
        {
        case 1:
            printf("\nescreva o numero a ser procurado\n");
            scanf("%d", &num);
            buscar(raiz, num);
            break;
        case 2:
            printf("\nescreva o numero a ser inserido\n");
            scanf("%d", &num);
            inserirOrden(&raiz, num);
            mostraArvore(raiz, 1);
            break;
        case 3:
            printf("\nescreva o numero a ser removido\n");
            scanf("%d", &num);
            remover(&raiz, num);
            mostraArvore(raiz, 1);
            break;

        case 4:
            // printf("escreva o numero a ser removido\n");
            // scanf("%d", &num);
            minimo(raiz);
            mostraArvore(raiz, 1);
            break;
        case 5:
            // printf("escreva o numero a ser removido\n");
            // scanf("%d", &num);
            maximo(raiz);
            mostraArvore(raiz, 1);
            break;

        case 6:
            // liberar(&pont);
            printf("\nDe qual forma deseja imprimir? \n\t1 - Ordem simetrica \n\t2 - Pre-ordem \n\t3 - Pos-ordem  ");
            int respo2;

            scanf("%d", &respo2);

            switch (respo2)
            {
            case 1:
                mostraArvore(raiz, 1);
                break;
            case 2:
                mostraPre(raiz, 1);
                break;
            case 3:
                mostraPos(raiz, 1);
                break;
            default:

                printf("\nentrada invalida \n");
                break;
            }

            break;

        case 7:
            // nome personalizado, tem q ser global
            printf("\nQual sera o nome do arquivo?");
            scanf("%s", nome);
            strcat(nome, ".bin");

            FILE *arquivo = fopen(nome, "w");
            gravar(raiz, arquivo);

            fclose(arquivo);
            break;

        case 8:
            printf("\ninsira o numero para procurar o predecessor: ");
            scanf("%d", &num);
            fflush(stdin);
            predecessor(raiz, num);
            break;
        case 9:
            printf("\ninsira o numero para procurar o sucessor: ");
            scanf("%d", &num);
            fflush(stdin);
            sucessor(raiz, num);
            break;
        case 10:
            printf("\nQual e o nome do arquivo?");
            printf("\n(certifique-se de que o arquivo existe)\n");
            char str[50];
            scanf("%s", str);
            FILE *arquivo2 = fopen(str, "r");
            // destruir a arvore atual
            destruir(raiz);

            // reconstroi a raiz e passa pra atual
            raiz = reconstruir(arquivo2);
            fclose(arquivo2);
            // system("clear");
            mostraArvore(raiz, 1);
            break;
        case 11:
            // liberar(&pont);
            destruir(raiz);
            // free(raiz);
            system("clear");
            break;
        default:
            printf("\nentrada invalida \n");
            break;
        }
    } while (resp != 11);

    return 0;
}