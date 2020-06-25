//
// Created by plaut on 3/14/2017.
//

#include <stdio.h>
#include "bst.h"

#ifndef ASSIGNMENT2_RBT_H
#define ASSIGNMENT2_RBT_H

typedef struct rbt
{
    bst *tree;
    void (*display)(FILE *,void *);
    int (*compare)(void *,void *);
    int size;
    int words;
} rbt;

extern rbt *newRBT(void (*)(FILE *,void *),int (*)(void *,void *));
extern void insertRBT(rbt *,void *);
extern int findRBT(rbt *,void *);
extern void deleteRBT(rbt *,void *); //optional
extern int sizeRBT(rbt *);
extern int wordsRBT(rbt *);
extern void statisticsRBT(rbt *,FILE *);
extern void displayRBT(FILE *,rbt *);
extern void checkRBT(rbt *);    //optional

#endif //ASSIGNMENT2_RBT_H
