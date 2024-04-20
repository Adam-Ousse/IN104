//
// Created by adamg on 4/20/2024.
//

#ifndef IN104_LINEARMODELS_H
#define IN104_LINEARMODELS_H
#include "utils.h"
#include "array.h"
#include "mathextra.h"
typedef struct LinearRegression {
    array* weights;
    double bias;
} LinearRegression;
LinearRegression* LinearRegression_init(int num_features);
void LinearRegression_destroy(LinearRegression* model);
double MSE (array* predictions, array* target);
array* LinearRegression_predict(LinearRegression* model, array* inputs);
void LinearRegression_fit(LinearRegression* model, array* inputs, array* targets, double learning_rate, int num_epochs, double precision);
#endif //IN104_LINEARMODELS_H
