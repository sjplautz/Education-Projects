//
// Created by Stephen Plautz on 1/21/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include "sll.h"
#include "queue.h"

//creates a new queue with null head and tail values
queue *newQueue(void (*d)(FILE *,void *))
{
    queue *q = malloc(sizeof(queue));
    q->list  = newSLL(d);
    return q;
}

//adds an item to the back of the list based queue
void enqueue(queue *items,void *value)
{
    insertSLL(items->list,sizeSLL(items->list),value);
}

//removes the first item from the queue
void *dequeue(queue *items)
{
    return removeSLL(items->list,0);
}

//displays the first item of the queue
void *peekQueue(queue *items)
{
    return getSLL(items->list,sizeSLL(items->list-1));
}

//returns the amount of elements in the queue
int sizeQueue(queue *items)
{
    return(sizeSLL(items->list));
}

//displays the elements of the queue
void displayQueue(FILE *fp,queue *items)
{
    displaySLL(fp,items->list);
}