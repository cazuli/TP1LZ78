#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "lz78.h"

int main(int argc, char **argv) {
	FILE* infile;
	FILE* outfile;
	
	int i;
	scanf("%d", &i);
	if(i==0){
		infile = fopen("teste.txt","r");
		if(infile==NULL){
			printf("Erro na abertura do arquivo 1\n");
			return 1;
		}
		outfile = fopen("out.l78","wb");
		if(outfile==NULL){
			printf("Erro na abertura do arquivo 2\n");
			return 1;
		}
		learquivo(infile, outfile);
	}
	else{
		infile = fopen("out.l78","rb");
		if(infile==NULL){
			printf("Erro na abertura do arquivo 3\n");
			return 1;
		}
		outfile = fopen("out.txt","wt");
		if(outfile==NULL){
			printf("Erro na abertura do arquivo 4\n");
			return 1;
		}
		lebinario(infile, outfile);
		//learquivo(outfile, infile);
		//lebinario(infile, outfile);
	}
	fclose(outfile);
	fclose(infile);

	


    return 0;
}

