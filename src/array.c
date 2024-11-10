
#include "array.h"



array* array_init(int n, int m,double c){
	array* A = malloc(sizeof(array));

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
array* array_rand_init(int n, int m){
    array* A = malloc(sizeof(array));
    A-> shape= malloc(sizeof(int)*2);
    A->shape[0]=n;
    A->shape[1]=m;
    A-> values = (double**)malloc(sizeof(double*)*n);
    for(int i=0;i<n;i++){
        A->values[i]=(double*)malloc(sizeof(double)*m);
        for(int j=0;j<m;j++){
            A->values[i][j]=rand()%10000;
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
    assert(A!=NULL);
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
        for (int i = 0; i < A->shape[0]; i++) {
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
//    printf("A shape : (%d,%d)\n",A->shape[0],A->shape[1]);
//    printf("B shape : (%d,%d)\n",B->shape[0],B->shape[1]);
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
    assert(same_size(A,B));
	array* C = array_init(A->shape[0],A->shape[1],0);
	for(int i=0; i<A->shape[0];i++){
		for(int j =0;j<B->shape[1];j++){
			C->values[i][j] = A->values[i][j] + B->values[i][j];
		}
	}
	return C;
}
array* sum_to_all_col(array* A, array* B){
    assert(A->shape[0]==B->shape[0] && B->shape[1]==1);
    array* C = array_init(A->shape[0],A->shape[1],0);
    for(int j =0;j<A->shape[1];j++){
    for(int i=0; i<A->shape[0];i++){
            C->values[i][j] = A->values[i][j] + B->values[i][0];
        }
    }
    return C;
}

array* sum_to_all_row(array* A, array* B){
//    printf("A shape : (%d,%d)\n",A->shape[0],A->shape[1]);
//    printf("B shape : (%d,%d)\n",B->shape[0],B->shape[1]);
    assert(A->shape[1]==B->shape[1] && B->shape[0]==1);
    array* C = array_init(A->shape[0],A->shape[1],0);
    for(int i =0;i<A->shape[0];i++){
        for(int j=0; j<A->shape[1];j++){
            C->values[i][j] = A->values[i][j] + B->values[0][j];
        }
    }
    return C;
}

array* subtract(array* A, array* B){
    assert(same_size(A,B));
	array* C = array_init(A->shape[0],A->shape[1],0);
	for(int i=0; i<A->shape[0];i++){
		for(int j =0;j<B->shape[1];j++){
			C->values[i][j] = A->values[i][j] - B->values[i][j];
		}
	}
	return C;
}

array* elementwise_product(array* A, array* B){
	assert(same_size(A,B));
	array* C = array_init(A->shape[0],A->shape[1],0);
	for(int i=0; i<A->shape[0];i++){
		for(int j =0;j<B->shape[1];j++){
			C->values[i][j] = A->values[i][j] * B->values[i][j];
		}
	}
	return C;
}

array* elementwise_division(array* A, array* B){
    assert(same_size(A,B));
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
array* divisionc(array* A, double c){
    assert(c!=0);
    array* C = array_init(A->shape[0],A->shape[1],0);
    for(int i=0; i<A->shape[0];i++){
        for(int j =0;j<A->shape[1];j++){
            C->values[i][j] = A->values[i][j] / c;
        }
    }
    return C;
}

array* dot_product(array* A, array* v) {
    // Assuming v is a column vector
    assert(A->shape[1] == v->shape[0] && v->shape[1] == 1);
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
    row_2 = (int)min(row_2,X->shape[0]);
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

array* sample(array* X, int n){
    //a uniform sample of X of n samples
    assert(n <= X->shape[0]); // Ensure we are not asking for more samples than available

    // Calculate the interval between samples
    double interval = (double)X->shape[0] / n;

    // Create a new array to store the sampled elements
    array* result = array_init(n, X->shape[1], 0);

    // Select samples at even intervals
    for(int i = 0; i < n; i++) {
        int index = (int)(interval * i);
        result->values[i] = X->values[index];
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

void mean_normalize(array* X,double* coef){
//    double minn = min_array(X);
//    double maxx = max_array(X);
//    double meann = mean(X);
//    double* coef_for_anti_normalize = malloc(sizeof(double)*3);
//    coef_for_anti_normalize[0] = meann;
//    coef_for_anti_normalize[1] = minn;
//    coef_for_anti_normalize[2] = maxx;
    for(int i=0;i<X->shape[0]; i++){
        for(int j=0;j<X->shape[1]; j++){
            /*X->values[i][j] -= minn;*/
            X->values[i][j] -= coef[0];
            X->values[i][j] /= (coef[2]-coef[1]);
        }
    }
}   
void min_max_normalize(array* X, double* coef){
//    if(make_new_coef){
//        double minn = min_array(X);
//        double maxx = max_array(X);
//
//        coef[0] = minn;
//        coef[1] = maxx;
//    }

    for(int i=0;i<X->shape[0]; i++){
        for(int j=0;j<X->shape[1]; j++){
            /*X->values[i][j] -= minn;*/
            X->values[i][j] -= coef[0];
            X->values[i][j] /= (coef[1]-coef[0]);
        }
    }

}
void min_max_unnormalize(array* X, double* coef){
    for(int i=0;i<X->shape[0]; i++){
        for(int j=0;j<X->shape[1]; j++){
            X->values[i][j] *= (coef[1]-coef[0]);
            X->values[i][j]+=coef[0];
//            X->values[i][j] += coef[0];
        }
    }
}
void mean_unnormalize(array* X, double* coef){
    for(int i=0;i<X->shape[0]; i++){
        for(int j=0;j<X->shape[1]; j++){
            X->values[i][j] *= (coef[2]-coef[1]);
            X->values[i][j]+=coef[0];
//            X->values[i][j] += coef[0];
        }
    }
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
    assert(A->shape[0]==A->shape[1]);
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

double optimal_learning_rate(array* A){
    assert(A->shape[0]==A->shape[1]);
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
    r = max_array(c)+min_array(c)/2.0;
    array_destroy(c);
    return 2000/r;
}
bool same_size(array* A, array* B){
    return A->shape[0]==B->shape[0] && A->shape[1]==B->shape[1];
}
bool equal(array* A, array* B){
    assert(same_size(A,B));
    for (int i=0; i<A->shape[0]; i++){
        for (int j=0; j<A->shape[1]; j++){
            if (A->values[i][j]!=B->values[i][j]){
                return false;
            }
        }
    }
    return true;
}

double rand_normal(double mean, double stddev) {
    // Using Box-Muller transform
    double u = (double)rand() / RAND_MAX;
    double v = (double)rand() / RAND_MAX;
    double z = sqrt(-2.0 * log(u)) * cos(2.0 * M_PI * v);
    return mean + z * stddev;
}

// He-initialization function for a weight matrix
array* he_init(int fan_in, int fan_out) {
    array* weights = array_init(fan_in, fan_out, 0);
    double stddev = sqrt(2.0 / fan_in);
    for (int i = 0; i < fan_in; i++) {
        for (int j = 0; j < fan_out; j++) {
            weights->values[i][j] = rand_normal(0, stddev);
        }
    }
    return weights;
}

array* collapse_sum(array* A){
    array* result = array_init(1,A->shape[1],0);
    for(int j=0;j<A->shape[1];j++){
        for(int i=0;i<A->shape[0];i++){
            result->values[0][j]+=A->values[i][j];
        }
    }
    return result;
}


