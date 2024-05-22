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
            array_destroy(delta);
            delta = elementwise_product(prodd_w,z_prime);
            array_destroy(z_prime);
            array_destroy(transposed_W);
            array_destroy(prodd_w);
        }
        adam_update(network,dw,db,i,learning_rate);
        array_destroy(dw);
        array_destroy(db);
    }
    array_destroy(delta);
//    network_info(network);

}

void adam_update(ANN* network, array* dw, array* db, int i, double learning_rate){
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
    array* new_w = subtract(network->weights[i],learning_rate_m_hat_div_epsilon);
//    printf("new_w\n");
//    info(new_w);
    array* new_b = subtract(network->biases[i],learning_rate_m_hat_div_epsilon_b);
//    printf("new_b\n");
//    info(new_b);
    array_destroy(network->weights[i]);
    network->weights[i]=new_w;
    array_destroy(network->biases[i]);
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
        output = forward(network,x);//this is the a[-1] it shoudlnt be array_destroyed
        temp= subtract(output,y);
        temp_2 = elementwise_product(temp,temp);
        loss->values[i][0] = mean(temp_2);
        array_destroy(temp);
        array_destroy(temp_2);
        backward(network,x,y,learning_rate);
        if(i%1 ==0){
            printf("Epoc : %d , loss :%lf\n", i,loss->values[i][0]);
        }
        for(int j=1;j<network->num_layers_no_input;j++){//a[0] is the input
            array_destroy(network->a[j]);
            array_destroy(network->z[j]);
        }
        array_destroy(network->z[0]);
        array_destroy(output);
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
