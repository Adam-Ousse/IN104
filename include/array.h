#ifndef ARRAY_H
#define ARRAY_H
#define maxWidth 6

typedef struct matrix {
    unsigned int len;
    int* shape;
    double **values;
} array;

array* array_init(int n, int m, double v);
array* vector_col_init(int n, double c);
array* vector_row_init(int m, double c);
void array_destroy(array* A);
array* transpose(array* A);
void print(array* A);
array* eye(int n, int m);
array* prod(array* A, array* B);
array* sum(array* A, array* B);
array* subtract(array* A, array* B);
array* elementwise_product(array* A, array* B);
array* elementwise_division(array* A, array* B);
array* sumc(array* A, double c);
array* prodc(array* A, double c);
/**
 *
 * @param A Matrice
 * @param v Vecteur colonne
 * @return Le produit A*v
 */
array* dot_product(array* A, array* v) ;
array* linspace(double start, double stop, int num);
#endif /* ARRAY_H */
