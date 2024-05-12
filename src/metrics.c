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
//    printf("predictions shape[0] : %d shape[1] : %d, targets shape[0] : %d shape[1] : %d\n",predictions->shape[0],predictions->shape[1],target->shape[0],target->shape[1]);
    assert(predictions->shape[0]==target->shape[0] && predictions->shape[1]==1 &&target->shape[1]==1 );
//    for(int i=0;i<predictions->shape[0];i++){
//        result+= (predictions->values[i][0] -target->values[i][0])*(predictions->values[i][0] -target->values[i][0]);
//    }
//
//    return  result/predictions->shape[0];
    array* error = subtract(predictions,target);
    result= 1.0/(2*predictions->shape[0]) * prod(transpose(error),error)->values[0][0];
    array_destroy(error);
    return result;
}