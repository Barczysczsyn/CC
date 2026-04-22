// ctrl+shift + I
#include <stdio.h>
#include <iostream>
#include <cstring>

#define MAX 50
#define MAX_PALAVRA 6

using namespace std;

class MT
{
public:
    // alfabeto /// estados
    char a[MAX], q[MAX][MAX_PALAVRA];
    char qo[MAX_PALAVRA];
    char mDelta[MAX][MAX][3][MAX_PALAVRA];
    int tamQ = 0, tamF = 0, tamA = 0;

    MT() {};

    // i e os q
    // j e os A
    // delta tem 3 valores em string
    // 0 e o destino
    // 1 e L ou R
    // 2 e o valor q deixa
    // FIXME so um deles e string, fica meiio chola
    char *delta(char *estado, char simbolo, int index)
    {
        int i, j;

        for (i = 0; i < tamQ; i++)
        {

            if (strcmp(q[i], estado) == 0)
            {

                for (j = 0; j < tamA; j++)
                {
                    if (a[j] == simbolo)
                    {

                        // cout << "\nretornando " << mDelta[i][j][index];
                        // fflush(stdout);
                        return mDelta[i][j][index];
                    }
                }
            }
        }
        // erro
        cout << "\nerro, valor nao encontrado\n"
             << "no estado " << estado << " com o simbolo " << simbolo << " e indice " << index;
        fflush(stdout);
        // retornar null causara segfault
        return NULL;
    }
};

int main()
{
    do
    {

        MT mt;
        printf("\nescreva os estados, comecando com o inicial, exceto qa e qr (entre com - para parar)");
        fflush(stdout);
        fflush(stdin);
        do
        {
            scanf("%s", mt.q[mt.tamQ]);
            mt.tamQ++;
            fflush(stdin);
        } while (strcmp(mt.q[mt.tamQ - 1], "-"));
        // para apagar o -
        mt.tamQ--;
        strcpy(mt.q[mt.tamQ], "qa");
        mt.tamQ++;
        strcpy(mt.q[mt.tamQ], "qr");
        mt.tamQ++;

        // qinicial e atribuido para o primeiro estado
        strcpy(mt.qo, mt.q[0]);

        // for (int i = 0; i < mt.tamQ; i++)
        // {
        //     cout << '\n'
        //          << mt.q[i];
        // }

        mt.tamF = 0;
        /*
            printf("\nescreva os estados de aceitacao (entre com - para parar)");
            do
            {
                scanf("%s", mt.finais[mt.tamF]);
                mt.tamF++;
            } while (strcmp(mt.q[mt.tamF-1], "-"));
            mt.tamF--;

            mt.tamA = 0;
        */

        // mt.a[mt.tamA] = '*';
        // mt.tamA++;
        printf("\nescreva o alfabeto (entre com - para parar)");
        do
        {
            // nao achei jeito mais facil de fazer
            char sss[MAX_PALAVRA];
            scanf("%s", sss);
            mt.a[mt.tamA] = sss[0];
            mt.tamA++;
        } while (mt.a[mt.tamA - 1] != '-');
        mt.tamA--;
        // adiciona o vazio e o *
        mt.a[mt.tamA] = '-';
        mt.tamA++;

        // cout << "\n"
        //      << mt.tamA;

        // printf("\n a %s", mt.a);
        /*
        printf("\nescreva o ngc q deixa sla kkkkkk (entre com - para parar)");
        do
        {
            scanf("%c", mt.a[mt.tamA]);
            mt.tamA++;
        } while (mt.a[mt.tamA] != '-');
        mt.tamA--;

        */

        printf("\nescreva a matriz delta");
        // TODO verificacoes
        //-2 pra nao pegar qa e qr
        for (int i = 0; i < mt.tamQ - 2; i++)
        {
            for (int j = 0; j < mt.tamA; j++)
            {
                int erro;
                do
                {
                    erro = 0;
                    printf("\nescreva o destino para %s | %c (qa para aceitar, qr para rejeitar):", mt.q[i], mt.a[j]);

                    scanf("%s", mt.mDelta[i][j][0]);
                    // passa por todos os estados para verr se o inserido existe
                    for (int k = 0; k < mt.tamQ; k++)
                        if (strcmp(mt.q[k], mt.mDelta[i][j][0]) == 0)
                        {
                            erro++;
                        }

                    if (!erro)
                        cout << "\nestado nao esta entre o conjunto de estados";

                } while (erro == 0);

                erro = 0;
                do
                {
                    // FIXME so pode ser caractere
                    printf("\nescreva o valor a ser gravado para %s | %c (- para vazio):", mt.q[i], mt.a[j]);

                    scanf("%s", mt.mDelta[i][j][2]);

                    if (!strchr(mt.a, mt.mDelta[i][j][2][0]))
                        cout << "\ncaractere nao esta entre o alfabeto ";

                } while (!strchr(mt.a, mt.mDelta[i][j][2][0]));

                do
                {
                    printf("\nescreva se vai para a esquerda ou direita (L ou R) para %s | %c :", mt.q[i], mt.a[j]);

                    scanf("%s", mt.mDelta[i][j][1]);
                    // FIXME so pode ser caractere
                    // muda so a primeira letra para minusculo
                    mt.mDelta[i][j][1][0] = tolower(mt.mDelta[i][j][1][0]);

                    if (mt.mDelta[i][j][1][0] != 'l' && mt.mDelta[i][j][1][0] != 'r')
                        cout << "\nprecisa ser R ou L";
                } while (mt.mDelta[i][j][1][0] != 'l' && mt.mDelta[i][j][1][0] != 'r');
            }
        }

        // printa  a matriz
        for (int i = 0; i < mt.tamQ - 2; i++)
        {
            cout << "\n";
            for (int j = 0; j < mt.tamA; j++)
            {
                cout << "|";
                for (int k = 0; k < 3; k++)
                {
                    cout << mt.mDelta[i][j][k] << " ";
                }
            }
        }
        cout << "\n";

        do
        {
            char string[MAX];
            char str[MAX];
            string[0] = '-';
            string[1] = '\0';
            printf("\nescreva a string a ser rodada");
            fflush(stdout);
            scanf("%s", str);
            fflush(stdin);
            strcat(string, str);
            strcat(string, "-");

            // cout << "\nstring " << string;

            int marcador = 1;
            char qatual[MAX_PALAVRA];
            strcpy(qatual, mt.qo);

            do
            {
                // EXECUCAO

                // m e marcador temporario
                int m = marcador;
                // simbolo e o valor marcado, a ser substituido
                char simbolo = string[m];

                // cout << "\nmandando simbolo " << simbolo;

                // printar
                cout << "\n";
                for (unsigned i = 0; i < strlen(string); i++)
                {
                    if (i != marcador)
                    {

                        cout << string[i];
                    }
                    else
                    {
                        cout << '(' << qatual << ')';
                    }
                }
                // cout << "\n";
                fflush(stdout);
                // printar

                // anda com o marcador
                if (mt.delta(qatual, string[m], 1)[0] == 'r')
                {
                    marcador++;
                }
                else if (mt.delta(qatual, string[m], 1)[0] == 'l')
                {
                    marcador--;

                    // teste de erro
                    if (marcador < 0)
                    {
                        cout << "\nerro no marcador";
                        break;
                    }
                }
                else
                {
                    // erro
                    cout << "\ninstrucao:" << mt.delta(qatual, string[m], 1)[0];
                    printf("\nexecucao terminada, erro");
                    break;
                }
                // anda com o marcador

                // substitui o valor marcado na string
                if (mt.delta(qatual, simbolo, 2)[0] != '-')
                {
                    string[m] = mt.delta(qatual, simbolo, 2)[0];
                }

                fflush(stdout);
                // vai para o destino
                strcpy(qatual, mt.delta(qatual, simbolo, 0));

                // cout << "\n o erro deve ser isso daqui " << mt.delta(qatual, string[m], 2)[0];

                // cout << "\nnova string " << string;
                // fflush(stdout);

                if (strcmp(qatual, "qa") == 0)
                {
                    printf("\nexecucao terminada, palavra aceita");
                    break;
                }
                if (strcmp(qatual, "qr") == 0)
                {
                    printf("\nexecucao terminada, palavra rejeitada");
                    break;
                }

            } while (true);
            fflush(stdin);

            cout << "\nstring final: " << string;
            int resp = 0;

            printf("\nDeseja testar outra string? \n 1 - sim \n 2 - nao");
            fflush(stdout);
            fflush(stdin);
            scanf("%d ", &resp);

            if (resp == 2)
            {
                break;
            }

        } while (true);

        int resp2 = 0;

        printf("\nDeseja testar outra máquina de turing? \n 1 - sim \n 2 - nao");
        fflush(stdin);
        scanf("%d ", &resp2);

        if (resp2 == 2)
        {
            break;
        }
    } while (true);
    return 0;
}