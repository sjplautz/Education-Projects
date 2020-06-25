//
// Created by Stephen Plautz on 1/19/2017.
//

#ifndef __SLL_INCLUDED__
#define __SLL_INCLUDED__
#include <stdio.h>

typedef struct sllnode
{
    void *value;
    struct sllnode *next;
} sllnode;

typedef struct sll
{
    sllnode *head;
    sllnode *tail;
    int size;
    void (*display)(FILE *,void *);
} sll;

extern sll *newSLL(void (*d)(FILE *,void *));            //constructor
extern sllnode *newSllNode(void*);                       //constructor
extern void insertSLL(sll *items,int index,void *value); //stores a generic value
extern void *removeSLL(sll *items,int index);            //returns a generic value
extern void unionSLL(sll *recipient,sll *donor);         //merge two lists into one
extern void *getSLL(sll *items, int index);              //get the value at the index
extern int sizeSLL(sll *items);
extern void displaySLL(FILE *,sll *items);

#endif
