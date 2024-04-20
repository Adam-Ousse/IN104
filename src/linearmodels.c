//
// Created by adamg on 4/20/2024.
//

#include "linearmodels.h"



LinearRegression* LinearRegression_init(int num_features) {
    LinearRegression* model = malloc(sizeof(LinearRegression));
    model->weights = vector_col_init(num_features,0);
    model->bias = 0;
    return model;
}

void LinearRegression_destroy(LinearRegression* model) {
    array_destroy(model->weights);
    free(model);
}

double MSE (array* predictions, array* target){
    double result=0;
//    printf("predictions shape[0] : %d shape[1] : %d, targets shape[0] : %d shape[1] : %d\n",predictions->shape[0],predictions->shape[1],target->shape[0],target->shape[1]);
    assert(predictions->shape[0]==target->shape[0] && predictions->shape[1]==1 &&target->shape[1]==1 );
    for(int j=0;j<predictions->shape[1];j++){
        result+= (predictions->values[j][0] -target->values[j][0])*(predictions->values[j][0] -target->values[j][0]);
    }
    return  result/predictions->shape[0];

}
array* LinearRegression_predict(LinearRegression* model, array* inputs) {
    array* predictions = dot_product(inputs, model->weights);
    array* biases = vector_col_init(inputs->shape[0], model->bias);
    array* result = sum(predictions, biases);
    array_destroy(predictions);
    array_destroy(biases);
    return result;
}

void LinearRegression_fit(LinearRegression* model, array* inputs, array* targets, double learning_rate, int num_epochs, double precision) {
    int epochs = 0;
    while(epochs<num_epochs && MSE(LinearRegression_predict(model,inputs),targets)>precision){
        epochs++;
        array* predictions = LinearRegression_predict(model, inputs);
        /*printf("predictions \n");
        print(predictions);*/
        array* errors = subtract(targets, predictions);
        array* inputs_transpose = transpose(inputs);
        array* gradient = prod(inputs_transpose, errors);
        array* gradient_scaled = prodc(gradient,  learning_rate );
        /*printf("gradient \n");
        print(gradient_scaled);*/
        array* weights_updated = sum(model->weights, gradient_scaled);
        double bias_gradient = sumc(errors, 0)->values[0][0] ;
        model->bias -= learning_rate * bias_gradient;
        array_destroy(model->weights);
        model->weights = weights_updated;

        printf("Epoch %d MSE : %lf \n",epochs , MSE(predictions,targets));
        printf("weights : ");
        print(model->weights);
        printf("bias : %lf",model->bias);
        array_destroy(predictions);
        array_destroy(errors);
        array_destroy(inputs_transpose);
        array_destroy(gradient);
        array_destroy(gradient_scaled);
    }
}