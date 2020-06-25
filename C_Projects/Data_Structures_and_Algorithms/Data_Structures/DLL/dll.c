//
// Created by Stephen Plautz on 1/19/2017.
//

#include<stdio.h>
#include<stdlib.h>
#include "dll.h"


//creates a new doubly linked list with a null head and tail value
dll *newDLL(void (*d)(FILE *,void *)) //d is the display function
{
    dll *items = malloc(sizeof(dll));
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

//allocates a new dll node in memory
dllnode *newDllNode(void *val)
{
    dllnode *p = malloc(sizeof(dllnode));
    if (p == 0)
    {
        fprintf(stderr,"out of memory\n");
        exit(-1);
    }
    p->value = val;
    p->next = NULL;
    p->prev = NULL;
    return p;
}

//creates a new node and stores it in the proper index with a generic value
void insertDLL(dll *items,int index,void *value) //stores a generic value
{
    dllnode *new = newDllNode(value);

    //case where it is being inserted to the front of the list
    if(index == 0)
    {
        //in the case that the list was empty, both the head and tail will be initialized
        if(items->size == 0)
        {
            items->head = new;
            items->tail = new;;
        }
        else
        {
            new->next = items->head;
            items->head->prev = new;
            items->head = new;
        }
    }

    //if inserted at end of list
    else if(index == items->size)
    {
        new->prev = items->tail;
        items->tail->next = new;
        items->tail = new;
    }

    //all other cases
    else
    {
        //traverse from front if faster
        if((items->size)/2 > index)
        {
            dllnode *ptr = items->head;
            for(int i = 1; i < index; i++)
            {
                ptr = ptr->next;
            }
            new->next = ptr->next;
            new->prev = ptr;
            ptr->next->prev = new;
            ptr->next = new;
        }

        else
        {
            dllnode *ptr = items->tail;
            for(int i = items->size; i > index; i--)
            {
                ptr = ptr->prev;
            }
            new->prev = ptr;
            new->next = ptr->next;
            ptr->next->prev = new;
            ptr->next = new;
        }
    }
    items->size++;
}

//returns a generic value and removes the node at the index given
void *removeDLL(dll *items,int index)
{
    dllnode* ptr = items->head;
    void* value = 0;

    if(index == 0)
    {
        if(items->head == items->tail)
            items->head = items->tail = NULL;

        else
        {
            items->head = items->head->next;
            items->head->prev = NULL;
        }
    }

    else if(index == items->size-1)
    {
        ptr = items->tail;
        items->tail = ptr->prev;
        ptr->prev->next = NULL;
    }

    else
    {
        if(index < (items->size)/2)
        {
            for(int i = 0; i < index; i++)
                ptr = ptr->next;
            ptr->prev->next = ptr->next;
            ptr->next->prev = ptr->prev;
        }

        else
        {
            ptr = items->tail;
            for(int i = items->size; i > index; i--)
                ptr = ptr->prev;
            ptr->prev->next = ptr->next;
            ptr->next->prev = ptr->prev;
        }
    }

    value = ptr->value;
    ptr = NULL;
    items->size--;
    return value;
}

//merges two linked lists into one
void unionDLL(dll *recipient,dll *donor)
{
    if(recipient->size == 0 && donor->size == 0)
        return;
    if (recipient->size == 0)
        recipient->head = donor->head;
    else
        recipient->tail->next = donor->head;
    if (donor->size > 0)
    {
        donor->head->prev = recipient->tail;
        recipient->tail = donor->tail;
    }
    recipient->size = recipient->size + donor->size;
    donor->size = 0;
    donor->head = donor->tail = NULL;
}

//gets the value of a node at a given index
void *getDLL(dll *items, int index)
{
    dllnode *temp;
    temp = items->head;

    if(index < items->size/2)
    {
        for(int i = 0; i < index; i++)
            temp = temp->next;
    }
    else
    {
        for(int i = items->size-1; i > index; i--)
            temp = temp->prev;
    }

    return temp->value;
}

//returns the amount of nodes in the singly linked list
int sizeDLL(dll *items)
{
    return items->size;
}

//prints the values in the order stored in the linked list
void displayDLL(FILE *fp,dll *items)
{
    dllnode *disp = items->head;

    fprintf(fp,"[");
    while (disp != NULL) {
        items->display(fp, disp->value);
        if (disp->next != NULL) fprintf(fp,",");
        disp = disp->next;
    }
    fprintf(fp,"]");
}