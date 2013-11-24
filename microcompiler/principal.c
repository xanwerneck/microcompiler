/* Alexandre Werneck 1320614 3WA */
/* Stefano 3WA */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "gera.h"


int main(void)
{
	int i = 0,resultado = 0;
	funcp my_func;
	FILE *my_fp;
    unsigned char * code;

	
	my_fp = fopen ("programa.sb", "r");

	if (my_fp == NULL){
		perror ("Nao conseguiu abrir arquivo!");
	}	
	
	gera(my_fp, (void**)&code , &my_func ) ;


	while(i < 100){
		printf("Posicao %d %02x\n" ,i ,code[i]);
		i++;
	}
	

	resultado = (*my_func)(5, 4, 3, 0, 0);

	
	printf ("Resultado de compila: %d\n", resultado);


	return 0;
}
/*
	int i, j;
	funcp f;

        unsigned char codigo[15] = {0x55,0x89,0xe5,0x8b,0x45,0x08,0xe8,0xfc,0xff,0xff,0xff,0x89,0xec,0x5d,0xc3};

	i = (int)&add - (int)&codigo[11];
	
	codigo[10]  = i >> 24;
	codigo[9]   = i >> 16;
	codigo[8]   = i >> 8;
	codigo[7]   = i;

	printf("Valor %08x \n" , i);
	printf("Valor %02x \n" , codigo[7]);
	printf("Valor %02x \n" , codigo[8]);
	printf("Valor %02x \n" , codigo[9]);
	printf("Valor %02x \n" , codigo[10]);

	f = (funcp)codigo;

	j = (*f)(10);

	printf("Valor %d \n" , j);

	return 0;
*/



