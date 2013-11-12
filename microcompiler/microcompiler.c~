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

char * traduzMaq (char * entrada)
{
}

void gera(FILE *f, void **code, funcp *entry)
{
	FILE * out_ass, *out_maq;
	char * temp, * resposta;
	char tempc;
	int max_buff = 1000;
	int i,tam_resp;
	
	out_ass = fopen("saida.s", "wr");		//Cria o arquivo assembly.
	if (out_ass==NULL)
	{
		printf("Erro ao abrir o arquivo de saida assembly\n");
		exit(1);
	}

	out_maq = fopen("saida.txt", "wr");		//Cria o arquivo codigo de maquina.
	if (out_maq==NULL)
	{
		printf("Erro ao abrir o arquivo de saida codigo de maquina\n");
		exit(1);
	}
	
	while(!feof(f)){					//Le o arquivo de entrada.
		fgets(temp, max_buff , f);
		resposta = traduzAss(temp);
		fprintf(out_ass, resposta);		//Gera o arquivo assembly.
	}
	
	while(!feof(out_ass)){				//Le o arquivo assembly.
		fgets(temp, max_buff , out_ass);
		resposta = traduzMaq(temp);
		fprintf(out_maq, resposta);		//Gera o arquivo cosdigo de maquina.
	}

	for (i=0; fscanf(out_maq,"%c",&tempc)==1;i++)	//Descobre o tamanho de memoria para o codigo de maquina.
	{
		while(tempc=='\n' || tempc==' ')
			fscanf(out_maq,"%c",&tempc);
	}
	
	tam_resp = i/2;							//Tamanho do vetor resposta = i/2.

	resposta = (char *)malloc(tam_resp*sizeof(char));		//Aloca o resposta.
	
	if(resposta==NULL)
	{
		printf("Erro de memoria\n");
		exit(1);
	}

	for(i=0;i<tam_resp; i+=2)
	{
		fscanf(out_maq, " %2[^\n]",resposta[i]);		//Le o arquivo codigo de maquina e coloca no string resposta.
	}
	
	*code = &resposta;
	
}



 
