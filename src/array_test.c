//
// Created by adamg on 5/10/2024.
//

#include "array_test.h"
#include "array.h"
#include "utils.h"

void main_test(){
    test_array_init();
    test_transpose();
}
void test_array_init() {
    array* A = array_init(2, 2, 0);
    assert(A->shape[0] == 2);
    assert(A->shape[1] == 2);
    assert(A->values[0][0] == 0);
    array_destroy(A);
    Log(LOG_INFO,"array_init passed ✅");
}

void test_transpose() {
    array* A = array_rand_init(rand()%10 +1,rand()%10 +1);
    array* T = transpose(A);
    assert(T->shape[0] == A->shape[1]);
    assert(T->shape[1] == A->shape[0]);
    assert(equal(transpose(T),A));
    array_destroy(A);
    array_destroy(T);
    Log(LOG_INFO,"transpose passed ✅");
}




