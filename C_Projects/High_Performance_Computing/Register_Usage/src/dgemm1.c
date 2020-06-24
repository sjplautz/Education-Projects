#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//multiplies to matrices a and b and prints the time spent
double* dgemm1(double* a, double* b, int n){
    double* c = malloc(n * n * sizeof(double));
    long double sec = 0;
    clock_t before = clock();

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            register double r = c[i*n+j];
            for(int k = 0; k < n; k++){
                r += a[i*n+k] * b[k*n+j];
            }
            c[i*n+j] = r;
        }
    }

    double difference = clock() - before;
    sec = difference / CLOCKS_PER_SEC;
    printf("dgemm1: %Lfs elapsed \n", sec);
    return c;
}