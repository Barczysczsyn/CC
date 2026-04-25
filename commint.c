#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>

#define true 1
#define false 0
#define MAX_LINHA 100
int main()
{
    char linha[MAX_LINHA];
    // mudar isso daqui fez funcionar aparentemente
    char *comandos[50];

    /*
    pid_t pid;
   // fork another process
    pid = fork();
    if (pid < 0)
    { // error occurred
        fprintf(stderr, "Fork Failed");
        exit(-1);
    }
    else if (pid == 0)
    { // child process

    }
    else
    { // parent process
        // parent will wait for the child to complete
        wait(NULL);
        printf("Child Complete");
        // exit(0);
    }
    //*/

    // corpo do ngc

    while (true)
    {
        int argcont = 0, back = 0;

        // printing current working directory
        char caminho[50];
        printf("\n%s>", getcwd(caminho, 100));

        fgets(linha, MAX_LINHA, stdin);

        linha[strcspn(linha, "\n")] = '\0';

        char *token = strtok(linha, " \t");

        while (token != NULL)
        {
            comandos[argcont] = token;
            ++argcont;
            token = strtok(NULL, " \t");
        }
        comandos[argcont] = NULL;

        // printar os comandos
        /*
        for (int i = 0; i < argcont; i++)
            printf("\ncomandos[%d] %s", i, comandos[i]);
            //*/

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

        // exit vai ficar aqui fora mesmo

        if (strcmp(comandos[0], "exit") == 0)
        {
            // FIXME se vc dar exit fora da pasta inicial ele nao sai na hora
            break;
            return 0;
            // exit(0);
        }
        else
        {

            pid_t pid;
            // fork another process
            pid = fork();
            if (pid < 0)
            { // error occurred
                fprintf(stderr, "Fork Failed");
                exit(-1);
            }
            else if (pid == 0)
            { // child process

                // EXECUTAR O COMANDO
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
                        // nunnes
                        perror("o cara digitou uma coisa mto doida");
                    }
                }
                if (strcmp(comandos[0], "cd") == 0)
                {
                    chdir(comandos[1]);
                    // opendir(comandos[1]);
                }

                if (strcmp(comandos[0], "tree") == 0)
                {
                    // eu imagino q seja assim
                    char pastaAlvo[100] = "/proc/";
                    strcat(pastaAlvo, comandos[1]);
                    printf("%s", pastaAlvo);
                    DIR *pasta = opendir(pastaAlvo);
                    if (pasta == NULL)
                    {
                        printf("Error: Não foi possivel abrir a pasta.");
                    }
                    struct dirent *de;

                    while ((de = readdir(pasta)) != NULL)
                    {
                        // de = readdir(pasta);
                        printf("\n arquivo '%s'", de->d_name);
                        fflush(stdout);
                        // nesse ponto ele consegue ler todos os arquivos

                        // if (strcmp(de->d_name, "stat") == 0)
                        // {
                        // achou a pasta certa
                        // abrir a pasta

                        FILE *fptr = fopen("stat", "r"); // Open in "r" (read) mode
                        char buffer[500];

                        if (fptr == NULL)
                        { // Verify the file opened successfully
                            printf("\nError: Could not open file.");
                            return 1;
                        }

                        // Read until fgets returns NULL (end of file)
                        while (fgets(buffer, 500, fptr))
                        {
                            fgets(buffer, 500, fptr);
                            printf("%s", buffer);
                        }
                        //}
                    }
                }
                // EXECUTAR O COMANDO
            }
            else
            { // parent process
                // parent will wait for the child to complete
                wait(NULL);
                // printf("Child Complete");
                //  exit(0);
            }
        }
    }
    // chdir("..");
    //  corpo do ngc
}