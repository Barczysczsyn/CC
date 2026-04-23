// algumas funcoes uteis para matrizes
#include <stdio.h>

void printaMatrizI(int tamx, int tamy, int matriz[tamx][tamy])
{
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
            }else{

                printf("[%d]");
            }
        }
    }
}

int main()
{
}