#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "print-avl.h"

struct s_no

{

	int32_t chave:28;

	int32_t bal:2;

	int32_t reservado:2; /* sem uso */

	struct s_no* esq;

	struct s_no* dir;

};
typedef struct s_no Arv;

struct s_arq_no

{

	int32_t chave:28;

	int32_t bal:2;

	uint32_t esq:1;

	uint32_t dir:1;

};

void imprimeNo(int c, int b, int null)
{
	for (int i = 0; i < b; i++)
	{
		printf("   ");
	}
	if (null == 1)
	{
		printf("*\n");
	}
	else if(null == 2){
		printf("(%d)\n", c);
	}else
	{
		printf("%d\n", c);
	}
}

void mostraArvore(Arv *a, int b)
{
	if (a == NULL)
	{
		imprimeNo('*', b, 1);
		return;
	}
	mostraArvore(a->dir, b + 1);

	imprimeNo(a->chave, b, 0);
	imprimeNo(a->bal, b, 2);

	mostraArvore(a->esq, b + 1);
}

void mostraPre(Arv *a, int b)
{
	if (a == NULL)
	{
		imprimeNo('*', b, 1);
		return;
	}
	imprimeNo(a->chave, b, 0);


	imprimeNo(a->bal, b, 2);

	mostraPre(a->dir, b + 1);
	mostraPre(a->esq, b + 1);
}
void mostraPos(Arv *a, int b)
{
	if (a == NULL)
	{
		imprimeNo('*', b, 1);
		return;
	}
	mostraPos(a->dir, b + 1);
	mostraPos(a->esq, b + 1);

	imprimeNo(a->chave, b, 0);
	imprimeNo(a->bal, b, 2);
}
void destruir(Arv *pt)
{
	if (pt != NULL)
	{

		if (pt->dir != NULL)
		{
			destruir(pt->dir);
			// free(pt->dir);
		}
		if (pt->esq != NULL)
		{
			destruir(pt->esq);
			// free(pt->esq);
		}
		free(pt);
	}
	else
	{
		printf("\nArvore vazia\n");
	}
}

void gravar(Arv *pont, FILE *stream)
{
	size_t tam = sizeof(struct s_arq_no);

	// converte s_no para s_arq_no
	struct s_arq_no *arq;
	arq = malloc(sizeof(struct s_arq_no));

	arq->chave = pont->chave;
	arq->bal = pont->bal;
	if (pont->esq != NULL)
	{
		arq->esq = 1;
	}
	if (pont->dir != NULL)
	{
		arq->dir = 1;
	}

	// e assim ou tem que escrever item por item?
	fwrite(arq, tam, 1, stream);
	if (pont->esq != NULL)
	{
		gravar(pont->esq, stream);
	}
	if (pont->dir != NULL)
	{
		gravar(pont->dir, stream);
	}
}

Arv *reconstruir(FILE *stream)
{
	//  faz primeiro so com a raiz pra guardar ela
	size_t tam = sizeof(struct s_arq_no);
	struct s_arq_no *ra;
	ra = malloc(tam);

	//  faz primeiro so com a raiz pra guardar ela
	size_t tam2 = sizeof(Arv);
	Arv *raiz;
	raiz = malloc(tam2);

	fread(ra, tam, 1, stream); // teoricamente e assim

	// coloca a do arquivo na real
	raiz->chave = ra->chave;
	raiz->bal = ra->bal;

	if (ra->esq == 1)
	{

		raiz->esq = reconstruir(stream);
	}
	else
	{
		raiz->esq = NULL;
	}

	if (ra->dir == 1)
	{

		raiz->dir = reconstruir(stream);
	}
	else
	{
		raiz->dir = NULL;
	}
	return raiz;
}

void buscar(Arv *raiz, int x)
{
	if (raiz == NULL)
	{
		printf("\nArvore vazia\n");
	}
	else
	{
		Arv *pont = raiz;

		// encontrar o no
		while (pont != NULL && pont->chave != x)
		{
			if (pont->chave > x)
			{
				pont = pont->esq;
			}
			if (pont->chave < x)
			{
				pont = pont->dir;
			}
		}

		if (pont == NULL)
		{

			printf("\nvalor nao encontrado\n");
		}
		else if (pont->chave == x)
		{

			// encontrou
			printf("\nvalor %d encontrado\n", x);
		}
	}
}

/// FABRICIO ///

void inicio_no(Arv** pt, int x) {
	*pt = malloc(sizeof(Arv));
	(*pt)->esq = NULL;
	(*pt)->dir = NULL;
	(*pt)->chave = x;
	(*pt)->bal = 0;
	fflush(stdout);
}

void caso1(Arv **pt, int *h) {
	Arv *ptU = (*pt)->esq;
	if(ptU->bal == -1) {

		//rotacao direita
		(*pt)->esq = ptU->dir;
		ptU->dir = *pt;
		(*pt)->bal = 0;
		*pt = ptU;
	} else {
		//rotacao dupla direita
		Arv *ptV = ptU->dir;
		///erro
		ptU->dir = ptV->esq;
		ptV->esq = ptU;
		(*pt)->esq = ptV->dir;
		ptV->dir = *pt;


		if(ptV->bal == -1) {
			(*pt)->bal = 1;
		} else {
			(*pt)->bal = 0;
		}

		if(ptV->bal == 1) {

			(*pt)->bal = -1;
		} else {
			(*pt)->bal = 0;
		}
		*pt = ptV;
	}
	(*pt)->bal = 0;
	*h = 0;
}
void caso2(Arv **pt, int *h) {
	Arv *ptU = (*pt)->dir;
	if(ptU->bal == 1) {
		//rotacao esquerda
		(*pt)->dir = ptU->esq;
		ptU->esq = *pt;
		(*pt)->bal = 0;
		*pt = ptU;
	} else {
		//rotacao dupla esquerda
		Arv *ptV = ptU->esq;
		ptU->esq = ptV->dir;

		ptV->dir = ptU;
		(*pt)->dir = ptV->esq;
		ptV->esq = *pt;


		if(ptV->bal == 1) {
			(*pt)->bal = -1;
		} else {
			(*pt)->bal = 0;
		}

		if(ptV->bal == -1) {
			(*pt)->bal = 1;
		} else {
			(*pt)->bal = 0;
		}
		*pt = ptV;
	}

	(*pt)->bal = 0;
	*h = 0;
}

void ins_AVL(Arv **pt, int x, int *h) {

	if((*pt) == NULL) {
		inicio_no(pt,x);
		*h = 1;

	} else {
		//acho q e asssim
		if(x == (*pt)->chave) {
			printf("\nchave ja existe");
			return;
		}
		if( x < (*pt)->chave) {
			//SOCORRO
			ins_AVL(&(*pt)->esq,x,h);
			if(*h) {
				switch((*pt)->bal) {
				case 1:
					(*pt)->bal = 0;
					*h = 0;
					break;
				case 0:
					(*pt)->bal = -1;
					break;
				case -1:

					caso1(pt,h);
					break;
				}
			}
		} else {
			ins_AVL((&(*pt)->dir),x,h);
			if(*h) {
				switch((*pt)->bal) {
				case -1:
					fflush(stdout);
					(*pt)->bal = 0;
					*h = 0;
					break;
				case 0:
					(*pt)->bal = 1;
					break;
				case 1:

					caso2(pt,h);
					break;
				}
			}

		}
	}
}

/// FABRICIO ///



int rem(Arv *pt, Arv *ptAnt) {


	// nao tem filhos
	if (pt->esq == NULL && pt->dir == NULL)
	{
		//vamo ver se funciona
		if (ptAnt->esq == pt)
		{
			ptAnt->esq = NULL;
		}
		if (ptAnt->dir == pt)
		{
			ptAnt->dir = NULL;
		}
		free(pt);
		return 1;
	}
	// tem so um filho
	else if (pt->esq != NULL && pt->dir == NULL)
	{
		if (ptAnt->esq == pt)
		{
			ptAnt->esq = NULL;
		}
		if (ptAnt->dir == pt)
		{
			ptAnt->dir = NULL;
		}
		free(pt);
		return 1;
	}
	else if (pt->dir != NULL && pt->esq == NULL)
	{
		if (ptAnt->esq == pt)
		{
			ptAnt->esq = NULL;
		}
		if (ptAnt->dir == pt)
		{
			ptAnt->dir = NULL;
		}
		else
		{
			// erro
		}
		free(pt);
		return 1;
	}
	else
		// tem 2 filhos
	{
		Arv *suc = pt->dir;
		if (suc->esq == NULL)
			// o sucessor e direto
		{
			// muda o proximo do anterior
			if (ptAnt->esq == pt)
			{
				ptAnt->esq = suc;
			}
			if (ptAnt->dir == pt)
			{
				ptAnt->dir = suc;
			}
			suc->esq = pt->esq;
			free(pt);
			return 1;
		}
		else
		{

			// o sucessor nao e direto
			// encontra o sucessor verdadeiro
			while (suc->esq != NULL)
			{
				suc = suc->esq;
			}

			Arv *aesq, *adir, *sucdir;

			// guarda as arvores esquerda e direita restantes
			aesq = pt->esq;
			adir = pt->dir;
			sucdir = suc->dir;

			// faz as trocas
			suc->dir = adir;
			adir->esq = sucdir;
			suc->esq = aesq;

			// muda o proximo do anterior
			if (ptAnt->esq == pt)
			{
				ptAnt->esq = suc;
			}
			if (ptAnt->dir == pt)
			{
				ptAnt->dir = suc;
			}

			// remove o pont

			free(pt);
			return 1;
		}
	}

}
int remov(Arv **pt, Arv *ptAnt, int x, int *h) {
		int ret = 0;
	if((*pt)->chave == x) {

		if(ptAnt == NULL) {
			free(pt);
			pt = NULL;
			*h = 1;
		} else {
			rem(*pt,ptAnt);
			*h = 1;
			return 1;
		}
	} else {
		if((*pt)->esq !=NULL) {
			remov(&(*pt)->esq,*pt,x,h);
			if(h) {
				ret = 1;
				//isso ja calcula o h
				switch((*pt)->bal) {
				case -1:
					fflush(stdout);
					(*pt)->bal = 0;
					break;
				case 0:
					(*pt)->bal = -1;
					break;
				case 1:
					caso2(pt,h);
					break;
				}

			}
		}
		if((*pt)->dir !=NULL) {
			remov(&(*pt)->esq,*pt,x,h);
			if(h) {
				ret = 1;
				//foi tirado o h
				switch((*pt)->bal) {
				case 1:
					fflush(stdout);
					(*pt)->bal = 0;
					break;
				case 0:
					(*pt)->bal = -1;
					break;
				case -1:
					//nao tem mais pq mandar o h, mas provavelmente vai da erro
					caso1(pt,h);
					break;
				}

			}
		}

	}
		return ret;
}
int main()
{
	int resp;

	Arv *raiz;
	raiz = NULL;

	char nome[30];
	int h;

	do
	{
		printf("\nSelecione a operacao desejada: \n\t1 - Busca \n\t2 - Insercao \n\t3 - Remocao. \n\t4 - Imprimir.\n\t5 - Gravar em arquivo binario.\n\t6 - Ler arvore em arquivo binario.\n\t7 - Sair.\n");

		scanf("%d", &resp);
		int num;
		switch (resp)
		{
		case 1:
			printf("\nescreva o numero a ser procurado\n");
			scanf("%d", &num);
			buscar(raiz, num);
			break;
		case 2:
			printf("\nescreva o numero a ser inserido\n");
			scanf("%d", &num);
			ins_AVL(&raiz, num,&h);
			//nao sei oq C) h
			imprime_arvore(raiz);
			break;
		case 3:
			printf("\nescreva o numero a ser removido\n");
			scanf("%d", &num);
			remov(&raiz,NULL, num,&h);
			imprime_arvore(raiz);
			break;

		case 4:
			// liberar(&pont);
			printf("\nDe qual forma deseja imprimir? \n\t1 - Ordem simetrica \n\t2 - Pre-ordem \n\t3 - Pos-ordem  ");
			int respo2;

			scanf("%d", &respo2);

			switch (respo2)
			{
			case 1:
				mostraArvore(raiz,1);
				break;
			case 2:
				mostraPre(raiz, 1);
				break;
			case 3:
				mostraPos(raiz, 1);
				break;
			default:

				printf("\nentrada invalida \n");
				break;
			}

			break;

		case 5:
			// nome personalizado, tem q ser global
			printf("\nQual sera o nome do arquivo?");
			scanf("%s", nome);
			strcat(nome, ".bin");

			FILE *arquivo = fopen(nome, "w");
			gravar(raiz, arquivo);

			fclose(arquivo);
			break;

		case 6:
			printf("\nQual e o nome do arquivo?");
			printf("\n(certifique-se de que o arquivo existe)\n");
			char str[50];
			scanf("%s", str);
			FILE *arquivo2 = fopen(str, "r");
			// destruir a arvore atual
			destruir(raiz);

			// reconstroi a raiz e passa pra atual
			raiz = reconstruir(arquivo2);
			fclose(arquivo2);
			// system("clear");
			imprime_arvore(raiz);
			break;
		case 7:
			// liberar(&pont);
			destruir(raiz);
			// free(raiz);
			system("clear");
			break;
		default:
			printf("\nentrada invalida \n");
			break;
		}
	} while (resp != 7);

	return 0;
}