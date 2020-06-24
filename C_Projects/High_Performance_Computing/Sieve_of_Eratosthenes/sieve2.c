#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char *argv[])
{
    long count = 0;                /* Local prime count */
    double elapsed_time = 0;       /* Parallel execution time */
    long first_multiple_index = 0; /* Index of first multiple */
    long global_count = 0;         /* Global prime count */
    long high_value = 0;           /* Highest value on this proc */
    long i = 0;                    /* Counter value */
    int id = 0;                    /* Process ID number */
    long low_value = 0;            /* Lowest value on this proc */
    char *marked;                  /* Portion of 2,...,'n' */
    char *primes;
    long n = 0;                    /* Sieving from 2, ..., 'n' */
    int p = 0;                     /* Number of processes */
    long proc0_size = 0;           /* Size of proc 0's subarray */
    long prime = 0;                /* Current prime */
    long size = 0;                 /* Elements in 'marked' */
    long index = 0;         /*holds index of last prime so no redundant searching is done */
    long primes_size = 0;

    MPI_Init(&argc, &argv);

    /* Start the timer */
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Barrier(MPI_COMM_WORLD);
    elapsed_time = -MPI_Wtime();

    //error handling to avoid case where user does not specify a number range on command line program call
    if (argc != 2)
    {
        if (!id)
            printf("Command line: %s <m>\n", argv[0]);
        MPI_Finalize();
        exit(1);
    }

    n = atol(argv[1]);

    low_value = 2 + id * (n - 1) / p;
    high_value = 1 + (id + 1) * (n - 1) / p;
    size = high_value - low_value + 1;

    //if subarray has even size cut it in half and determine new low value
    if (size % 2 == 0)
    {
        size = (size / 2);
        if (low_value % 2 == 0)
            low_value++; //skips using the first even element
    }
    //if subarray has odd size determine how it must be cut based upon low value
    else
    {
        if (low_value % 2 == 0)
        {
            size = (size / 2);
            low_value++;
        }
        else
            size = (size / 2) + 1;
    }

    /* Bail out if all the primes used for sieving are
      not all held by process 0 */
    proc0_size = (n - 1) / p;
    if ((2 + proc0_size) < (long)sqrt((long double)n))
    {
        if (!id)
            printf("Too many processes\n");
        MPI_Finalize();
        exit(1);
    }

    //allocating this processes share of the array except for even indices
    marked = (char *)malloc(size);

    if (marked == NULL)
    {
        printf("Cannot allocate enough memory\n");
        MPI_Finalize();
        exit(1);
    }

    //initializing the array to hold marked status of integer values in range of process
    for (i = 0; i < size; i++)
    {
        marked[i] = 0;
    }

    //primes will represent 0 - sqrt(n)
    primes = (char *)malloc(sqrt(n));
    
    //might have to be changed
    primes_size = sqrt(n);
    
    //printf("primes size: %lu\n", primes_size);
    for (i = 0; i <= primes_size; i++)
    {
        if(i%2 == 0){
            //printf("marking off: %lu\n", i);
            primes[i] = 1;
        }
        else
            primes[i] = 0;
    }

    //prime starts as 3
    prime = 3;
    index = 3;

    //main loop for seiving primes used by all processes
    do
    {
        //printf("prime: %lu  low value: %lu: ", prime, low_value);
        if (!(low_value % prime))
        {
            if (!id)
                first_multiple_index = prime;
            else
                first_multiple_index = 0;
        }

        else
        {
            if (!id)
            {
                first_multiple_index = ((prime - (low_value % prime)) / 2) + prime;
            }
            else
            {
                //first find first multiple value
                long first_multiple_value = (low_value + (prime - (low_value % prime)));

                //if it wasnt even all is good
                if (first_multiple_value % 2 != 0)
                    first_multiple_index = ((prime - (low_value % prime)) / 2);
                //else the next multiple index needs to be found as the first one does not exist
                else
                {
                    first_multiple_index = ((prime - (low_value % prime)) / 2) + ((prime / 2) + 1);
                }
            }
            //printf("else ");
        }

        //printf("first multiple index: %lu\n", first_multiple_index);

        //marking all multiples of the current prime within range assigned to this process
        //prime - 1 to account for missing even indices
        for (i = first_multiple_index; i < size; i += prime)
            marked[i] = 1;

        //removing prime multiples from prime list
        for(i = index + prime; i < primes_size; i+= prime){
            primes[i] = 1;
        }

        //skipping all marked indices and evens
        //process 0 gets the next prime to sieve
        if (!id || id > 0)
        {
            while (primes[++index]);
            prime = index;
        }

    } while (prime * prime <= n);

    //counting the total number of primes determined within process range
    count = 0;
    //printf("size: %lu\n", size);
    for (i = 0; i < size; i++)
    {
        if (!marked[i])
        {
            count++;
        }
    }

    //if being done in parallel, reduce the total of all processes prime counts into one global prime count
    if (p > 1)
    {
        MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    /* Stop the timer */
    elapsed_time += MPI_Wtime();

    /* Print the results */
    if (!id)
    {
        printf("There are %lu primes less than or equal to %lu\n",
               ++global_count, n);
        printf("SIEVE (%d) %10.6f\n", p, elapsed_time);
    }

    MPI_Finalize();
    return 0;
}