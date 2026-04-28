#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

int main()
{

    DIR *pastaAlvo;
    struct dirent *de;
    pastaAlvo = opendir("memes");

    while ((de = readdir(pastaAlvo)) != NULL)
    {
        char comando[30] = "mv ";
        printf("\npasta %s\n",de->d_name);
        fflush(stdout);
        strcat(comando,"memes");
        strcat(comando," teste");
        //snprintf(comando,sizeof(comando),"md %s %s",de->d_name,"penisimenso");
        printf("\ncomando %s\n",comando);
        fflush(stdout);

        system(comando);
    }
    return 0;
}
