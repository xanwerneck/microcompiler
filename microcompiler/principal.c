/* Alexandre Werneck 1320614 3WA */
/* Stephano 3WA */

#include <stdio.h>
#include <stdlib.h>

#include "microcompiler.h"

/*
int main(void){
	void * Code;
	funcp * fun;

	FILE *fil;
	fil = fopen("retorna_constante.txt" , "w+");

	gera(fil, &Code, fun);

	fputs("CONTEUDO",fil);
	fclose(fil);
}
*/
static void error (const char *msg, int line) {
  fprintf(stderr, "Erro: %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
}

int main (void) {
  int line = 1;
  int  c;
  FILE *myfp;

  if ((myfp = fopen ("retorna_constante.txt", "r")) == NULL) {
    perror ("nao conseguiu abrir arquivo!");
    exit(EXIT_FAILURE);
  }

  while ((c = fgetc(myfp)) != EOF) {
    switch (c) {
      case 'f': {  /* function */
        char c0;
        if (fscanf(myfp, "unction%c", &c0) != 1) error("comando invalido", line);
        printf("function\n");
        break;
      }
      case 'e': {  /* end */
        char c0;
        if (fscanf(myfp, "nd%c", &c0) != 1) error("comando invalido", line);
        printf("end\n");
        break;
      }
      case 'v': case 'p': {  /* atribuicao */
        int i0;
        char v0 = c, c0;

        if (fscanf(myfp, "%d = %c", &i0, &c0) != 2)
          error("comando invalido", line);

        if (c0 == 'c') { /* call */
          int f, i1;
          char v1;
          if (fscanf(myfp, "all %d %c%d", &f, &v1, &i1) != 3) 
            error("comando invalido", line);
          printf("%c%d = call %d %c%d\n", v0, i0, f, v1, i1);
        }
        else { /* operacao aritmetica */
          int i1, i2;
          char v1 = c0, v2, op;
          if (fscanf(myfp, "%d %c %c%d", &i1, &op, &v2, &i2) != 4)
            error("comando invalido", line);
          printf("%c%d = %c%d %c %c%d\n", v0, i0, v1, i1, op, v2, i2);
        }
        break;
      }
      case 'r': {  /* ret */
        int i0, i1;
        char v0, v1;
        if (fscanf(myfp, "et? %c%d %c%d", &v0, &i0, &v1, &i1) != 4)
           error("comando invalido", line);
        printf("ret? %c%d %c%d\n", v0, i0, v1, i1);
        break;
      }
      default: error("comando desconhecido", line);
    }
    line ++;
    fscanf(myfp, " ");
  }
  return 0;
}
