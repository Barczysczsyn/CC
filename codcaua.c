#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Biblioteca necessaria para a função chdir()

#define MAX_LINE 200
#define MAX_ARGS 200

int main()
{
    char linha[MAX_LINE];
    char *args[MAX_ARGS]; // em args[0] normalmente estara o cd ou ja o nome do arquivo
                          // se for cd em args[0], em args[1] estara o diretorio
                          // se for ja o nome do arquivo

    printf("Digite o & separado do nome do programa por um espaço \" exemplo: helloworld & \" \n");

    // loop infinito do interpretador (REPL)
    while (1)
    {
        printf("cmd> ");
        fflush(stdout); // o prompt e impresso na tela imediatamente, diferente do printf

        // 1. LER A STRING
        // le a entrada do usuario. Se falhar (ex: Ctrl+D), sai do loop, enter nao sai do loop.
        if (fgets(linha, MAX_LINE, stdin) == NULL)
        {
            printf("\n");
            break;
        }

        // o fgets adiciona um '\n' (quebra de linha) no final da string.
        // precisa remover isso e substituir por um terminador nulo '\0'.
        linha[strcspn(linha, "\n")] = '\0';

        // se o usuário apenas apertou "Enter" (linha vazia), apenas recomeça o loop.
        if (strlen(linha) == 0)
        {
            continue;
        }

        // 2. DIVIDIR A STRING
        int i = 0;
        int background = 0;

        // a função strtok quebra a string toda vez que encontra um espaço ou tab (" \t")
        char *token = strtok(linha, " \t");

        while (token != NULL)
        {
            args[i] = token;
            i++;
            token = strtok(NULL, " \t"); // pega o proximo pedaço, parametro NULL faz continuar do ulitmo lugar achado
        }
        args[i] = NULL; // e necessario que o array termine em NULL para a futura função execlp()

        // verifica se o ultimo argumento digitado foi o '&'
        if (i > 0 && strcmp(args[i - 1], "&") == 0)
        {
            background = 1;     // marca a flag indicando que não devemos esperar o filho
            args[i - 1] = NULL; // remove o '&' do array de argumentos (o exec() não precisa dele)
            i--;                // ajusta a contagem de argumentos
        }

        // 3. DECIDIR O QUE FAZER

        // verifica se e exit
        if (strcmp(args[0], "exit") == 0)
        {
            printf("Encerrando o interpretador...\n");
            break; // quebra o loop e o programa termina
        }
        // verifica se e cd
        else if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            { // se o usuario digitou apenas "cd" sem o caminho
                printf("Erro: cd precisa de um caminho como argumento.\n");
            }
            else
            {
                // tenta mudar de diretoorio usando a chamada de sistema chdir
                if (chdir(args[1]) != 0)
                {
                    // se o chdir retornar algo diferente de 0, deu erro (ex: caso a pasta nao existir)
                    perror("Erro no comando cd");
                }
            }
        }
        // se nao for exit nem cd, entao e um programa externo
        else
        {
            printf("\n--- PREPARANDO PARA EXECUTAR PROGRAMA ---\n");
            printf("Programa alvo: %s\n", args[0]);
            printf("Rodar em background (&)? %s\n", background ? "SIM" : "NAO");

            // imprime os argumentos que serão passados para o execlp()
            int j = 0;
            while (args[j] != NULL)
            {
                printf("Argumento [%d]: %s\n", j, args[j]);
                j++;
            }

            printf("-----------------------------------------\n\n");
            // ./helloworld
            // aqui abaixo entrarao o fork() e o execlp()
        }
    }

    return 0;
}