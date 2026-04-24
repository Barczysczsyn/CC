// algumas funcoes uteis para matrizes
#include <stdio.h>

void printaMatrizI(int tamx, int tamy, int matriz[tamx][tamy])
{
    // TODO so funciona com positivos
    for (int i = 0; i < tamx; i++)
    {
        for (int j = 0; j < tamy; j++)
        {
            if (matriz[i][j] < 9)
            {

                printf("[  %d  ]");
            }
            else if (matriz[i][j] < 99)
            {
                printf("[  %d ]");
            }
            else if (matriz[i][j] < 999)
            {
                printf("[ %d ]");
            }
            else if (matriz[i][j] < 9999)
            {
                printf("[ %d]");
            }
            else
            {

                printf("[%d]");
            }
        }
        printf("\n");
    }
}
void printaMatrizC(int tamx, int tamy, char matriz[tamx][tamy])
{
    // TODO so funciona com positivos
    for (int i = 0; i < tamx; i++)
    {
        for (int j = 0; j < tamy; j++)
        {
            if (strlen(matriz[i]) == 1)
            {

                printf("[  %s  ]");
            }
            else if (strlen(matriz[i]) == 2)
            {
                printf("[  %d ]");
            }
            else if (strlen(matriz[i]) == 3)
            {
                printf("[ %d ]");
            }
            else if (strlen(matriz[i]) == 4)
            {
                printf("[ %d]");
            }
            else
            {

                printf("[%d]");
            }
        }
        printf("\n");
    }
}

int main()
{
}