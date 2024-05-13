//
// Created by adamg on 5/13/2024.
//

#include "ANN.h"
//input layer =1
ANN* ANN_init(int num_layers, int* layer_sizes){
    ANN* model = malloc(sizeof(ANN));
    model->num_layers = num_layers;


    model->layer_sizes = layer_sizes;
    model->weights = malloc(sizeof(array*)*num_layers);
    model->biases = malloc(sizeof(array*)*num_layers);
    for(int i=0;i<num_layers-1;i++){
        model->weights[i] = array_rand_init(layer_sizes[i],layer_sizes[i+1]);
        model->biases[i] = vector_col_init(layer_sizes[i+1],0);
    }
    model->learning_rate=0;
    return model;
}