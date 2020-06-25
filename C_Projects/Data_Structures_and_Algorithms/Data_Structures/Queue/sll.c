//
// Created by Stephen Plautz on 1/19/2017.
//

#include<stdio.h>
#include<stdlib.h>
#include "sll.h"


//creates a new singly linked list with a null head and tail value
sll *newSLL(void (*d)(FILE *,void *)) //d is the display function
{
    sll *items = malloc(sizeof(sll));
    if (items == 0)
    {
        fprintf(stderr,"out of memory");
        exit(-1);
    }
    items->head = 0;
    items->tail = 0;
    items->size = 0;
    items->display = d;
    return items;
}

//allocates a new sll node in memory
sllnode *newSllNode(void *val)
{
    sllnode *p = malloc(sizeof(sllnode));
    if (p == 0)
    {
        fprintf(stderr,"out of memory\n");
        exit(-1);
    }
    p->value = val;
    p->next = NULL;
    return p;
}

//creates a new node and stores it in the proper index with a generic value
void insertSLL(sll *items,int index,void *value) //stores a generic value
{
    sllnode *new = newSllNode(value);

    //case where it is being inserted to the front of the list
    if(index == 0)
    {
        //in the case that the list was empty, both the head and tail will be initialized
        if(items->size == 0)
        {
            items->head = new;
            items->tail = new;
        }
        else
        {
            new->next = items->head;
            items->head = new;
        }
    }

    //case where inserted to end of list
    else if(index == items->size)
    {
        items->tail->next = new;
        items->tail = new;
    }

    else
    {
        sllnode *ptr = items->head;
        for(int i = 1; i < index; i++)
            ptr = ptr->next;

        new->next = ptr->next;
        ptr->next = new;
    }
    items->size++;
}

//returns a generic value and removes the node at the index given
void *removeSLL(sll *items,int index)
{

    sllnode *ptr = items->head;
    void * value = 0;
    items->size--;

    if(index == 0)
    {
        value = ptr->value;
        if(items->head == items->tail)
            items->head = items->tail = NULL;
        else
            items->head = items->head->next;
    }

    else
    {
        for(int i = 1; i < index; i++)
            ptr = ptr->next;
        value = ptr->next->value;
        if(index == items->size)
            items->tail = ptr;
        ptr->next = ptr->next->next;
    }

    ptr = NULL;
    return value;
}

//merges two linked lists into one
void unionSLL(sll *recipient,sll *donor)
{
    if(recipient->size && donor->size == 0)
        return;
    if (recipient->size > 0 && donor->size == 0)
        recipient->tail->next = donor->head;

    else if (donor->size > 0 && recipient->size == 0)
    {
        recipient->head = donor->head;
        recipient->tail = donor->tail;
    }

    else
    {
        recipient->tail->next = donor->head;
        recipient->tail = donor->tail;
    }

    recipient->size = (recipient->size + donor->size);
    donor->size=0;
    donor->head = donor->tail = NULL;
}

//gets the value of a node at a given index
void *getSLL(sll *items, int index)
{
    sllnode *temp;
    temp = items->head;

    for(int i = 0; i < index; i++)
    {
        temp = temp->next;
    }
    return temp->value;
}

//returns the amount of nodes in the singly linked list
int sizeSLL(sll *items)
{
    return items->size;
}

//prints the values in the order stored in the linked list
void displaySLL(FILE *fp,sll *items)
{
    sllnode* disp = items->head;

    fprintf(fp,"[");
    while(disp != NULL)
    {
        items->display(fp, disp->value);
        if(disp->next != NULL) fprintf(fp,",");
        disp = disp->next;
    }
    fprintf(fp,"]");
}