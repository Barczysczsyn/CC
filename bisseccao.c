#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main() {

	double a,b;
	int coef[11];
	int grau;

	printf("\nescreva o grau do polinomio: ");
	scanf("%d", &grau);
	//grau é um a mais na vdd
	grau++;

	for(int i = 0; i < grau; i++) {

		printf("\nescreva os coeficientes do polinomio (X^%d): ",i);
		scanf("%d", &coef[i]);

	}

	do {
		printf("\nescreva o intervalo inicial \n de: ");
		scanf("%lf", &a);
		printf(" ate: ");
		scanf("%lf", &b);
		fflush(stdin);
	} while(a >= b);

	double erromin;
		printf("\nescreva o erro: ");
		scanf("%lf", &erromin);
		printf("%lf",erromin);
//verificacao pra os intervalo ta certo

//comeca
clock_t tempo;
int iter = 0;
double erro = erromin;
double xn_1 = 0;
tempo = clock();
double novo;

printf("\nn |     A     |     B     | Xn = a+b/2 |  f(Xn)  |   ERRO  ");
	
	do{
		//xn_1 = 0;
	    iter++;
		novo = (a + b)/2;
		//ele salva o fxn por algum motivo
		double fxn = 0;

		printf("\n");
		for(int j = 0; j < grau; j++) {
			fxn += coef[j] * pow(novo,j);
			//printf("+ %d(%lf)^%d",coef[j],novo,j);
		}

		if(iter != 1){
			erro = fabs(novo-xn_1);

			printf("\n%d| %.7lf | %.7lf | %.7lf | %.7lf | %.7lf",iter-1,a,b,novo,fxn,erro);
		}else{
			printf("\n%d | %.7lf | %.7lf | %.7lf | %.7lf | ---",iter-1,a,b,novo,fxn);
		    
		}

			xn_1 = novo;

		if(fxn < 0){
			a = novo;
		}else if(fxn > 0){
			b = novo;
		}else{
			//sei la
		}

	}while( erro >= erromin);
	clock_t tempoAtual = clock();

	printf("\nraiz encontrada %.7lf",novo);
	printf("\ntempo de execução %.7lf",(double)(tempoAtual- tempo)/ CLOCKS_PER_SEC);

	return 0;
}
