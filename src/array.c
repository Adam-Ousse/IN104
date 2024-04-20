#include "utils.h"
#include "array.h"



array* array_init(int n, int m,double c){
	array* A = malloc(sizeof(array));
	A-> len = n;
	A-> shape= malloc(sizeof(int)*2);
	A->shape[0]=n;
	A->shape[1]=m;
	A-> values = (double**)malloc(sizeof(double*)*n);
	for(int i=0;i<n;i++){
		A->values[i]=(double*)malloc(sizeof(double)*m);
		for(int j=0;j<m;j++){
			A->values[i][j]=c;
		}
	}
	return A;
}

array* vector_col_init(int n, double c){
	array* V = array_init(n,1,c);
	return V;
}

array* vector_row_init(int m, double c){
	array* V = array_init(1,m,c);
	return V;
}
void array_destroy(array* A){
	for(int i=0;i<A->shape[0];i++){
		free(A->values[i]);
		
	}
	free(A->values);
	free(A->shape);
}

array* transpose(array* A) {
	assert(A->shape[0]!=0 || A->shape[1]!=0);
    array* T = array_init(A->shape[1], A->shape[0], 0);
    for (int i = 0; i < A->shape[0]; i++) {
        for (int j = 0; j < A->shape[1]; j++) {
            T->values[j][i] = A->values[i][j];
        }
    }
    return T;
}
void print(array* A){
    if(A->shape[0]==1){//pour vecteur colonne
        for(int j=0;j<A->shape[1]-1;j++){
            printf("%.*lf,",1, A->values[0][j]);
            // printf("%4.2lf ",A->values[i][j]);
        }
        printf("%.*lf\n",1, A->values[0][A->shape[1]-1]);
    }else {
        for (int i = 0; i < A->len; i++) {
            for (int j = 0; j < A->shape[1]; j++) {
                printf("%-*.*lf ", maxWidth, 1, A->values[i][j]);
                // printf("%4.2lf ",A->values[i][j]);
            }
            printf("\n");
        }
    }
 }

array* eye(int n, int m){
	array* M = array_init(n, m , 0);
	for(int i=0;i<(int) min(n,m);i++){
		M->values[i][i] = 1;
	}
	return M;
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


array* sum(array* A, array* B){
	assert(A->shape[0] == B->shape[0] && A->shape[1] == B->shape[1]);
	array* C = array_init(A->shape[0],A->shape[1],0);
	for(int i=0; i<A->shape[0];i++){
		for(int j =0;j<B->shape[1];j++){
			C->values[i][j] = A->values[i][j] + B->values[i][j];
		}
	}
	return C;
}

array* subtract(array* A, array* B){
	assert(A->shape[0] == B->shape[0] && A->shape[1] == B->shape[1]);
	array* C = array_init(A->shape[0],A->shape[1],0);
	for(int i=0; i<A->shape[0];i++){
		for(int j =0;j<B->shape[1];j++){
			C->values[i][j] = A->values[i][j] - B->values[i][j];
		}
	}
	return C;
}

array* elementwise_product(array* A, array* B){
	assert(A->shape[0] == B->shape[0] && A->shape[1] == B->shape[1]);
	array* C = array_init(A->shape[0],A->shape[1],0);
	for(int i=0; i<A->shape[0];i++){
		for(int j =0;j<B->shape[1];j++){
			C->values[i][j] = A->values[i][j] * B->values[i][j];
		}
	}
	return C;
}

array* elementwise_division(array* A, array* B){
	assert(A->shape[0] == B->shape[0] && A->shape[1] == B->shape[1]);
	array* C = array_init(A->shape[0],A->shape[1],0);
	for(int i=0; i<A->shape[0];i++){
		for(int j =0;j<B->shape[1];j++){
			C->values[i][j] = A->values[i][j]/B->values[i][j];
		}
	}
	return C;
}

array* sumc(array* A, double c){
	array* C = array_init(A->shape[0],A->shape[1],0);
	for(int i=0; i<A->shape[0];i++){
		for(int j =0;j<A->shape[1];j++){
			C->values[i][j] = A->values[i][j] +c;
		}
	}
	return C;
}

array* prodc(array* A, double c){
	array* C = array_init(A->shape[0],A->shape[1],0);
	for(int i=0; i<A->shape[0];i++){
		for(int j =0;j<A->shape[1];j++){
			C->values[i][j] = A->values[i][j] * c;
		}
	}
	return C;
}


array* dot_product(array* A, array* v) {
    // Assuming v is a column vector
    assert(A->shape[1] == v->shape[0]);
    array* result = array_init(A->shape[0], 1, 0);
    for (int i = 0; i < A->shape[0]; i++) {
        for (int j = 0; j < A->shape[1]; j++) {
            result->values[i][0] += A->values[i][j] * v->values[j][0];
        }
    }
    return result;
}
/**
 * Comme np.linspace()
 * @param start positon initiale
 * @param stop position finale
 * @param num nombre de valeurs
 * @return vecteur ligne contenat des valeurs à distance égale
 */
array* linspace(double start, double stop, int num){
    array* V =vector_row_init(num,start);
    double c = (stop-start)/num;
    for(int j=1;j<num;j++){
        V->values[0][j]+=c*j;
    }
    return V;
}

array* col_subset(array* X, int column){
    array* result = vector_col_init(X->shape[0],0);
    for(int i=0;i<X->shape[0];i++){
        result->values[i][0]= X->values[i][column];
    }
    return result;
}

void info(array* X){
    printf("Shape : (%d, %d)\n",X->shape[0],X->shape[1]);
}