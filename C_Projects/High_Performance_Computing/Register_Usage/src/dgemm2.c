#include<stdio.h>
#include<stdlib.h>
#include<time.h>

//multiplies to matrices a and b and prints the time spent
double* dgemm2(double* a, double* b, int n){
    double* c = malloc(n * n * sizeof(double));
    long double sec = 0;
    clock_t before = clock();

    for(int i = 0; i < n; i+=2){
        for(int j = 0; j < n; j+=2){
            for(int k = 0; k < n; k+=2){
                c[i * n + j]             =  a[i * n + k] * b[k * n + j] + a[i * n + k + 1] * b[(k + 1) * n + j] + c[i * n + j];
                c[(i + 1) * n + j]       =  a[(i + 1) * n + k] * b[k * n + j] + a[(i + 1) * n + k + 1] * b[(k + 1) * n + j] + c[(i + 1) * n + j];
                c[i * n + (j + 1)]       =  a[i * n + k] * b[k * n + (j + 1)] + a[i * n + k + 1] * b[(k + 1) * n + (j + 1)] + c[i * n + (j + 1)];
                c[(i + 1) * n + (j + 1)] =  a[(i + 1) * n + k] * b[k * n + (j + 1)] + a[(i + 1) * n + k + 1] * b[(k + 1) * n + (j + 1)] + c[(i + 1) * n + (j + 1)];
            }
        }
    }

    double difference = clock() - before;
    sec = difference / CLOCKS_PER_SEC;
    printf("dgemm2: %Lfs elapsed \n", sec);
    return c;
}