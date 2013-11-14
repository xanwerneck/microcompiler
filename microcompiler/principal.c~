/* Alexandre Werneck 1320614 3WA */
/* Stefano 3WA */

#include <stdio.h>
#include <stdlib.h>

#include "microcompiler.h"


int main(void){
	void * Code;
	funcp * fun;

	FILE *f;
  if ((f = fopen ("retorna_constante.txt", "r")) == NULL) {
    perror ("nao conseguiu abrir arquivo!");
    exit(EXIT_FAILURE);
  }

	gera(f, &Code, fun);

	fclose(f);
}

static void error (const char *msg, int line) {
  fprintf(stderr, "Erro: %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
}


