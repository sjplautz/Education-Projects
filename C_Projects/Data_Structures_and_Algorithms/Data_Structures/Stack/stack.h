//
// Created by Stephen Plautz on 1/19/2017.
//

#include "dll.h"
#ifndef __STACK_INCLUDED__
#define __STACK_INCLUDED__

typedef struct stack
{
    dll *dlist;
} stack;

stack *newStack(void (*d)(FILE *,void *));   //constructor
void push(stack *items,void *value);         //stores a generic value
void *pop(stack *items);                     //returns a generic value
void *peekStack(stack *items);               //returns a generic value
int sizeStack(stack *items);
void displayStack(FILE *,stack *items);

#endif