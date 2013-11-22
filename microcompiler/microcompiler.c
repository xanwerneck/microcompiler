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

/**** Código das funções encapsuladas pelo módulo *****/

/*** Código das funções principais de manipulação ****/

void libera(void *p)
{
	free(p);
}

void gera(FILE *f, void **code , funcp * entry)
{ 
	/* Variáveis locais */
	unsigned char * my_array[10];
	int posic_func_ar[10];
	int c, line =1, i=0;
	int posic_array = 0;
	int posic_ret[10][3];
	int in_retorno = 0;

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

	posic_ret[0][0] = 5;

	while ((c = fgetc(f)) != EOF) {
	    switch (c) {
	      case 'f': {  /* function */
				if (fscanf(f, "unction") != 0) error("comando invalido", line);

				my_array[i] = (unsigned char*) malloc (128 * sizeof(unsigned char)); 
				posic_func_ar[i] = posic_array;

				posic_array = monta_array( my_array[i] , inicio , posic_array , 3 );
				posic_array = monta_array (my_array[i], alocaVarLocais, posic_array, 3);

				break;
			  }
		  case 'e': {  /* end */
				char c0;
				if (fscanf(f, "nd%c", &c0) != 1) error("comando invalido", line);

				posic_array = monta_array(my_array[i], fim, posic_array, 4);

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
				  //printf("%c%d = call %d %c%d\n", v0, i0, fil, v1, i1);


				  if(v0 == 'p'){


				  }else if(v0 == 'v'){
					 
                    int val=0;
					unsigned char param2[] = {0x8b, 0x45};
				    posic_array = monta_array (my_array[i], param2, posic_array, 2);

					if(v1 == 'v'){
						my_array[i][posic_array] = -((i1+1) * 4);
					}else{
						my_array[i][posic_array] = (i1 * 4) + 8;
					}
					posic_array++;
											
					unsigned char param21[] = {0x50, 0xe8}; /* Move 0 para eax retornar 0 */
					posic_array = monta_array (my_array[i], param21, posic_array, 2);

					/*** adicionar destino ***/
					*( (int *) &my_array[i][posic_array] ) = 0;
					posic_array += 4;

					val = (int)&my_array[fil][posic_func_ar[fil]] - (int)&my_array[i][posic_array];

					printf("Qual valor 1 ? %08x\n" , val);
					printf("Qual valor 2 ? %08x\n" , (int)&my_array[fil][posic_func_ar[fil]]);
					printf("Qual valor 3 ? %08x\n" , (int)&my_array[i][posic_array]);
	
					my_array[i][posic_array-1]   = val >> 24;
					my_array[i][posic_array-2]   = val >> 16;
					my_array[i][posic_array-3]   = val >> 8;
					my_array[i][posic_array-4]   = val;

					printf("Onde estou ? %d" , posic_array);

					
					posic_ret[in_retorno-1][1] =  posic_array;

					if(in_retorno > 0){
						posic_ret[in_retorno][0] += 9;
					}

				  }

				  
				}
				else { /* operacao aritmetica */
				  int i1, i2;
				  char v1 = c0, v2, op;
				  if (fscanf(f, "%d %c %c%d", &i1, &op, &v2, &i2) != 4)
					error("comando invalido", line);
				  //printf("%c%d = %c%d %c %c%d\n", v0, i0, v1, i1, op, v2, i2);

					if(op == '+'){

						posic_array = monta_array (my_array[i], MovEdx, posic_array, 2);
							  
						if(v1=='p'){
								my_array[i][posic_array] = (i1 * 4) + 8;
						}else{
							    my_array[i][posic_array] = -((i1+1) * 4);
						}

						posic_array++;

						if(v2 == '$'){

							unsigned char param21[] = {0x81, 0xc2};	
							posic_array = monta_array (my_array[i], param21, posic_array, 2);
								  
							*( (int *) &my_array[i][posic_array] ) = i2;
							posic_array += 4;

						}else if(v2 == 'v'){

							posic_array = monta_array (my_array[i], MovEcx, posic_array, 2);

							my_array[i][posic_array] = -(i2 * 4);
							posic_array++;

							posic_array = monta_array (my_array[i], ecxMaisEdx, posic_array, 2);
								  
						}else if(v2 == 'p')	{

							posic_array = monta_array (my_array[i], MovEcx, posic_array, 2);

							my_array[i][posic_array] = (i2 * 4) + 8;
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
					if(op == '-'){ 	

						posic_array = monta_array (my_array[i], MovEdx, posic_array, 2);
							  
						if(v1=='p'){

								my_array[i][posic_array] = (i1 * 4) + 8;							//Passa v1 pro registrador edx

						}else{																			//Se v1 for uma variavel

								my_array[i][posic_array] = -((i1+1) * 4);									//Passa v1 pro registrador edx

						}

						posic_array++;

						if(v2 == '$'){

							unsigned char param2[] = {0x81, 0xea};									//Faz a subtracao.
							posic_array = monta_array (my_array[i], param2, posic_array, 2);
								  
							*( (int *) &my_array[i][posic_array] ) = i2;
							posic_array += 4;	

						}else if(v2 == 'v'){
							
							posic_array = monta_array (my_array[i], MovEcx, posic_array, 2);

							my_array[i][posic_array] = -(i2 * 4);
							posic_array++;
															
							posic_array = monta_array (my_array[i], ecxMenosEdx, posic_array, 2);		//Subtrai ecx com edx
								  
						}else if(v2 == 'p'){
							
							posic_array = monta_array (my_array[i], MovEcx, posic_array, 2);

							my_array[i][posic_array] = (i2 * 4) + 8;
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
					if(op == '*'){
	
						posic_array = monta_array (my_array[i], MovEdx, posic_array, 2);
							  
						if(v1=='p'){

								my_array[i][posic_array] = (i1 * 4) + 8;							//Passa v1 pro registrador edx	

						}else{																			//Se v1 for uma variavel

								my_array[i][posic_array] = -((i1+1) * 4);									//Passa v1 pro registrador edx

						}

						posic_array++;

						if(v2 == '$'){

							unsigned char param2[] = {0x69, 0xd2};									//Faz a multiplicacao.
							posic_array = monta_array (my_array[i], param2, posic_array, 2);
								  
							*( (int *) &my_array[i][posic_array] ) = i2;
							posic_array += 4;	

						}else if(v2 == 'v'){

							posic_array = monta_array (my_array[i], MovEcx, posic_array, 2);

							my_array[i][posic_array] = -((i2+1) * 4);
							posic_array++;
															
							posic_array = monta_array (my_array[i], ecxVezesEdx, posic_array, 3);		//Multiplica ecx com edx
								  
						}else if(v2 == 'p'){

							posic_array = monta_array (my_array[i], MovEcx, posic_array, 2);

							my_array[i][posic_array] = (i2 * 4) + 8;
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

			/* Caso não seja a primeira condição de retorno, adiciona os elementos a andarem na pilha */
			//posic_ret[in_retorno][0] += 5;

			if(v0 == '$'){
				if(v1=='$'){

					my_array[i][posic_array] = 0xba; /* condição de teste para retorno  - 0xb8 */
					posic_array++;                 
					*( (int *) &my_array[i][posic_array] ) = i0;
					posic_array += 4;	

					my_array[i][posic_array] = 0xb8; /* condição de retorno  - 0xb8 */
					posic_array++;                 
					*( (int *) &my_array[i][posic_array] ) = i1;
					posic_array += 4;	
					
					unsigned char param2[] = {0x83, 0xfa, 0x00, 0x74};
				    posic_array = monta_array (my_array[i], param2, posic_array, 4);
						
					my_array[i][posic_array] = posic_ret[in_retorno-1][0];
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

					posic_ret[in_retorno][0] += 7;

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

			

			//printf("ret? %c%d %c%d\n", v0, i0, v1, i1);
		break;
	      }
	      default: error("comando desconhecido", line);
	    }
	    line ++;
	    fscanf(f, " ");
	  }

    remontaRetorno(my_array[i-1], in_retorno, posic_ret);

	*code = my_array[0];

	*entry = (funcp)my_array[i-1];

}

/*** Código das funções auxiliares ****/

/*
* Função remontaRetorno
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

		printf("Contador 1: %d" , my_array[posicao[(contador-1)-(temp-1)][1]]);

		printf("Contador 2: %d" , my_array[posicao[temp-1][1]]);

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
* Param:
* @ mensagem = String contendo a mensagem de erro para o usuário
* @ line = Linha de ocorrência do erro
*
* Ret:
* Sem retorno, a funçã odeve imprimir a mensagem de erro e a linha correspondente
*
*/
static void error(char * Mensagem, int line){
	printf("Erro: %s na linha %d\n", Mensagem , line );
}




 

 
