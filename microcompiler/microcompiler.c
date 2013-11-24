/* 
*
* TRABALHO - Software Básico - 2013.2 - 3WA
* Alunos:
* Alexandre Werneck 1320614
* Stefano
*
*/

#include <stdio.h>
#include <stdlib.h>  
#include <string.h>

#include "microcompiler.h"

#define MAX_FUNC 10

/***** Variaveis encapsuladas no modulo ******/	
static unsigned char * TodasFuncoes;
static unsigned char * my_array[MAX_FUNC];

/**** Código das funções encapsuladas pelo módulo *****/

/*
* Monta o array para uma função definida no vetor de funcoes
*/
static int monta_array(unsigned char * my_array ,unsigned char * to_add , int posic_array , int qtde );

/*
* Remonta o deslocamento para retorno da função em questão
*/
static void remontaRetorno(unsigned char * my_array , int contador , int posicao[][3] );

/*
* Apresenta a posição e mensagem de erro na manipulação de arquivo
*/
static void error(char * Mensagem, int line);

/*
* Monta todas as funções em único array que será passado como parâmetro
*/
static int AdicionaFuncoes(unsigned char * Todas, unsigned char * my_array, int inic_array, int fim_array, int posic_geral);

/*
* Desaloca todos os espacos de my_array
*/
static void DesalocaFuncoesArray(unsigned char ** my_array);

/*** Código das funções principais de manipulação ****/


void libera(void *p)
{
	if(my_array != NULL){
		DesalocaFuncoesArray(my_array);
	}
	if(p != NULL){
		free(p);
	}

}

void gera(FILE *f, void **code , funcp * entry)
{ 

	int posic_func_ar[MAX_FUNC];
	int c, line =1, i=0;
	int posic_array = 0;
	int posic_ret[MAX_FUNC][3];
	int in_retorno  = 0;
	int posic_geral = 0;

	/* Montagem da pilha no início da função */
	unsigned char inicio[3]        = {0x55, 0x89, 0xe5};

	/* Montagem da pilha no fim da função */
	unsigned char fim[4]           = {0x89, 0xec, 0x5d, 0xc3};	

	/* Inicialização de até 10 variáveis locais na pilha */
	unsigned char alocaVarLocais[] = {0x83, 0xec, 0x40};  

	/* Variaveis Gerais */
	unsigned char MovAtribuir[] = {0x89, 0x55};	 
	unsigned char MovEdx[]      = {0x8b ,0x55};
	unsigned char ecxMaisEdx[]  = {0x01, 0xca};
	unsigned char MovEcx[]      = {0x8b, 0x4d};
	unsigned char ecxMenosEdx[] = {0x29, 0xca};
	unsigned char ecxVezesEdx[] = {0x0f, 0xaf, 0xd1};

	TodasFuncoes = (unsigned char*) malloc (1280 * sizeof(unsigned char)); 
		/* Posição de memória que vai receber o código de todas as funções geradas */

	while ((c = fgetc(f)) != EOF) {
	    switch (c) {
	      case 'f': {  /* function */
				if (fscanf(f, "unction") != 0) error("comando invalido", line);

				my_array[i]      = (unsigned char*) malloc (128 * sizeof(unsigned char)); 
				
				posic_ret[0][0]  = 5;
				posic_func_ar[i] = posic_array;

				posic_array      = monta_array( my_array[i] , inicio , posic_array , 3 );
				posic_array      = monta_array (my_array[i], alocaVarLocais, posic_array, 3);

				break;
			  }
		  case 'e': {  /* end */

				if (fscanf(f, "nd") != 0) error("comando invalido aqui", line);

				posic_array = monta_array(my_array[i], fim, posic_array, 4);

				if(in_retorno > 0){
					remontaRetorno(my_array[i], in_retorno, posic_ret);
				}

				posic_geral = AdicionaFuncoes(TodasFuncoes , my_array[i], posic_func_ar[i], posic_array, posic_geral);

				/* Reinicializa variaveis para novas funcoes */
				posic_array = 0;
				in_retorno = 0;

				i++;
				
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

				  if(v0 == 'v' || v0 == 'p'){
					 
                    	int val=0;
						unsigned char param2[] = {0xff, 0x75};
						unsigned char param5[] = {0x83, 0xc4, 0x04, 0x89, 0x45};

					    posic_array = monta_array (my_array[i], param2, posic_array, 2);

						if(v1 == 'v'){
							my_array[i][posic_array] = -((i1+1) * 4);
						}else{
							my_array[i][posic_array] = (i1 * 4) + 8;
						}
						posic_array++;
												
						my_array[i][posic_array] = 0xe8;
						posic_array++;

						*( (int *) &my_array[i][posic_array] ) = 0;
						posic_array += 4;

						val = (int)&my_array[fil][posic_func_ar[fil]] - (int)&my_array[i][posic_array];
						
						my_array[i][posic_array-1]   = val >> 24;
						my_array[i][posic_array-2]   = val >> 16;
						my_array[i][posic_array-3]   = val >> 8;
						my_array[i][posic_array-4]   = val;	
						
					    posic_array = monta_array (my_array[i], param5, posic_array, 5);

						if(v0 == 'v'){
							my_array[i][posic_array] = -((i0+1) * 4);
						}else{
							my_array[i][posic_array] = (i0 * 4) + 8;
						}
						posic_array++;

						if(in_retorno > 0){
							if(v1 == 'v'){
								posic_ret[in_retorno][0] += 19;
							}else if(v1 == 'p'){
								posic_ret[in_retorno][0] += 14;
							}
						}

				  }

				  
				}
				else { /* operacao aritmetica */
				  int i1, i2;
				  char v1 = c0, v2, op;
				  if (fscanf(f, "%d %c %c%d", &i1, &op, &v2, &i2) != 4)
					error("comando invalido", line);

					/* INICIO = Soma */
					if(op == '+'){

						/* Tratamento para o parametro v1 */
						if(v1 == '$'){

							my_array[i][posic_array] = 0xba;
						    posic_array++;

							*( (int *) &my_array[i][posic_array] ) = i1;
							posic_array += 4;

						}else if(v1 == 'v' || v1 == 'p'){
							posic_array = monta_array (my_array[i], MovEdx, posic_array, 2);
							  
							if(v1=='p'){
									my_array[i][posic_array] = (i1 * 4) + 8;
							}else{
									my_array[i][posic_array] = -((i1+1) * 4);
							}

							posic_array++;
						}

						/* Tratamento para o parametro v2 */
						if(v2 == '$'){

							unsigned char param2[] = {0x81, 0xc2};	
							posic_array = monta_array (my_array[i], param2, posic_array, 2);
								  
							*( (int *) &my_array[i][posic_array] ) = i2;
							posic_array += 4;

						}else if(v2 == 'v' || v2 == 'p'){

							posic_array = monta_array (my_array[i], MovEcx, posic_array, 2);

							if(v2 == 'v'){
								my_array[i][posic_array] = -((i2+1) * 4);
							}else if(v2 == 'p'){
								my_array[i][posic_array] = (i2 * 4) + 8;
							}							
							posic_array++;

							posic_array = monta_array (my_array[i], ecxMaisEdx, posic_array, 2);
								  
						}

						posic_array = monta_array (my_array[i], MovAtribuir, posic_array, 2);	

						if(v0=='p')															
							my_array[i][posic_array] = (i0 * 4) + 8;
						else
							my_array[i][posic_array] = -((i0+1) * 4);

						posic_array++;

						if(in_retorno > 0){
							if(v2 == '$'){
								posic_ret[in_retorno][0] += 12;
							}else{
								posic_ret[in_retorno][0] += 11;
							}
						}

					}
					/* FIM = Soma */

					/* INICIO = Subtracao */
					if(op == '-'){ 	

						/* Tratamento para o parametro v1 */
						if(v1 == '$'){

							my_array[i][posic_array] = 0xba;
						    posic_array++;

							*( (int *) &my_array[i][posic_array] ) = i1;
							posic_array += 4;

						}else{
							posic_array = monta_array (my_array[i], MovEdx, posic_array, 2);
							  
							if(v1=='p'){
									my_array[i][posic_array] = (i1 * 4) + 8;							//Passa v1 pro registrador edx
							}else{																			//Se v1 for uma variavel
									my_array[i][posic_array] = -((i1+1) * 4);									//Passa v1 pro registrador edx
							}
						
							posic_array++;

						}

						/* Tratamento para o parametro v2 */
						if(v2 == '$'){

							unsigned char param2[] = {0x81, 0xea};									//Faz a subtracao.
							posic_array = monta_array (my_array[i], param2, posic_array, 2);
								  
							*( (int *) &my_array[i][posic_array] ) = i2;
							posic_array += 4;	

						}else if(v2 == 'v' || v2 == 'p'){
							
							posic_array = monta_array (my_array[i], MovEcx, posic_array, 2);

							if(v2 == 'v'){
								my_array[i][posic_array] = -((i2+1) * 4);
							}else if(v2 == 'p'){
								my_array[i][posic_array] = (i2 * 4) + 8;
							}							
							posic_array++;
															
							posic_array = monta_array (my_array[i], ecxMenosEdx, posic_array, 2);		//Subtrai ecx com edx
								  
						}

						posic_array = monta_array (my_array[i], MovAtribuir, posic_array, 2);	

						if(v0=='p')															
							my_array[i][posic_array] = (i0 * 4) + 8;
						else
							my_array[i][posic_array] = -((i0+1) * 4);

						posic_array++;

						if(in_retorno > 0){
							if(v2 == '$'){
								posic_ret[in_retorno][0] += 12;
							}else{
								posic_ret[in_retorno][0] += 11;
							}
						}

					}
					/* FIM = Subtracao */

					/* INICIO = Multipicacao */
					if(op == '*'){

						/* Tratamento para o parametro v1 */
						if(v1 == '$'){

							my_array[i][posic_array] = 0xba;
						    posic_array++;

							*( (int *) &my_array[i][posic_array] ) = i1;
							posic_array += 4;

						}else{
							posic_array = monta_array (my_array[i], MovEdx, posic_array, 2);
							  
							if(v1=='p'){
									my_array[i][posic_array] = (i1 * 4) + 8;							//Passa v1 pro registrador edx	
							}else{																			//Se v1 for uma variavel
									my_array[i][posic_array] = -((i1+1) * 4);									//Passa v1 pro registrador edx
							}
							posic_array++;
						}

						/* Tratamento para o parametro v2 */
						if(v2 == '$'){

							unsigned char param2[] = {0x69, 0xd2};									//Faz a multiplicacao.
							posic_array = monta_array (my_array[i], param2, posic_array, 2);
								  
							*( (int *) &my_array[i][posic_array] ) = i2;
							posic_array += 4;	

						}else if(v2 == 'v' || v2 == 'p'){

							posic_array = monta_array (my_array[i], MovEcx, posic_array, 2);

							if(v2 == 'v'){
								my_array[i][posic_array] = -((i2+1) * 4);
							}else if(v2 == 'p'){
								my_array[i][posic_array] = (i2 * 4) + 8;
							}							
							posic_array++;
															
							posic_array = monta_array (my_array[i], ecxVezesEdx, posic_array, 3);		//Multiplica ecx com edx
								  
						}

						posic_array = monta_array (my_array[i], MovAtribuir, posic_array, 2);	

						if(v0=='p')															
							my_array[i][posic_array] = (i0 * 4) + 8;
						else
							my_array[i][posic_array] = -((i0+1) * 4);

						posic_array++;

						if(in_retorno > 0){
							posic_ret[in_retorno][0] += 12;
						}

					}
					/* FIM = Multipicacao */

				}
				break;
	      }
	      case 'r': {  /* ret */
			int i0=0, i1=0;
			char v0, v1;
			in_retorno++;

			posic_ret[in_retorno][0] = 0;
			

			if (fscanf(f, "et? %c%d %c%d", &v0, &i0, &v1, &i1) != 4)
			   error("comando invalido", line);


			if(v0 == '$'){
				if(v1=='$'){

					my_array[i][posic_array] = 0xba; 
					posic_array++;  

					*( (int *) &my_array[i][posic_array] ) = i0;
					posic_array += 4;	

					my_array[i][posic_array] = 0xb8;
					posic_array++;       

					*( (int *) &my_array[i][posic_array] ) = i1;
					posic_array += 4;	
					
					unsigned char param2[] = {0x83, 0xfa, 0x00, 0x74};
				    posic_array = monta_array (my_array[i], param2, posic_array, 4);
						
					my_array[i][posic_array]   = posic_ret[in_retorno-1][0];
					posic_ret[in_retorno-1][1] =  posic_array;
					posic_array++;

					unsigned char param5[] = {0xb8, 0x00, 0x00, 0x00, 0x00}; /* Move 0 para eax retornar 0 */
					posic_array = monta_array (my_array[i], param5, posic_array, 5);

					if(in_retorno > 0){

						posic_ret[in_retorno][0] += 20;

					}

					posic_ret[in_retorno][0] += 5;
				}
				if(v1=='p'){

					my_array[i][posic_array] = 0xba; /* condição de retorno  - 0xba */
					posic_array++;                 
					*( (int *) &my_array[i][posic_array] ) = i0;
					posic_array += 4;	

					unsigned char param2[] = {0x8b, 0x45};
				    posic_array = monta_array (my_array[i], param2, posic_array, 2);

					my_array[i][posic_array] = (i1 * 4) + 8;
					posic_array++;
					
					unsigned char param4[] = {0x83, 0xfa, 0x00, 0x74};
				    posic_array = monta_array (my_array[i], param4, posic_array, 4);
						
					my_array[i][posic_array] = posic_ret[in_retorno-1][0];
					posic_ret[in_retorno-1][1] =  posic_array;
					posic_array++;

					unsigned char param5[] = {0xb8, 0x00, 0x00, 0x00, 0x00}; /* Move 0 para eax retornar 0 */
					posic_array = monta_array (my_array[i], param5, posic_array, 5);
					
					if(in_retorno > 0){
						posic_ret[in_retorno][0] += 18;
					}

					posic_ret[in_retorno][0] += 7;

				}
				if(v1 == 'v'){

					my_array[i][posic_array] = 0xba; /* condição de retorno  - 0xba */
					posic_array++;                 
					*( (int *) &my_array[i][posic_array] ) = i0;
					posic_array += 4;	

					unsigned char param2[] = {0x8b, 0x45};
				    posic_array = monta_array (my_array[i], param2, posic_array, 2);

					my_array[i][posic_array] = -((i1+1) * 4);
					posic_array++;
					
					unsigned char param4[] = {0x83, 0xfa, 0x00, 0x74};
				    posic_array = monta_array (my_array[i], param4, posic_array, 4);
						
					my_array[i][posic_array] = posic_ret[in_retorno-1][0];
					posic_ret[in_retorno-1][1] =  posic_array;
					posic_array++;

					unsigned char param5[] = {0xb8, 0x00, 0x00, 0x00, 0x00}; /* Move 0 para eax retornar 0 */
					posic_array = monta_array (my_array[i], param5, posic_array, 5);

					if(in_retorno > 0){
						posic_ret[in_retorno][0] += 18;
					}

					posic_ret[in_retorno][0] += 8;

				}
				
			}
			else if(v0 == 'p'){

				if(v1=='$'){

					my_array[i][posic_array] = 0xb8; /* condição de retorno  - 0xb8 */
					posic_array++;                 
					*( (int *) &my_array[i][posic_array] ) = i1;
					posic_array += 4;	

					unsigned char param2[] = {0x83, 0x7d};
				    posic_array = monta_array (my_array[i], param2, posic_array, 2);

					my_array[i][posic_array] = (i0 * 4) + 8;
					posic_array++;
					
					unsigned char param4[] = {0x00, 0x74};
				    posic_array = monta_array (my_array[i], param4, posic_array, 2);
						
					my_array[i][posic_array] = posic_ret[in_retorno-1][0];
					posic_ret[in_retorno-1][1] =  posic_array;
					posic_array++;

					unsigned char param5[] = {0xb8, 0x00, 0x00, 0x00, 0x00}; /* Move 0 para eax retornar 0 */
					posic_array = monta_array (my_array[i], param5, posic_array, 5);
							
					if(in_retorno > 0){
						posic_ret[in_retorno][0] += 16;
					}

					posic_ret[in_retorno][0] += 2;

				}
				if(v1 == 'p'){
					unsigned char param2[] = {0x8b, 0x55};
				    posic_array = monta_array (my_array[i], param2, posic_array, 2);

					my_array[i][posic_array] = (i0 * 4) + 8;
					posic_array++;

					unsigned char param21[] = {0x8b, 0x45};
				    posic_array = monta_array (my_array[i], param21, posic_array, 2);

					my_array[i][posic_array] = (i1 * 4) + 8;
					posic_array++;
					
					unsigned char param4[] = {0x83, 0xfa, 0x00, 0x74};
				    posic_array = monta_array (my_array[i], param4, posic_array, 4);
						
					my_array[i][posic_array] = posic_ret[in_retorno-1][0];
					posic_ret[in_retorno-1][1] =  posic_array;
					posic_array++;

					unsigned char param5[] = {0xb8, 0x00, 0x00, 0x00, 0x00}; /* Move 0 para eax retornar 0 */
					posic_array = monta_array (my_array[i], param5, posic_array, 5);
					
					if(in_retorno > 0){
						posic_ret[in_retorno][0] += 16;
					}

					posic_ret[in_retorno][0] += 2;
				}
				if(v1 == 'v'){

					unsigned char param2[] = {0x8b, 0x55};
				    posic_array = monta_array (my_array[i], param2, posic_array, 2);

					my_array[i][posic_array] = (i0 * 4) + 8;
					posic_array++;

					unsigned char param21[] = {0x8b, 0x45};
				    posic_array = monta_array (my_array[i], param21, posic_array, 2);

					my_array[i][posic_array] = -((i1+1) * 4);
					posic_array++;
					
					unsigned char param4[] = {0x83, 0xfa, 0x00, 0x74};
				    posic_array = monta_array (my_array[i], param4, posic_array, 4);
						
					my_array[i][posic_array] = posic_ret[in_retorno-1][0];
					posic_ret[in_retorno-1][1] =  posic_array;
					posic_array++;

					unsigned char param5[] = {0xb8, 0x00, 0x00, 0x00, 0x00}; /* Move 0 para eax retornar 0 */
					posic_array = monta_array (my_array[i], param5, posic_array, 5);

					if(in_retorno > 0){
						posic_ret[in_retorno][0] += 16;
					}

					posic_ret[in_retorno][0] += 2;

				}

			}else{

				
				if(v1=='$'){

					my_array[i][posic_array] = 0xb8; /* condição de retorno  - 0xb8 */
					posic_array++;                 
					*( (int *) &my_array[i][posic_array] ) = i1;
					posic_array += 4;	

					unsigned char param2[] = {0x83, 0x7d};
				    posic_array = monta_array (my_array[i], param2, posic_array, 2);

					my_array[i][posic_array] = -((i0+1) * 4);
					posic_array++;
					
					unsigned char param4[] = {0x00, 0x74};
				    posic_array = monta_array (my_array[i], param4, posic_array, 2);
						
					my_array[i][posic_array] = posic_ret[in_retorno-1][0];
					posic_ret[in_retorno-1][1] =  posic_array;
					posic_array++;

					unsigned char param5[] = {0xb8, 0x00, 0x00, 0x00, 0x00}; /* Move 0 para eax retornar 0 */
					posic_array = monta_array (my_array[i], param5, posic_array, 5);

					if(in_retorno > 0){
						posic_ret[in_retorno][0] += 16;
					}

					posic_ret[in_retorno][0] += 9;

				}
				else if(v1 == 'p'){

					unsigned char param2[] = {0x8b, 0x55};
				    posic_array = monta_array (my_array[i], param2, posic_array, 2);

					my_array[i][posic_array] = -((i0+1) * 4);
					posic_array++;

					unsigned char param21[] = {0x8b, 0x45};
				    posic_array = monta_array (my_array[i], param21, posic_array, 2);
										
					my_array[i][posic_array] = (i1 * 4) + 8;
					posic_array++;
					
					unsigned char param4[] = {0x83, 0xfa, 0x00, 0x74};
				    posic_array = monta_array (my_array[i], param4, posic_array, 4);
						
					my_array[i][posic_array] = posic_ret[in_retorno-1][0];
					posic_ret[in_retorno-1][1] =  posic_array;
					posic_array++;

					unsigned char param5[] = {0xb8, 0x00, 0x00, 0x00, 0x00}; /* Move 0 para eax retornar 0 */
					posic_array = monta_array (my_array[i], param5, posic_array, 5);
					
					if(in_retorno > 0){
						posic_ret[in_retorno][0] += 16;
					}

					posic_ret[in_retorno][0] += 9;
				}
				else{

					unsigned char param2[] = {0x8b, 0x55};
				    posic_array = monta_array (my_array[i], param2, posic_array, 2);

					my_array[i][posic_array] = -((i0+1) * 4);
					posic_array++;

					unsigned char param21[] = {0x8b, 0x45};
				    posic_array = monta_array (my_array[i], param21, posic_array, 2);
										
					my_array[i][posic_array] = -((i1+1) * 4);
					posic_array++;
					
					unsigned char param4[] = {0x83, 0xfa, 0x00, 0x74};
				    posic_array = monta_array (my_array[i], param4, posic_array, 4);
						
					my_array[i][posic_array] = posic_ret[in_retorno-1][0];
					posic_ret[in_retorno-1][1] =  posic_array;
					posic_array++;

					unsigned char param5[] = {0xb8, 0x00, 0x00, 0x00, 0x00}; /* Move 0 para eax retornar 0 */
					posic_array = monta_array (my_array[i], param5, posic_array, 5);

					if(in_retorno > 0){
						posic_ret[in_retorno][0] += 16;
					}

					posic_ret[in_retorno][0] += 9;
				}
			}

			
		break;
	      }
	      default: error("comando desconhecido", line);
	    }
	    line ++;
	    fscanf(f, " ");
	  }


	  /* Todas as funcoes com o codigo de maquina gerado para retorno */
	*code = TodasFuncoes;

	  /* Endereco da ultima funcao do arquivo para retorno */
	*entry = (funcp)my_array[i-1];

}

/*** Código das funções auxiliares ****/

/*
* Função remontaRetorno
*
* Responsavel por colocar o deslocamento de retorno nas posicoes corretas
* esta funcao e chamada sempre ao termino de cada leitura completa de funcao.
* Valido principalmente em funcoes com mais de um retorno.
*
* Param:
* @ my_array = array contendo o código de máquina
* @ contador = número de vezes passado em ret no arquivo mainpulado
* @ posicao[][3] = vetor de posicoes dentro do array do código de máquina a serem alterados
*
* Ret:
* Não possui retorno, mas o array manipulado deve ser modificado com troca de posicoes de retorno
*
*/
static void remontaRetorno(unsigned char * my_array , int contador , int posicao[][3] )
{
	int temp = 1;
	unsigned char tempC[1];
	while(temp <= contador){

		tempC[0] = my_array[posicao[(contador-1)-(temp-1)][1]];
		my_array[posicao[(contador-1)-(temp-1)][1]] = my_array[posicao[temp-1][1]];
		my_array[posicao[temp-1][1]] = tempC[0];
		temp++;
		contador--;

	}
}

/*
* Função monta_array
*
* Monta o codigo de maquina dentro do array especifico de uma funcao lida no programa
* A quantidade informa o numero de elementos a serem adicionados no array e o parametro
* posic_array informa a primeira posicao a ser adicionado o elemento char. O retorno
* e a proxima posicao a ser utilizada neste array.
*
* Param:
* @ my_array = array contendo o código de máquina
* @ to_add = array contendo itens a serem adicionados em my_array
* @ posic_array = Inteiro contendo a posição atual do array
* @ qtde = quantidade de itens a serem adicionados em my_array
*
* Ret:
* @ posic_array = Nova posição do array my_array
*
*/

static int monta_array(unsigned char * my_array ,unsigned char * to_add , int posic_array , int qtde )
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

/*
* Função error
*
* Apresenta uma mensagem de erro caso seja detectado algum erro na
* montagem do codigo com a respectiva linha do problema.
*
* Param:
* @ mensagem = String contendo a mensagem de erro para o usuário
* @ line = Linha de ocorrência do erro
*
* Ret:
* Sem retorno, a funcao deve imprimir a mensagem de erro e a linha correspondente
*
*/
static void error(char * Mensagem, int line){
	printf("Erro: %s na linha %d\n", Mensagem , line );
}

/*
* Função AdicionaFuncoes
*
* Agrupa todos os arrays montados por cada funcao em uma única variavel que sera
* retornada pela funcao gera.
*
* Param:
* @ Todas = Ponteiro para o array a receber todo o codigo de maquina gerado
* @ my_array = Conteudo com codigo de maquina da funcao lida
* @ inic_array = posicao de inicio da funcao
* @ fim_array = posicao de fim da funcao
* @ posic_geral = posicao atual do array todasfuncoes
*
* Ret:
* @ posic_geral = Nova posição do array todasfuncoes
*
*/
static int AdicionaFuncoes(unsigned char * Todas, unsigned char * my_array, int inic_array, int fim_array, int posic_geral)
{
	int i = inic_array;
	while(i < fim_array)
	{
		Todas[posic_geral] = my_array[i] ;
                posic_geral++;
		i++;
	}
	return posic_geral;
}

/*
* Função DesalocaFuncoesArray
*
* Funcao que limpa todos os espacos alocados pelas funcoes armazenadas em my_array.
* Como essas estruturas ficaram gravadas em todasfuncoes e possivel realizar esse
* free nos elementos
*
* Param:
* @ my_array = Conteudo com codigo de maquina da funcao lida
* @ i = posicao da ultima funcao gerada + 1
*
* Ret:
* Sem retorno - A funcao deve ter desalocado este espaco.
*
*/
static void DesalocaFuncoesArray(unsigned char ** my_array)
{
	int j = 0;
	while(j < MAX_FUNC){
		free(my_array[j]);
		j++;
	}

}

/***  Fim do codigo das funcoes auxiliares ****/

/*** Fim do modulo de implementacao do modulo GERA ****/

 

 
