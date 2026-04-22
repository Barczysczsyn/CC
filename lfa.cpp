#include <iostream>

class AFD
{
    char a[], q[];
    char qo[];
    char finais[];
    char Delta[][];
    int tamQ = 0, tamF = 0, tamA = 0;

    AFD() {};

};
 class AFN
{
    char a[], q[];
    char qo[];
    char finais[];
    char Delta[][];
    int tamQ = 0, tamF = 0, tamA = 0;

    AFN() {};

};

int main()
{
    AFD afd = new AFD();
    printf("\nescreva os estados (entre com - para parar)");
    do
    {
        scanf("%d", afd.q[afd.tamQ]);
        afd.tamQ++;
    } while (afd.q[afd.tamQ] != '-');
    afd.tamQ--;

    afd.tamF = 0;

    printf("\nescreva os estados de aceitacao (entre com - para parar)");
    do
    {
        scanf("%d", finais[afd.tamF]);
        afd.tamF++;
    } while (q[afd.tamF] != '-');
    afd.tamF--;

    afd.tamA = 0;

    printf("\nescreva o alfabeto");
    do
    {
        scanf("%d", afd.a[afd.tamA]);
        afd.tamA++;
    } while (afd.a[afd.tamA] != '-');
    afd.tamA--;

    for (int i = 0; i < afd.tamQ; i++)
    {
        for (int j = 0; j < afd.tamA; j++)
        {
            int erro;
            do
            {
                erro = 0;
                printf("\ndelta(q%c, %c) = ", afd.q[i], afd.a[j]);
                scanf(" %c*[^\n]", afd.Delta[i][j]);

                //fflush(stdin);

                if (afd.Delta[i][j] == 'x')
                    printf("\nElemento nao pertence ao conjunto de estados");

            } while (erro);
        }
    }
    return 0;
}