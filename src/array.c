#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define maxWidth 6
typedef struct matrix{
	unsigned int len;
	int* shape;
	double **values;
}array;

array* array_init(int n, int m,double v){
	array* A = malloc(sizeof(array));
	A-> len = n;
	A-> shape= malloc(sizeof(int)*2);
	A->shape[0]=n;
	A->shape[1]=m;
	A-> values = (double**)malloc(sizeof(double*)*n);
	for(int i=0;i<n;i++){
		A->values[i]=(double*)malloc(sizeof(double)*m);
		for(int j=0;j<m;j++){
			A->values[i][j]=v;
		}
	}
	return A;
}


void array_destroy(array* A){
	for(int i=0;i<A->shape[0];i++){
		free(A->values[i]);
		
	}
	free(A->values);
	free(A->shape);
}
void print(array* A){
 	for(int i=0;i<A->len;i++){
 		for(int j=0;j<A->shape[1];j++){
 			printf("%-*.*lf ", maxWidth, 1, A->values[i][j]);
 			// printf("%4.2lf ",A->values[i][j]);
 		}
 		printf("\n");
 	}
 }
array* prod(array* A, array*B){
	assert(A->shape[1]== B->shape[0]);
	array* C = array_init(A->shape[0],B->shape[1],0);
	for(int i=0; i<A->shape[0];i++){
		for(int j =0;j<B->shape[1];j++){
			for(int k=0; k<A->shape[1];k++){
				C->values[i][j]+= A->values[i][k] * B->values[k][j]; 
			}
		}
	}
	return C;
}