#include <stdio.h>
#include <stdlib.h>
#include "darray.h"
#include "queue.h"
#include "integer.h"
#include "strings.h"
#include "scanner.h"
#include "comparator.h"
#include "printer.h"
#include "binomial.h"
#include <limits.h>


typedef struct vertex
{
    int visited;
    void* num;
    int key;  //initialized to infinity
    int steps;
    BinomialNode* node; //parent
    DArray* neighbors;
    struct vertex* predecessor;  //necessary? set where?
} vertex;


//is update vertex used anywhere outside binomial?
//vertex compare use
//displaying the visited vertices queue

typedef struct neighbor
{
    vertex* vert;
    int weight;
} neighbor;

vertex *newVertex(void (*display)(FILE *f, void *d), void* x)
{
    vertex *v  = malloc(sizeof(vertex));
    v->visited = 0;
    v->num     = x;
    v->key     = INT_MAX;
    v->steps   = 0;
    v->node    = newBinomialNode(display, x);
    v->neighbors = newDArray(display);
    v->predecessor = NULL;
    return v;
}

//object simulates an edge
neighbor *newNeighbor(vertex* v, int w)
{
    neighbor* n = malloc(sizeof(neighbor));
    n->vert = v; //holds a vertex
    n->weight = w;
    return n;
}

static void updateVertex(void*, BinomialNode*);
int getMax(FILE*);
void makeList(FILE *, vertex**, Printer);
int compareVertices(void*, void*);
static void display(Comparator, Printer, queue*);

int main(int argc, char** argv)
{
    if(argc == 0)
        printf("yo");

    Comparator comp = compareVertices;
    Printer print = displayInteger;

    FILE* input = fopen(argv[1], "r");

    int max = getMax(input);

    //build list of static size to hold maximum value indices
    vertex* A[max+1];
    for(int i = 0; i < max+1; i++)
    {
        A[i] = newVertex(print, NULL);
    }

    //closing and reopening the file to reset feof
    fclose(input);
    input = fopen(argv[1], "r");

    makeList(input, A, print);

    Binomial* b = newBinomial(print, comp, updateVertex);

    //Create a queue to hold visited vertices
    queue* visited = newQueue(print);

    //Add all vertices to a new binomial heap
    for(int i = 0; i < max+1; i++)
    {
        if(A[i]->num != NULL)
        {
            BinomialNode *bnode = insertBinomial(b, A[i]);
            A[i]->node = bnode;
        }
    }

    vertex *u = NULL;
    vertex* v = NULL;
    neighbor * n = NULL;
    while (sizeBinomial(b) != 0)
    {
        //Extract from heap vertex u
        u = extractBinomial(b);

        //if u has no predecessor *may need to update this condition*
        if (u->predecessor == NULL) //Checking for disjoint vertex
        {
            //Set u's key to 0
            u->key = 0;

            //Display the visited vertices queue if there's stuff in it
            if(sizeQueue(visited) != 0)
                display(comp, print, visited); //have to make display function
        }

        //Add vertex u to visited vertices queue
        enqueue(visited, u);

        //Mark u as visited
        u->visited = 1;

        //for each wertex adjacent with u
        for(int i = 0; i < sizeDArray(u->neighbors); i++)
        {
            //Let v be the vertex adjacent with u
            n = getDArray(u->neighbors, i);
            v = n->vert;

            if (v->visited == 0)
            {
                //if v does not have a parent or u->key + e->weight < v->key
                if (v->predecessor == NULL || u->key + n->weight < v->key)
                {
                    //Set v's parent to u
                    v->predecessor = u;

                    //Update v's key to the new lower distance
                    v->key = (u->key + n->weight);

                    //Update the number of steps for v to be its parent's + 1
                    v->steps = u->steps + 1;

                    //Call decreaseKey on v's binomial node pointer "position"
                    decreaseKeyBinomial(b, v->node, v);
                }
            }
        }
    }
    display(comp, print, visited);

    return 0;
}

static void display(Comparator comp, Printer print, queue* q)
{
    int i = 0;
    Binomial* b = newBinomial(print, comp, updateVertex);
    vertex*   v = NULL;
    int s = sizeQueue(q);

    for(int f = 0; f < s; f++)
    {
        v = dequeue(q);
        if(v->steps > i)
            i = v->steps;
        insertBinomial(b, v);
    }

    DArray* d = newDArray(NULL);
    for(int x = 0; x <= i; x++)
    {
        insertDArray(d, newQueue(NULL));
    }

    s = sizeBinomial(b);
    for(int c = 0; c < s; c++)
    {
        v = ((vertex*) extractBinomial(b));
        enqueue(getDArray(d, v->steps), v);
    }

    for(int y = 0; y <= i; y++)
    {
        printf("%d :", y);
        while(sizeQueue(getDArray(d, y)) > 0)
        {
            v = dequeue(getDArray(d, y));
            printf(" ");
            displayInteger(stdout, v->num);
            if(v->predecessor != NULL)
            {
                printf("(");
                displayInteger(stdout, v->predecessor->num);
                printf(")");
                printf("%d",v->key);
            }
        }
        printf("\n");
    }
    printf("----\n");
}

static void updateVertex(void* v, BinomialNode* bn)
{
    ((vertex *) v)->node = bn;
}

int getMax(FILE* fp)
{
    int max = INT_MIN;

    char* a = readToken(fp);
    char* b = readToken(fp);
    char* c = readToken(fp);

    while(!feof(fp))
    {
        if(strcmp(c, ";") != 0)
            c = readToken(fp);
        if(atoi(a) > max)
            max = atoi(a);
        if(atoi(b) > max)
            max = atoi(b);

        a = readToken(fp);
        b = readToken(fp);
        c = readToken(fp);
    }
    return max;
}

void makeList(FILE* fp, vertex** A, Printer p)
{
    char* a;
    char* b;
    char* c;
    int i, j, f, weight = 0;
    int min = INT_MAX;
    neighbor* temp = NULL;

    a = readToken(fp);
    b = readToken(fp);
    c = readToken(fp);

    while(!feof(fp))
    {
        i = atoi(a);
        if(i < min)
            min = i;
        j = atoi(b);
        if(j < min)
            min = j;

        if(strcmp(c, ";") == 0)
            weight = 1;
        else
        {
            weight = atoi(c);
            c = readToken(fp);  //gets rid of semicolon if weight was given
        }

        if(A[i]->num != NULL)
        {
            if(A[j]->num != NULL)
            {
                for(int x = 0; x < sizeDArray(A[i]->neighbors); x++)
                {
                    temp = getDArray(A[i]->neighbors, x);
                    if(temp->vert == A[j])
                    {
                        if(weight < temp->weight)
                        {
                            f = 1;
                            temp->weight = weight;
                        }
                        break;
                    }
                }
                if(f == 0)
                    insertDArray(A[i]->neighbors, newNeighbor(A[j], weight));
                f = 0;

                for(int x = 0; x < sizeDArray(A[j]->neighbors); x++)
                {
                    temp = getDArray(A[j]->neighbors, x);
                    if(temp->vert == A[i])
                    {
                        if(weight < temp->weight)
                        {
                            f = 1;
                            temp->weight = weight;
                        }
                        break;
                    }
                }
                if(f == 0)
                    insertDArray(A[j]->neighbors, newNeighbor(A[i], weight));
                f = 0;
            }


            else
            {
                A[j] = newVertex(p, newInteger(j));

                insertDArray(A[j]->neighbors, newNeighbor(A[i], weight));
                insertDArray(A[i]->neighbors, newNeighbor(A[j], weight));
            }
        }

        else if(A[i]->num == NULL)
        {
            if(A[j]->num == NULL)
            {
                A[i] = newVertex(p, newInteger(i));
                A[j] = newVertex(p, newInteger(j));

                insertDArray(A[i]->neighbors, newNeighbor(A[j], weight));
                insertDArray(A[j]->neighbors, newNeighbor(A[i], weight));
            }

            else
            {
                A[i] = newVertex(p, newInteger(i));

                insertDArray(A[i]->neighbors, newNeighbor(A[j], weight));
                insertDArray(A[j]->neighbors, newNeighbor(A[i], weight));
            }
        }

        a = readToken(fp);
        b = readToken(fp);
        c = readToken(fp);
    }
    A[min]->key = 0;
}

int compareVertices(void* a, void* b)
{
    vertex* l = (vertex*)a;
    vertex* r = (vertex*)b;

    if(a == NULL)
        return 1;
    if(b == NULL)
        return -1;
    if(l->key == r->key)
        return compareInteger(l->num, r->num);
    else return (l->key - r->key);
}