//
// Created by plaut on 3/31/2017.
//

#include <stdlib.h>
#include "binomial.h"
#include "sll.h"
#include "queue.h"

struct BinomialNode
{
    void *value;
    DArray *children;
    struct BinomialNode *parent;
    void (*display)(FILE *,void *);
};

struct Binomial
{
    DArray *rootlist;
    int (*compare)(void *,void *);
    void (*update)(void *,BinomialNode *);
    BinomialNode *extreme;
    int size;
    void (*display)(FILE *,void *);
};

static void merge(Binomial*, DArray*);
static BinomialNode *getSubHeap(DArray*, int);
static BinomialNode* bubbleUp(Binomial*, BinomialNode*);
static void consolidate(Binomial*, BinomialNode*);
static BinomialNode* combine(Binomial*, BinomialNode*, BinomialNode*);
static void updateExtreme(Binomial*);
static void printLevelOrder(FILE*, BinomialNode*);

BinomialNode* newBinomialNode(void (*display)(FILE *,void *),void *value)
{
    BinomialNode* n = malloc(sizeof(BinomialNode));
    n->value = value;
    n->children = newDArray(display);
    n->parent = NULL;
    n->display = display;
    return n;
}

Binomial* newBinomial(void (*d)(FILE *,void *), int (*c)(void *,void *), void (*u)(void *,BinomialNode *))
{
    Binomial* b = malloc(sizeof(Binomial));
    b->display = d;
    b->compare = c;
    b->update = u;
    b->rootlist = newDArray(d);
    b->extreme = NULL;
    b->size = 0;
    return b;
}

static void merge(Binomial* b, DArray* donor)
{
    BinomialNode* temp = NULL;
    for(int i = 0; i < sizeDArray(donor); i++)
    {
        temp = getDArray(donor, i);
        temp->parent = temp;
        consolidate(b, temp);
    }
}

BinomialNode* insertBinomial(Binomial *b,void *value)
{
    BinomialNode* n = newBinomialNode(b->display, value);
    n->parent = n;
    n->children = newDArray(b->display);
    consolidate(b, n);
    b->size++;
    return n;
}

void decreaseKeyBinomial(Binomial *b,BinomialNode *n,void *value)
{
    n->value = value;
    bubbleUp(b, n);
    updateExtreme(b);
}

static BinomialNode* bubbleUp(Binomial *b, BinomialNode* n)
{
    void* v = NULL;

    if(n->parent == n)
        return n;

    else if(b->compare(n->value, n->parent->value) >= 0)
        return n;

    else
    {
        if(b->update != NULL) b->update(n->value, n->parent);
        if(b->update != NULL) b->update(n->parent->value, n);
        v = n->value;
        n->value = n->parent->value;
        n->parent->value = v;
        return bubbleUp(b, n->parent);
    }
}

void deleteBinomial(Binomial *b,BinomialNode *n)
{
    decreaseKeyBinomial(b, n, NULL);
    extractBinomial(b);
}

void *extractBinomial(Binomial *b)
{
    BinomialNode* y  = b->extreme;
    for(int i = 0; i < sizeDArray(b->rootlist); i++)
    {
        if(y == getDArray(b->rootlist, i))
        {
            setDArray(b->rootlist, i, NULL);
            break;
        }
    }

    merge(b, y->children);
    b->size--;

    void* store = y->value;
    updateExtreme(b);

    return store;
}

BinomialNode *getSubHeap(DArray* d, int deg)
{
    if(deg == sizeDArray(d))
        return NULL;
    else
        return getDArray(d, deg);
}

static void consolidate(Binomial* b, BinomialNode* n)
{
    int degree = sizeDArray(n->children);
    BinomialNode* temp = getSubHeap(b->rootlist, degree);

    while(temp != NULL)
    {
        n = combine(b,n, temp);
        setDArray(b->rootlist, degree, NULL);
        ++degree;
        temp = getSubHeap(b->rootlist, degree);
    }

    setDArray(b->rootlist, degree, n);

    updateExtreme(b);
}

static BinomialNode* combine(Binomial* b, BinomialNode* x, BinomialNode* y)
{
    int i = 0;
    if(b->compare(x->value, y->value) < 0)
    {
        i = sizeDArray(y->children);
        setDArray(x->children, i, y);
        y->parent = x;
        return x;
    }
    else
    {
        i = sizeDArray(x->children);
        setDArray(y->children, i, x);
        x->parent = y;
        return y;
    }
}

int sizeBinomial(Binomial *b)
{
    return b->size;
}

static void updateExtreme(Binomial* b)
{
    BinomialNode* temp = NULL;
    b->extreme = NULL;
    for(int i = 0; i < sizeDArray(b->rootlist); i++)
    {
        temp = getDArray(b->rootlist, i);

        if(temp == NULL) continue;

        if(b->extreme == NULL)
            b->extreme = temp;

        if(b->compare(temp->value, b->extreme->value) < 0)
            b->extreme = temp;
    }
}

void displayBinomial(FILE *fp,Binomial *b)
{
    BinomialNode* temp;
    for(int i = 0; i < sizeDArray(b->rootlist); i++)
    {
        temp = getDArray(b->rootlist, i);
        if(temp != NULL)
        {
            printLevelOrder(fp, temp);
            fprintf(fp, "----\n");
        }
    }
}

static void printLevelOrder(FILE* fp, BinomialNode* n)
{
    queue* q = newQueue(n->display);
    enqueue(q, n);
    enqueue(q, NULL);

    int level = 0;

    BinomialNode* temp = NULL;
    while(sizeQueue(q) != 0)
    {
        temp = dequeue(q);
        if(temp == NULL)
        {
            fprintf(fp, "\n");
            if(sizeQueue(q) != 0)
            {
                level++;
                fprintf(fp, "%d: ", level);
                enqueue(q, NULL);
            }
        }
        else
        {
            if(temp->parent == temp)
                fprintf(fp, "%d: ", level);
            displayBinomialNode(fp, temp);
            if(peekQueue(q) != NULL)
                fprintf(fp, " ");
            for(int i = 0; i < sizeDArray(temp->children); i++)
                enqueue(q, getDArray(temp->children, i));
        }
    }
}

void displayBinomialNode(FILE* fp, BinomialNode* n)
{
    n->display(fp, n->value);
    fprintf(fp, "-%d", sizeDArray(n->children));
    if(n->parent != n && n->parent != NULL)
    {
        fprintf(fp, "(");
        n->display(fp, n->parent->value);
        fprintf(fp, "-%d)", sizeDArray(n->parent->children));
    }
}
