/* Alexandre Werneck 1320614 3WA */
/* Stefano 3WA */

#include <stdio.h>
#include <stdlib.h>  
#include <string.h>

#include "microcompiler.h"

int monta_array(unsigned char * my_array ,unsigned char * to_add , int posic_array , int qtde );

void remontaRetorno(unsigned char * my_array , int contador , int posicao[][3] );

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
	int posic_ret[10][3];
	int existe_conta = 0;

	unsigned char begin[3] = {0x55, 0x89, 0xe5};
	unsigned char fim[4]   = {0x89, 0xec, 0x5d, 0xc3};	
	unsigned char ret[1] = {0xb8};		
	unsigned char alocaVarLocais[] = {0x83, 0xec, 0x40};  

	while ((c = fgetc(f)) != EOF) {
	    switch (c) {
	      case 'f': {  /* function */
				char c0;
				if (fscanf(f, "unction%c", &c0) != 1) error("comando invalido", line);

				my_array[i] = (unsigned char*) malloc (128 * sizeof(unsigned char)); /* aloca espaco */

				posic_array = monta_array( my_array[i] , begin , posic_array , 3 );

				posic_array = monta_array (my_array[i], alocaVarLocais, posic_array, 3);/* Inicializa as variaveis */

				break;
			  }
		  case 'e': {  /* end */
				char c0;
				if (fscanf(f, "nd%c", &c0) != 1) error("comando invalido", line);
					posic_array = monta_array(my_array[i], fim, posic_array, 4);
				i++;	//Passa pra prox posicao do array de funcoes.
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

					if(op == '+'){

						unsigned char MovAtribuir[] = {0x89, 0x55};	 
						unsigned char param2[] = {0x8b ,0x55};									//Passa v1 pro registrador edx
						posic_array = monta_array (my_array[i], param2, posic_array, 2);
							  
						if(v1=='p')															
						{
								my_array[i][posic_array] = (i1 * 4) + 8;
						}
						else
						{
							my_array[i][posic_array] = -((i1+1) * 4);
						}

						posic_array++;

						if(v2 == '$')																//Soma com uma cte.
						{
						unsigned char param21[] = {0x81, 0xc2};									//Faz a soma.
						posic_array = monta_array (my_array[i], param21, posic_array, 2);
								  
						*( (int *) &my_array[i][posic_array] ) = i2;
						posic_array += 4;	
						}
						else if(v2 == 'v')														//Soma com outra variavel.
						{
						unsigned char ecxMaisEdx[] = {0x01, 0xca};
						unsigned char param22[] = {0x8b, 0x4d};									//Passando v2 para ecx
						posic_array = monta_array (my_array[i], param22, posic_array, 2);
						my_array[i][posic_array] = -(i2 * 4);
						posic_array++;
															//Soma ecx com edx
						posic_array = monta_array (my_array[i], ecxMaisEdx, posic_array, 2);
								  
						}
						else if(v2 == 'p')														//Soma com outro parametro.
						{
						unsigned char ecxMaisEdx[] = {0x01, 0xca};
						unsigned char param22[] = {0x8b, 0x4d};									//Passando v2 para ecx
						posic_array = monta_array (my_array[i], param22, posic_array, 2);
						my_array[i][posic_array] = (i2 * 4) + 8;
						posic_array++;								
						posic_array = monta_array (my_array[i], ecxMaisEdx, posic_array, 2);	//Soma ecx com edx
								  

						}

						posic_array = monta_array (my_array[i], MovAtribuir, posic_array, 2);	

						if(v0=='p')															
							my_array[i][posic_array] = (i0 * 4) + 8;
						else
							my_array[i][posic_array] = -((i0+1) * 4);
						posic_array++;

						if(existe_conta > 0){
							if(v2 == '$'){
								posic_ret[existe_conta][0] += 12;
							}else{
								posic_ret[existe_conta][0] += 11;
							}
						}

					}
					if(op == '-'){

						unsigned char MovAtribuir[] = {0x89, 0x55};
						unsigned char param2[] = {0x8b ,0x55};									//Faz o mov		
						posic_array = monta_array (my_array[i], param2, posic_array, 2);
							  
						if(v1=='p')																//Se v1 for um parametro														
						{
								my_array[i][posic_array] = (i1 * 4) + 8;							//Passa v1 pro registrador edx					
						}
						else
						{																			//Se v1 for uma variavel
							my_array[i][posic_array] = -(i1 * 4);									//Passa v1 pro registrador edx
						}

						posic_array++;

						if(v2 == '$')																//Soma com uma cte.
						{
						unsigned char param21[] = {0x81, 0xea};									//Faz a subtracao.
						posic_array = monta_array (my_array[i], param21, posic_array, 2);
								  
						*( (int *) &my_array[i][posic_array] ) = i2;
						posic_array += 4;	
						}
						else if(v2 == 'v')														//Soma com outra variavel.
						{
						unsigned char ecxMenosEdx[] = {0x29, 0xca};
						unsigned char param22[] = {0x8b, 0x4d};									//Passando v2 para ecx
						posic_array = monta_array (my_array[i], param22, posic_array, 2);
						my_array[i][posic_array] = -(i2 * 4);
						posic_array++;
															
						posic_array = monta_array (my_array[i], ecxMenosEdx, posic_array, 2);		//Subtrai ecx com edx
								  
						}
						else if(v2 == 'p')														//Soma com outro parametro.
						{
						unsigned char ecxMenosEdx[] = {0x29, 0xca};	
						unsigned char param22[] = {0x8b, 0x4d};									//Passando v2 para ecx
						posic_array = monta_array (my_array[i], param22, posic_array, 2);
						my_array[i][posic_array] = (i2 * 4) + 8;
						posic_array++;
														
						posic_array = monta_array (my_array[i], ecxMenosEdx, posic_array, 2);		//Subtrai ecx com edx
								  

						}

						posic_array = monta_array (my_array[i], MovAtribuir, posic_array, 2);	

						if(v0=='p')															
							my_array[i][posic_array] = (i0 * 4) + 8;
						else
							my_array[i][posic_array] = -(i0 * 4);
						posic_array++;

						if(existe_conta > 0){
							if(v2 == '$'){
								posic_ret[existe_conta][0] += 12;
							}else{
								posic_ret[existe_conta][0] += 11;
							}
						}

					}
					if(op == '*'){

						unsigned char MovAtribuir[] = {0x89, 0x55};
						unsigned char param2[] = {0x8b ,0x55};									//Faz o mov		
						posic_array = monta_array (my_array[i], param2, posic_array, 2);
							  
						if(v1=='p')																//Se v1 for um parametro														
						{
								my_array[i][posic_array] = (i1 * 4) + 8;							//Passa v1 pro registrador edx					
						}
						else
						{																			//Se v1 for uma variavel
							my_array[i][posic_array] = -(i1 * 4);									//Passa v1 pro registrador edx
						}

						posic_array++;

						if(v2 == '$')																//Multiplicacao com uma cte.
						{
						unsigned char param21[] = {0x69, 0xd2};									//Faz a multiplicacao.
						posic_array = monta_array (my_array[i], param21, posic_array, 2);
								  
						*( (int *) &my_array[i][posic_array] ) = i2;
						posic_array += 4;	
						}
						else if(v2 == 'v')														//Multiplicacao com outra variavel.
						{
						unsigned char ecxVezesEdx[] = {0x0f, 0xaf, 0xd1};
						unsigned char param22[] = {0x8b, 0x4d};									//Passando v2 para ecx
						posic_array = monta_array (my_array[i], param22, posic_array, 2);
						my_array[i][posic_array] = -(i2 * 4);
						posic_array++;
															
						posic_array = monta_array (my_array[i], ecxVezesEdx, posic_array, 3);		//Multiplica ecx com edx
								  
						}
						else if(v2 == 'p')														//Multiplicacao com outro parametro.
						{
						unsigned char ecxVezesEdx[] = {0x0f, 0xaf, 0xd1};	
						unsigned char param22[] = {0x8b, 0x4d};									//Passando v2 para ecx
						posic_array = monta_array (my_array[i], param22, posic_array, 2);
						my_array[i][posic_array] = (i2 * 4) + 8;
						posic_array++;
														
						posic_array = monta_array (my_array[i], ecxVezesEdx, posic_array, 3);		//Multiplica ecx com edx
								  

						}

						posic_array = monta_array (my_array[i], MovAtribuir, posic_array, 2);	

						if(v0=='p')															
							my_array[i][posic_array] = (i0 * 4) + 8;
						else
							my_array[i][posic_array] = -(i0 * 4);
						posic_array++;

						if(existe_conta > 0){
							posic_ret[existe_conta][0] += 12;
						}

					}

				}
				break;
	      }
	      case 'r': {  /* ret */
			int i0=0, i1=0;
			char v0, v1;
			existe_conta++;
			if (fscanf(f, "et? %c%d %c%d", &v0, &i0, &v1, &i1) != 4)
			   error("comando invalido", line);
			if(v0 == '$'){
				if(i0 == 0){
					if(v1=='$'){
						my_array[i][posic_array] = ret[0]; /* condição de retorno  - 0xb8 */
						posic_array++;                 
						*( (int *) &my_array[i][posic_array] ) = i1;
						posic_array += 4;	

						unsigned char param2[] = {0x83, 0xf8};
				        posic_array = monta_array (my_array[i], param2, posic_array, 2);

						unsigned char param21[] = {0x00, 0x74};
						posic_array = monta_array (my_array[i], param21, posic_array, 2);

						my_array[i][posic_array] = posic_ret[existe_conta][0];
						posic_array++;

						posic_ret[existe_conta][1] =  posic_array;
					}
					if(v1=='p'){
						unsigned char param2[] = {0x8b, 0x01};
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
						
						posic_array = monta_array (my_array[i], param2, posic_array, 2);

					}
					if(v1 == 'v'){

						unsigned char param3[] = {0x8b, 0x45, 0xfc};
						posic_array = monta_array (my_array[i], param3, posic_array, 3);

					}
				}else{

					/* nao retorna */

				}
			}
			else if(v0 == 'p'){
				unsigned char param2[] = {0x8b, 0x45};
				posic_array = monta_array (my_array[i], param2, posic_array, 2);

				if(v1=='$'){
					my_array[i][posic_array] = -(i0 * 4);			/* Deslocamento de EBP para variavel */
				    posic_array++;

					unsigned char param2[] = {0x83, 0x7d};
				    posic_array = monta_array (my_array[i], param2, posic_array, 2);

					my_array[i][posic_array] = (i1 * 4) + 8;
					posic_array++;

					unsigned char param21[] = {0x00, 0x74};
				    posic_array = monta_array (my_array[i], param21, posic_array, 2);

					my_array[i][posic_array] = posic_ret[existe_conta][0];
					posic_array++;


				}
				if(v1=='p' || v1 == 'v'){
					unsigned char param2[] = {0x8b, 0x01};
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

					
					posic_array = monta_array (my_array[i], param2, posic_array, 2);

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

     remontaRetorno(my_array[i], existe_conta, posic_ret);

	*code = my_array[0];

	*entry = (funcp)my_array[i-1];

}

void remontaRetorno(unsigned char * my_array , int contador , int posicao[][3] )
{
	while(contador > 0){
		if(contador == 1){
			my_array[posicao[contador][1]] = 0x00;
		}else{
			my_array[posicao[contador][1]] = my_array[posicao[contador+1][1]]; 
		}
		contador--;
	}
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




 

 
