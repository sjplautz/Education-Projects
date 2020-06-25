//
// Created by Stephen Plautz on 1/19/2017.
//

#ifndef __DLL_INCLUDED__
#define __DLL_INCLUDED__
#include <stdio.h>

typedef struct dllnode
{
    void *value;
    struct dllnode *next;
    struct dllnode *prev;
} dllnode;

typedef struct dll
{
    dllnode *head;
    dllnode *tail;
    int size;
    void (*display)(FILE *,void *);
} dll;

extern dll *newDLL(void (*d)(FILE *,void *));            //constructor
extern dllnode *newDllNode(void *);
extern void insertDLL(dll *items,int index,void *value); //stores a generic value
extern void *removeDLL(dll *items,int index);            //returns a generic value
extern void unionDLL(dll *recipient,dll *donor);         //merge two lists into one
extern void *getDLL(dll *items, int index);              //get the value at the index
extern int sizeDLL(dll *items);
extern void displayDLL(FILE *,dll *items);

#endif