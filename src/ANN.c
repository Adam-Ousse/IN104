//
// Created by adamg on 5/13/2024.
//

#include "ANN.h"

//input layer =1
ANN* ANN_init(int num_layers, int* layer_sizes){
    ANN* model = malloc(sizeof(ANN));
    model->num_layers = num_layers;
    model->layer_sizes = layer_sizes;
    model->weights = malloc(sizeof(array*)*(num_layers-1));
    model->biases = malloc(sizeof(array*)*(num_layers-1));
    for(int i=0;i<num_layers-1;i++){
        model->weights[i] = array_rand_init(layer_sizes[i],layer_sizes[i+1]);
        model->biases[i] = vector_col_init(layer_sizes[i+1],0);
    }
    model->learning_rate=0;
    return model;
}

array* ForwardPass(ANN* model,array* X){
    assert(X->shape[0]==model->layer_sizes[0]);
//    array *Y = array_init(model->layer_sizes[model->num_layers-1],X->shape[1],0);
    array* product = dot_product(X,model->weights[0]);
    array* Z = sum(product,model->biases[0]);
    array* A = transform(Z,sigmoid);
    for(int j =1; j <model->num_layers-1;j++){
        array_destroy(product);
        array_destroy(Z);
        product = dot_product(A,model->weights[j]);
        Z= sum(product,model->biases[j]);
        array_destroy(A);
        A = transform(Z,sigmoid);
    }
    array_destroy(product);
    array_destroy(Z);
    product = dot_product(A,model->weights[model->num_layers-1]);
//    Z= sum(product,model->biases[model->num_layers-1]);
    array_destroy(A);
    array* Y = sum(product,model->biases[model->num_layers-1]);
    array_destroy(product);
    array_destroy(Z);
    return Y;
}