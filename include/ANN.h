//
// Created by adamg on 5/13/2024.
//

#ifndef IN104_ANN_H
#define IN104_ANN_H

#endif //IN104_ANN_H
#include "array.h"
#include "mathextra.h"
typedef struct ANN{
    int num_layers;
    int* layer_sizes;
    array** weights;
    array** biases;
    double learning_rate;

} ANN;
