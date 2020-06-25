//
// Created by plaut on 3/31/2017.
//

#include "darray.h"
#include <stdlib.h>

struct DArray
{
    void **array;                   //an array of void pointers
    int capacity;                   //total number of slots
    int size;                       //number of filled slots
    void (*display)(FILE *,void *); //display function
};

DArray *newDArray(void (*display)(FILE *f, void *d))
{
    DArray *d = malloc(sizeof(DArray));
    d->array = malloc(sizeof(void*));
    d->display = display;
    d->capacity = 1;
    d->size = 0;
    return d;
}

void insertDArray(DArray *a,void *v)
{
    a->size++;
    if(a->size > a->capacity)
    {
        a->capacity *= 2;
        a->array = realloc(a->array, sizeof(void*) * a->capacity);
    }
    a->array[a->size-1] = v;
}

void *removeDArray(DArray *a)
{
    if(a->size == 0)
    {
        fprintf(stderr, "ERROR: removing an element from an empty array\n");
        exit(-1);
    }

    void* val = a->array[a->size-1];
    a->size--;
    a->array[a->size] = NULL;

    if(a->capacity > 1 && (a->size < a->capacity/4))
    {
        a->capacity /= 2;
        a->array = realloc(a->array, sizeof(void*) * a->capacity);
    }

    return val;
}

void *getDArray(DArray *a, int index)
{
    void* v;
    if(index > a->capacity)
    {
        fprintf(stdout, "ERROR: accessing an element not in the array\n");
        exit(-1);
    }
    else
    {
        v = a->array[index];
        return v;
    }
}

void setDArray(DArray *a, int index, void *value)
{
    if(index > (a->capacity))
    {
        fprintf(stdout, "ERROR: setting an index not in the array\n");
        exit(-1);
    }
    else if(index == (a->size))
        insertDArray(a, value);
    else
        a->array[index] = value;
}

int sizeDArray(DArray *a)
{
    return a->size;
}

void displayDArray(FILE *f,DArray *a)
{
    fprintf(f, "[");
    for(int i = 0; i < a->size; i++)
    {
        if(i < (a->size-1))
            fprintf(f, "%d,", *((int*)a->array[i]));
        else
            fprintf(f, "%d", *((int*)a->array[i]));
    }
    fprintf(f, "]");
    fprintf(f, "[");
    fprintf(f, "%d", (a->capacity - a->size));
    fprintf(f, "]");
}