//
// Created by adamg on 5/12/2024.
//

#ifndef IN104_METRICS_H
#define IN104_METRICS_H
#include "array.h"
#include "utils.h"
double MSE (array* predictions, array* target);
double R2(array* predictions, array* target);

#endif //IN104_METRICS_H
