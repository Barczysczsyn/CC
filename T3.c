#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{

    char entrada[50];

    int quantum;

    strcpy(entrada, argv[1]);
    quantum = argv[2];

    FILE *arqv = fopen(entrada, "r");

    if (strcmp(argv[3], "-seq"))
    {
    }
    else
    {
    }

    strcat(entrada, ".out");
    FILE *saida = fopen(entrada, "w");

    fclose(arqv);

    return 0;
}