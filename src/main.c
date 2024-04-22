#include "utils.h"
#include "array.h"
#include "data.h"
#include "linearmodels.h"
int main(){
	/*array* A = array_init(4,3,0);
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
    array* X = linspace(0,10,100);
    printf("X : \n");
    print(X);
    array_destroy(X);
    *//*char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current working directory: %s\n", cwd);*//*
    printf("data.csv : \n");
    array* Y =read_file("../data/data.csv",";");
    print(Y);
    array_destroy(Y);*/
    array* Y = subset(read_file("../data/height_weight.csv",","),0,10000);
    printf("Y\n");
//    print(Y);
    printf("%d lignes and %d colonnes\n",Y->shape[0], Y->shape[1]);
    printf("%lf moyenne de la derniere colonne \n",mean(col_subset(Y,1,2)));

    LinearRegression* Model=LinearRegression_init(1);
    array* X= col_subset(Y,0,1);
    array* y =col_subset(Y,1,2);
    LinearRegression_fit(Model, X, y,10000,0.0000000001 ,true, false);
    printf("MSE : %lf \n",MSE(LinearRegression_predict(Model, X), y));
    printf("a :%lf \n",Model->weights->values[0][0]);
    info(Model->weights);
    printf("b :%lf\n",Model->bias);

    array_destroy(Y);
    array_destroy(X);
    array_destroy(y);
    LinearRegression_destroy(Model);

//    print(X);
//    print(LinearRegression_predict(Model, X));
	return 0;
}