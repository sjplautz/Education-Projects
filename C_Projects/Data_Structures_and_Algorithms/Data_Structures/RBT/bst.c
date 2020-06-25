//
// Created by plaut on 3/1/2017.
//

#include "bst.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
//macro to determine the minimum of two values
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


static bstNode* insertHelp(bst*, bstNode*, void*);
static bstNode* predecessor(bstNode*);
static bstNode* successor(bstNode*);
static bstNode* helpFind(bst*, bstNode*, void*);
static bstNode* newBSTNode(bstNode*, void*);
static int minDepth(bstNode*, bst*);
static int minDepthHelp(bstNode*);
static int maxDepth(bstNode*);


//allocates space for a new BST
extern bst *newBST(void (*display)(FILE *f,void *a),int (*comp)(void *b,void *c))
{
    bst* temp  = malloc(sizeof(bst));
    temp->root = NULL;
    temp->size = 0;
    temp->display = display;
    temp->compare = comp;
    return temp;
}

//inserts a node into a bst
extern bstNode *insertBST(bst *b,void *key)
{
    bstNode* temp = NULL;

    if(b->root == NULL)
    {

        temp = newBSTNode(temp, key);
        temp->parent = temp;
        b->root = temp;
        b->root->parent = temp;
        b->size++;
        return temp;
    }

    else
        return insertHelp(b, b->root, key);
}

//private helper function to insert a node into a bst
static bstNode *insertHelp(bst* b, bstNode *node, void* key)
{
    bstNode* temp = NULL;

    if (b->compare(key, node->value) < 0)
    {
        if(node->left != NULL)
            return insertHelp(b, node->left, key);

        else
        {
            b->size++;
            temp = newBSTNode(node, key);
            node->left = temp;
            return temp;
        }
    }

    else if (b->compare(key, node->value) > 0)
    {
        if(node->right != NULL)
            return insertHelp(b, node->right, key);

        else
        {
            b->size++;
            temp = newBSTNode(node, key);
            node->right = temp;
            return temp;
        }
    }
    return temp;
}

//returns integer for frequency if bst exists and 0 if not
extern int findBST(bst *b,void *p)
{
    bstNode* temp = b->root;
    if(temp == NULL)
        return 0;
    else
    {
        if(findBSTNode(b, p))
            return 1;
    }

    return 0;
}

//determines whether a value is in a tree or not
extern bstNode *findBSTNode(bst *b,void *p)
{
    bstNode* temp = b->root;
    if(temp == NULL)
        return temp;

    if(b->compare(temp->value, p) == 0)
        return temp;

    else
    {
        temp = helpFind(b, b->root, p);
        return temp;
    }
}

//helper function to determine if node exists in a tree
static bstNode* helpFind(bst* b, bstNode* node, void* key)
{
    //printf("comparing %s and %s\n",((string*)getVal(node))->value, ((string*)key)->value);
    if (b->compare(key, node->value) < 0)
    {
        if(node->left != NULL)
            return helpFind(b, node->left, key);
        else
            return NULL;
    }

    else if (b->compare(key, node->value) > 0)
    {
        if(node->right != NULL)
            return helpFind(b, node->right, key);
        else
            return NULL;
    }

    else if (b->compare(key, node->value) == 0)
        return node;

    return NULL;
}

//moves a node to a leaf to prepare it for pruning
extern bstNode *swapToLeafBSTNode(bstNode *b)
{
    if(b->left == NULL && b->right == NULL)
        return b;

    bstNode* np = NULL;
    if (b->left != NULL)
        np = predecessor(b->left);
    else
        np = successor(b->right);

    void* temp = NULL;
    temp = b->value;
    b->value = np->value;
    np->value = temp;

    return swapToLeafBSTNode(np);
}

static bstNode* predecessor(bstNode *root)
{
    bstNode* temp = root;
    while(temp->right != NULL)
    {
        temp = temp->right;
    }
    return temp;
}

static bstNode* successor(bstNode* root)
{
    bstNode* temp = root;
    while (temp->left != NULL)
    {
        temp = temp->left;
    }
    return temp;
}

//disconnects a leaf from a tree
extern void pruneBSTNode(bst *a,bstNode *b)
{
    bstNode* temp = NULL;

    temp = b->parent;
    if(temp->right != NULL && a->compare(temp->right->value, b->value) == 0)
        temp->right = NULL;
    else
        temp->left = NULL;

    a->size--;
}

//returns the size (number of nodes) of the tree
extern int sizeBST(bst *b)
{
    return b->size;
}

extern void statisticsBST(bst *b,FILE *f)
{
    int nodes = 0;
    int minD = 0;
    int maxD = 0;

    bstNode* temp = b->root;

    //needs to be modified
    nodes = sizeBST(b);
    minD = minDepth(temp, b);
    maxD = maxDepth(temp);

    fprintf(f, "Nodes: %d\n", nodes);
    fprintf(f, "Minimum depth: %d\n", minD);
    fprintf(f, "Maximum depth: %d\n", maxD);
}

//display function to print out level order traversal of BST
extern void displayBST(FILE *f,bst *b)
{
    int line = 0;
    if(b->size == 0)
    {
        fprintf(f, "0:\n");
        return;
    }

    else
    {
        bstNode* current = NULL;
        queue* q = newQueue(b->display);
        enqueue(q, b->root);
        enqueue(q, NULL);

        while(sizeQueue(q) > 0)
        {
            current = dequeue(q);
            if (current == NULL)
            {
                fprintf(f, "\n");
                if(sizeQueue(q) > 0)
                {
                    fprintf(f,"%d: ",line);
                    line++;
                    enqueue(q, NULL);
                }

            }

            else
            {
                //if it is the root condition
                if(current == b->root)
                {
                    fprintf(f,"%d: ",line);
                    line++;
                    if(current->left == NULL && current->right == NULL)
                        fprintf(f, "=");
                    b->display(f, current->value);
                    fprintf(f,"(");
                    b->display(f, current->parent->value);
                    fprintf(f,")-");
                }

                else
                {
                    if(current->left == NULL && current->right == NULL)
                        fprintf(f, "=");
                    b->display(f, current->value);
                    fprintf(f,"(");
                    b->display(f, current->parent->value);
                    fprintf(f,")-");

                    if(current == current->parent->left)
                        fprintf(f, "l");
                    else if(current == current->parent->right)
                        fprintf(f, "r");
                }

                if(peekQueue(q) != NULL)
                    fprintf(f, " ");
                if (current->left != NULL)
                    enqueue(q, current->left);
                if (current->right != NULL)
                    enqueue(q, current->right);
            }
        }
    }
    return;
}

//constructor for BST nodes private to module
static bstNode* newBSTNode(bstNode *n, void* p)
{
    bstNode* newNode = malloc(sizeof(bstNode));
    newNode->value  = p;
    newNode->parent = n;
    newNode->left   = NULL;
    newNode->right  = NULL;

    return newNode;
}

static int minDepth(bstNode* temp, bst* b)
{
    if(b->root == NULL)
        return 0;
    if(b->root->left == NULL || b->root->right == NULL)
        return 1;

    else
        return minDepthHelp(temp);
}

static int minDepthHelp(bstNode* temp)
{
    if(temp == NULL)
        return 0;
    else
    {
        int ldepth = minDepthHelp(temp->left);
        int rdepth = minDepthHelp(temp->right);

        if(ldepth < rdepth)
            return (ldepth+1);
        else
            return (rdepth+1);
    }
}

//a function for finding the maximum depth of a BST in linear time
static int maxDepth(bstNode* temp)
{
    if(temp == NULL)
        return 0;
    else
    {
        int rDepth = maxDepth(temp->right);
        int lDepth = maxDepth(temp->left);

        if(lDepth > rDepth)
            return (lDepth + 1);
        else
            return (rDepth + 1);
    }
}