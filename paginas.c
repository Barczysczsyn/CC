#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// TODO verificar se eu tirei a IA
FILE *arqv;

void FIFO(int tamPag, int tamMem, int tamEnd, int *enderecos)
{
    // tabela de páginas
    int TP[tamMem / tamPag];
    int pont = 0; // posicao atual na TP, so usa na FIFO
    int end = 0;  // endereco atual a ser acessado
    int erros = 0, acessados = 0;

    printf("\nFIFO:");
    while (end < tamEnd)
    {
        int flag = 0;
        ++acessados;
        //[x] para ser a mesma pagina tem que ser exatamente igual?
        // ve se ja existe a pagina na memoria
        for (int i = 0; i < (tamMem / tamPag); i++)
        {
            if (TP[i] == enderecos[end] / tamPag)
            {
                // tem que sair do while ali de cima
                flag = 1;
                //printf("\nPagina encontrada no endereco %d, pagina %d", enderecos[end], enderecos[end] / tamPag);
            }
        }

        if (!flag)
        {
            // se há erro de pagina
            ++erros;
            TP[pont] = (enderecos[end] / tamPag);
            printf("\nErro de pagina no endereco %d, pagina %d", enderecos[end], enderecos[end] / tamPag);
            ++pont;
            // enche o resto dos enderecos
            /*
            for (int i = 0; i < tamPag - 1; i++)
            {
                // [x] e se o arquivo nao caber na memoria? nao tem como
                //TODO ver se o programa trunca os tamanhos de pagina corretamente
                // [x] pagina pode ser fracionada? nao
                TP[pont] = enderecos[end];
                pont++;
            }
                */
        }
        // vai pro proximo endereco
        ++end;
        // se acabar a TP ele volta pro comeco
        if (pont > (tamMem / tamPag))
        {
            pont = 0;
        }
    }

    printf("\nErros: %d \nTaxa de erro por acesso %.2f %%", erros, (float)(erros * 100) / acessados);
}

void OPT(int tamPag, int tamMem, int tamEnd, int *enderecos)
{
    // tabela de páginas
    int TP[tamMem / tamPag];
    int end = 0; // endereco atual a ser acessado
    int erros = 0, acessados = 0;

    int tempoDesuso[tamMem / tamPag];
    // inicializa a tabela de desuso
    for (int i = 0; i < (tamMem / tamPag); i++)
    {
        tempoDesuso[i] = 0;
    }

    printf("\nOPT:");
    while (end < tamEnd)
    {
        int flag = 0;
        ++acessados;
        //[x] para ser a mesma pagina tem que ser exatamente igual?
        // ve se ja existe a pagina na memoria
        for (int i = 0; i < (tamMem / tamPag); i++)
        {
            if (TP[i] == enderecos[end] / tamPag)
            {
                // tem que sair do while ali de cima
                flag = 1;
                //printf("\nPagina encontrada no endereco %d, pagina %d", enderecos[end], enderecos[end] / tamPag);
            }
        }

        if (!flag)
        {
            int pont = 0;
            // registra o tempo em desuso de cada pagina na TP
            for (int i = 0; i < (tamMem / tamPag); i++)
            {
                int endAux = end;
                while (endAux != tamEnd)
                {

                    if (enderecos[endAux] == TP[i])
                    {
                        tempoDesuso[i] = (endAux - end);
                        break;
                    }
                    ++endAux;
                }
            }

            //ve qual tempo o maior tempo em desuso
            for (int i = 0; i < (tamMem / tamPag); i++)
            {
                if(tempoDesuso[i] > tempoDesuso[pont]){
                    pont = i;
                }
            }

            // se há erro de pagina
            ++erros;
            TP[pont] = (enderecos[end] / tamPag);
            printf("\nErro de pagina no endereco %d, pagina %d", enderecos[end], enderecos[end] / tamPag);
        }
        // vai pro proximo endereco
        ++end;
    }

    printf("\nErros: %d \nTaxa de erro por acesso %.2f %%", erros, (float)(erros * 100) / acessados);
}

void LRU(int tamPag, int tamMem, int tamEnd, int *enderecos)
{
    // tabela de páginas
    int TP[tamMem / tamPag];
    int TO[tamMem / tamPag]; // tabela de ordem
    int end = 0;             // endereco atual a ser acessado
    int ordem = 1;           // ordem em que as paginas foram colocadas, substitiu a data
    int erros = 0, acessados = 0;

    // inicializa a tabela de ordem
    for (int i = 0; i < (tamMem / tamPag); i++)
    {
        TO[i] = 0;
    }

    printf("\nLRU:");
    while (end < tamEnd)
    {
        int flag = 0;
        ++acessados;
        //[x] para ser a mesma pagina tem que ser exatamente igual?
        // ve se ja existe a pagina na memoria
        for (int i = 0; i < (tamMem / tamPag); i++)
        {
            if (TP[i] == enderecos[end] / tamPag)
            {
                // tem que sair do while ali de cima
                flag = 1;
                //printf("\nPagina encontrada no endereco %d, pagina %d", enderecos[end], enderecos[end] / tamPag);
            }
        }

        if (!flag)
        {
            // pra selecionar o lugar
            int pont = TO[0];

            // pega a pagina mais antiga, ou a ultima
            for (int i = 1; i < (tamMem / tamPag); i++)
            {
                if (TO[i] < TO[pont])
                {
                    pont = i;
                }
            }

            // se há erro de pagina
            ++erros;
            TP[pont] = (enderecos[end] / tamPag);
            // recebe a ordem atual
            TO[pont] = ordem;
            ++ordem;
            printf("\nErro de pagina no endereco %d, pagina %d", enderecos[end], enderecos[end] / tamPag);
            // enche o resto dos enderecos
            /*
            for (int i = 0; i < tamPag - 1; i++)
            {
                // [x] e se o arquivo nao caber na memoria? nao tem como
                //TODO ver se o programa trunca os tamanhos de pagina corretamente
                // [x] pagina pode ser fracionada? nao
                TP[pont] = enderecos[end];
                pont++;
            }
                */
        }
        // vai pro proximo endereco
        ++end;
    }

    printf("\nErros: %d \nTaxa de erro por acesso %.2f %%", erros, (float)(erros * 100) / acessados);
}

int main(int argc, char **argv)
{
    int tamPag = atoi(argv[1]);
    int tamMem = atoi(argv[2]);
    arqv = fopen(argv[3], "r");
    printf("%s", argv[3]);
    fflush(stdout);

    // TODO tamanho variavel
    char *string;
    // TODO ver exatamente qual é o tamanho dos endereços
    int enderecos[6500];

    int cont = 0;
    // TODO funciona? retorna EOF
    while (fscanf(arqv, "%d", &(enderecos[cont])) != EOF)
    {
        ++cont;
        // printf("%d ",cont);
        // so pra pegar os espacos entre os numeros
        char espaco;
        fscanf(arqv, "%c", &espaco);
    }

    for (int i = 0; i < cont; i++)
    {
        printf("%d ", enderecos[i]);
    }
    FIFO(tamPag, tamMem, cont, enderecos);
    OPT(tamPag, tamMem, cont, enderecos);
    LRU(tamPag, tamMem, cont, enderecos);

    return 0;
}