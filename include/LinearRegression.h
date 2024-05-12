//
// Created by adamg on 4/20/2024.
//

#ifndef IN104_LINEARREGRESSION_H
#define IN104_LINEARREGRESSION_H
#include "utils.h"
#include "array.h"
#include "mathextra.h"
typedef struct LinearRegression {
    array* weights;
    double bias;
    double learning_rate;
    double* coef;
} LinearRegression;
LinearRegression* LinearRegression_init(int num_features);
void LinearRegression_destroy(LinearRegression* model);

array* LinearRegression_predict(LinearRegression* model, array* inputs);
void LinearRegression_fit(LinearRegression* model, array* inputs, array* targets,int num_epochs, double precision,bool debug ,bool normalize );
void LinearRegression_fit_stochastic(LinearRegression* model, array* inputs, array* targets, double learning_rate, int num_epochs,int batch_size, double precision,bool debug);

void reset(LinearRegression* model);
#endif //IN104_LINEARREGRESSION_H
