//
// Created by adamg on 5/13/2024.
//

#ifndef IN104_ANN_H
#define IN104_ANN_H

#endif //IN104_ANN_H
#include "array.h"
#include "mathextra.h"

typedef enum ACTIVATION_FUNCTION {
    SIGMOID,
    RELU,
    TANH,
    SOFTMAX,
    IDENTITY
} ACTIVATION_FUNCTION;
typedef struct layer {
    array* weights;  // Weight matrix
    array* biases;   // Bias vector
    array* activation;   // Output vector
    array* delta;    // Delta for backpropagation
    ACTIVATION_FUNCTION activation_function;
} layer;
typedef struct ann {
    int num_layers;
    int* layer_sizes; // Array of sizes of each layer
    layer** layers;
    ACTIVATION_FUNCTION* activation_functions;
} ANN;
layer* layer_init(int input_size, int output_size, ACTIVATION_FUNCTION activation_function);
ANN* ANN_init(int num_layers, int* layer_sizes, ACTIVATION_FUNCTION* activation_functions);
void ANN_destroy(ANN* network);
void layer_destroy(layer* l);
array* activate(array* input, array* weights, array* biases, double (*activation_func)(double));
array* forward_propagate(ANN* network, array* input);