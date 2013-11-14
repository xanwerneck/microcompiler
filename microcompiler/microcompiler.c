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

void traduz (char * entrada , int * posicao, char * resposta)
{

/* posicao deve ser incrementado pela funcao */
	
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


void gera(FILE *mfile, void **code, funcp *entry)
{

	unsigned char inicio[] = {0x55, 0x89, 0xe5, 0x83, 0xec, 0x50};

	while ((c = fgetc(mfile)) != EOF) {
		switch (c) {
		  case 'f': {
			if (fscanf(myfp, "unction") != 0){ 
			  puts("Comando Invalido");
			}
		  }
		  case 'r':{
		    /* trata retorno */
		  }
		  case 'p':{
			/* trata parametro */
		  }
		  case 'v':{
			/* trata variavel */
		  }
		  case '$':{
			/* trata constante */
		  }
	}
	return (funcp)cod_maquina;
	
}

void gera_retorno(unsigned char * cod_maquina)
{
/* preenche o vetor que vai retornar */
}
 
