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
	unsigned char * my_array[10];
	int c, line =1, i=0;
	int posic_array = 0;

	unsigned char begin[3] = {0x55, 0x89, 0xe5};
	unsigned char fim[4]   = {0x89, 0xec, 0x5d, 0xc3};	

	unsigned char ret[1] = {0xb8};		
	
	while ((c = fgetc(f)) != EOF) {
	    switch (c) {
	      case 'f': {  /* function */
				char c0;
				if (fscanf(f, "unction%c", &c0) != 1) error("comando invalido", line);

				my_array[i] = (unsigned char*) malloc (128 * sizeof(unsigned char)); /* aloca espaco */

				posic_array = monta_array( my_array[i] , begin , posic_array , 3 );

				unsigned char temp[] = {0x83, 0xec, 0x20};  /* Inicializa as variaveis */
				posic_array = monta_array (my_array[i], temp, posic_array, 3);

				break;
			  }
		  case 'e': {  /* end */
				char c0;
				if (fscanf(f, "nd%c", &c0) != 1) error("comando invalido", line);
					posic_array = monta_array(my_array[i], fim, posic_array, 4);	
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
				  
				  if(v1 == 'p'){ 

					  if(v0 == 'p'){ /* param = param + $ */

						  if(op == '+'){
							  unsigned char param2[] = {0x81, 0x45};
						      posic_array = monta_array (my_array[i], param2, posic_array, 2);
							  
							  my_array[i][posic_array] = (i1 * 4) + 8;			
							  posic_array++;

							  *( (int *) &my_array[i][posic_array] ) = i2;
							  posic_array += 4;		
						  }
						  if(op == '-'){

							  unsigned char param2[] = {0x81, 0x6d};
						      posic_array = monta_array (my_array[i], param2, posic_array, 2);
							  
							  my_array[i][posic_array] = (i1 * 4) + 8;			
							  posic_array++;

							  *( (int *) &my_array[i][posic_array] ) = i2;
							  posic_array += 4;	

						  }
						  if(op == '*'){

							 /* falta implementação */

						  }
						 

					  }else{ /* param = var + $ */
						  
						if(op == '+'){
							  unsigned char param2[] = {0x8b ,0x55};
						      posic_array = monta_array (my_array[i], param2, posic_array, 2);
							  
							  my_array[i][posic_array] = (i1 * 4) + 8;			
							  posic_array++;

							  unsigned char param21[] = {0x81, 0xc2};
						      posic_array = monta_array (my_array[i], param21, posic_array, 2);

							  *( (int *) &my_array[i][posic_array] ) = i2;
							  posic_array += 4;	

							  unsigned char param3[] = {0x89, 0x55, 0xfc}; /* 0xfc é a primeira variavel, tem que corrigir */
						      posic_array = monta_array (my_array[i], param3, posic_array, 3);	
						  }
						  if(op == '-'){

							  unsigned char param2[] = {0x8b ,0x55};
						      posic_array = monta_array (my_array[i], param2, posic_array, 2);
							  
							  my_array[i][posic_array] = (i1 * 4) + 8;			
							  posic_array++;

							  unsigned char param21[] = {0x81, 0xea};
						      posic_array = monta_array (my_array[i], param21, posic_array, 2);

							  *( (int *) &my_array[i][posic_array] ) = i2;
							  posic_array += 4;	

							  unsigned char param3[] = {0x89, 0x55, 0xfc}; /* 0xfc é a primeira variavel, tem que corrigir */
						      posic_array = monta_array (my_array[i], param3, posic_array, 3);

						  }
						  if(op == '*'){

							 /* falta implementação */

						  }

					  }

				  }else if(v1 == 'v'){ 

					  if(v0 == 'p'){ /* var = param + $ */

						  if(op == '+'){
							  unsigned char param2[] = {0x81, 0x45};
						      posic_array = monta_array (my_array[i], param2, posic_array, 2);
							  
							  my_array[i][posic_array] = (i1 * 4) + 8;			
							  posic_array++;

							  *( (int *) &my_array[i][posic_array] ) = i2;
							  posic_array += 4;		
						  }
						  if(op == '-'){
							  
							  unsigned char param2[] = {0x8b ,0x55};
						      posic_array = monta_array (my_array[i], param2, posic_array, 2);
							  
							  my_array[i][posic_array] = (i1 * 4) + 8;			
							  posic_array++;

							  unsigned char param21[] = {0x2d, 0xe0};
						      posic_array = monta_array (my_array[i], param21, posic_array, 2);

							  *( (int *) &my_array[i][posic_array] ) = i2;
							  posic_array += 4;	

							  unsigned char param22[] = {0x89 ,0x55};
						      posic_array = monta_array (my_array[i], param22, posic_array, 2);

							  my_array[i][posic_array] = 0xfc;	
							  posic_array++;

						  }
						  if(op == '*'){

							 /* falta implementação */

						  }
						 

					  }else{ /* var = var + $ */

						   if(op == '+'){
							  unsigned char param3[] = {0x8b, 0x4d, 0xfc}; /* 0xfc é a primeira variavel, tem que corrigir */
						      posic_array = monta_array (my_array[i], param3, posic_array, 3);

							  unsigned char param21[] = {0x81, 0xc1};
						      posic_array = monta_array (my_array[i], param21, posic_array, 2);

							  *( (int *) &my_array[i][posic_array] ) = i2;
							  posic_array += 4;	

							  unsigned char param31[] = {0x89, 0x4d, 0xfc}; /* 0xfc é a primeira variavel, tem que corrigir */
						      posic_array = monta_array (my_array[i], param31, posic_array, 3);
						  }
						  if(op == '-'){
							  
							  unsigned char param3[] = {0x8b, 0x4d, 0xfc}; /* 0xfc é a primeira variavel, tem que corrigir */
						      posic_array = monta_array (my_array[i], param3, posic_array, 3);

							  unsigned char param21[] = {0x81, 0xe9};
						      posic_array = monta_array (my_array[i], param21, posic_array, 2);

							  *( (int *) &my_array[i][posic_array] ) = i2;
							  posic_array += 4;	

							  unsigned char param31[] = {0x89, 0x4d, 0xfc}; /* 0xfc é a primeira variavel, tem que corrigir */
						      posic_array = monta_array (my_array[i], param31, posic_array, 3);

						  }
						  if(op == '*'){

							 /* falta implementação */

						  }

					  }

				  }

				}
				break;
	      }
	      case 'r': {  /* ret */
			int i0=0, i1=0;
			char v0, v1;
			if (fscanf(f, "et? %c%d %c%d", &v0, &i0, &v1, &i1) != 4)
			   error("comando invalido", line);
			if(v0 == '$'){
				if(i0 == 0){
					if(v1=='$'){
						my_array[i][posic_array] = ret[0]; /* condição de retorno  - 0xb8 */
						posic_array++;                 
						*( (int *) &my_array[i][posic_array] ) = i1;
						posic_array += 4;						
					}
					if(v1=='p'){
						unsigned char param4[] = {0x89, 0xe9, 0x83, 0xc1};

						if(v1 == 'v'){
							unsigned char param4[] = {0x89, 0xe9, 0x83, 0xe9};
						}

						posic_array = monta_array (my_array[i], param4, posic_array, 4);

						if(v1 == 'p'){
							my_array[i][posic_array] = (i1 * 4) + 8;			/* Deslocamento de EBP para parametro */
						}else{
							my_array[i][posic_array] = (i1 * 4) + 4;			/* Deslocamento de EBP para var */
						}
						posic_array++;

						unsigned char param2[] = {0x8b, 0x01};
						posic_array = monta_array (my_array[i], param2, posic_array, 2);

					}
					if(v1 == 'v'){

						unsigned char param3[] = {0x8b, 0x45, 0xfc};
						posic_array = monta_array (my_array[i], param3, posic_array, 3);

					}
				}
			}
			else if(v0 == 'p'){
				unsigned char param2[] = {0x83, 0x7d};
				posic_array = monta_array (my_array[i], param2, posic_array, 2);
				my_array[i][posic_array] = (i0 * 4) + 8;			/* Deslocamento de EBP para parametro */
				posic_array++;
				my_array[i][posic_array] = 0x00;
				posic_array++;

				if(v1=='$'){
					unsigned char param2[] = {0x74, 0x00};
					posic_array = monta_array (my_array[i], param2, posic_array, 2);  
					
					my_array[i][posic_array] = ret[0]; /* condição de retorno  - 0xb8 */
					posic_array++;                 
					*( (int *)&my_array[i][posic_array] ) = i1;
					posic_array += 4;

				}
				if(v1=='p' || v1 == 'v'){
					unsigned char param4[] = {0x89, 0xe9, 0x83, 0xc1};

					if(v1 == 'v'){
						unsigned char param4[] = {0x89, 0xe9, 0x83, 0xe9};
					}

					posic_array = monta_array (my_array[i], param4, posic_array, 4);

					if(v1 == 'p'){
						my_array[i][posic_array] = (i1 * 4) + 8;			/* Deslocamento de EBP para parametro */
					}else{
						my_array[i][posic_array] = (i1 * 4) + 4;			/* Deslocamento de EBP para var */
					}
					posic_array++;

					unsigned char param2[] = {0x8b, 0x01};
					posic_array = monta_array (my_array[i], param2, posic_array, 2);

				}

			}
			//printf("ret? %c%d %c%d\n", v0, i0, v1, i1);
		break;
	      }
	      default: error("comando desconhecido", line);
	    }
	    line ++;
	    fscanf(f, " ");
	  }


	*code = my_array[i];

	*entry = (funcp)my_array[i];

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




 

 
