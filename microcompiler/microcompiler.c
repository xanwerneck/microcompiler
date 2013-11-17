/* Alexandre Werneck 1320614 3WA */
/* Stefano 3WA */

#include <stdio.h>
#include <stdlib.h>  
#include <string.h>

#include "microcompiler.h"

int monta_array(unsigned char * my_array ,unsigned char * to_add , int posic_array , int qtde );

void error(char * Mensagem, int line);

void error(char * Mensagem, int line){
	printf("Erro: %s na linha %d\n", Mensagem , line );
}

void libera(void *p)
{
	free(p);
}

void gera(FILE *f, void **code , funcp * entry)
{ 

	unsigned char * my_array;
	int c, line =1;
	int posic_array = 0;
	funcp aqui;
	int teste2;

	unsigned char begin[3] = {0x55, 0x89, 0xe5};

	unsigned char teste[12] = {0x55,0x89,0xe5,0xb8,0x01,0x00,0x00,0x00,0x89,0xec,0x5d,0xc3};

	unsigned char fim[4] = {0x89, 0xec, 0x5d, 0xc3};
	
	unsigned char ret[1] = {0xb8};

	/* Monta o array incial */
	my_array = (unsigned char*) malloc (2048 * sizeof(unsigned char));

	posic_array = monta_array( my_array , begin , posic_array , 3 );	
	
	while ((c = fgetc(f)) != EOF) {
	    switch (c) {
	      case 'f': {  /* function */
		char c0;
		if (fscanf(f, "unction%c", &c0) != 1) error("comando invalido", line);
		//printf("function\n");
		break;
	      }
	      case 'e': {  /* end */
		char c0;
		if (fscanf(f, "nd%c", &c0) != 1) error("comando invalido", line);
		//printf("end\n");
		break;
	      }
	      case 'v': case 'p': {  /* atribuicao */
		int i0;
		char v0 = c, c0;

		if (fscanf(f, "%d = %c", &i0, &c0) != 2)
		  error("comando invalido", line);

		if (c0 == 'c') { /* call */
		  int fil, i1;
		  char v1;
		  if (fscanf(f, "all %d %c%d", &fil, &v1, &i1) != 3) 
		    error("comando invalido", line);
		  //printf("%c%d = call %d %c%d\n", v0, i0, fil, v1, i1);
		}
		else { /* operacao aritmetica */
		  int i1, i2;
		  char v1 = c0, v2, op;
		  if (fscanf(f, "%d %c %c%d", &i1, &op, &v2, &i2) != 4)
		    error("comando invalido", line);
		  //printf("%c%d = %c%d %c %c%d\n", v0, i0, v1, i1, op, v2, i2);
		}
		break;
	      }
	      case 'r': {  /* ret */
			int i0=0, i1=0;
			char v0, v1;
			if (fscanf(f, "et? %c%d %c%d", &v0, &i0, &v1, &i1) != 4)
			   error("comando invalido", line);
			if(i0 == 0){
				if(v1=='$'){
					my_array[posic_array] = ret[0];
					posic_array++;
					*( (int *) &my_array[posic_array] ) = i1;
					posic_array += 4;						
				}
				if(v1=='p'){

				}
				if(v1=='v'){

				}
			}else{
				
			}
			//printf("ret? %c%d %c%d\n", v0, i0, v1, i1);
		break;
	      }
	      default: error("comando desconhecido", line);
	    }
	    line ++;
	    fscanf(f, " ");
	  }


	posic_array = monta_array(my_array, fim, posic_array, 4);		

	*code = my_array;

	aqui = (funcp)teste;
	printf("Valor: %x" , teste[12]);
	//teste2 = (*aqui)();
	printf("Valor: %d" , teste2);
	
}

int monta_array(unsigned char * my_array ,unsigned char * to_add , int posic_array , int qtde )
{
	int i =0;
	while(i < qtde)
	{
		my_array[posic_array] = to_add[i];
                posic_array++;
		i++;
	}
	return posic_array;
}



 

 
