//
// Created by adamg on 5/13/2024.
//

#include "ANN.h"

//input layer =1
layer* layer_init(int input_size, int output_size, ACTIVATION_FUNCTION activation_function) {
    layer* l = malloc(sizeof(layer));
    l->weights = he_init(input_size, output_size);
    l->biases = vector_row_init(output_size, 0);
    l->activation = array_init(1, output_size, 0);
    l->delta = array_init(1, output_size, 0);
    l->activation_function= activation_function;
    return l;
}
ANN* ANN_init(int num_layers, int* layer_sizes, ACTIVATION_FUNCTION* activationFunctions) {
    //num of layers including both input and output
    ANN* network = malloc(sizeof(ANN));
    network->activation_functions=malloc(sizeof(ACTIVATION_FUNCTION)*(num_layers-1));
    memcpy(network->activation_functions,activationFunctions,sizeof(ACTIVATION_FUNCTION)*(num_layers-1));
    network->num_layers = num_layers;
    //we don't need to make a layer for the inputs
    network->layer_sizes = malloc(sizeof(int) * (num_layers-1));
    memcpy(network->layer_sizes, layer_sizes, sizeof(int) * num_layers);

    network->layers = malloc(sizeof(layer*) * num_layers);
    //the first layer is the first hidden layer of size input layer size * first hidden layer size
    for (int i = 0; i < num_layers-1; i++) {
        network->layers[i] = layer_init(layer_sizes[i], layer_sizes[i+1],network->activation_functions[i]);
    }

    return network;
}
void layer_destroy(layer* l) {
    array_destroy(l->weights);
    array_destroy(l->biases);
    array_destroy(l->activation);
    array_destroy(l->delta);
    free(l);
}

void ANN_destroy(ANN* network) {
    for (int i = 0; i < network->num_layers-1; i++) {
        layer_destroy(network->layers[i]);
    }
    free(network->layers);
    free(network->layer_sizes);
    free(network->activation_functions);
    free(network);
}

array* activate(array* input, array* weights, array* biases, double (*activation_func)(double)) {
    assert(input->shape[1] == weights->shape[0]);
//    printf("input shape %d %d\n",input->shape[0],input->shape[1]);
//    printf("weights shape %d %d\n",weights->shape[0],weights->shape[1]);
//    printf("biases shape %d %d\n",biases->shape[0],biases->shape[1]);
    assert(weights->shape[1] == biases->shape[1]);
    array* product = prod(input, weights);
    array* z = sum_to_all_row(product, biases);
    array* result = transform(z, activation_func);
    array_destroy(z); // Free the intermediate array
    array_destroy(product); // Free the intermediate array
    return result;
}
array* forward_propagate(ANN* network, array* input) {
    array* current_input = input;
    for (int i = 0; i < network->num_layers-1; i++) {
        layer* l = network->layers[i];

            // Hidden and output layers
        array_destroy(l->activation);
            switch (l->activation_function) {
                case SIGMOID:

                    l->activation = activate(current_input, l->weights, l->biases, sigmoid);
                    break;
                case RELU:
                    l->activation = activate(current_input, l->weights, l->biases, relu);
                    break;
                case TANH:
                    l->activation = activate(current_input, l->weights, l->biases, tanh);
                    break;
                case IDENTITY:
                    l-> activation = activate(current_input, l->weights, l->biases, identity);
                    break;
                default:
                    l->activation = activate(current_input, l->weights, l->biases, sigmoid);
            }

            if (i > 1) {
                array_destroy(current_input); // Free the previous input if it's not the very first input
            }
            current_input = l->activation;

    }
    return network->layers[network->num_layers-2]->activation;
}
