#include "utils.h"
#include "array.h"


	
int main(){

	array* A = array_init(4,3,0);
	A->values[0][0] = 6;
	A->values[1][2]= 9;
	A->values[1][1] = 10;
	array* B = array_init(3,6,9);
	B->values[2][2] = 20;
	printf("A : \n");
	print(A);
	printf("B : \n");
	print(B);
	printf("A*B\n");
	print(prod(A,B));
	printf("A+1\n");
	print(sum(A,array_init(4,3,1)));
	print(sumc(A,1));
	printf("A * 2\n");
	print(prodc(A,2));
	array* T= prodc(eye(4,4),2);
	array* V = vector_col_init(4,1);
	printf("T : \n");
	print(T);
	printf("V \n");
	print(V);
	printf("T.V\n");
	print(dot_product(T,V));
	array_destroy(A);
	array_destroy(B);
	array_destroy(T);
	array_destroy(V);
	return 0;
}