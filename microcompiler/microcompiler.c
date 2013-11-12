/* Alexandre Werneck 1320614 3WA */
/* Stefano 3WA */

#include <stdio.h>
#include <stdlib.h>  

typedef int (*funcp) ();

static void error (const char *msg, int line) {
  fprintf(stderr, "Erro: %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
}

void libera(void *p)
{

}

char * traduzAss (char * entrada)
{
}

void traduz (char * entrada , int * posicao, char * resposta)
{
	
}

void gera(FILE *f, void **code, funcp *entry)
{

	char * temp, * resposta;
	int max_buff = 1000;
	int tam_resp=0, cont=0;
	
	while(!feof(out_ass)){				//Le o arquivo assembly.
		fgets(temp, max_buff , out_ass);
		traduz(temp, &cont, resposta);
	}

		
	*code = &resposta;
	
}



 
