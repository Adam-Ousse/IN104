#include "mathextra.h"

double min(double a, double b){
	if(a<b){
		return a;
	}
	return b;
}

double max(double a, double b){
	if(a>b){
		return a;
	}
	return b;
}

double sigmoid(double x){
    return 1.0/(1+exp(-x));
}
double sigmoid_prime(double x){
    return sigmoid(x)*(1-sigmoid(x));
}
double relu(double x){
    if(x>0){
        return x;
    }
    return 0;
}
double relu_prime(double x){
    if(x>=0){
        return 1;
    }
    return 0;
}
double leaky_relu(double x){
    if(x>0){
        return x;
    }
    return 0.01*x;
}
double leaky_relu_prime(double x){
    if(x>0){
        return 1;
    }
    return 0.01;
}
double tanh_prime(double x){
    return 1-pow(tanh(x),2);
}
double tanh(double x){
    return (exp(x)-exp(-x))/(exp(x)+exp(-x));
}
double softmax(double* x, int n){
    double sum = 0;
    for(int i=0; i<n; i++){
        sum+=exp(x[i]);
    }
    for(int i=0; i<n; i++){
        x[i] = exp(x[i])/sum;
    }
    return 0;
}

double identity(double x){
    return x;
}
double square(double x){
    if (x <1 && x>-1){
        return 1;
    }
    return 0;
}