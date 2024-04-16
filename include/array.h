#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



typedef struct matrix {
    unsigned int len;
    int* shape;
    double **values;
} array;

array* array_init(int n, int m, double v);
void array_destroy(array* A);
void print(array* A);
array* prod(array* A, array* B);
array* sum(array* A, array* B);
array* subtract(array* A, array* B);
array* elementwise_product(array* A, array* B);
array* elementwise_division(array* A, array* B);
array* scalar_sum(array* A, double c);
array* scalar_product(array* A, double c);
#endif /* ARRAY_H */
