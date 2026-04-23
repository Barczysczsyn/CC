#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define true 1
#define false 0
#define MAX_LINHA 100
int main()
{
    char linha[MAX_LINHA];
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
        fgets(linha, MAX_LINHA, stdin);
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
            printf("\ncomandos[%d] %s", i, comandos[i]);

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

        if ((strcmp(comandos[0], "ls") == 0))
        {
            if (argcont == 1)
            {
                execlp("/bin/ls", "ls", NULL);
            }
            else if (strcmp(comandos[1], "-a") == 0)
            {
                execlp("/bin/ls", "ls", "-a", NULL);
            }
            else
            {
                //nunnes
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