//
// Created by plaut on 3/31/2017.
//

#include <stdio.h>
#include "darray.h"

#ifndef __BINOMIAL_INCLUDED__
#define __BINOMIAL_INCLUDED__

typedef struct BinomialNode BinomialNode;

 BinomialNode* newBinomialNode(void (*display)(FILE *,void *),void *value);
 void displayBinomialNode(FILE *fp,BinomialNode *n);

typedef struct Binomial Binomial;
                             //display                 //comparator             //updater
 Binomial *newBinomial(void (*d)(FILE *,void *), int (*c)(void *,void *), void (*u)(void *,BinomialNode *));
 BinomialNode *insertBinomial(Binomial *b,void *value);
 int sizeBinomial(Binomial *b);
 void deleteBinomial(Binomial *b,BinomialNode *n);
 void decreaseKeyBinomial(Binomial *b,BinomialNode *n,void *value);
 void *extractBinomial(Binomial *b);
 void displayBinomial(FILE *fp,Binomial *b);

#endif
