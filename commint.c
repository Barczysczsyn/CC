#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>

#define true 1
#define false 0
#define MAX_LINHA 100
#define MAX_COMANDOS 50

void printaArvore(char caminho[50], int rec)
{
    FILE *arqv = fopen(caminho, "r");

    char argumentos[4][50];
    // o pai é o argumentos 3
    fscanf(arqv, "%d (%[^)]) %c %d",
           argumentos[0],
           argumentos[1],
           argumentos[2],
           argumentos[3]);

    // imprimir esse arquivo
    printf("\n");
    for (int i = 0; i < rec; i++)
        printf(" ");
    // resolvi colocar assim
    printf("|-%d %s %c %d", argumentos[0], argumentos[1], argumentos[2], argumentos[3]);
}
void acharFilhos(char pai[10], int rec)
{

    // abrir a pasta onde estao os arquivos
    DIR *pasta = opendir("/proc");
    if (pasta == NULL)
    {
        printf("Error: Não foi possivel abrir a pasta.");
    }
    struct dirent *de;

    // ABRIR UM ARQUIVO
    while ((de = readdir(pasta)) != NULL)
    {
        // de = readdir(pasta);
        // printf("\n arquivo '%s'", de->d_name);
        //  fflush(stdout);

        // checa se o nome da pasta é um numero,e se nao é o proprio pai
        if ((de->d_name[strspn(de->d_name, "0123456789")] == '\0') && strcmp(de->d_name, pai) != 0)
        {

            char caminho[50] = "/proc/";
            strcat(caminho, de->d_name);
            strcat(caminho, "/stat");

            FILE *arqv = fopen(caminho, "r");

            if (arqv == NULL)
            { // Verify the file opened successfully
                printf("\nErro, arquivo não encontrado.");
                fflush(stdout);
                return;
            }

            // printf("\nbuffer %s", buffer);
            // fflush(stdout);
            FILE *arqv = fopen(caminho, "r");

            char argumentos[4][50];
            // o pai é o argumentos 3
            fscanf(arqv, "%d (%[^)]) %c %d",
                   argumentos[0],
                   argumentos[1],
                   argumentos[2],
                   argumentos[3]);

            // tem q ser assim

            if (strcmp(argumentos[3], pai) == 0)
            {
                // printf("\n checar %s",de->d_name);
                //  tal arquivo é filho desse
                // chamar recursivamente para achar os filhos dele tbm
                acharFilhos(de->d_name, rec + 1);

                // TODO existem nomes de arquivos com espaços
            }

            // fechar o arquivo, agora que arqc nao esta mais sendo usado
            fclose(arqv);
        }
    }
    // ABRIR UM ARQUIVO

    // pega o pai dele
}
int main()
{

    printf("\nPara executar seus programas, digite './' antes de seu nome \nPara executar um programa no background, digite & separado no final do comando");
    char linha[MAX_LINHA];
    // mudar isso daqui fez funcionar aparentemente
    char *comandos[MAX_COMANDOS];

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
            break;
            // return 0;
            //  exit(0);
        }
        else if (strcmp(comandos[0], "cd") == 0)
        {
            chdir(comandos[1]);
            // opendir(comandos[1]);
        }
        else if (strcmp(comandos[0], "tree") == 0)
        {
            char pastaAlvo[50] = "/proc/";
            strcat(pastaAlvo, comandos[1]);
            strcat(pastaAlvo, "/stat");

            // printf("\npasta %s",pastaAlvo);

            FILE *fptr = fopen(pastaAlvo, "r"); // Open in "r" (read) mode
            char buffer[50];

            if (fptr == NULL)
            { // Verify the file opened successfully
                printf("\nErro, arquivo não encontrado.");
                return 1;
            }

            // Read until fgets returns NULL (end of file)
            fgets(buffer, 50, fptr);
            // printf("%s", buffer);
            acharFilhos(comandos[1], 1);
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
                execvp(comandos[0], comandos);
            }
            else
            { // parent process
                // parent will wait for the child to complete
                // TODO se certificar de que isso funciona
                if (back == 0)
                {
                    wait(NULL);
                }
                // printf("Child Complete");
                //  exit(0);
            }
        }
    }
    // chdir("..");
    //  corpo do ngc
    return 0;
}