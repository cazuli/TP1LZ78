//Codigo Luiz Eduardo
#include <stdio.h>
#include <stdlib.h>
#include "lz78.h"


trie * percorre(FILE * infile, FILE * outfile, trie * raiz, char valor, unsigned int codigo, unsigned int pai){
//funcao para preencher a trie
	if(raiz==NULL){
		if(valor!=-1){
			trie *nova = malloc(sizeof(trie));
			nova->chave=valor;
			nova->id=codigo;
			nova->irmao=NULL;
			nova->filho=NULL;
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
//destroi a arvore
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
//funcao para ler caracteres do arquivo para preencher a trie
{

	char texto;

		texto = fgetc(infile);
		raiz = percorre(infile, outfile, raiz, texto, codigo, pai);
		
		return raiz;
	
}

int checkbytes(unsigned int i){
//retorna numero de bytes dado int
	unsigned int p2=255;
	for(int j=1; j<=4; j++){
		if(p2>=i){
			return j;
		}else{
			p2 = p2 + (p2 << 8);
		}
	}
	return 0;
}

void letrie(trie * raiz, int bytes, char* vet, unsigned int tam, unsigned int pai){
//preenche vetor a ser usado no arquivo de saide com a trie
	if(raiz!=NULL){
		for(int i=1; i<=bytes; i++){
			vet[(raiz->id)*(bytes+1)+i-1] = pai >> (bytes-i)*8;
		}
		vet[((raiz->id)*(bytes+1))+bytes]=raiz->chave;
		letrie(raiz->irmao, bytes, vet, tam, pai);
		letrie(raiz->filho, bytes, vet, tam, raiz->id);
	}
}

void criavetor(FILE * outfile, unsigned int tam, trie * raiz, int bytes){
//	cria vetor e escreve a compressão no aruivo de saida
	char *vet;
	vet = (char*)malloc(tam*(bytes+1));
	vet[0]=bytes;
	for(int i=1; i<=bytes; i++){
		vet[i] = tam >> (bytes-i)*8;
	}
	letrie(raiz, bytes, vet, tam, 0);
	fwrite(vet , 1 , tam*(bytes+1) , outfile );
	
	if(vet!=NULL){
		free(vet);
	}
	
}

void imprime(FILE * outfile, unsigned int i, trie * raiz)
//pega bytes e chama cria vetor
{
	int bytes;
	bytes = checkbytes(i);
	criavetor(outfile, i, raiz, bytes);
	
}

void learquivo(FILE * infile, FILE * outfile)
//cria trie faz as operacoes de compressao
{
	trie * raiz=NULL;
	unsigned int i=1;

	while(!feof(infile)){
		raiz = lechar(infile, outfile, raiz, i, 0);
		i++;
	}
	i--;
	imprime(outfile, i, raiz);
	raiz = destroi(raiz);
	
	
}

void imprimevetor(FILE * outfile, unsigned int posicao, int bytes, unsigned char *vet){
//descomprime arquivo
	unsigned int pai=0;
	for(int i=0; i<bytes; i++){
		pai = pai<<8;
		pai=pai+vet[posicao*(bytes+1)+i];
	}
	if(pai!=0){
		imprimevetor(outfile, pai-1, bytes, vet);
	}
	if(vet[(posicao*(bytes+1))+bytes]!=0){
		fprintf(outfile ,"%c", vet[(posicao*(bytes+1))+bytes]);
	}
	
	
}

void levetor(FILE * infile, FILE * outfile, unsigned int tam, int bytes){
//le arquivo comprimido e cria vetor para realizar descompressao	
	unsigned char *vet;
	vet = (char*)malloc(tam*(bytes+1));
	fread(vet , sizeof(char) , tam*(bytes+1), infile );
	for(unsigned int i=0; i<tam; i++){
		imprimevetor(outfile, i, bytes, vet);
	}
	if(vet!=NULL){
		free(vet);
	}
	
}
void lebinario(FILE * infile, FILE * outfile)
{//faz operacoes de descompressao
	int bytes;
	unsigned int tam=0;

	bytes=fgetc(infile);
	for(int i=0; i<bytes; i++){
		tam=tam<<8;
		tam=tam+fgetc(infile);
	}
	levetor(infile, outfile, tam-1, bytes);
	
	
}

