#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>  // para waitpid()
#include <sys/types.h> // para pid_t
#include <dirent.h>    // para opendir(), readdir()
#include <ctype.h>     // para isdigit()

#define MAX_LINE 200 
#define MAX_ARGS 20  
#define MAX_PROCS 1000 

// estrutura para armazenar os dados de cada processo lido do /proc
typedef struct {
    int pid;
    char nome[256];
    char estado;
    int ppid;
} ProcessoInfo;

// funcao recursiva para imprimir a arvore de processos
void printa_arvore_processos(int target_pid, ProcessoInfo *procs, int num_procs, int nivel) {
    for (int i = 0; i < num_procs; i++) {
        if (procs[i].pid == target_pid) {
            // de acordo com o nivel da arvore
            for (int j = 0; j < nivel; j++) {
                printf("    ");
            }
            if (nivel > 0) printf("|- ");
            
            // imprime o processo atual
            printf("%d (%s) - Estado: %c\n", procs[i].pid, procs[i].nome, procs[i].estado);
            
            // busca recursivamente os filhos deste processo
            for (int c = 0; c < num_procs; c++) {
                if (procs[c].ppid == target_pid) {
                    printa_arvore_processos(procs[c].pid, procs, num_procs, nivel + 1);
                }
            }
            return; // retorna apos encontrar e imprimir a raiz procurada
        }
    }
}

// funcao que le o diretorio /proc e carrega todos os processos em memoria
void command_tree(int raiz_pid) {
    DIR *dir;
    struct dirent *ent;
    ProcessoInfo *procs = malloc(sizeof(ProcessoInfo) * MAX_PROCS);
    int num_procs = 0;

    if (procs == NULL) {
        perror("Erro de alocacao de memoria");
        return;
    }

    if ((dir = opendir("/proc")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            // verifica se o nome da pasta comeca com um numero (indicando um pid)
            if (isdigit(ent->d_name[0])) {
                char path[256];
                snprintf(path, sizeof(path), "/proc/%s/stat", ent->d_name);
                
                FILE *f = fopen(path, "r");
                if (f) {
                    // o formato do arquivo stat e: pid (nome) estado ppid ...
                    // a mascara %[^)] pega todo o texto do nome ate fechar os parenteses
                    if (fscanf(f, "%d (%[^)]) %c %d", 
                               &procs[num_procs].pid, 
                               procs[num_procs].nome, 
                               &procs[num_procs].estado, 
                               &procs[num_procs].ppid) == 4) {
                        num_procs++;
                    }
                    fclose(f);
                }
            }
        }
        closedir(dir);
        
        printf("Arvore de processos para a raiz PID %d:\n", raiz_pid);
        printa_arvore_processos(raiz_pid, procs, num_procs, 0);
    } else {
        perror("Nao foi possivel abrir o diretorio /proc");
    }

    free(procs);
}

int main() {
    char linha[MAX_LINE];
    char *args[MAX_ARGS];
    printf("O & deve estar separado ao realizar a execucao\nExemplo:\nHelloWorld &\n");
    // loop infinito do interpretador (repl)
    while (1) {
        printf("cmd> ");
        fflush(stdout); 

        // 1. ler a string
        if (fgets(linha, MAX_LINE, stdin) == NULL) {
            printf("\n");
            break; 
        }

        linha[strcspn(linha, "\n")] = '\0';

        if (strlen(linha) == 0) {
            continue;
        }

        // 2. dividir a string 
        int i = 0;
        int background = 0;

        char *token = strtok(linha, " \t");
        
        while (token != NULL) {
            args[i] = token;
            i++;
            token = strtok(NULL, " \t"); 
        }
        args[i] = NULL; 

        // verifica se o ultimo argumento digitado foi o '&'
        if (i > 0 && strcmp(args[i-1], "&") == 0) {
            background = 1;     // marca a flag indicando que nao devemos esperar o filho
            args[i-1] = NULL;   // remove o '&' do array de argumentos
            i--;                // ajusta a contagem de argumentos
        }

        // 3. decidir o que fazer
        
        // comando exit
        if (strcmp(args[0], "exit") == 0) {
            printf("Encerrando o interpretador...\n");
            break; 
        }
        // comando cd
        else if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {  
                printf("Erro: cd precisa de um caminho como argumento.\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("Erro no comando cd"); 
                }
            }
        }
        // comando tree pid
        else if (strcmp(args[0], "tree") == 0) {
            if (args[1] == NULL) {
                printf("Erro: tree precisa de um PID como argumento\n");
            } else {
                int target_pid = atoi(args[1]);
                if (target_pid <= 0) {
                    printf("Erro: PID invalido.\n");
                } else {
                    command_tree(target_pid);
                }
            }
        }
        // executando programas
        else {
            pid_t pid = fork(); // cria o processo filho

            if (pid < 0) {
                // erro no fork
                perror("Erro ao executar fork");
            } 
            else if (pid == 0) {
                // codigo executado pelo processo filho
                // nao consegui fazer com execlp pois o mesmo necessita que a passagem dos parametros
                // seja feita individualmente, enquanto que com o execvp posso apenas passar um array
                if (execvp(args[0], args) == -1) {
                    perror("Erro ao executar o comando");
                }
                // se o exec falhar, e fundamental encerrar o filho para nao criar um fork bomb
                exit(1); 
            } 
            else {
                // codigo executado pelo processo pai
                if (!background) {
                    // se nao tiver o '&', espera o filho terminar
                    int status;
                    waitpid(pid, &status, 0);
                } else {
                    // se tiver o '&', apenas informa que esta rodando e libera o terminal
                    printf("[Rodando em background] PID %d\n", pid);
                }
            }
        }
    }

    return 0;
}