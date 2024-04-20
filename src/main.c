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
    array* Y =read_file("../data/area_price.csv",",");
//    print(Y);
    printf("%d lines and %d columns\n",Y->shape[0], Y->shape[1]);
    LinearRegression* Model=LinearRegression_init(1);
    LinearRegression_fit(Model, col_subset(Y,0), col_subset(Y,1),0.1,1000,1 );
//    Model->weights->values[0][0] = 3;
//    Model->bias =5;
//    print(Model->weights);
//    info(Model->weights);
//    printf("%lf\n",Model->bias);
//    array_destroy(Y);
//    print(LinearRegression_predict(Model, transpose(linspace(0,10,10))));
	return 0;
}