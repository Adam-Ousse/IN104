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
    for(int i=0;i<predictions->shape[0];i++){
        result+= (predictions->values[i][0] -target->values[i][0])*(predictions->values[i][0] -target->values[i][0]);
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

void LinearRegression_fit(LinearRegression* model, array* inputs, array* targets, int num_epochs, double precision,bool debug ,bool normalization ) {
    int epochs = 0;
    double* coef_x, *coef_y;
    double learning_rate = 0.2/max(gershgorin_radius(inputs), gershgorin_radius(targets));
    printf("learning rate : %lf\n",learning_rate);
    if(normalization){
        coef_x = normalize(inputs);
        coef_y = normalize(targets);
        printf("min X : %lf , max X : %lf",coef_x[0],coef_x[1]);
        printf("min Y : %lf , max Y : %lf",coef_y[0],coef_y[1]);
    }


    while(epochs<num_epochs && MSE(LinearRegression_predict(model,inputs),targets)>precision){
        epochs++;
        array* predictions = LinearRegression_predict(model, inputs);
        array* errors = subtract(targets, predictions);

        //for loop
        for(int i=0;i<model->weights->shape[0];i++){
            double gradient =0;
            for(int j=0;j<inputs->shape[0];j++){
                double temp = (targets->values[j][0]-predictions->values[j][0])* inputs->values[j][i];
                gradient-= temp;
            }
            model->weights->values[i][0] -=2*learning_rate* gradient/(inputs->shape[0]);
        }
        double bias_gradient = 0;
        for(int j=0;j<inputs->shape[0];j++){
            bias_gradient-=(targets->values[j][0]-predictions->values[j][0]);
        }
        model->bias -= 2*learning_rate* bias_gradient/(inputs->shape[0]);

        //method matricielle
//        array* errors = subtract(targets, predictions);
//        array* inputs_transpose = transpose(inputs);
//        array* gradient = dot_product(inputs_transpose, errors);
//        array* gradient_scaled = prodc(gradient,  2*learning_rate/(errors->shape[0]) );
//        array* weights_updated = sum(model->weights, gradient_scaled);
//        double bias_gradient =2*learning_rate* sum_all(errors)/(errors->shape[0]) ;
//        model->bias -=  bias_gradient;
//        array_destroy(model->weights);
//        model->weights = weights_updated;
        if(debug){
            printf("Epoch %d MSE : %lf \n",epochs , MSE(predictions,targets));
            printf("weights : ");
            print(model->weights);
            printf("bias : %lf\n",model->bias);
        }
        array_destroy(predictions);
        array_destroy(errors);
        /*array_destroy(inputs_transpose);
        array_destroy(gradient);
        array_destroy(gradient_scaled);*/
    }
    if(normalization){
        model->bias *= (coef_y[1]-coef_y[0]);

        for(int k =0; k<model->weights->shape[0];k++){
            model->weights->values[k][0] *= (coef_y[1]-coef_y[0])/(coef_x[1]-coef_x[0]);
        }

//    model->bias *= (coef_x[1]-coef_x[0]);
//    model->bias += coef_x[0];

        unnormalize(inputs,coef_x);
        unnormalize(targets,coef_y);

    }



}

void LinearRegression_fit_stochastic(LinearRegression* model, array* inputs, array* targets, double learning_rate, int num_epochs,int batch_size, double precision,bool debug ) {
    int epochs = 0;
    double* coef_x = normalize(inputs);

    while(epochs<num_epochs && MSE(LinearRegression_predict(model,inputs),targets)>precision){
        epochs++;
        array* predictions = LinearRegression_predict(model, inputs);
        array* errors = subtract(targets, predictions);

        //for loop
        for(int i=0;i<model->weights->shape[0];i++){
            for(int k=0; k<inputs->shape[0];k+=batch_size){
                double gradient =0;
                double bias_gradient = 0;
                for(int j=k;j<(int)min(k+batch_size,inputs->shape[0]);j++){
                    double temp = (targets->values[j][0]-predictions->values[j][0])* inputs->values[j][i];
                    gradient= temp;
                    bias_gradient+= (targets->values[j][0]-predictions->values[j][0]);
                    model->weights->values[i][0] +=learning_rate* gradient/(inputs->shape[0]);
                    model->bias += learning_rate* bias_gradient/(inputs->shape[0]);
                    array_destroy(predictions);
                    array_destroy(errors);
                    predictions = LinearRegression_predict(model, inputs);
                    errors = subtract(targets, predictions);
                }

            }
        }


        //method matricielle
//        array* errors = subtract(targets, predictions);
//        array* inputs_transpose = transpose(inputs);
//        array* gradient = dot_product(inputs_transpose, errors);
//        array* gradient_scaled = prodc(gradient,  2*learning_rate/(errors->shape[0]) );
//        array* weights_updated = sum(model->weights, gradient_scaled);
//        double bias_gradient =2*learning_rate* sum_all(errors)/(errors->shape[0]) ;
//        model->bias -=  bias_gradient;
//        array_destroy(model->weights);
//        model->weights = weights_updated;
        if(debug){
            printf("Epoch %d MSE : %lf \n",epochs , MSE(predictions,targets));
            printf("weights : ");
            print(model->weights);
            printf("bias : %lf",model->bias);
        }
        array_destroy(predictions);
        array_destroy(errors);
        /*array_destroy(inputs_transpose);
        array_destroy(gradient);
        array_destroy(gradient_scaled);*/
    }
    for(int k =0; k<model->weights->shape[0];k++){
        model->weights->values[k][0] -= coef_x[0];
        model->weights->values[k][0] /= (coef_x[1]-coef_x[0]);

    }
//    model->bias *= (coef_x[1]-coef_x[0]);
//    model->bias += coef_x[0];
    unnormalize(inputs,coef_x);


}
