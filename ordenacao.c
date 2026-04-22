#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/stat.h>

void printaNum(int tam, int32_t numeros[tam]) {

	printf("\n");
	for (int i = 0; i < tam; i++)
	{
		printf(" %d ",numeros[i]);
	}
}

void trocar(int32_t* n1, int32_t* n2) {
	int32_t aux = *n1;
	*n1 = *n2;
	*n2 = aux;
}
void bubbleSort(int tam, int32_t numeros[tam]) {
	for (int j = 0; j < tam; j++)
	{
		//printaNum(tam,numeros);
		for (int i = 0; i < tam-1; i++)
		{
			if(numeros[i] > numeros[i+1]) {
				trocar(&numeros[i],&numeros[i+1]);
				// int aux = numeros[i];
				// numeros[i] = numeros[i+1];
				// numeros[i+1] = aux;
			}
		}
	}
}

void bubbleSortM(int tam, int32_t numeros[tam]) {
	int mudou = 0,n = tam, guarda=tam;
	do {
		//printaNum(tam,numeros);
		mudou = 0;
		int i = 0;//era pra ser 1
		while(i<n) {
			if(numeros[i] > numeros[i+1]) {
				//trocar
				trocar(&numeros[i],&numeros[i+1]);
				mudou = 1;
				guarda = i;
			}
			i++;
		}
		n = guarda;
	} while(mudou > 0);
}
void insertionSortF(int tam, int32_t numeros[tam]) {
	for (int i = 1; i <= tam; i++)
	{
		//printaNum(tam,numeros);
		//int32_t prov = numeros[i];
		int32_t valor = numeros[i];
		int j = i-1;
		//funciona com o 0, talvez nao devia
		while(j >= 0 && valor < numeros[j]) {
			//esta salvo no valor
			numeros[j+1] = numeros[j];
			j--;
		}
		numeros[j+1] =  valor;
	}

}

void intercalarF(int tam,int32_t L[tam], int ini1, int ini2, int fim2) {
	//tmp era passado por referencia, mas acho que isso nao e necessario
	//int32_t tmp[(fim2-ini1)];
	int32_t *tmp = malloc((fim2-ini1)*sizeof(int32_t));
	int fim1 = ini2-1;
	//nro C) o numero de elementos no array
	//ind C) o indice pra se colocar no array temporario tmp
	//printf("\n ini1 %d fim1 %d ini2 %d fim2 %d",ini1,fim1,ini2,fim2);
	int nro = 0,ind = 0;

	///so pra salvar
	int inicio = ini1;

	//talvez seja isso
	//fim2--;
	//ind comecava com ini1, mas isso causa acesso invalido
	while(ini1 <= fim1 && ini2 <= fim2) {

		if(L[ini1] < L[ini2]) {
			tmp[ind] = L[ini1];
			ini1++;
		} else {
			tmp[ind] = L[ini2];
			ini2++;
		}
		ind++;
		nro++;
	}
	//caso tenha sobrado so uma, e colocada assim
	while(ini1 <= fim1) {
		tmp[ind] = L[ini1];
		ini1++;
		ind++;
		nro++;
	}
	while(ini2 <= fim2) {
		tmp[ind] = L[ini2];
		ini2++;
		ind++;
		nro++;
	}
	//printf("\ntmp ");
	//printaNum((fim2-inicio),tmp);
	//coloca tudo de volta no principal
	for (int i = 0; i < nro; i++)
	{
		L[i+inicio]  = tmp[i];
	}


}

void mergesort(int tam, int32_t numeros[tam],int esq, int dir) {
	if(esq <dir) {
		int centro = (esq+dir)/2;
		mergesort(tam,numeros,esq,centro);
		mergesort(tam,numeros,centro+1,dir);
		intercalarF(tam,numeros,esq,centro+1,dir);
		//printaNum(tam,numeros);
	}
}

void quicksortF(int tam, int32_t numeros[tam],int ini, int fim) {
	if((fim-ini) < 2) {
		if(fim-ini == 1) {
			if(numeros[ini] > numeros[fim]) {
				//trocar
				trocar(&numeros[ini],&numeros[fim]);

			}
		}
	} else {
		//printaNum(tam,numeros);
		int i = ini,j = fim-1;
		//pivo e o key
		int32_t key = numeros[fim];
		while(j>=i) {
			while (numeros[i] < key)
			{
				i++;
			}
			while (numeros[j] > key)
			{
				j--;
			}
			if(j >= i) {
				trocar(&numeros[i],&numeros[j]);
				i++;
				j--;
			}


		}
		trocar(&numeros[i],&numeros[fim]);
		quicksortF(tam,numeros,ini,i-1);
		quicksortF(tam,numeros,i+1,fim);

	}

}

//7
void quicksortM3(int tam, int32_t numeros[tam],int ini, int fim) {
	if((fim-ini) < 2) {
		if(fim-ini == 1) {
			if(numeros[ini] > numeros[fim]) {
				//trocar
				trocar(&numeros[ini],&numeros[fim]);

			}
		}
	} else {
		//printaNum(tam,numeros);
		int i = ini,j = fim-1;
		//mediana de 3
		//int32_t centro = numeros[ini+fim/2];
		int32_t comeco = (rand() % (fim-ini)) + ini, centro = (rand() % (fim-ini)) + ini, final = (rand() % (fim-ini)) + ini;
		int32_t key;

		//escolhe a mediana
		if((numeros[comeco] < numeros[centro] && numeros[centro] < numeros[final]) ||(numeros[final] < numeros[centro] && numeros[centro] < numeros[comeco]) ) {
			key = numeros[centro];
		} else if((numeros[centro] < numeros[final] && numeros[final] < numeros[comeco])||(numeros[comeco] < numeros[final] && numeros[final] < numeros[centro])) {
			key = numeros[final];
		} else {
			key = numeros[comeco];
		}

		while(j>=i) {
			while (numeros[i] < key)
			{
				i++;
			}
			while (numeros[j] > key)
			{
				j--;
			}
			if(j >= i) {
				trocar(&numeros[i],&numeros[j]);
				i++;
				j--;
			}


		}
		trocar(&numeros[i],&numeros[fim]);
		quicksortM3(tam,numeros,ini,i-1);
		quicksortM3(tam,numeros,i+1,fim);

	}

}

void quicksortR(int tam, int32_t numeros[tam],int ini, int fim) {
	if((fim-ini) < 2) {
		if(fim-ini == 1) {
			if(numeros[ini] > numeros[fim]) {
				//trocar
				trocar(&numeros[ini],&numeros[fim]);

			}
		}
	} else {
		//printaNum(tam,numeros);
		int i = ini,j = fim;
		//pivo NAO e o key
		int32_t pivo = (rand() % (fim-ini) + ini);
		trocar(&numeros[pivo],&numeros[fim]);
		int32_t key = numeros[fim];

		while(j>i) {
			while (numeros[i] < key)
			{
				i++;
			}
			while (numeros[j] > key)
			{
				j--;
			}


			trocar(&numeros[i],&numeros[j]);
			i++;
			j--;
			


		}
		trocar(&numeros[i],&numeros[fim]);
		quicksortR(tam,numeros,ini,i-1);
		quicksortR(tam,numeros,i+1,fim);

	}

}


void heapify(int32_t numeros[], int tam, int i) {
	//printaNum(tam,numeros);

	int maior = i;

	int esq = 2 * i + 1;

	int dir = 2 * i + 2;

	if (esq < tam && numeros[esq] > numeros[maior]) {
		maior = esq;
	}

	if (dir < tam && numeros[dir] > numeros[maior]) {
		maior = dir;
	}

	if (maior != i) {
		trocar(&numeros[i],&numeros[maior]);

		heapify(numeros, tam, maior);
	}
}

void heapSort(int32_t numeros[], int n) {

	for (int i = n / 2 - 1; i >= 0; i--) {
		heapify(numeros, n, i);
	}

	for (int i = n - 1; i > 0; i--) {

		trocar(&numeros[0],&numeros[i]);

		heapify(numeros, i, 0);
	}
}

int main(int argc, char* argv[]) {
	if(argc != 4) {
		printf("\nErro: numero de argumentos invalido \nNome do arquivo - algoritmo - entrada - saida");
		return 0;
	}
	//*
	int algoritmo = atoi(argv[1]);


	char str[50];
	strcpy(str,argv[2]);
	//printf("\n %s",str);
	char saida[50];
	strcpy(saida,argv[3]);

	FILE *arquivo = fopen(str, "r");

	struct stat buf;

	stat(str, &buf);
	size_t size = buf.st_size;

	//printf("\nsize %ld",size);
	size_t tamArq = size/sizeof(int32_t);//tamanho do arquivo

	//printf("\ntamArq %ld",tamArq);
	//int numeros[tamArq];
	int *numeros = malloc(size);
	//int tam = sizeof(int32_t);


	for (int i = 0; i < (int) tamArq; i++)
	{
		fread(numeros, size, tamArq, arquivo);
	}

	//printf("\n coisa");
	//fflush(stdout);

	clock_t tempo;
	clock_t tempoAtual;
	switch (algoritmo)
	{
	case 1:


		tempo = clock();

		bubbleSort(tamArq,numeros);

		//clock_t tempoAtual;
		tempoAtual = clock();
		printf("\ntempo de execucao %.3lf",(double)(tempoAtual- tempo)/ CLOCKS_PER_SEC);
		break;
	case 2:
		//clock_t tempo;
		tempo = clock();

		bubbleSortM(tamArq,numeros);
		//clock_t tempoAtual;
		tempoAtual = clock();
		printf("\ntempo de execucao %.3lf",(double)(tempoAtual- tempo)/ CLOCKS_PER_SEC);
		break;

	case 3:
		//clock_t tempo;
		tempo = clock();
		insertionSortF(tamArq,numeros);
		//clock_t tempoAtual;
		tempoAtual = clock();
		printf("\ntempo de execucao %.3lf",(double)(tempoAtual- tempo)/ CLOCKS_PER_SEC);
		break;

	case 4:
		//clock_t tempo;
		tempo = clock();
		mergesort(tamArq,numeros,0,tamArq-1);
		//clock_t tempoAtual;
		tempoAtual = clock();
		printf("\ntempo de execucao %.3lf",(double)(tempoAtual- tempo)/ CLOCKS_PER_SEC);
		break;
	case 5:
		//clock_t tempo;
		tempo = clock();
		quicksortF(tamArq,numeros,0,tamArq-1);
		//clock_t tempoAtual;
		tempoAtual = clock();
		printf("\ntempo de execucao %.3lf",(double)(tempoAtual- tempo)/ CLOCKS_PER_SEC);
		break;
	case 6:
		//clock_t tempo;
		tempo = clock();
		quicksortR(tamArq,numeros,0,tamArq-1);
		//clock_t tempoAtual;
		tempoAtual = clock();
		printf("\ntempo de execucao %.3lf",(double)(tempoAtual- tempo)/ CLOCKS_PER_SEC);
		break;
	case 7:
		//clock_t tempo;
		tempo = clock();
		quicksortM3(tamArq,numeros,0,tamArq-1);
		//clock_t tempoAtual;
		tempoAtual = clock();
		printf("\ntempo de execucao %.3lf",(double)(tempoAtual- tempo)/ CLOCKS_PER_SEC);
		break;
	case 8:
		//clock_t tempo;
		tempo = clock();
		heapSort(numeros,tamArq);
		//clock_t tempoAtual;
		tempoAtual = clock();
		printf("\ntempo de execucao %.3lf",(double)(tempoAtual- tempo)/ CLOCKS_PER_SEC);
		break;

	default:
		printf("\nentrada invalida");
		break;
	}

	//printaNum(tam,numeros);

	FILE *arq = fopen(saida,"w");
	fwrite(saida, size, tamArq, arq);

	//*/
//TODO tira isso
	// //preenche aleatoriamente

	/*/


	srand(time(NULL));
	int tam = 20;
	int32_t numeros[tam];
	for (int i = 0; i < tam-1; i++)
	{
		numeros[i] = rand() % 100;
	}

	printaNum(tam,numeros);

	clock_t tempo;
	tempo = clock();

	//mergesort(tam,numeros,0,tam);
	insertionSortF(tam,numeros);
	printaNum(tam,numeros);

	clock_t tempoAtual = clock();
	printf("\ntempo de execucao %.9lf",(double)(tempoAtual- tempo)/ CLOCKS_PER_SEC);

	//*/
	//preenche aleatoriamente

	return 0;
}