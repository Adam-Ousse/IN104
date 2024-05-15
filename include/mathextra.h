#ifndef MATHEXTRA_H
#define MATHEXTRA_H
#include <math.h>
double min(double a, double b);
double max(double a, double b);
double sigmoid(double x);
double sigmoid_prime(double x);
double relu(double x);
double relu_prime(double x);
double leaky_relu(double x);
double leaky_relu_prime(double x);
double tanh(double x);
double tanh_prime(double x);
double softmax(double* x, int n);
double identity(double x);
#endif