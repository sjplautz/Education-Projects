//
// Created by Stephen Plautz on 1/21/2017.
//

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "dll.h"

//creates a new stack with NULL head and tail pointers
stack *newStack(void (*d)(FILE *,void *))
{
    stack *items=malloc(sizeof(stack));
    items->dlist=newDLL(d);
    return items;
}

//adds an item to the top of the stack
void push(stack *items,void *value)
{
    insertDLL(items->dlist,0,value);
}

//removes the element at the top of the stack
void *pop(stack *items)
{
    return removeDLL(items->dlist,0);
}

//diplays the element at the top of the stack
void *peekStack(stack *items)
{
    return getDLL(items->dlist,0);
}

//returns the amount of elements in the stack
int sizeStack(stack *items)
{
    return sizeDLL(items->dlist);
}

//displays the elements of the stack
void displayStack(FILE *fp,stack *items)
{
    displayDLL(fp,items->dlist);
}
