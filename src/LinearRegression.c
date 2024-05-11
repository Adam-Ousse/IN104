//
// Created by adamg on 4/20/2024.
//

#include "LinearRegression.h"



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

double MSE (array* predictions, array* target){
    double result=0;
//    printf("predictions shape[0] : %d shape[1] : %d, targets shape[0] : %d shape[1] : %d\n",predictions->shape[0],predictions->shape[1],target->shape[0],target->shape[1]);
    assert(predictions->shape[0]==target->shape[0] && predictions->shape[1]==1 &&target->shape[1]==1 );
//    for(int i=0;i<predictions->shape[0];i++){
//        result+= (predictions->values[i][0] -target->values[i][0])*(predictions->values[i][0] -target->values[i][0]);
//    }
//
//    return  result/predictions->shape[0];
array* error = subtract(predictions,target);
result= 1.0/(2*predictions->shape[0]) * prod(transpose(error),error)->values[0][0];
return result;
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
        model->learning_rate = optimal_learning_rate( prod(transpose(inputs),inputs));
    }
    if(normalize){
        coef = min_max_normalize(inputs);
    }
//    if(model->bias ==0){model->bias = mean(inputs);}
    Log(LOG_INFO,"Learning rate %g",model->learning_rate);
    if(debug){
        printf("learning rate : %lf\n",model->learning_rate);
    }

    double mse = MSE(LinearRegression_predict(model,inputs),targets);
    Log(LOG_INFO,"Initial MSE : %lf",mse);
    while(epochs<num_epochs && mse>precision){
        epochs++;
        array* predictions = LinearRegression_predict(model, inputs);
        array* errors = subtract(predictions, targets);
        Log(LOG_INFO,"b errors : %lf", model->learning_rate* sum_all(errors)/(double)(inputs->shape[0]));
        model->bias -= 10000*model->learning_rate* sum_all(errors)/(double)(inputs->shape[0]);
        //for loop
        for(int j=0;j<model->weights->shape[0];j++){
            double gradient =0;
            for(int i=0;i<inputs->shape[0];i++){

                double temp = errors->values[i][j]* inputs->values[i][j];
                gradient+= temp;
            }
            Log(LOG_INFO,"weight errors : %lf", model->learning_rate* gradient/(double)(inputs->shape[0]));
            model->weights->values[j][0] -=model->learning_rate* gradient/(double)(inputs->shape[0]);
        }
//        double bias_gradient = 0;
//        for(int j=0;j<inputs->shape[0];j++){
//            bias_gradient+=errors->values[j][0];
//        }


        //method matricielle
//        array* errors = subtract(targets, predictions);
//        array* inputs_transpose = transpose(inputs);
//        array* gradient = dot_product(inputs_transpose, errors);
//        array* gradient_scaled = prodc(gradient,  learning_rate/(errors->shape[0]) );
//        array* weights_updated = sum(model->weights, gradient_scaled);
//        double bias_gradient =learning_rate* sum_all(errors)/(errors->shape[0]) ;
//        model->bias -=  bias_gradient;
//        array_destroy(model->weights);
//        model->weights = weights_updated;
        mse= MSE(LinearRegression_predict(model,inputs),targets);
        if(debug){

            printf("Epoch %d MSE : %lf \n",epochs , MSE(predictions,targets));
//            printf("weights : ");
//            print(model->weights);
//            printf("bias : %lf\n",model->bias);
        }
        array_destroy(predictions);
        array_destroy(errors);
        /*array_destroy(inputs_transpose);
        array_destroy(gradient);
        array_destroy(gradient_scaled);*/
    }


//    model->bias *= (coef_x[1]-coef_x[0]);
//    model->bias += coef_x[0];
if(normalize){
    min_max_unnormalize(inputs,coef);
}


//        unnormalize(targets,coef_y);

    }






double R2(array* predictions, array* target){
    double mean_target = mean(target);
    array* errors = subtract(target, predictions);
    array* errors_mean = sumc(errors, -mean_target);
    double ssres = sum_all(elementwise_product(errors,errors));
    double sstot = sum_all(elementwise_product(errors_mean,errors_mean));
    array_destroy(errors);
    array_destroy(errors_mean);
    return 1 - (ssres/sstot);


}

void reset(LinearRegression* model){
for(int i=0;i<model->weights->shape[0];i++){
        model->weights->values[i][0] = 0;
    }
    model->bias = 0;
}