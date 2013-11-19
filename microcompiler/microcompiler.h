#include<stdio.h>
#include<stdlib.h>

 typedef int (*funcp) (int a, int b, int c, int d, int e);

/**
* Funcao geradora do codigo de maquina
*/
void gera(FILE *f, void **code , funcp * entry);


/**
* Funcao de liberacao dos espacos alocados na memoria
* para execucao das funcoes
*/
void libera(void *code);
