/* Alexandre Werneck 1320614 3WA */
/* Stefano 3WA */

#include <stdio.h>
#include <stdlib.h>  
#include <string.h>




typedef int (*funcp) ();

struct registrador{
	char name[2];
	int state;
};typedef struct registrador Registrador;

static void error (const char *msg, int line) {
  fprintf(stderr, "Erro: %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
}

void libera(void *p)
{

}
	/*edx será guardado para fazer comparacoes*/
	/* posicao deve ser incrementado pela funcao */
	
	/*int eax = 0;
	int ecx = 1;
	int edx = 2;
	int ebx = 3;
	int esp = 4;
	int ebp = 5;
	int i;
	char  strtemp[20];
	//char mov[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	char ret[10] = {0x83, 0xf0, 0x00, 0x75, 0x05, 0xb8, 0x00, 0x00, 0x00, 0x0};	//ret[1] varia de acordo com o reg. ret[2] varia de acorodo com o que esta sendo comparado.
	*/
	

void gera(FILE *f, void **code, funcp *entry)
{

	char resposta[1000], temp[100];
	int max_buff = 1000;
	int tam_resp=0, cont=0;
	char c;

	Registrador eax, ebx, ecx, edx;

	char fun[3] = {0x55, 0x89, 0xe5};
	char fim[1] = {0xc3};
	
	
	
	while(!feof(f)){				//Le o arquivo de entrada.
		
		fgets(temp, max_buff , f);

		c = temp[0];

		switch (c){
			case 'f':
				strcat(resposta,fun);
			case 'e':
				strcat(resposta,fim);
				eax.state = ebx.state = ecx.state = edx.state = 0;
			case 'r':

		}
	}

		
	*code = &resposta;
	
}



 

 
