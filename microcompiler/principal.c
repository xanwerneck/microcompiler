#include <stdio.h>

#include "microcompiler.h"

int main(void){
	void * Code;
	funcp * fun;

	FILE *fil;
	fil = fopen("SB.txt" , "w+");

	gera(fil, &Code, fun);

	fputs("CONTEUDO",fil);
	fclose(fil);
}