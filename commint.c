#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <ctype.h>

#define true 1
#define false 0
#define MAX_LINHA 100
#define MAX_COMANDOS 50

typedef struct
{
    int pid;
    char nome[256];
    char estado;
    int ppid;
} Proc;

void imprimirArvore(int atual, Proc *procs, int proccont, int rec)
{
    for (int i = 0; i < proccont; i++)
    {
        // o processo é filho desse
        if (procs[i].pid == atual)
        {
            for (int j = 0; j < rec; j++)
            {
                printf(" ");
            }
            if (rec > 0)
            {
                printf("|-");
            }

            char estado[25];

            switch (procs[i].estado){

                case 'R':
                strcpy(estado,"Running ou Runnable");
                break;
                case 'S':
                strcpy(estado,"Interruptible Sleep");
                break;
                case 'D':
                strcpy(estado,"Uninterruptible Sleep");
                break;
                case 'I':
                strcpy(estado,"Idle");
                break;
                case 'T':
                strcpy(estado,"Stopped or Traced");
                break;
                case 'X':
                strcpy(estado,"Dead");
                break;
                case 'Z':
                strcpy(estado,"Zombie");
                break;
            }

            printf("%d | %s | %s\n", procs[i].pid, procs[i].nome, estado);

            for (int k = 0; k < proccont; k++)
            {
                if (procs[k].ppid == atual)
                {
                    imprimirArvore(procs[k].pid, procs, proccont, rec + 1);
                }
            }
            return;
        }
    }
}

void acharFilhos(int pai)
{
    DIR *pastaAlvo;
    struct dirent *de;
    Proc *procs = malloc(sizeof(Proc) * 1000);
    int proccont = 0;

    if (procs == NULL)
    {
        perror("malloc do procs");
        return;
    }

    if ((pastaAlvo = opendir("/proc")) != NULL)
    {
        while ((de = readdir(pastaAlvo)) != NULL)
        {
            if (isdigit(de->d_name[0]))
            {
                char path[520];
                snprintf(path, sizeof(path), "/proc/%s/stat", de->d_name);

                FILE *arqv = fopen(path, "r");
                if (arqv)
                {
                    if (fscanf(arqv, "%d (%[^)]) %c %d",
                               &procs[proccont].pid,
                               procs[proccont].nome,
                               &procs[proccont].estado,
                               &procs[proccont].ppid) == 4)
                    {
                        proccont++;
                    }
                    fclose(arqv);
                }
            }
        }
        closedir(pastaAlvo);

        printf("\nArvore de processos de %d:\n", pai);

        printf("\nNumero de PID | Nome | Estado\n");

        imprimirArvore(pai, procs, proccont, 0);
    }
    else
    {
        perror("\nErro ao abrir proc");
    }

    free(procs);
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
            acharFilhos(atoi(comandos[1]));
        }
        else if (strcmp(comandos[0], "cd") == 0)
        {
            chdir(comandos[1]);
            // opendir(comandos[1]);
        }
        else

        {

            pid_t pid;
            // fork another process
            pid = fork();
            if (pid < 0)
            { // error occurred
                fprintf(stderr, "Fork falhou");
                exit(-1);
            }
            else if (pid == 0)
            { // child process
                if(execvp(comandos[0], comandos) == -1){
                    perror("\nErro ao executar programa");
                    exit(0);
                }
            }
            else
            { // parent process
                // parent will wait for the child to complete
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