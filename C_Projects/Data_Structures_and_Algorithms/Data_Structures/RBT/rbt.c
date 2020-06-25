//
// Created by plaut on 3/14/2017.
//

#include "rbt.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct rbtValue
{
    void* value;
    int frequency;
    int color;
    void (*display)(FILE*, void*);
    int  (*compare)(void*, void*);
} rbtValue;

static void displayRBTValue(FILE*, void*);
static int compareRBTValue(void*, void*);
static rbtValue* newRBTValue(void*, void (*)(FILE *,void *), int (*)(void *,void *));
static void rotate(bst*, bstNode*, bstNode*);
static void insertionFixUp(bst*, bstNode*);
static int color(bstNode*);
static bstNode* uncle(bstNode*);
static int linear(bstNode*);

extern rbt *newRBT(void (*display)(FILE *f, void *a), int(*compare)(void *b, void *c))
{
    rbt* temp  = malloc(sizeof(rbt));
    temp->tree  = newBST(displayRBTValue, compareRBTValue);
    temp->display = display;
    temp->compare = compare;
    temp->size = 0;  //total of all nodes
    temp->words = 0; //total of all frequencies
    return temp;
}

//constructor for rbt values private to module
static rbtValue* newRBTValue(void* p, void (*d)(FILE *f,void *a), int (*c)(void *b,void *d))
{
    rbtValue* newVal = malloc(sizeof(rbtValue));
    newVal->value   = p;
    newVal->display = d;
    newVal->compare = c;
    newVal->frequency = 1;
    newVal->color = 1; // 1 for red node 0 for black node

    return newVal;
}

//inserts a node into a RBT and then calls helper functions to ensure good insertion
extern void insertRBT(rbt *b, void *key)
{
    rbtValue* r = newRBTValue(key, b->display, b->compare);
    bstNode* temp = findBSTNode(b->tree, r);

    if(temp == NULL)
    {
        insertBST(b->tree, r);
        temp = findBSTNode(b->tree, r);
        insertionFixUp(b->tree, temp);
        b->words++;
    }
    else
    {
        ((rbtValue*)temp->value)->frequency++;
        b->words++;
        free(r);
    }
}

static void insertionFixUp(bst* b, bstNode* node)
{
    while(1)
    {
        if(b->root == node)
            break;

        if(color(node->parent) == 0)
            break;

        if(uncle(node) != NULL && color(uncle(node)) == 1)
        {
            ((rbtValue*)(node->parent->value))->color = 0;
            ((rbtValue*)(uncle(node)->value))->color = 0;
            ((rbtValue*)(node->parent->parent->value))->color = 1;
            node = node->parent->parent;
        }
        else
        {
            if (linear(node) == 0)
            {
                bstNode* oldNode   = node;
                bstNode* oldParent = node->parent;

                rotate(b, node, node->parent);
                node = oldParent;
                node->parent = oldNode;
            }

            ((rbtValue*)node->parent->value)->color = 0;
            ((rbtValue*)(node->parent->parent->value))->color = 1;
            rotate(b, node->parent, node->parent->parent);
            break;
        }
    }
    ((rbtValue*)(b->root->parent->value))->color = 0;
    ((rbtValue*)(b->root->value))->color = 0;
}

static void rotate(bst* t, bstNode* a, bstNode* b)
{
    //right rotate
    if(b->left == a)
    {
        b->left = a->right;
        if(b->left)
            b->left->parent = b;
        if(b->parent != b)
            a->parent = b->parent;
        else
        {
            a->parent = a;
            t->root = a;
        }
        if(a->parent->left == b)
            a->parent->left = a;
        else
            a->parent->right = a;
        a->right = b;
        b->parent = a;
    }
    //left rotate
    else
    {
        b->right = a->left;
        if(b->right)
            b->right->parent = b;
        if(b->parent != b)
            a->parent = b->parent;
        else
        {
            a->parent = a;
            t->root = a;
        }
        if(a->parent->right == b)
            a->parent->right = a;
        else
            a->parent->left = a;
        a->left = b;
        b->parent = a;
    }
}

//a helper function that returns the color of a passed in node
static int color(bstNode* node)
{
    if (node == NULL)
        return 0;
    else
        return ((rbtValue*)(node->value))->color;
}

//a helper function that returns the uncle of a passed in node
static bstNode* uncle(bstNode* node)
{
    if(node->parent == node->parent->parent->left)
        return node->parent->parent->right;
    else
        return node->parent->parent->left;
}

//a helper function that determines whether an inserted node is linear with respect to its parent and grandparent
static int linear(bstNode* node)
{
    if(node->parent == node->parent->parent->left && node == node->parent->left)
        return 1;
    else if(node->parent == node->parent->parent->right && node == node->parent->right)
        return 1;
    else
        return 0;
}

extern int findRBT(rbt *b, void *key)
{
    rbtValue* r = newRBTValue(key, b->display, b->compare);
    bstNode* tempN = findBSTNode(b->tree, r);
    int i = 0;

    if(tempN == NULL)
    {
        free(r);
        return 0;
    }

    else
    {
        //fprintf(f, "%d", ((rbtValue*)g)->frequency);
        i = ((rbtValue*)tempN->value)->frequency;
        free(r);
        return i;
    }
}

extern int sizeRBT(rbt *b)
{
    return b->size;
}

extern int wordsRBT(rbt *b)
{
    return b->words;
}

extern void statisticsRBT(rbt *b,FILE *fp)
{
    fprintf(fp, "Words/Phrases: %d\n", b->words);
    statisticsBST(b->tree,fp);
}

extern void displayRBT(FILE *fp,rbt *b)
{
    displayBST(fp, b->tree);
}

//private function to display the value help by a vbstValue object (string or int)
static void displayRBTValue(FILE* f, void* g)
{
    ((rbtValue*)g)->display(f,((rbtValue*)g)->value);
    if(((rbtValue*)g)->frequency > 1)
    {
        fprintf(f, "-");
        fprintf(f, "%d", ((rbtValue*)g)->frequency);
    }
    fprintf(f, "-");
    if(((rbtValue*)g)->color == 0)
        fprintf(f, "B");
    else
        fprintf(f, "R");
}

//private helper function separates the desired values from rbtValue objects
static int compareRBTValue(void* a, void* b)
{
    void* l = ((rbtValue*)a)->value;
    void* r = ((rbtValue*)b)->value;

    return ((rbtValue*)a)->compare(l, r);
}

void deleteRBT(rbt* t, void* v)
{
    (void) t;
    (void) v;
}
