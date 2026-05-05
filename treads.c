#include <stdio.h>
#include <pthread.h>

char string[51];
void inverte(int num)
{

    printf("\nporragames");
    fflush(stdout);
    int tam = sizeof(string);

    if (num == 1)
    {
        int i = 0, j = tam;
        while (i < tam / 8)
        {
            char aux = string[i];
            string[i] = string[j];
            string[j] = aux;
            ++i;
            --j;
        }
    }
    if (num == 2)
    {
        int i = tam / 8, j = tam - (tam / 8);
        while (i < tam / 8)
        {
            char aux = string[i];
            string[i] = string[j];
            string[j] = aux;
            ++i;
            --j;
        }
    }
    if (num == 3)
    {
        int i = tam / 4, j = tam - (tam / 4);
        while (i < 3 * (tam) / 8)
        {
            char aux = string[i];
            string[i] = string[j];
            string[j] = aux;
            ++i;
            --j;
        }
    }
    if (num == 4)
    {
        int i = 3 * (tam) / 8, j = (tam / 2) + (tam / 8);
        while (i < tam / 2)
        {
            char aux = string[i];
            string[i] = string[j];
            string[j] = aux;
            ++i;
            --j;
        }
    }
}

int main()
{

    FILE *in = fopen("in.txt", "r");
    int tam;
    pthread_t t1, t2, t3, t4;


    fgets(string, 50, in);


    pthread_create(t1, NULL, inverte, 1);
    pthread_create(t2, NULL, inverte, 2);
    pthread_create(t3, NULL, inverte, 3);
    pthread_create(t4, NULL, inverte, 4);
    

    fclose(in);
    FILE *out = fopen("out.txt", "w");
    fwrite(string, 51, 1, out);

    fclose(out);

    return 0;
}