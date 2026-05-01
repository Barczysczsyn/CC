#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main()
{
    char pastaAlvo[30] = "teste";
    DIR *arqv;
    struct dirent *de;
    arqv = opendir(pastaAlvo);
    char novoNome[20];

    while ((de = readdir(arqv)) != NULL)
    {
        // provavelmente vou ter q melhorar isso ne
        // vai ficar muito pesado
        if ((strcmp(de->d_name, ".") != 0) && (strcmp(de->d_name, "..") != 0))
        {
            char comando[50]; // = "mv teste/";
            // printf("\npasta %s\n", de->d_name);
            // fflush(stdout);
            // strcat(comando, de->d_name);
            // strcat(comando, " teste/novonome");
            snprintf(comando, sizeof(comando) - 1, "mv %s/%s %s/%s", pastaAlvo, de->d_name, pastaAlvo, novoNome);
            printf("\ncomando %s\n", comando);
            fflush(stdout);
            system(comando);
        }
    }
    return 0;
}
