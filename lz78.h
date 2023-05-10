#include <stdio.h>
#include <stdlib.h>

typedef struct arvore_trie{
	struct arvore_trie* irmao;
	struct arvore_trie* filho;
	char chave;
	unsigned int id;
} trie;

trie * percorre(FILE * infile, FILE * outfile, trie * raiz, char valor, unsigned int codigo, unsigned int pai);
trie* destroi(trie * raiz);
trie * lechar(FILE * infile, FILE * outfile, trie * raiz, unsigned int codigo, unsigned int pai);
int checkbytes(unsigned int i);
void letrie(trie * raiz, int bytes, char* vet, unsigned int tam, unsigned int pai);
void criavetor(FILE * outfile, unsigned int tam, trie * raiz, int bytes);
void imprime(FILE * outfile, unsigned int i, trie * raiz);
void learquivo(FILE * infile, FILE * outfile);
void imprimevetor(FILE * outfile, unsigned int posicao, int bytes,unsigned char *vet);
void levetor(FILE * infile, FILE * outfile, unsigned int tam, int bytes);
void lebinario(FILE * infile, FILE * outfile);

