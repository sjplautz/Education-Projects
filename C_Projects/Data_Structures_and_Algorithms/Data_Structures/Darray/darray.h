//
// Created by plaut on 3/31/2017.
//

#include <stdio.h>
#include <stdlib.h>

#ifndef __DARRAY_INCLUDED__
#define __DARRAY_INCLUDED__

typedef struct DArray DArray; //forward declaration of the DArray struct

DArray *newDArray(void (*display)(FILE *,void *));
void insertDArray(DArray *a,void *v);
void *removeDArray(DArray *a);
void *getDArray(DArray *a,int index);
void setDArray(DArray *a,int index,void *value);
int sizeDArray(DArray *a);
void displayDArray(FILE *,DArray *a);

#endif
