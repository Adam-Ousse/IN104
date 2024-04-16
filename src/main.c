#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "array.h"


int main(){

	array* A = array_init(4,3,0);
	A->values[0][0] = 6;
	A->values[1][2]= 9;
	A->values[1][1] = 10;
	array* B = array_init(3,6,9);
	B->values[2][2] = 20;
	print(A);
	print(B);
	printf("\n");
	print(prod(A,B));
	printf("%lf \n",A->values[0][0]);
	array_destroy(A);
	return 0;
}