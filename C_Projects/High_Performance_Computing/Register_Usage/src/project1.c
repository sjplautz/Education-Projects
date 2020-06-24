#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<math.h>
#include"../lib/dgemm0.h"
#include"../lib/dgemm1.h"
#include"../lib/dgemm2.h"
#include"../lib/dgemm3.h"

void generate(double*, int);
double randFunction();
void p(double*, int);
void combine(double*, double*, int);
double compare(double*, double*, int);

int main(__attribute__((unused)) int argc, char** argv){
    
    int input_size = atoi(argv[1]);
    double* a = malloc(input_size * input_size * sizeof(double));
    double* b = malloc(input_size * input_size * sizeof(double));
    double* d0; double* d1; double* d2; double* d3;

    generate(a, input_size);
    generate(b, input_size);
    
    d0 = dgemm0(a, b, input_size);
    d1 = dgemm1(a, b, input_size);
    d2 = dgemm2(a, b, input_size);
    d3 = dgemm3(a, b, input_size);

    double max1 = compare(d0, d1, input_size);
    double max2 = compare(d0, d2, input_size);
    double max3 = compare(d0, d3, input_size);

    printf("maximum element difference of resultant matrix from dgemm0 and dgemm1: %f\n", max1);
    printf("maximum element difference of resultant matrix from dgemm0 and dgemm2: %f\n", max2);
    printf("maximum element difference of resultant matrix from dgemm0 and dgemm3: %f\n\n", max3);

    return 0;
}

//fills the array with randomly generated doubles
void generate(double* array, int n){
    for(int i = 0; i < n * n; i++){
        array[i] = randFunction();
    }
}

//randomly generates doubles
double randFunction(){
    double result = 0;
    double min = 0;
    double max = 10;
    double range = (max - min);
    double div = RAND_MAX / range;
    result = min + (rand() / div);
    return result;
}

//prints a matrix in array form
void p(double* a, int n){
    int check = 0;
    for(int i = 0; i < n*n; i++){
        check++;
        //if the index is currently the end of a matrix row
        if(check == n){
            check = 0;
            printf("%f \n", a[i]);
        }
        else
            printf("%f ", a[i]);
    }
    printf("\n");
}

double compare(double* a, double* b, int n){
    double max = 0;
    double current = 0;

    for(int i = 0; i < n * n; i++){
        current = fabs(a[i] - b[i]);
        if(current > 0)
            max = current;
    }
    return max;
}
