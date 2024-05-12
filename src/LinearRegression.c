//
// Created by adamg on 4/20/2024.
//

#include "LinearRegression.h"
#include "metrics.h"


LinearRegression* LinearRegression_init(int num_features) {
    LinearRegression* model = malloc(sizeof(LinearRegression));
    model->weights = vector_col_init(num_features,0);
    model->bias = 0;
    model->learning_rate=0;

    return model;
}

void LinearRegression_destroy(LinearRegression* model) {
    array_destroy(model->weights);

    free(model);

}

array* LinearRegression_predict(LinearRegression* model, array* inputs) {

    array* predictions = dot_product(inputs, model->weights);

    array* biases = array_init(inputs->shape[0],inputs->shape[1], 10000*model->bias);
    array* result = sum(predictions, biases);
    array_destroy(predictions);
    array_destroy(biases);
    return result;
}

void LinearRegression_fit(LinearRegression* model, array* inputs, array* targets, int num_epochs, double precision,bool debug,bool normalize) {
    int epochs = 0;
    double* coef;
//    double learning_rate =2/gershgorin_radius( dot_product(transpose(inputs),inputs));
    if(model->learning_rate ==0){
        array * eigen_matrix= prod(transpose(inputs),inputs);
        model->learning_rate = optimal_learning_rate( eigen_matrix);
        array_destroy(eigen_matrix);
    }
//    if(model->bias ==0){model->bias = mean(inputs);}
    Log(LOG_INFO,"Learning rate %g",model->learning_rate);
    if(debug){
        printf("learning rate : %lf\n",model->learning_rate);
    }
    array* predictions = LinearRegression_predict(model, inputs);
    array* errors = subtract(predictions, targets);
    double mse = MSE(predictions,targets);
    Log(LOG_INFO,"Initial MSE : %lf",mse);
    while(epochs<num_epochs && mse>precision) {
        epochs++;
        Log(LOG_INFO, "b errors : %lf", model->learning_rate * sum_all(errors) / (double) (inputs->shape[0]));
        model->bias -= 1000 / (double) (inputs->shape[0]) * model->learning_rate * sum_all(errors);

        array* transposed_inputs = transpose(inputs);
        array* dot_product_result = dot_product(transposed_inputs, errors);
        array* prodc_result = prodc(dot_product_result, model->learning_rate / (double) (inputs->shape[0]));

        array* old_weights = model->weights;
        model->weights = subtract(model->weights, prodc_result);

        array_destroy(transposed_inputs);
        array_destroy(dot_product_result);
        array_destroy(prodc_result);
        array_destroy(old_weights);
        //for loop
//        model->bias -= 10000*model->learning_rate* sum_all(errors)/(double)(inputs->shape[0]);
//        for(int j=0;j<model->weights->shape[0];j++){
//            double gradient =0;
//            for(int i=0;i<inputs->shape[0];i++){
//
//                double temp = errors->values[i][j]* inputs->values[i][j];
//                gradient+= temp;
//            }
//            Log(LOG_INFO,"weight errors : %lf", model->learning_rate* gradient/(double)(inputs->shape[0]));
//            model->weights->values[j][0] -=model->learning_rate* gradient/(double)(inputs->shape[0]);
//        }
        array_destroy(predictions);
        predictions= LinearRegression_predict(model, inputs);
        array_destroy(errors);
        errors = subtract(predictions, targets);
        mse= MSE(predictions,targets);
        if(debug){

            printf("Epoch %d MSE : %lf \n",epochs , MSE(predictions,targets));
//            printf("weights : ");
//            print(model->weights);
//            printf("bias : %lf\n",model->bias);
        }

//    }
    }
    array_destroy(predictions);
    array_destroy(errors);

    }

void reset(LinearRegression* model){
for(int i=0;i<model->weights->shape[0];i++){
        model->weights->values[i][0] = 0;
    }
    model->bias = 0;
}