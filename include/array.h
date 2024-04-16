#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define maxWidth 6

typedef struct matrix {
    unsigned int len;
    int* shape;
    double **values;
} array;

array* array_init(int n, int m, double v);
void array_destroy(array* A);
void print(array* A);
array* prod(array* A, array* B);

#endif /* ARRAY_H */
