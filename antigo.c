#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRAVOU 'x'

//retorna qual e o proximo valor se esta no estado q e recebe o simbolo c
char delta(const char q, const char c, char mDelta[10][10], char *A, char *Q, int tamQ, int tamA)
{ 
    for (int i = 0; i < tamQ; i++)
    {
        if (q == Q[i])
        {
            for (int j = 0; j < tamA; j++)
            {
                if (c == A[j])
                {
                    return mDelta[i][j];
                }
            }
        }
    }
    return TRAVOU;
}

// verifica se c pertence a Str e retorna a posicao
int pertence(char c, char *Str)
{
    int i;

    for (i = 0; Str[i] != '\0'; i++)
        if (c == Str[i])
            return i;

    return -1; // retorna uma falha
}

// verifica se um conjunto pertence a outro conjunto
int pertConj(char *Q, char *F, int tamQ, int tamF)
{
    int i, resp;
    for (i = 0; i < tamF; i++)
    {
        resp = pertence(F[i], Q);
        if (resp == -1)
        {
            return -1;
        }
    }
    return 1;
}

//separa uma string com base em um caractere
void split(char *Q, char *str, char sep, int *tam)
{
    // SUGESTAO: PODERIA strtok com um laco
    int i, j = 0;
    for (i = 0; i < *tam; i++)
        if (str[i] != sep) // se nÃ£o encontrou o separador
            Q[j++] = str[i];
    *tam = j; // guarda o tamanho dos elementos
}

//  Funcao para a entrada de uma string
//o nome foi mudado
void getsT(char *str, int t, int *tamW)
{
    fgets(str, t, stdin); //   coloca \n e \0 no fim //stdin e entrada padrao (teclado)
    *tamW = strlen(str) - 1;
    str[*tamW] = '\0';
}

int main()
{
    //variaveis para pegar as respostas do usuario no final
    int resp1, resp2;
    //o loop do automato
    do
    {
        //variaveis para o armazenamento do automato
        char Q[10], A[10], qo, F[10], q, c, mDelta[10][10];
        //variavel para o conjunto de estados
        char str[30] = "";
        //variaveis para o tamanho dos conjuntos 
        int taux, tamQ, tamF, tamA;
        //variaveis para armazenar se houve falha no programa
        int Falha = 0, erro = 0;
        system("cls");

        printf("Entre com o conjunto de Estados, separados por virgulas: ");

        getsT(str, 30, &tamQ);
        fflush(stdin);
        split(Q, str, ',', &tamQ);

        printf("\nEntre com os simbolos do alfabeto, separados por virgulas: ");

        getsT(str, 30, &tamA);
        fflush(stdin);
        split(A, str, ',', &tamA);

        //do while de verificacao para que nao aceite valores incorretos
        do
        {
            erro = 0;
            printf("\nEntre com o estado inicial: ");
            scanf(" %c%*[^\n]", &qo);
            fflush(stdin);
            setbuf(stdin, NULL);
            //checa entre os estados disponiveis
            if (pertence(qo, Q) == -1)
            {
                erro = 1;
                printf("\nValor nao pertence aos estados disponiveis ");
            }
        } while (erro);

        //do while de verificacao para que nao aceite valores incorretos
        do
        {
            erro = 0;
            printf("\nEntre com os estados de F, separados por virgulas: ");
            getsT(str, 30, &tamF);
            fflush(stdin);
            setbuf(stdin, NULL);
            split(F, str, ',', &tamF);
            //checa entre os estados disponiveis
            if (pertConj(Q, F, tamQ, tamF) == -1)
            {
                erro = 1;
                printf("\nValor nao pertence aos estados disponiveis ");
            }
        } while (erro);

        // Leitura da matriz Delta
        //matriz que mostra o estado de destino para determinado estado e simbolo
        for (int i = 0; i < tamQ; i++)
            for (int j = 0; j < tamA; j++)
            {
                do
                {
                    erro = 0;
                    printf("\ndelta(q%c, %c) = ", Q[i], A[j]);
                    scanf(" %c*[^\n]", &mDelta[i][j]);

                    fflush(stdin);

                    if (mDelta[i][j] == 'x')
                        erro = 1;

                    if (erro == 1)
                    {
                        printf("\nElemento nao pertence ao conjunto de estados");
                    }

                } while (erro);
            }

        do
        {
            //p mostra onde se esta na palavra, precisa ser reiniciado com 0
            int p = 0;
            //a palavra a ser lida
            char palavra[20];
            //o tamanho da palavra
            int tampalavra;
            //falha recebe algum possivel erro no automato
            Falha = 0;
            printf("\nEntre com a palavra a ser verificada: ");
            scanf("%s", palavra);
            printf("\nPalavra : %s\n", palavra);
            //o primeiro estado e o inicial
            q = qo;
            c = palavra[p++]; 
            printf("\nSequencia de estados: ");
            printf("%c ", q);
            //le toda a palavra
            while (c != '\0')
            {

                q = delta(q, c, mDelta, A, Q, tamQ, tamA);

                if (q == TRAVOU)
                {
                    Falha = 1;
                    printf("\nO automato travou ");
                    break;
                }

                c = palavra[p++]; 
                printf(" >>> %c", q);
            }
            //checa se a palavra e reconhecida, se nao e, ou se houve falha no automato
            if (pertence(q, F) == -1 || Falha)
                printf("\nPalavra nao reconhecida ");
            else
                printf("\nPalavra reconhecida ");

            printf("\nDeseja escrever uma nova palavra? \n\t1 - Sim \n\t2 - Nao .\n");
            scanf("%d", &resp1);
            fflush(stdin);
        } while (resp1 == 1);
        printf("\nDeseja continuar o programa? \n\t1 - Sim \n\t2 - Nao .\n");
        scanf("%d", &resp2);
        fflush(stdin);
    } while (resp2 == 1);
}