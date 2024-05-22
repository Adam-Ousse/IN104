//
// Created by adamg on 5/13/2024.
//

#include "ANN.h"
#include "metrics.h"
//input layer =1
ANN* ANN_init(int num_layers, int* layer_sizes, ACTIVATION_FUNCTION* activation_functions){
    ANN* network = malloc(sizeof(ANN));
    network->num_layers = num_layers;
    network->num_layers_no_input=num_layers-1;
    network->layer_sizes = malloc(sizeof(int) * num_layers);
    if (network->layer_sizes == NULL) {
        Log(LOG_ERROR, "Failed to allocate memory for layer_sizes");
        // handle error, for example by returning NULL or by freeing any previously allocated memory
    }
    memcpy(network->layer_sizes, layer_sizes, sizeof(int) * num_layers);
    for(int i=0;i<num_layers;i++){
        printf("%d \n", network->layer_sizes[i]);
    }

    network->activation_functions = malloc(sizeof(ACTIVATION_FUNCTION) * network->num_layers_no_input);
    memcpy(network->activation_functions, activation_functions, sizeof(ACTIVATION_FUNCTION) * network->num_layers_no_input);
    network->weights = malloc(sizeof(array*) * (network->num_layers_no_input ));
    network->biases = malloc(sizeof(array*) * (network->num_layers_no_input ));
    network->a = malloc(sizeof(array*) * (network->num_layers ));
    network->z = malloc(sizeof(array*) * (network->num_layers_no_input ));
    network->m = malloc(sizeof(array*) * (network->num_layers_no_input ));
    network->v = malloc(sizeof(array*) * (network->num_layers_no_input ));
    network->m_b = malloc(sizeof(array*) * (network->num_layers_no_input ));
    network->v_b = malloc(sizeof(array*) * (network->num_layers_no_input ));
    network->t = 0;
    for(int i=0;i<network->num_layers_no_input;i++){
        network->weights[i]=he_init(network->layer_sizes[i],network->layer_sizes[i+1]);
        network->m[i] = array_init(network->weights[i]->shape[0],network->weights[i]->shape[1],0);
        network->v[i] = array_init(network->weights[i]->shape[0],network->weights[i]->shape[1],0);
        network->biases[i] = vector_row_init(network->layer_sizes[i+1],0);
        network->m_b[i] = vector_row_init(network->layer_sizes[i+1],0);
        network->v_b[i] = vector_row_init(network->layer_sizes[i+1],0);
    }
    return network;
}
void ANN_destroy(ANN* network) {
    free(network->layer_sizes);
    free(network->activation_functions);
    for (int i = 0; i < network->num_layers_no_input; i++) {
        array_destroy(network->weights[i]);
        array_destroy(network->biases[i]);
        array_destroy(network->m[i]);
        array_destroy(network->v[i]);
        array_destroy(network->m_b[i]);
        array_destroy(network->v_b[i]);
        array_destroy(network->a[i]);
        array_destroy(network->z[i]);
    }
    array_destroy(network->a[network->num_layers_no_input]);
    free(network->a);
    free(network->z);
    free(network->weights);
    free(network->biases);
    free(network->m);
    free(network->v);
    free(network->m_b);
    free(network->v_b);
    free(network);
}
array* activation(array* z, ACTIVATION_FUNCTION activation){
    switch (activation) {
        case SIGMOID:
            return transform(z, sigmoid);
        case RELU:
            return transform(z, relu);
        case TANH:
            return transform(z, tanh);
        case IDENTITY:
            return transform(z, identity);
        default:
            return transform(z, sigmoid);
    }
}
array* activation_derivative(array* z, ACTIVATION_FUNCTION activation){
    switch (activation) {
        case SIGMOID:
            return transform(z, sigmoid_prime);
        case RELU:
            return transform(z, relu_prime);
        case TANH:
            return transform(z, tanh_prime);
        case IDENTITY:
            return transform(z, identity);
        default:
            return transform(z, sigmoid_prime);
    }
}

array* forward(ANN* network, array* x){
    assert(x->shape[1] == network->layer_sizes[0]);
    network->a[0]=x;
    for(int i=0;i<network->num_layers_no_input;i++){
//        printf("cehckpoint");
//        printf("i:%d, weights shape : (%d,%d) , a shape (%d,%d)\n",i,network->weights[i]->shape[0],network->weights[i]->shape[1],network->a[i]->shape[0],network->a[i]->shape[1]);
//        network_info(network);
        array* prod_array = prod(network->a[i],network->weights[i]);

        network->z[i] = sum_to_all_row(prod_array,network->biases[i]);
//        printf("z shape : (%d,%d)\n",network->z[i]->shape[0],network->z[i]->shape[1]);
        network->a[i+1] = activation(network->z[i],network->activation_functions[i]);
        array_destroy(prod_array);
    }

    return network->a[network->num_layers_no_input];
}
void backward(ANN* network, array* x, array* y, double learning_rate){
    int m =x->shape[0];
    array* delta = subtract(network->a[network->num_layers_no_input],y);
//    if(network->t>1){
//        for (int i = 0; i < network->num_layers_no_input; i++) {
//            array_destroy(network->m[i]);
//            array_destroy(network->v[i]);
//            array_destroy(network->m_b[i]);
//            array_destroy(network->v_b[i]);
//        }
//    }
    for(int i=network->num_layers_no_input-1;i>=0;i--){
//        printf("hi \n");
        array* transpose_a = transpose(network->a[i]);
        array* prod_w = prod(transpose_a,delta);
        array* dw = divisionc(prod_w,m);
        array_destroy(prod_w);
        array_destroy(transpose_a);
        array* sumdelta = collapse_sum(delta);
        array* db = divisionc(sumdelta,m);
        array_destroy(sumdelta);
        if (i!=0){
            array* z_prime = activation_derivative(network->z[i-1],network->activation_functions[i-1]);
            array* transposed_W = transpose(network->weights[i]);
//            printf("delta shape : (%d,%d)\n",delta->shape[0],delta->shape[1]);
//            printf("transposed shape : (%d,%d)\n",transposed_W->shape[0],transposed_W->shape[1]);
            array* prodd_w = prod(delta,transposed_W);
//            printf("z_prime shape : (%d,%d)\n",z_prime->shape[0],z_prime->shape[1]);

            delta = elementwise_product(prodd_w,z_prime);
            array_destroy(z_prime);
            array_destroy(transposed_W);
            array_destroy(prodd_w);
        }
        adam_update(network,network->weights[i],network->biases[i],dw,db,i,learning_rate);
    }
//    network_info(network);

}

void adam_update(ANN* network, array* w,array* b, array* dw, array* db, int i, double learning_rate){
    network->t +=1;
    array* beta_1_m = prodc(network->m[i],beta1);
    array* beta_1_dw = prodc(dw,1-beta1);
//    printf("lol");
    array_destroy(network->m[i]);
    network->m[i] = sum(beta_1_m,beta_1_dw);
    array_destroy(beta_1_m);
    array_destroy(beta_1_dw);
    array* beta_2_v = prodc(network->v[i],beta2);
    array* dw_square = elementwise_product(dw,dw);
    array* beta_2_dw = prodc(dw_square,1-beta2);
    array_destroy(network->v[i]);
    network->v[i] = sum(beta_2_v,beta_2_dw);
    array_destroy(beta_2_v);
    array_destroy(dw_square);
    array_destroy(beta_2_dw);
    array* m_hat = divisionc(network->m[i],1-pow(beta1,network->t));//done
    array* v_hat = divisionc(network->v[i],1-pow(beta2,network->t));//done
    //Beta :
    array* beta_1_m_b = prodc(network->m_b[i],beta1);
    array* beta_1_db = prodc(db,1-beta1);
    network->m_b[i] = sum(beta_1_m_b,beta_1_db);
    array_destroy(beta_1_m_b);
    array_destroy(beta_1_db);
    array* beta_2_v_b = prodc(network->v_b[i],beta2);
    array* db_square = elementwise_product(db,db);
    array* beta_2_db = prodc(db_square,1-beta2);
    network->v_b[i] = sum(beta_2_v_b,beta_2_db);
    array_destroy(beta_2_v_b);
    array_destroy(db_square);
    array_destroy(beta_2_db);
    array* m_hat_b = divisionc(network->m_b[i],1-pow(beta1,network->t));
    array* v_hat_b = divisionc(network->v_b[i],1-pow(beta2,network->t));
    array* sqrt_v_hat = transform(v_hat,sqrt);
    array* sqrt_v_hat_b = transform(v_hat_b,sqrt);
    array* epsilon_v_hat = sumc(sqrt_v_hat,epsilon);
    array* epsilon_v_hat_b = sumc(sqrt_v_hat_b,epsilon);
    array* m_hat_div_epsilon = elementwise_division(m_hat,epsilon_v_hat);
    array* m_hat_div_epsilon_b = elementwise_division(m_hat_b,epsilon_v_hat_b);
    array* learning_rate_m_hat_div_epsilon = prodc(m_hat_div_epsilon,learning_rate);
    array* learning_rate_m_hat_div_epsilon_b = prodc(m_hat_div_epsilon_b,learning_rate);
    array* new_w = subtract(w,learning_rate_m_hat_div_epsilon);
//    printf("new_w\n");
//    info(new_w);
    array* new_b = subtract(b,learning_rate_m_hat_div_epsilon_b);
//    printf("new_b\n");
//    info(new_b);
    array_destroy(w);
    network->weights[i]=new_w;
    array_destroy(b);
    network->biases[i]=new_b;
    array_destroy(m_hat);
    array_destroy(v_hat);
    array_destroy(m_hat_b);
    array_destroy(v_hat_b);
    array_destroy(sqrt_v_hat);
    array_destroy(sqrt_v_hat_b);
    array_destroy(epsilon_v_hat);
    array_destroy(epsilon_v_hat_b);
    array_destroy(m_hat_div_epsilon);
    array_destroy(m_hat_div_epsilon_b);
    array_destroy(learning_rate_m_hat_div_epsilon);
    array_destroy(learning_rate_m_hat_div_epsilon_b);

}

array* train(ANN* network, array* x, array* y, int epochs, double learning_rate){
    array* loss = vector_col_init(epochs,0);
    array* output;
    array* temp;
    array* temp_2;

    for(int i=0;i<epochs;i++){
//        network_info(network);
        output = forward(network,x);
        temp= subtract(output,y);
        temp_2 = elementwise_product(temp,temp);
        loss->values[i][0] = mean(temp_2);
        array_destroy(temp);
        array_destroy(temp_2);
        backward(network,x,y,learning_rate);
        if(i%1 ==0){
            printf("Epoc : %d , loss :%lf\n", i,loss->values[i][0]);
        }
    }
    return loss;
}
void network_info(ANN* network){
    printf("Sizes : ");
    for(int i=0;i<network->num_layers;i++){
        printf("%d ",network->layer_sizes[i]);
    }
    printf("\n Activation functions : ");
    for(int i=0;i<network->num_layers_no_input;i++){
        switch (network->activation_functions[i]){
            case RELU :
                printf("RELU ");
                break;
            case SIGMOID :
                printf("SIGMOID ");
                break;
            case IDENTITY:
                printf("IDENTITY ");
                break;
        }
    }
    for(int i=0;i<network->num_layers_no_input;i++){
        printf("Weights %d : shape : (%d,%d) \n",i,network->weights[i]->shape[0],network->weights[i]->shape[1]);
        printf("Biases %d : shape : (%d,%d) \n",i, network->biases[i]->shape[0],network->biases[i]->shape[1]);

    }
    printf("\n");
}
//ANN* ANN_init(int num_layers, int* layer_sizes, ACTIVATION_FUNCTION* activationFunctions) {
//    //num of layers including both input and output
//    ANN* network = malloc(sizeof(ANN));
//    network->activation_functions=malloc(sizeof(ACTIVATION_FUNCTION)*(num_layers-1));
//    memcpy(network->activation_functions,activationFunctions,sizeof(ACTIVATION_FUNCTION)*(num_layers-1));
//    network->num_layers = num_layers;
//    //we don't need to make a layer for the inputs
//    network->layer_sizes = malloc(sizeof(int) * (num_layers-1));
//    memcpy(network->layer_sizes, layer_sizes, sizeof(int) * num_layers);
//
//    network->layers = malloc(sizeof(layer*) * num_layers);
//    //the first layer is the first hidden layer of size input layer size * first hidden layer size
//    for (int i = 0; i < num_layers-1; i++) {
//        network->layers[i] = layer_init(layer_sizes[i], layer_sizes[i+1],network->activation_functions[i]);
//    }
//
//    return network;
//}
//void layer_destroy(layer* l) {
//    array_destroy(l->weights);
//    array_destroy(l->biases);
//    array_destroy(l->activation);
//    array_destroy(l->preactivation);
//    array_destroy(l->delta);
//    free(l);
//}
//
//void ANN_destroy(ANN* network) {
//    for (int i = 0; i < network->num_layers-1; i++) {
//        layer_destroy(network->layers[i]);
//    }
//    free(network->layers);
//    free(network->layer_sizes);
//    free(network->activation_functions);
//    free(network);
//}
//
//array* activate(array* input, array* weights, array* biases, double (*activation_func)(double)) {
//    assert(input->shape[1] == weights->shape[0]);
////    printf("input shape %d %d\n",input->shape[0],input->shape[1]);
////    printf("weights shape %d %d\n",weights->shape[0],weights->shape[1]);
////    printf("biases shape %d %d\n",biases->shape[0],biases->shape[1]);
//    assert(weights->shape[1] == biases->shape[1]);
//    array* product = prod(input, weights);
//    array* z = sum_to_all_row(product, biases);
//    array* result = transform(z, activation_func);
//    array_destroy(z); // Free the intermediate array
//    array_destroy(product); // Free the intermediate array
//    return result;
//}
//void network_info(ANN* network) {
//    for (int layer_index = 0; layer_index < network->num_layers - 1; layer_index++) {
//        layer *l = network->layers[layer_index];
//        printf("Layer %d\n", layer_index);
//        printf("preactivation shape : (%d,%d)\n", l->preactivation->shape[0], l->preactivation->shape[1]);
//        printf("activation shape : (%d,%d)\n", l->activation->shape[0], l->activation->shape[1]);
//        printf("weights shape : (%d,%d)\n", l->weights->shape[0], l->weights->shape[1]);
//        printf("biases shape : (%d,%d)\n", l->biases->shape[0], l->biases->shape[1]);
//    }
//}
//array* forward_propagate(ANN* network, array* input) {
//    array* current_input = input;
//    Log(LOG_INFO,"Forward propagation input : (%d, %d)\n",input->shape[0],input->shape[1]);
//    for (int i = 0; i < network->num_layers-1; i++) {
//
//        layer* l = network->layers[i];
//
//            // Hidden and output layers
////        array_destroy(l->activation);
//            switch (l->activation_function) {
//                case SIGMOID:
//                    l->preactivation = activate(current_input,l->weights,l->biases,identity);
//                    l->activation = activate(current_input, l->weights, l->biases, sigmoid);
//
//                    break;
//                case RELU:
//                    l->preactivation = activate(current_input, l->weights, l->biases, identity);
//                    l->activation = activate(current_input, l->weights, l->biases, relu);
//                    break;
//                case TANH:
//                    l->preactivation = activate(current_input, l->weights, l->biases, identity);
//                    l->activation = activate(current_input, l->weights, l->biases, tanh);
//                    break;
//                case IDENTITY:
//                    l-> activation = activate(current_input, l->weights, l->biases, identity);
//                    l->preactivation = l->activation;
//                    break;
//                default:
//                    l->preactivation = activate(current_input,l->weights,l->biases,identity);
//                    l->activation = activate(current_input, l->weights, l->biases, sigmoid);
//            }
////        printf("Layer inside propagation %d\n", i);
////        printf("preactivation shape : (%d,%d)\n", l->preactivation->shape[0], l->preactivation->shape[1]);
////        printf("activation shape : (%d,%d)\n", l->activation->shape[0], l->activation->shape[1]);
////        printf("weights shape : (%d,%d)\n", l->weights->shape[0], l->weights->shape[1]);
////        printf("biases shape : (%d,%d)\n", l->biases->shape[0], l->biases->shape[1]);
////            if (i > 1) {
////                array_destroy(current_input); // Free the previous input if it's not the very first input
////            }
//            current_input = l->activation;
//
//    }
////    printf("Done propagating\n");
//    return network->layers[network->num_layers-2]->activation;
//}
//void backpropagate(ANN* network, array* input, array* target, double learning_rate) {
//    //forward propagation
//    int N = input->shape[0];
////    printf("inside bakcprop\n");
////    network_info(network);
//    for (int training_index = 0; training_index < N; training_index++) {
//        array *training_input = subset(input, training_index, training_index + 1);
//        array *training_target = subset(target, training_index, training_index + 1);
//        layer *output_layer = network->layers[network->num_layers - 2];
//
//        array *error = subtract(training_input, training_target); // Assuming array_subtract is defined
//        array *Z = subset(output_layer->preactivation, training_index, training_index + 1);
////        printf("checkpoin\n");
////        print(Z);
//        Log(LOG_INFO, "Checkpoint 1");
//        Log(LOG_INFO, "Calculating delta for output layer for input number %d\n", training_index);
//        switch (output_layer->activation_function) {
//            case SIGMOID:
//                output_layer->delta = elementwise_product(error, transform(Z, sigmoid_prime));
//                break;
//            case RELU:
//                output_layer->delta = elementwise_product(error, transform(Z, relu_prime));
//                break;
//            case TANH:
//                output_layer->delta = elementwise_product(error, transform(Z, tanh_prime));
//                break;
//            case IDENTITY:
//                output_layer->delta = prodc(error, 1);
//                break;
//            default:
//                output_layer->delta = elementwise_product(error, transform(Z, sigmoid_prime));
//        }
////        printf("Delta for output layer \n");
////        info(output_layer->delta);
//        array_destroy(error);
//
//        layer *l;
//        for (int layer_index = network->num_layers - 3; layer_index >= 0; layer_index--) {
//            l = network->layers[layer_index];
//            Z = subset(l->preactivation, training_index, training_index + 1);
//            array *delta1 = network->layers[layer_index + 1]->delta;
//            array *transposed_weights = transpose(network->layers[layer_index + 1]->weights);
//            array *prod1 = prod(delta1, transposed_weights);
////            printf("layer %d , delta1 shape : (%d,%d) , weight sahpe : (%d,%d), prod1 shape : (%d,%d)\n",layer_index,delta1->shape[0],delta1->shape[1],transposed_weights->shape[0],transposed_weights->shape[1],prod1->shape[0],prod1->shape[1]);
////            printf("Z shape : (%d,%d)\n",Z->shape[0],Z->shape[1]);
//            Log(LOG_INFO, "Checkpoint 2");
//            Log(LOG_INFO, "Calculating delta for layer %d for input number %d\n",layer_index, training_index);
//            switch (l->activation_function) {
//                case SIGMOID:
//                    l->delta = elementwise_product(prod1, transform(Z, sigmoid_prime));
//                    break;
//                case RELU:
//                    l->delta = elementwise_product(prod1, transform(Z, relu_prime));
//                    break;
//                case TANH:
//                    l->delta = elementwise_product(prod1, transform(Z, tanh_prime));
//                    break;
//                case IDENTITY:
//                    l->delta = prodc(prod1, 1);
//                    break;
//                default:
//                    l->delta = elementwise_product(prod1, transform(Z, sigmoid_prime));
//            }
//            array_destroy(transposed_weights);
//            array_destroy(prod1);
//
//        }
//        for (int layer_index = 0; layer_index < network->num_layers - 1; layer_index++) {
//            l = network->layers[layer_index];
//            array *delta = l->delta;
////            printf("delta %d\n",layer_index);
////            print(delta);
//            array* activation_previous_layer ;
//            if(layer_index==0){
//                activation_previous_layer = subset(input, training_index, training_index + 1);
//            }else{
//                activation_previous_layer = subset(network->layers[layer_index - 1]->activation, training_index, training_index + 1);
//            }
//            array* scaled_delta = prodc(delta, learning_rate);
//
//            array* new_bias = subtract(l->biases, scaled_delta);
//
//            array_destroy(l->biases);
//            l->biases = new_bias;
//
//
//            array_destroy(scaled_delta);
////            printf("weights shape : (%d,%d)\n",l->weights->shape[0],l->weights->shape[1]);
////            printf("activation_previous_layer shape : (%d,%d)\n",activation_previous_layer->shape[0],activation_previous_layer->shape[1]);
////            printf("delta shape : (%d,%d)\n",delta->shape[0],delta->shape[1]);
//            for(int i=0;i<l->weights->shape[0];i++){
//                for(int j=0;j<l->weights->shape[1];j++){
//                    double delta_w = activation_previous_layer->values[0][i] * delta->values[0][j];
//                    l->weights->values[i][j] -= learning_rate*delta_w/N;
//                }
//            }
////            print(l->weights);
//        }
//    }
//}
//
//void train(ANN* network, array* input, array* target, double learning_rate,int num_epochs, double precision){
//    int n=0;
//
//    double mse = MSE(forward_propagate(network,input),target);
////        network_info(network);
//        char a[]="adam";
////        scanf("%s",&a);
//    printf("Initial MSE : %lf\n",mse);
//    while(n<num_epochs && mse>precision){
//        n++;
//        backpropagate(network,input,target,learning_rate);
////        network_info(network);
////        scanf("%s",a);
//
//        mse = MSE(forward_propagate(network,input),target);
////        printf("shape output : %d,%d\n",network->layers[network->num_layers-2]->activation->shape[0],network->layers[network->num_layers-2]->activation->shape[1]);
//        printf("Current MSE after epoch %d: %lf\n", n, mse);
//    }
//}
////void backpropagate(ANN* network, array* input, array* target, double learning_rate) {
////    //forward propagation
////    int N = input->shape[0];
////
//////    forward_propagate(network, input);
////
////    for (int training_index = 0; training_index < N; training_index++) {
////        Log(LOG_INFO,"Checkpoint 1");
////        Log(LOG_INFO,"Calculating delta for output layer for input number %d\n",training_index);
////
////        layer *output_layer = network->layers[network->num_layers - 2];
////        array *error = subtract(subset(output_layer->activation, training_index, training_index + 1),
////                                subset(target, training_index,
////                                       training_index + 1)); // Assuming array_subtract is defined
////        printf("Checkpoint 1\n");
////                                       //        printf("Error %d: \n",training_index);
//////        info(error);
////        for (int j = 0; j < output_layer->delta->shape[1]; j++) {
////            double delta = 0;
////            switch (output_layer->activation_function) {
////                case SIGMOID:
////                    delta = (sigmoid_prime(output_layer->preactivation->values[0][j]) * error->values[0][j]) / N;
////
////                    break;
////                case RELU:
////                    delta = (relu_prime(output_layer->preactivation->values[0][j]) * error->values[0][j]) / N;
////                    break;
////                case TANH:
////                    delta = (tanh_prime(output_layer->preactivation->values[0][j]) * error->values[0][j]) / N;
////                    break;
////                case IDENTITY:
////                    delta = error->values[0][j] / N;
////                    break;
////                default:
////                    delta = (sigmoid_prime(output_layer->preactivation->values[0][j]) * error->values[0][j]) / N;
////            }
////            output_layer->delta->values[0][j] = delta;
////        }
//////        printf("Delta for output layer %d: \n",training_index);
//////        print(output_layer->delta);
//////        printf("Checkpoint 2\n");
////        layer *l;
////        for (int layer_index = network->num_layers - 3; layer_index >= 0; layer_index--) {
////            l = network->layers[layer_index];
////            array *Z = subset(l->preactivation, training_index, training_index + 1);
//////            printf("Checkpoint 2.1\n");
//////            printf("Z : \n");
//////            info(Z);
////            for (int j = 0; j < l->delta->shape[1]; j++) {
////                double delta = 0;
//////                printf("j<%d, i<%d : \n", l->delta->shape[1],network->layers[layer_index + 1]->delta->shape[1]);
//////                info(network->layers[layer_index + 1]->weights);
////                switch (l->activation_function) {
////                    case SIGMOID:
////                        for (int i = 0; i < network->layers[layer_index + 1]->delta->shape[1]; i++) {
////                            delta += network->layers[layer_index + 1]->delta->values[0][i] *
////                                     network->layers[layer_index + 1]->weights->values[j][i];
////                        }
////                        delta *= sigmoid_prime(Z->values[0][j]) / N;
////                        break;
////                    case RELU:
////                        for (int i = 0; i < network->layers[layer_index + 1]->delta->shape[1]; i++) {
//////                            printf("%d %d\n",j,i);
//////                            print(network->layers[layer_index + 1]->delta);
//////                            print(network->layers[layer_index + 1]->weights);
////                            delta += network->layers[layer_index + 1]->delta->values[0][i] *
////                                     network->layers[layer_index + 1]->weights->values[j][i];
//////                            printf("success");
////                        }
////                        delta *= relu_prime(Z->values[0][j]) / N;
//////                        printf("Checkpoint 2.2\n");
////                        break;
////                    case TANH:
////                        for (int i = 0; i < network->layers[layer_index + 1]->delta->shape[1]; i++) {
////                            delta += network->layers[layer_index + 1]->delta->values[0][i] *
////                                     network->layers[layer_index + 1]->weights->values[j][i];
////                        }
////                        delta *= tanh_prime(Z->values[0][j]) / N;
////                        break;
////                    case IDENTITY:
////                        for (int i = 0; i < network->layers[layer_index + 1]->delta->shape[1]; i++) {
////                            delta += network->layers[layer_index + 1]->delta->values[0][i] *
////                                     network->layers[layer_index + 1]->weights->values[j][i];
////                        }
////                        delta /= N;
////                        break;
////                    default:
////                        for (int i = 0; i < network->layers[layer_index + 1]->delta->shape[1]; i++) {
////                            delta += network->layers[layer_index + 1]->delta->values[0][i] *
////                                     network->layers[layer_index + 1]->weights->values[j][i];
////                        }
////                        delta *= sigmoid_prime(Z->values[0][j]) / N;
////                }
////
////                l->delta->values[0][j] = delta;
////            }
//////            printf("Checkpoint 3\n");
////        }
////
////        for (int layer_index = network->num_layers - 2; layer_index >= 0; layer_index--) {
//////            printf("Checkpoint 4\n");
////            l = network->layers[layer_index];
////            double delta_w=0;
////            double delta_b = 0;
////            for (int i = 0; i < l->weights->shape[0]; i++) {
////                for (int j = 0; j < l->weights->shape[1]; j++) {
////
////                    if (layer_index == 0) {
////                        delta_w = l->delta->values[0][j] * input->values[training_index][i];
////                    } else {
////                        delta_w = l->delta->values[0][j] * network->layers[layer_index - 1]->activation->values[0][i];
////                    }
////                    l->weights->values[i][j] += learning_rate * delta_w;
////                }
////            }
////            for (int j = 0; j < l->biases->shape[1]; j++) {
////                delta_b = l->delta->values[0][j];
////                l->biases->values[0][j] += learning_rate * delta_b;
////            }
////        }
////    }
////}