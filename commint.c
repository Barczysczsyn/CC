#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define true 1
#define false 0

char *ler(int m, char *linha, char *palavra)
{

    // ler palavra
    while (linha[m] != ' ')
    {
        palavra[m] = linha[m];
        m++;
    }

    return palavra;
}

void leTudo(char *linha, char ***comandos)
{
    // char **comandos[10][30];

    int m = 0, f = 0, n = 0;

    // ler palavra
    while (linha[m] != '\0')
    {
        if (linha[m] != ' ')
        {
            *(comandos[n][f]) = linha[m];
            printf("\n%c", *(comandos[n][f]));
            printf("penis");
            fflush(stdout);
            m++;
            f++;
        }
        else
        {
            m++;
            n++;
            f = 0;
        }
    }

    return;
}
int main()
{
    char linha[12];
    char **comandos;
    int argcont = 0;
    int back;
    pid_t pid;
    /* fork another process */
    pid = fork();
    if (pid < 0)
    { /* error occurred */
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    { /* child process */

        execlp("/bin/ls", "ls", "-l", NULL);
    }
    else
    { /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);
        printf("Child Complete");
        // exit(0);
    }

    // corpo do ngc

    while (true)
    {
        // LER O COMANDO
        fgets(linha, 12, stdin);
        // leTudo(linha, &comandos);

        linha[strcspn(linha, "\n")] = '\0';
        char *token = strtok(linha, " \t");

        printf("linha: %s", linha);
        fflush(stdout);

        while (token != NULL)
        {
            comandos[argcont] = token;
            argcont++;
            token = strtok(NULL, " \t");
        }
        comandos[argcont] = NULL;

        // printar os comandos
        for (int i = 0; i < argcont; i++)
            printf("comandos[%d] %s\n", i, comandos[i]);

        fflush(stdout);
        // printar os comandos

        // mudei de string pra char
        if (argcont > 0 && comandos[argcont - 1][0] == '&')
        {
            back = 1;
            comandos[argcont - 1] = NULL;
            argcont--;
        }

        // LER O COMANDO

        if (strcmp(comandos[0], "ls") == 0)
        {

            if (strcmp(comandos[1], "-a") == 0)
            {
                execlp("/bin/ls", "ls", "-a", NULL);
            }
            else
            {
                execlp("/bin/ls", "ls", NULL);
            }
        }
        if (strcmp(comandos[0], "cd") == 0)
        {

            /// execlp("/bin/ls", "ls", NULL);
            chdir(comandos[1]);
        }
    }
    // corpo do ngc
}