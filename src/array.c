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
    if(A->shape[0]==1){//pour vecteur ligne
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
	for(int i=0; i< A->shape[0];i++){
		for(int j =0;j< B->shape[1];j++){
			for(int k=0; k< A->shape[1];k++){
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

array* col_subset(array* X, int column_1 , int column_2){
    assert(column_2>column_1);
    array* result = array_init(X->shape[0],column_2-column_1,0);
    //loop copy
    for(int i=0;i<X->shape[0];i++){
        for(int j=0;j<column_2-column_1;j++) {
            result->values[i][j] = X->values[i][j+column_1];
        }
    }
    return result;
}


array* subset(array* X , int row_1, int row_2){
    assert(row_1<row_2);
    array* result = array_init(row_2-row_1,X->shape[1],0);
    //for loop copy
    /*for(int i=0;i<row_2-row_1;i++){
        for(int j=0;j<X->shape[1];j++){
            result->values[i][j] = X->values[row_1+i][j];
        }
    }*/
    //memory manipulation (tested works)
    for(int i=0;i<row_2-row_1;i++){
        result->values[i] = X->values[row_1+i];
    }
    return result;
}
void info(array* X){
    printf("Shape : (%d, %d)\n",X->shape[0],X->shape[1]);
    printf("Max : %.2lf \n", max_array(X));
    printf("Min : %.2lf \n", min_array(X));
}

double sum_all(array* X){
    double result = 0;
    for(int i=0;i<X->shape[0];i++){
        for(int j=0;j<X->shape[1];j++){
            result+= X->values[i][j];
        }
    }
    return result;
}

double min_array(array* X){
    double result=X->values[0][0];
    for(int i=0;i<X->shape[0];i++){
        for(int j=0;j<X->shape[1];j++){
            if(X->values[i][j] <result){
                result = X->values[i][j];
            }
        }
    }
    return result;
}

double max_array(array* X){
    double result=X->values[0][0];
    for(int i=0;i<X->shape[0];i++){
        for(int j=0;j<X->shape[1];j++){
            if(X->values[i][j] >result){
                result = X->values[i][j];
            }
        }
    }
    return result;
}

double* normalize(array* X){
    double minn = min_array(X);
    double maxx = max_array(X);

    double* coef_for_anti_normalize = malloc(sizeof(double)*2);
    coef_for_anti_normalize[0] = minn;
    coef_for_anti_normalize[1] = maxx;
    for(int i=0;i<X->shape[0]; i++){
        for(int j=0;j<X->shape[1]; j++){
            /*X->values[i][j] -= minn;*/
            X->values[i][j] /= (maxx-minn);
        }
    }
    return coef_for_anti_normalize;
}

void unnormalize(array* X, double* coef){
    for(int i=0;i<X->shape[0]; i++){
        for(int j=0;j<X->shape[1]; j++){
            X->values[i][j] *= (coef[1]-coef[0]);
//            X->values[i][j] += coef[0];
        }
    }
    free(coef);
}
double mean(array* A){
    double s=0;
    int n=A->shape[0];
    int m=A->shape[1];
    assert (n*m != 0);
    for (int i=0; i<n; i++){
        for (int j=0; j<m; j++){
            s+=A->values[i][j];
        }
    }
    return s/(n*m);
}

array* transform(array* A, double (*func)(double)){
    int n=A->shape[0];
    int m=A->shape[1];
    array* B= array_init(n,m,0);
    for (int i=0; i<n; i++){
        for (int j=0; j<m; j++){
            B->values[i][j]=func(A->values[i][j]);
        }
    }
    return B;
}

double gershgorin_radius(array* A){
    info(A);
    int n=A->shape[0];
    int m=A->shape[1];
    array* c = array_init(1,n,0);
    double r=0;
    for (int i=0; i<n; i++){
        double s=0;
        for (int j=0; j<m; j++){
            if (i!=j){
                s+=fabs(A->values[i][j]);
            }
        }
        c->values[0][i]=(s+A->values[i][i])/n;
    }
    r = max_array(c);
    printf("upper bound : %lf\n",r);
    array_destroy(c);
    return r;
}
