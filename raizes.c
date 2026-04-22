#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main() {

	int int1,int2;
	int coef[11];
	int grau;



	printf("\nescreva o grau do polinomio: ");
	scanf("%d", &grau);
	grau++;


	for(int i = 0; i < grau; i++) {

		printf("\nescreva os coeficientes do polinomio (X^%d): ",i);
		scanf("%d", &coef[i]);

	}

	do {
		printf("\nescreva o intervalo inicial \n de: ");
		scanf("%d", &int1);
		printf(" ate: ");
		scanf("%d", &int2);
	} while(int1 >= int2);
//verificacao pra os intervalo ta certo

//comeca
	clock_t tempo;
	tempo = clock();
	int a[7],b[7];
	int k = 0;

	for(int i = int1; i < int2; i++) {

		int pol1 = 0, pol2 = 0;

		//printf("\ny = ");
		for(int j = 0; j < grau; j++) {
			pol1 += coef[j] * pow(i,j);

			//printf(" + %dX^%d",coef[j],j);
			pol2 += coef[j] * pow(i+1,j);
		}

//talvez tenha formas melhores
		if(pol1 * pol2 < 0) {
			a[k] = i;
			b[k] = i+1;
			k++;
		}
		else if(pol1 == 0) {
			/*
						a[k] = i-1;
						b[k] = i+1;
						k++;
						*/
			printf("\nraiz exata: %d",i);
		}
		else if(pol2 == 0) {
			/*
						a[k] = i;
						b[k] = i+2;
						k++;
						*/
			printf("\nraiz exata: %d",i+1);
		}
	}
	if(k == 0) {

		printf("\nNao existem raizes reais");
	} else {
		printf("\nNumero de raizes: %d",k);
		for (int i = 0; i < k; i++)
		{
			printf("\nraiz no intervalo [%d ; %d]",a[i],b[i]);
		}
	}
	clock_t tempoAtual = clock();
	printf("\ntempo de execucao %lf",(double)(tempoAtual- tempo)/ CLOCKS_PER_SEC);

	return 0;
}
