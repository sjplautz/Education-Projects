#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//multiplies to matrices a and b and prints the time spent
double *dgemm3(double *a, double *b, int n)
{
    double *c = malloc(n * n * sizeof(double));
    long double sec = 0;
    clock_t before = clock();

    for (int i = 0; i < n; i += 2)
    {
        for (int j = 0; j < n; j += 2)
        {
            int t = i * n + j; int tt = t + n;
            register double c00 = c[t]; register double c01 = c[t + 1]; 
            register double c10 = c[tt]; register double c11 = c[tt + 1];

            for (int k = 0; k < n; k += 2)
            {
                int ta = i * n + k; int tta = ta + n;
                int tb = k * n + j; int ttb = tb + n;
                register double a00 = a[ta]; register double a10 = a[tta]; 
                register double b00 = b[tb]; register double b01 = b[tb + 1];

                c00 += a00*b00 ; c01 += a00*b01 ; c10 += a10*b00 ; c11 += a10*b01 ;
                a00 = a[ta+1]; a10 = a[tta+1]; b00 = b[ttb]; b01 = b[ttb+1];
                c00 += a00*b00 ; c01 += a00*b01 ; c10 += a10*b00 ; c11 += a10*b01 ;
            }
            c[t] = c00;
            c[t+1] = c01;
            c[tt] = c10;
            c[tt+1] = c11;
        }
    }

    double difference = clock() - before;
    sec = difference / CLOCKS_PER_SEC;
    printf("dgemm3: %Lfs elapsed \n", sec);
    return c;
}