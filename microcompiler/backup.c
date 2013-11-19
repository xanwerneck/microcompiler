/* Alexandre Werneck 1320614 3WA */
/* Stefano 3WA */

#include <stdio.h>
#include <stdlib.h>  
#include <string.h>

#include "microcompiler.h"

int monta_array(unsigned char * my_array ,unsigned char * to_add , int posic_array , int qtde );

int faz_conta (unsigned char* my_array, int posic_array, char operador, int val1, int val2);

int tipo_$p (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2);

int tipo_$v (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2);

int tipo_pp (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2);

int tipo_p$ (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2);

int tipo_pv (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2);

int tipo_vv (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2);

int tipo_v$ (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2);

int tipo_vp (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2);

int faz_call (unsigned char* my_array, int posic_array , int val1);

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
				  if(v0 == 'p'){
					  unsigned char temp[] = {0x89, 0xe9, 0x83, 0xc1};
					  posic_array = monta_array (my_array[i], temp, posic_array, 4);
					  my_array[i][posic_array] = (i1 * 4) + 8;
					  posic_array++;

					  unsigned char temp2[] = {0x8b, 0x01};
					  posic_array = monta_array (my_array[i], temp2, posic_array, 2);

				  }else{

					  unsigned char temp[] = {0x89, 0xe9, 0x83, 0xc1};
					  posic_array = monta_array (my_array[i], temp, posic_array, 4);
					  my_array[i][posic_array] = (i1 * 4) + 4;
					  posic_array++;
					  
					  unsigned char temp2[] = {0x8b, 0x01};
					  posic_array = monta_array (my_array[i], temp2, posic_array, 2);


				  }
				 // posic_array = faz_call (my_array[i] , posic_array , (int)&my_array[i]);
				}
				else { /* operacao aritmetica */
				  int i1, i2;
				  char v1 = c0, v2, op;
				  if (fscanf(f, "%d %c %c%d", &i1, &op, &v2, &i2) != 4)
					error("comando invalido", line);
				  //printf("%c%d = %c%d %c %c%d\n", v0, i0, v1, i1, op, v2, i2);
				  if(v0 == 'p'){
					  unsigned char temp[] = {0x89, 0xe9, 0x83, 0xc1};
					  posic_array = monta_array (my_array[i], temp, posic_array, 4);
					  my_array[i][posic_array] = (i1 * 4) + 8;
					  posic_array++;

					  unsigned char temp2[] = {0x8b, 0x01};
					  posic_array = monta_array (my_array[i], temp2, posic_array, 2);
				  }else{
					  unsigned char temp[] = {0x89, 0xe9, 0x83, 0xc1};
					  posic_array = monta_array (my_array[i], temp, posic_array, 4);
					  my_array[i][posic_array] = (i1 * 4) + 4;
					  posic_array++;
					  
					  unsigned char temp2[] = {0x8b, 0x01};
					  posic_array = monta_array (my_array[i], temp2, posic_array, 2);
				  }
		  
				  switch(v1){
					case '$':
						if(v2=='$'){
							posic_array = faz_conta(my_array[i], posic_array, op, i1, i2);
						}else if(v2 == 'p'){
							posic_array = tipo_$p(my_array[i], posic_array, op, i1, i2);							
						}else{
							posic_array = tipo_$v(my_array[i], posic_array, op, i1, i2);	
						}
					  break;
					case 'p':
						if(v2=='$'){
							posic_array = tipo_p$(my_array[i], posic_array, op, i1, i2);
						}else if(v2 == 'p'){
							posic_array = tipo_pp(my_array[i], posic_array, op, i1, i2);							
						}else{
							posic_array = tipo_pv(my_array[i], posic_array, op, i1, i2);	
						}
					  break;
					case 'v':
						if(v2=='$'){
							posic_array = tipo_v$(my_array[i], posic_array, op, i1, i2);
						}else if(v2 == 'p'){
							posic_array = tipo_vp(my_array[i], posic_array, op, i1, i2);							
						}else{
							posic_array = tipo_vv(my_array[i], posic_array, op, i1, i2);	
						}
					  break;

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

int faz_conta (unsigned char* my_array, int posic_array, char operador, int val1, int val2) {
	my_array[posic_array] = 0xb8;
	posic_array++;
	*( (int *) &my_array[posic_array]) = val1; /* Acrescenta o valor do primeiro parametro */
	posic_array += 4;	                       /* Caminha 4 posicoes no array */
	switch (operador) {
		case '+':{{
			unsigned char temp[] = {0x83, 0xc0};
			posic_array = monta_array (my_array, temp, posic_array, 2);}
			my_array[posic_array] = val2;
			posic_array ++;
			{
			unsigned char temp[] = {0x89, 0x11};
			posic_array = monta_array (my_array, temp, posic_array, 2);
			}
			break;
		}
		case '-':{{
			unsigned char temp[] = {0x83, 0xea};
			posic_array = monta_array (my_array, temp, posic_array, 2);}
			my_array[posic_array] = val2;
			posic_array ++;
			{
			unsigned char temp[] = {0x89, 0x11};
			posic_array = monta_array (my_array, temp, posic_array, 2);
			}
			break;
		}
		case '*':{{
			unsigned char temp[] = {0x6b, 0xd2};
			posic_array = monta_array (my_array, temp, posic_array, 2);}
			my_array[posic_array] = val2;
			posic_array ++;
			{
			unsigned char temp[] = {0x89, 0x11};
			posic_array = monta_array (my_array, temp, posic_array, 2);
			}
			break;
		}
	}
	return posic_array;
}

int tipo_$p (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2){
	switch (operador) {
		case '+': case '-': {
			cod_de_maq[posicao_atual] = 0xb8;
			posicao_atual ++;
			*( (int *) &cod_de_maq[posicao_atual]) = idx1;
			posicao_atual += 4;
			{
			unsigned char temp1[] = {0x89, 0xe8, 0x83, 0xc0};
			posicao_atual = monta_array (cod_de_maq, temp1, posicao_atual, 4);
			}
			cod_de_maq[posicao_atual] = (idx2 * 4) + 8;
			posicao_atual ++;
			{
			unsigned char temp2[] = {0x8b, 0x00};
			posicao_atual = monta_array (cod_de_maq, temp2, posicao_atual, 2);
			}
			if (operador == '+') {
				unsigned char temp3[] = {0x01, 0xc2, 0x89, 0x11};
				posicao_atual = monta_array (cod_de_maq, temp3, posicao_atual, 4);
			}
			else {
				unsigned char temp4[] = {0x29, 0xc2, 0x89, 0x11};
				posicao_atual = monta_array (cod_de_maq, temp4, posicao_atual, 4);
			}
			break;
		}
		case '*': {
			cod_de_maq[posicao_atual] = 0xb8;
			posicao_atual ++;
			*( (int *) &cod_de_maq[posicao_atual]) = idx1;
			posicao_atual += 4;
			{
			unsigned char temp1[] = {0x89, 0xea, 0x83, 0xc2};
			posicao_atual = monta_array (cod_de_maq, temp1, posicao_atual, 4);
			}
			cod_de_maq[posicao_atual] = (idx2 * 4) + 8;
			posicao_atual ++;
			{
			unsigned char temp2[] = {0x8b, 0x12, 0x0f, 0xaf, 0xd0, 0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp2, posicao_atual, 7);
			}
			break;
		}
	}
	return posicao_atual;
}

int tipo_$v (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2) {
		switch (operador){
			case '+': case '-': {
				cod_de_maq[posicao_atual] = 0xba;
				posicao_atual ++;
				*( (int *) &cod_de_maq[posicao_atual]) = idx1;
				posicao_atual += 4;
				{
				unsigned char temp1[] = {0x89, 0xe8, 0x83, 0xe8};
				posicao_atual = monta_array (cod_de_maq, temp1, posicao_atual, 4);
				}
				cod_de_maq[posicao_atual] = (idx2 * 4) + 4;
				posicao_atual ++;
				{
				unsigned char temp2[] = {0x8b, 0x00};
				posicao_atual = monta_array (cod_de_maq, temp2, posicao_atual, 2);
				}
				if (operador == '+') {
					unsigned char temp3[] = {0x01, 0xc2, 0x89, 0x11};
					posicao_atual = monta_array (cod_de_maq, temp3, posicao_atual, 4);
				}
				else {
					unsigned char temp4[] = {0x29, 0xc2, 0x89, 0x11};
					posicao_atual = monta_array (cod_de_maq, temp4, posicao_atual, 4);
				}
				break;
			}
			case '*': {
				cod_de_maq[posicao_atual] = 0xb8;
				posicao_atual ++;
				*( (int *) &cod_de_maq[posicao_atual]) = idx1;
				posicao_atual += 4;
				{
				unsigned char temp1[] = {0x89, 0xea, 0x83, 0xea};
				posicao_atual = monta_array (cod_de_maq, temp1, posicao_atual, 4);
				}
				cod_de_maq[posicao_atual] = (idx2 * 4) + 4;
				posicao_atual ++;
				{
				unsigned char temp2[] = {0x8b, 0x12, 0x0f, 0xaf, 0xd0, 0x89, 0x11};
				posicao_atual = monta_array (cod_de_maq, temp2, posicao_atual, 7);
				}
				break;
			}
		}
		return posicao_atual;
}


int tipo_pp (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2){{
	unsigned char temp[] = {0x89, 0xea, 0x83, 0xc2};
	posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 4);}
	cod_de_maq[posicao_atual] = (idx1 * 4) + 8;
	posicao_atual ++;
	{
	unsigned char temp[] = {0x8b, 0x12, 0x89, 0xe8, 0x83, 0xc0};
	posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 6);
	}
	cod_de_maq[posicao_atual] = (idx2 * 4) + 8;
	posicao_atual ++;
	{
	unsigned char temp[] = {0x8b, 0x00};
	posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);
	}
	switch (operador) {
		case '+':{
			unsigned char temp[] = {0x01, 0xc2, 0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 4);
			break;
		}
		case '-':{
			unsigned char temp[] = {0x29, 0xc2, 0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 4);
			break;
		}
		case '*':{
			unsigned char temp[] = {0x0f, 0xaf, 0xd0, 0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 5);
			break;
		}
	}
	return posicao_atual;
}

int tipo_p$ (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2){
	/*
	{
	unsigned char temp[] = {0x89, 0xea, 0x83, 0xc2};
	posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 4);
	}
	cod_de_maq[posicao_atual] = (idx1 * 4) + 8;
	posicao_atual ++;
	{
	unsigned char temp[] = {0x8b, 0x12};
	posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);
	}
	*/
	switch (operador) {
		case '+':{{
			unsigned char temp[] = {0x83, 0xc2};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);}
			cod_de_maq[posicao_atual] = idx2;
			posicao_atual ++;
			{
			//unsigned char temp[] = {0x89, 0x11};
			//posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);
			}
			break;
		}
		case '-':{{
			unsigned char temp[] = {0x83, 0xea};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);}
			cod_de_maq[posicao_atual] = idx2;
			posicao_atual ++;
			{
			unsigned char temp[] = {0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);
			}
			break;
		}
		case '*':{{
			unsigned char temp[] = {0x6b, 0xd2};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);}
			cod_de_maq[posicao_atual] = idx2;
			posicao_atual ++;
			{
			unsigned char temp[] = {0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);
			}
			break;
		}
	}
	return posicao_atual;
}

int tipo_pv (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2){{
		unsigned char temp[] = {0x89, 0xea, 0x83, 0xc2};
		posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 4);}
		cod_de_maq[posicao_atual] = (idx1 * 4) + 8;
		posicao_atual ++;
		{
		unsigned char temp[] = {0x8b, 0x12, 0x89, 0xe8, 0x83, 0xe8};
		posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 6);
		}
		cod_de_maq[posicao_atual] = (idx2 * 4) + 4;
		posicao_atual ++;
		{
		unsigned char temp[] = {0x8b, 0x00};
		posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);
		}
		switch (operador) {
			case '+':{
				unsigned char temp[] = {0x01, 0xc2, 0x89, 0x11};
				posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 4);
				break;
			}
			case '-':{
				unsigned char temp[] = {0x29, 0xc2, 0x89, 0x11};
				posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 4);
				break;
			}
			case '*':{
				unsigned char temp[] = {0x0f, 0xaf, 0xd0, 0x89, 0x11};
				posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 5);
				break;
			}
		}
		return posicao_atual;
}

int tipo_vv (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2){
	unsigned char temp1[] = {0x89, 0xea, 0x83, 0xea};
	posicao_atual = monta_array (cod_de_maq, temp1, posicao_atual, 4);
	cod_de_maq[posicao_atual] = (idx1 * 4) + 4;
	posicao_atual ++;
	{
	unsigned char temp2[] = {0x8b, 0x12};
	posicao_atual = monta_array (cod_de_maq, temp2, posicao_atual, 2);
	}
	switch (operador) {
		case '+': case '-': {
			unsigned char temp3[] = {0x89, 0xe8, 0x83, 0xe8};
			posicao_atual = monta_array (cod_de_maq, temp3, posicao_atual, 4);
			cod_de_maq[posicao_atual] = (idx2 * 4) + 4;
			posicao_atual ++;
			{
			unsigned char temp4[] = {0x8b, 0x00};
			posicao_atual = monta_array (cod_de_maq, temp4, posicao_atual, 2);
			}
			if (operador == '+') {
				unsigned char temp5[] = {0x01, 0xc2, 0x89, 0x11};
				posicao_atual = monta_array (cod_de_maq, temp5, posicao_atual, 4);
			}
			else {
				unsigned char temp6[] = {0x29, 0xc2, 0x89, 0x11};
				posicao_atual = monta_array (cod_de_maq, temp6, posicao_atual, 4);
			}
			break;
		}
		case '*':{
			unsigned char temp3[] = {0x89, 0xe8, 0x83, 0xe8};
			posicao_atual = monta_array (cod_de_maq, temp3, posicao_atual, 4);
			cod_de_maq[posicao_atual] = (idx2 * 4) + 4;
			posicao_atual ++;
			{
			unsigned char temp4[] = {0x8b, 0x00, 0x0f, 0xaf, 0xd0, 0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp4, posicao_atual, 7);
			}
			break;
		}
	}
	return posicao_atual;
}

int tipo_v$ (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2){
	{unsigned char temp[] = {0x89, 0xea, 0x83, 0xea};
	posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 4);}
	cod_de_maq[posicao_atual] = (idx1 * 4) + 4;
	posicao_atual ++;
	{
	unsigned char temp[] = {0x8b, 0x12};
	posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);
	}
	switch (operador) {
		case '+':{{
			unsigned char temp[] = {0x83, 0xc2};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);}
			cod_de_maq[posicao_atual] = idx2;
			posicao_atual ++;
			{
			unsigned char temp[] = {0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);
			}
			break;
		}
		case '-':{{
			unsigned char temp[] = {0x83, 0xea};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);}
			cod_de_maq[posicao_atual] = idx2;
			posicao_atual ++;
			{
			unsigned char temp[] = {0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);
			}
			break;
		}
		case '*':{{
			unsigned char temp[] = {0x6b, 0xd2};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);}
			cod_de_maq[posicao_atual] = idx2;
			posicao_atual ++;
			{
			unsigned char temp[] = {0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);
			}
			break;
		}
	}
	return posicao_atual;
}

int tipo_vp (unsigned char* cod_de_maq, int posicao_atual, char operador, int idx1, int idx2){{
	unsigned char temp[] = {0x89, 0xea, 0x83, 0xea};
	posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 4);}
	cod_de_maq[posicao_atual] = (idx1 * 4) + 4;
	posicao_atual ++;
	{
	unsigned char temp[] = {0x8b, 0x12, 0x89, 0xe8, 0x83, 0xc0};
	posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 6);
	}
	cod_de_maq[posicao_atual] = (idx2 * 4) + 8;
	posicao_atual ++;
	{
	unsigned char temp[] = {0x8b, 0x00};
	posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 2);
	}
	switch (operador) {
		case '+':{
			unsigned char temp[] = {0x01, 0xc2, 0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 4);
			break;
		}
		case '-':{
			unsigned char temp[] = {0x29, 0xc2, 0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 4);
			break;
		}
		case '*':{
			unsigned char temp[] = {0x0f, 0xaf, 0xd0, 0x89, 0x11};
			posicao_atual = monta_array (cod_de_maq, temp, posicao_atual, 5);
			break;
		}
	}
	return posicao_atual;
}

int faz_call (unsigned char* my_array, int posic_array , int val1) {
	my_array[posic_array] = 0x51;
	posic_array++;
	
	my_array[posic_array] = 0xe8;
	posic_array++;

	*( (int *) &my_array[posic_array]) = val1;
	posic_array += 4;	                      
	
	unsigned char temp[] = {0x83 , 0xc4 , 0x04};
    posic_array = monta_array(my_array , temp, posic_array , 3);

	return posic_array;
}



 

 
