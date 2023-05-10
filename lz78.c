//Codigo Luiz Eduardo
#include <stdio.h>
#include <stdlib.h>
#include "lz78.h"






trie * percorre(FILE * infile, FILE * outfile, trie * raiz, char valor, unsigned int codigo, unsigned int pai){
	if(raiz==NULL){
		if(valor!=-1){
			trie *nova = malloc(sizeof(trie));
			nova->chave=valor;
			nova->id=codigo;
			nova->irmao=NULL;
			nova->filho=NULL;
			//fprintf(outfile, "%u%c", pai, nova->chave);
			//printf("%u:%u|%c- ",codigo, pai, nova->chave);
			return nova;
		}else{
			return NULL;
		}
	}else{
		if((raiz->chave)==valor){
			raiz->filho=lechar(infile, outfile, raiz->filho, codigo, raiz->id);
		}else{
			raiz->irmao=percorre(infile, outfile, raiz->irmao, valor, codigo, pai);
		}
		return raiz;
	}
	
}


trie* destroi(trie * raiz){
	if(raiz!=NULL){
		destroi(raiz->filho);
		destroi(raiz->irmao);
		free(raiz);
		return NULL;
	}else{
		return raiz;
	}
	
}
trie * lechar(FILE * infile, FILE * outfile, trie * raiz, unsigned int codigo, unsigned int pai)
{

	char texto;

		texto = fgetc(infile);
		raiz = percorre(infile, outfile, raiz, texto, codigo, pai);
		
		return raiz;
	
}

int checkbytes(unsigned int i){
	unsigned int p2=255;
	for(int j=1; j<=4; j++){
		if(p2>=i){
			//printf("%d %d\n", p2, i);
			return j;
		}else{
			p2 = p2 + (p2 << 8);
		}
	}
	return 0;
}

void letrie(trie * raiz, int bytes, char* vet, unsigned int tam, unsigned int pai){
	if(raiz!=NULL){
		//printf("%d\n", bytes);
		for(int i=1; i<=bytes; i++){
			//printf("1\n");
			vet[(raiz->id)*(bytes+1)+i-1] = pai >> (bytes-i)*8;
			//printf("%d\n", vet[(raiz->id)*(bytes+1)+i-1]);
		}
		//printf("2\n");
		vet[((raiz->id)*(bytes+1))+bytes]=raiz->chave;
		letrie(raiz->irmao, bytes, vet, tam, pai);
		letrie(raiz->filho, bytes, vet, tam, raiz->id);
	}
}

void criavetor(FILE * outfile, unsigned int tam, trie * raiz, int bytes){
	
	char *vet;
	vet = (char*)malloc(tam*(bytes+1));
	vet[0]=bytes;
	//printf("%d\n", bytes);
	for(int i=1; i<=bytes; i++){
		vet[i] = tam >> (bytes-i)*8;
	}
	//printf("%u\n", tam*(bytes+1));
	letrie(raiz, bytes, vet, tam, 0);
	//for(int i=0; i<tam*(bytes+1); i++){
		//fprintf(outfile ,"%c ",vet[i]);
	//}
	fwrite(vet , 1 , tam*(bytes+1) , outfile );
	//preprintree(raiz);
	
	
}

void imprime(FILE * outfile, unsigned int i, trie * raiz)
{
	int bytes;
	bytes = checkbytes(i);
	//printf("%d\n", bytes);
	criavetor(outfile, i, raiz, bytes);
	
}

void learquivo(FILE * infile, FILE * outfile)
{
	trie * raiz=NULL;
	unsigned int i=1;

	while(!feof(infile)){
		raiz = lechar(infile, outfile, raiz, i, 0);
		i++;
	}
	i--;
	//printf("%u\n", i);
	//preprintree(raiz);
	//printf("%d\n",i);
	imprime(outfile, i, raiz);
	raiz = destroi(raiz);
	
	
}

void imprimevetor(FILE * outfile, unsigned int posicao, int bytes, unsigned char *vet){
	
	unsigned int pai=0;
	//printf("%d: ",posicao);
	for(int i=0; i<bytes; i++){
		pai = pai<<8;
		pai=pai+vet[posicao*(bytes+1)+i];
		//printf("%d ", vet[posicao*(bytes+1)+i]);
	}
	//printf("\n");
	if(pai!=0){
		//printf("%u ", pai);
		imprimevetor(outfile, pai-1, bytes, vet);
	}
	if(vet[(posicao*(bytes+1))+bytes]!=0){
		fprintf(outfile ,"%c", vet[(posicao*(bytes+1))+bytes]);
	}
	
	
}

void levetor(FILE * infile, FILE * outfile, unsigned int tam, int bytes){
	
	unsigned char *vet;
	vet = (char*)malloc(tam*(bytes+1));
	fread(vet , sizeof(char) , tam*(bytes+1), infile );
	for(unsigned int i=0; i<tam; i++){
		//printf("%u:%u\n", i, tam);
		imprimevetor(outfile, i, bytes, vet);
	}
	
	
}
void lebinario(FILE * infile, FILE * outfile)
{
	int bytes;
	unsigned int tam=0;

	//printf("1\n");
	bytes=fgetc(infile);
	//printf("%d\n", bytes);
	for(int i=0; i<bytes; i++){
		tam=tam<<8;
		tam=tam+fgetc(infile);
		//printf("%d ", tam);
	}
	//printf("\n");
	//printf("%d\n%d\n", bytes, tam);
	levetor(infile, outfile, tam-1, bytes);
	
	
}

