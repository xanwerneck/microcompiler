#include <stdio.h>

typedef int (*funcp) (int x);

int add (int x) {
  return x+1;
}

int main(void) {

	int i, j, c;
	funcp f;

	c = 0;
	
    unsigned char codigo[16] = {0x55,0x89,0xe5,0x8b,0x45,0x08,0x50,0xe8,0xfc,0xff,0xff,0xff,0x89,0xec,0x5d,0xc3};

	i = (int)&add - (int)&codigo[12];
	printf("Valor inteiro %d \n" , i);
	
	codigo[11]   = i >> 24;
	codigo[10]   = i >> 16;
	codigo[9]    = i >> 8;
	codigo[8]    = i;

	printf("Valor %08x \n" , i);
	printf("Valor %08x \n" , (int)&add);
	printf("Valor %08x \n" , (int)&codigo[12]);


	while(c <= 16){
		printf("Valor %02x \n" , codigo[c]);
		c++;
	}
	
	f = (funcp)codigo;

	j = (*f)(10);

	printf("Valor FINAL:  %d \n" , j);

	return 0;

}
