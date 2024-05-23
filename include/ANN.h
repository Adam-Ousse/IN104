//
// Created by adamg on 5/13/2024.
//

#ifndef IN104_ANN_H
#define IN104_ANN_H
#define beta1  0.9
#define beta2 0.999
#define epsilon 1e-8
#include "array.h"
#include "mathextra.h"
typedef enum ACTIVATION_FUNCTION {
    SIGMOID,
    RELU,
    TANH,
    SOFTMAX,
    IDENTITY
} ACTIVATION_FUNCTION;
typedef struct ann {
    int num_layers;
    int num_layers_no_input;
    int* layer_sizes;
    ACTIVATION_FUNCTION* activation_functions;
    array** weights;
    array** biases;
    array** z;
    array** a;
    array** m;
    array** v;
    array** m_b;
    array** v_b;
    double t;
} ANN;

ANN* ANN_init(int num_layers, int* layer_sizes, ACTIVATION_FUNCTION* activation_functions);
void ANN_destroy(ANN* network);
array* activation(array* z, ACTIVATION_FUNCTION activation);
array* acrivation_derivative(array* z, ACTIVATION_FUNCTION activation);
array* forward(ANN* network, array* x);
void backward(ANN* network, array* x, array* y, double learning_rate);
void adam_update(ANN* network, array* dw, array* db, int i, double learning_rate);
array* predict(ANN* network, array* x);
array* train(ANN* network, array* x, array* y, int epochs, double learning_rate,double precision);
void network_info(ANN* network);
void reset_ann(ANN* network);
#endif //IN104_ANN_H