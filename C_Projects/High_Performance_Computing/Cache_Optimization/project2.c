#define _POSIX_C_SOURCE 199309L
#define BILLION 1000000000L

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <time.h>

void generate(double *, int);
void clear(double *, int);
double randFunction();
void p(double *, int);
void combine(double *, double *, int);
double compare(double *, double *, int);

double *ijk(double *, double *, int);
double *jik(double *, double *, int);
double *kij(double *, double *, int);
double *ikj(double *, double *, int);
double *jki(double *, double *, int);
double *kji(double *, double *, int);
double *blocked_ijk(double *, double *, int, int);
double *blocked_jik(double *, double *, int, int);
double *blocked_kij(double *, double *, int, int);
double *blocked_ikj(double *, double *, int, int);
double *blocked_jki(double *, double *, int, int);
double *blocked_kji(double *, double *, int, int);

int main(int argc, char **argv)
{

    int input_size = atoi(argv[1]);
    double *a = malloc(input_size * input_size * sizeof(double));
    double *b = malloc(input_size * input_size * sizeof(double));

    struct timespec start, end;

    FILE *output = fopen("results.txt", "a");

    //generating the input matrices to be multiplied
    generate(a, input_size);
    generate(b, input_size);

    //splitting up initial inefficient non-blocked call, so that jobs can be run without it
    if (argc == 2)
    {

        double t0, t1, t2, t3, t4, t5;

        double *c1;
        double *c2;
        double *c3;
        double *c4;
        double *c5;
        double *c6;

        //running all 12 versions of the matrix multiply
        clock_gettime(CLOCK_MONOTONIC, &start);
        c1 = ijk(a, b, input_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        t0 = (end.tv_sec - start.tv_sec) * BILLION + end.tv_nsec - start.tv_nsec;

        clock_gettime(CLOCK_MONOTONIC, &start);
        c2 = jik(a, b, input_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        t1 = (end.tv_sec - start.tv_sec) * BILLION + end.tv_nsec - start.tv_nsec;

        clock_gettime(CLOCK_MONOTONIC, &start);
        c3 = kij(a, b, input_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        t2 = (end.tv_sec - start.tv_sec) * BILLION + end.tv_nsec - start.tv_nsec;

        clock_gettime(CLOCK_MONOTONIC, &start);
        c4 = ikj(a, b, input_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        t3 = (end.tv_sec - start.tv_sec) * BILLION + end.tv_nsec - start.tv_nsec;

        clock_gettime(CLOCK_MONOTONIC, &start);
        c5 = jki(a, b, input_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        t4 = (end.tv_sec - start.tv_sec) * BILLION + end.tv_nsec - start.tv_nsec;

        clock_gettime(CLOCK_MONOTONIC, &start);
        c6 = kji(a, b, input_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        t5 = (end.tv_sec - start.tv_sec) * BILLION + end.tv_nsec - start.tv_nsec;

        //validating result matrices
        double max1 = compare(c1, c2, input_size);
        double max2 = compare(c1, c3, input_size);
        double max3 = compare(c1, c4, input_size);
        double max4 = compare(c1, c5, input_size);
        double max5 = compare(c1, c6, input_size);

        fprintf(output, "NEW BATCH OF TESTS\n");

        //runtime results
        fprintf(output, "\nmatrix size: %d\n", input_size);
        fprintf(output, "ijk runtime: %llu nanoseconds, %f seconds\n", (long long unsigned int)t0, (double)t0 / BILLION);
        fprintf(output, "jik runtime: %llu nanoseconds, %f seconds\n", (long long unsigned int)t1, (double)t1 / BILLION);
        fprintf(output, "kij runtime: %llu nanoseconds, %f seconds\n", (long long unsigned int)t2, (double)t2 / BILLION);
        fprintf(output, "ikj runtime: %llu nanoseconds, %f seconds\n", (long long unsigned int)t3, (double)t3 / BILLION);
        fprintf(output, "jki runtime: %llu nanoseconds, %f seconds\n", (long long unsigned int)t4, (double)t4 / BILLION);
        fprintf(output, "kji runtime: %llu nanoseconds, %f seconds\n\n", (long long unsigned int)t5, (double)t5 / BILLION);

        //verification results
        fprintf(output, "maximum element difference of resultant matrix from ijk and jik: %f\n", max1);
        fprintf(output, "maximum element difference of resultant matrix from ijk and kij: %f\n", max2);
        fprintf(output, "maximum element difference of resultant matrix from ijk and ikj: %f\n", max3);
        fprintf(output, "maximum element difference of resultant matrix from ijk and jki: %f\n", max4);
        fprintf(output, "maximum element difference of resultant matrix from ijk and kji: %f\n\n", max5);
    }
    else
    {
        int block_size = atoi(argv[2]);

        double t6, t7, t8, t9, t10, t11;

        double *c7;
        double *c8;
        double *c9;
        double *c10;
        double *c11;
        double *c12;

        clock_gettime(CLOCK_MONOTONIC, &start);
        c7 = blocked_ijk(a, b, input_size, block_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        t6 = (end.tv_sec - start.tv_sec) * BILLION + end.tv_nsec - start.tv_nsec;

        clock_gettime(CLOCK_MONOTONIC, &start);
        c8 = blocked_jik(a, b, input_size, block_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        t7 = (end.tv_sec - start.tv_sec) * BILLION + end.tv_nsec - start.tv_nsec;

        clock_gettime(CLOCK_MONOTONIC, &start);
        c9 = blocked_kij(a, b, input_size, block_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        t8 = (end.tv_sec - start.tv_sec) * BILLION + end.tv_nsec - start.tv_nsec;

        clock_gettime(CLOCK_MONOTONIC, &start);
        c10 = blocked_ikj(a, b, input_size, block_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        t9 = (end.tv_sec - start.tv_sec) * BILLION + end.tv_nsec - start.tv_nsec;

        clock_gettime(CLOCK_MONOTONIC, &start);
        c11 = blocked_jki(a, b, input_size, block_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        t10 = (end.tv_sec - start.tv_sec) * BILLION + end.tv_nsec - start.tv_nsec;

        clock_gettime(CLOCK_MONOTONIC, &start);
        c12 = blocked_kji(a, b, input_size, block_size);
        clock_gettime(CLOCK_MONOTONIC, &end);
        t11 = (end.tv_sec - start.tv_sec) * BILLION + end.tv_nsec - start.tv_nsec;

        double max7 = compare(c7, c8, input_size);
        double max8 = compare(c7, c9, input_size);
        double max9 = compare(c7, c10, input_size);
        double max10 = compare(c7, c11, input_size);
        double max11 = compare(c7, c12, input_size);

        fprintf(output, "NEW BATCH OF TESTS\n");

        //runtime results
        fprintf(output, "\nmatrix size: %d\n", input_size);
        fprintf(output, "block size: %d\n", block_size);
        fprintf(output, "blocked_ijk runtime: %llu nanoseconds, %f seconds\n", (long long unsigned int)t6, (double)t6 / BILLION);
        fprintf(output, "blocked_jik runtime: %llu nanoseconds, %f seconds\n", (long long unsigned int)t7, (double)t7 / BILLION);
        fprintf(output, "blocked_kij runtime: %llu nanoseconds, %f seconds\n", (long long unsigned int)t8, (double)t8 / BILLION);
        fprintf(output, "blocked_ikj runtime: %llu nanoseconds, %f seconds\n", (long long unsigned int)t9, (double)t9 / BILLION);
        fprintf(output, "blocked_jki runtime: %llu nanoseconds, %f seconds\n", (long long unsigned int)t10, (double)t10 / BILLION);
        fprintf(output, "blocked_kji runtime: %llu nanoseconds, %f seconds\n\n", (long long unsigned int)t11, (double)t11 / BILLION);

        fprintf(output, "maximum element difference of resultant matrix from blocked_ijk and blocked_jik: %f\n", max7);
        fprintf(output, "maximum element difference of resultant matrix from blocked_ijk and blocked_kij: %f\n", max8);
        fprintf(output, "maximum element difference of resultant matrix from blocked_ijk and blocked_ikj: %f\n", max9);
        fprintf(output, "maximum element difference of resultant matrix from blocked_ijk and blocked_jki: %f\n", max10);
        fprintf(output, "maximum element difference of resultant matrix from blocked_ijk and blocked_kji: %f\n\n", max11);
    }

    return 0;
}

//fills the array with randomly generated doubles
void generate(double *array, int n)
{
    for (int i = 0; i < n * n; i++)
    {
        array[i] = randFunction();
    }
}

//clears an array's contents
void clear(double *array, int n)
{
    for (int i = 0; i < n * n; i++)
    {
        array[i] = 0;
    }
}

//randomly generates doubles
double randFunction()
{
    double result = 0;
    double min = 0;
    double max = 10;
    double range = (max - min);
    double div = RAND_MAX / range;
    result = min + (rand() / div);
    return result;
}

//prints a matrix in array form
void p(double *a, int n)
{
    int check = 0;
    for (int i = 0; i < n * n; i++)
    {
        check++;
        //if the index is currently the end of a matrix row
        if (check == n)
        {
            check = 0;
            printf("%f \n", a[i]);
        }
        else
            printf("%f ", a[i]);
    }
    printf("\n");
}

//compares the resulting matrices to find max difference
double compare(double *c1, double *c2, int n)
{
    double diff = fabs(c1[0] - c2[0]);
    int i;
    for (i = 0; i < n * n; i++)
        if (fabs(c1[i] - c2[i]) > diff)
            diff = fabs(c1[i] - c2[i]);
    return diff;
}

double *ijk(double *a, double *b, int n)
{
    double *c = malloc(n * n * sizeof(double));
    clear(c, n);

    /* ijk */
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            register double sum = 0;
            for (int k = 0; k < n; k++)
            {
                sum += a[i * n + k] * b[k * n + j];
            }
            c[i * n + j] = sum;
        }
    }

    return c;
}

double *jik(double *a, double *b, int n)
{
    double *c = malloc(n * n * sizeof(double));
    clear(c, n);

    /* jik */
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < n; i++)
        {
            register double sum = c[i * n + j];
            for (int k = 0; k < n; k++)
            {
                sum += a[i * n + k] * b[k * n + j];
            }
            c[i * n + j] = sum;
        }
    }

    return c;
}

double *kij(double *a, double *b, int n)
{
    double *c = malloc(n * n * sizeof(double));
    clear(c, n);
    double r = 0;

    /* kij */
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            r = a[i * n + k];
            for (int j = 0; j < n; j++)
                c[i * n + j] += r * b[k * n + j];
        }
    }

    return c;
}

double *ikj(double *a, double *b, int n)
{
    double *c = malloc(n * n * sizeof(double));
    clear(c, n);
    double r = 0;

    /* ikj */
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < n; k++)
        {
            r = a[i * n + k];
            for (int j = 0; j < n; j++)
                c[i * n + j] += r * b[k * n + j];
        }
    }

    return c;
}

double *jki(double *a, double *b, int n)
{
    double *c = malloc(n * n * sizeof(double));
    clear(c, n);
    double r = 0;

    /* jki */
    for (int j = 0; j < n; j++)
    {
        for (int k = 0; k < n; k++)
        {
            r = b[k * n + j];
            for (int i = 0; i < n; i++)
                c[i * n + j] += a[i * n + k] * r;
        }
    }

    return c;
}

double *kji(double *a, double *b, int n)
{
    double *c = malloc(n * n * sizeof(double));
    clear(c, n);
    double r = 0;

    /* kji */
    for (int k = 0; k < n; k++)
    {
        for (int j = 0; j < n; j++)
        {
            r = b[k * n + j];
            for (int i = 0; i < n; i++)
                c[i * n + j] += a[i * n + k] * r;
        }
    }

    return c;
}

double *blocked_ijk(double *a, double *b, int n, int B)
{
    double *c = malloc(n * n * sizeof(double));
    clear(c, n);

    for (int i1 = 0; i1 < n; i1 += B)
    {
        for (int j1 = 0; j1 < n; j1 += B)
        {
            for (int k1 = 0; k1 < n; k1 += B)
            {
                /* ijk */
                for (int i = i1; i < i1 + B; i++)
                {
                    for (int j = j1; j < j1 + B; j++)
                    {
                        register double sum = c[i * n + j];
                        for (int k = k1; k < k1 + B; k++)
                        {
                            sum += a[i * n + k] * b[k * n + j];
                        }
                        c[i * n + j] = sum;
                    }
                }
            }
        }
    }

    return c;
}

double *blocked_jik(double *a, double *b, int n, int B)
{
    double *c = malloc(n * n * sizeof(double));
    clear(c, n);

    for (int j1 = 0; j1 < n; j1 += B)
    {
        for (int i1 = 0; i1 < n; i1 += B)
        {
            for (int k1 = 0; k1 < n; k1 += B)
            {
                /* jik */
                for (int j = j1; j < j1 + B; j++)
                {
                    for (int i = i1; i < i1 + B; i++)
                    {
                        register double sum = c[i * n + j];
                        for (int k = k1; k < k1 + B; k++)
                        {
                            sum += a[i * n + k] * b[k * n + j];
                        }
                        c[i * n + j] = sum;
                    }
                }
            }
        }
    }

    return c;
}

double *blocked_kij(double *a, double *b, int n, int B)
{
    double *c = malloc(n * n * sizeof(double));
    clear(c, n);
    double r = 0;

    for (int k1 = 0; k1 < n; k1 += B)
    {
        for (int i1 = 0; i1 < n; i1 += B)
        {
            for (int j1 = 0; j1 < n; j1 += B)
            {
                /* kij */
                for (int k = k1; k < k1 + B; k++)
                {
                    for (int i = i1; i < i1 + B; i++)
                    {
                        r = a[i * n + k];
                        for (int j = j1; j < j1 + B; j++)
                            c[i * n + j] += r * b[k * n + j];
                    }
                }
            }
        }
    }

    return c;
}

double *blocked_ikj(double *a, double *b, int n, int B)
{
    double *c = malloc(n * n * sizeof(double));
    clear(c, n);
    double r = 0;

    for (int i1 = 0; i1 < n; i1 += B)
    {
        for (int k1 = 0; k1 < n; k1 += B)
        {
            for (int j1 = 0; j1 < n; j1 += B)
            {
                /* ikj */
                for (int i = i1; i < i1 + B; i++)
                {
                    for (int k = k1; k < k1 + B; k++)
                    {
                        r = a[i * n + k];
                        for (int j = j1; j < j1 + B; j++)
                            c[i * n + j] += r * b[k * n + j];
                    }
                }
            }
        }
    }

    return c;
}

double *blocked_jki(double *a, double *b, int n, int B)
{
    double *c = malloc(n * n * sizeof(double));
    clear(c, n);
    double r = 0;

    for (int j1 = 0; j1 < n; j1 += B)
    {
        for (int k1 = 0; k1 < n; k1 += B)
        {
            for (int i1 = 0; i1 < n; i1 += B)
            {
                /* jki */
                for (int j = j1; j < j1 + B; j++)
                {
                    for (int k = k1; k < k1 + B; k++)
                    {
                        r = b[k * n + j];
                        for (int i = i1; i < i1 + B; i++)
                            c[i * n + j] += a[i * n + k] * r;
                    }
                }
            }
        }
    }

    return c;
}

double *blocked_kji(double *a, double *b, int n, int B)
{
    double *c = malloc(n * n * sizeof(double));
    clear(c, n);
    double r = 0;

    for (int k1 = 0; k1 < n; k1 += B)
    {
        for (int j1 = 0; j1 < n; j1 += B)
        {
            for (int i1 = 0; i1 < n; i1 += B)
            {
                /* kji */
                for (int k = k1; k < k1 + B; k++)
                {
                    for (int j = j1; j < j1 + B; j++)
                    {
                        r = b[k * n + j];
                        for (int i = i1; i < i1 + B; i++)
                            c[i * n + j] += a[i * n + k] * r;
                    }
                }
            }
        }
    }

    return c;
}