//
// Created by adamg on 5/12/2024.
//

#include "metrics.h"

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

double MSE (array* predictions, array* target){
    double result=0;
    assert(predictions->shape[0]==target->shape[0] && predictions->shape[1]==1 &&target->shape[1]==1 );

    array* error = subtract(predictions,target);
    array* transposed_error = transpose(error);
    array* product = prod(transposed_error, error);

    result= 1.0/(2*predictions->shape[0]) * product->values[0][0];

    array_destroy(error);
    array_destroy(transposed_error);
    array_destroy(product);

    return result;
}