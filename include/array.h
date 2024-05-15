#ifndef ARRAY_H
#define ARRAY_H
#define maxWidth 6
#include "utils.h"
typedef struct array {
    int* shape;
    double **values;
} array;
/**
 * Initialiser une matrice
 * @param n nombre de lignes
 * @param m nombre de colonnes
 * @param v valeur à remplir les cases
 * @return Une matrice/vecteur de taille n*m remplie de v
 */
array* array_init(int n, int m, double v);
array* array_rand_init(int n, int m);
/**
 *
 * @param n nombre de lignes
 * @param c valeur à remplir les cases
 * @return Un vecteur colonne de taille n remplie de c
 */
array* vector_col_init(int n, double c);
/**
 *
 * @param m nombre de colonne
 * @param c valeur à remplir les cases
 * @return Un vecteur ligne de taille m remplie de c
 */
array* vector_row_init(int m, double c);
/**
 *
 * @param A Matrice à détruire
 */
void array_destroy(array* A);
/**
 *
 * @param A Matrice à transposer
 * @return Matrice transposé
 */
array* transpose(array* A);
/**
 *
 * @param A Matrice à afficher
 */
void print(array* A);
/**
 *
 * @param n taille de la matrice identité
 * @return Matrice identité de taille n*n
 */
array* eye(int n, int m);
/**
 *
 * @param A Matrice
 * @param B Matrice
 * @return Produit matriciel A*B
 */
array* prod(array* A, array* B);
/**
 *
 * @param A Matrice
 * @param B Matrice
 * @return Somme de A et B : A+B
 */
array* sum(array* A, array* B);
array* sum_to_all_col(array* A, array* B);
/**
 *
 * @param A Matrice
 * @param B Matrice
 * @return Différence de A et B : A-B
 */
array* subtract(array* A, array* B);
/**
 * Nécessite que les matrices A et B sont de même taille
 * @param A Matrice
 * @param B Matrice
 * @return Produit élément par élément de A et B
 */
array* elementwise_product(array* A, array* B);
/**
 * Nécessite que les matrices A et B sont de même taille
 * @param A Matrice
 * @param B Matrice
 * @return Division élément par élément de A et B
 */
array* elementwise_division(array* A, array* B);
/**
 *
 * @param A Matrice
 * @param c scalaire
 * @return Somme de A et c : A+c
 */
array* sumc(array* A, double c);
/**
 *
 * @param A Matrice
 * @param c scalaire
 * @return Produit de A et c : A*c
 */
array* prodc(array* A, double c);
/**
 *
 * @param A Matrice
 * @param v Vecteur
 * @return Produit matriciel A*v
 */
array* dot_product(array* A, array* v) ;
/**
 *
 * @param start Debut du vecteur ligne
 * @param stop Fin du vecteur ligne (exclu)
 * @param num nombre de points
 * @return L'equivalent de np.linspace(start, stop, num)
 */
array* linspace(double start, double stop, int num);
/**
 *
 * @param X La matrice à diviser
 * @param column_1 Le numéro de la 1ere colonne
 * @param column_2 Le numéro de la 2eme colonne
 * @return La sous-matrice de X avec les colonnes de column_1 à column_2 (exclu)
 */
array* col_subset(array* X, int column_1 , int column_2);
/**
 *
 * @param X La matrice à diviser
 * @param row_1 Le numéro de la 1ere ligne
 * @param row_2 Le numéro de la 2eme ligne
 * @return La sous-matrice de X avec les lignes de row_1 à row_2 (exclu)
 */
array* subset(array* X , int row_1, int row_2);
/**
 * Affice la taille de la matrice
 * @param X Matrice
 */
void info(array* X);
/**
 *
 * @param X Matrice
 * @return La somme de tous les éléments de X
 */
double sum_all(array* X);
/**
 *
 * @param X Matrice
 * @return Le minimum de tous les éléments de X
 */
double min_array(array* X);
/**
 *
 * @param X Matrice
 * @return Le maximum de tous les éléments de X
 */
double max_array(array* X);
/**
 *
 * @param X Matrice à normaliser
 * @return Matrice normalisée (min-max)
 */
void min_max_normalize(array* X, double* coef);
void min_max_unnormalize(array* X, double* coef);
void* mean_normalize(array* X,double* coef);
void min_max_unnormalize(array* X, double* coef);
/**
 * Calcule la moyenne de tous les éléments de A
 * @param A Matrice
 * @return La moyenne de tous les éléments de A
 */
double mean(array* A);

/**
 * Applique une fonction sur tous les éléments de A
 * @param A Matrice
 * @param func Fonction à appliquer
 * @return Matrice transformée
 */
array* transform(array* A, double (*func)(double));
/**
 *
 * @param X Matrice à dénormaliser
 * @param coef Coefficients de normalisation
 */

double gershgorin_radius(array* A);
double optimal_learning_rate(array* A);
bool same_size(array* A, array* B);
bool equal(array* A, array* B);
double rand_normal(double mean, double stddev);
array* he_init(int fan_in, int fan_out);
array* sum_to_all_row(array* A, array* B);
array* sample(array* X, int n);
#endif /* ARRAY_H */
