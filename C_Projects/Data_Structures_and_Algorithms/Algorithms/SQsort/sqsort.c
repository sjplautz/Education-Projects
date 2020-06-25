//
// Created by plaut on 2/5/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "stack.h"
#include "comparator.h"
#include "scanner.h"
#include "integer.h"
#include "real.h"
#include "strings.h"
#include "printer.h"

queue *readIntegers(FILE *, queue *);
queue *readReals(FILE *, queue *);
queue *readStrings(FILE *, queue *);
void  sort(queue*, queue*, stack*, Comparator);

int passes = 0;
Comparator comp;
Printer print;

int main(int argc, char *argv[])
{
    FILE* inputF;
    Comparator comp;
    Printer print;

    if(argc == 3)
        inputF = fopen(argv[2], "r");

    else
        inputF = stdin;

    queue *out = NULL;
    queue *in = NULL;
    stack *s = NULL;

    //determines flag type  and build respective data structures
    switch(argv[1][1])
    {
        case 'd':	// decimal
            comp = intComparator;
            print = displayInteger;
            out = newQueue(print);
            in = newQueue(print);
            s = newStack(print);
            in = readIntegers(inputF, in);          //initialize output queue
            break;
        case 'r':    // real
            comp = realComparator;
            print = displayReal;
            out = newQueue(print);
            in = newQueue(print);
            s = newStack(print);
            in = readReals(inputF, in);
            break;
        case 's':   //string
            comp = stringComparator;
            print = displayString;
            out = newQueue(print);
            in = newQueue(print);
            s = newStack(print);
            in = readStrings(inputF, in);
            break;
        case 'v':   //author
            fprintf(stdout, "Stephen J. Plautz\n");
            return 0;
        default:
            fprintf(stdout, "unknown flag '");
            fprintf(stdout, "%s", argv[1]);
            fprintf(stdout, "', valid flags are -d, -r, -s, and -v\n\n");
            return(-1);
    }

    displayQueue(stdout, in);
    fprintf(stdout, "\n");
    sort(in, out, s, comp);

    return 0;
}

//a recursive function used to sort the values of input queue from greatest to smallest
void sort(queue *in, queue *out, stack *s, Comparator comp)
{
    queue* temp;
    int pushes = 0;
    void *peekQVal = NULL;
    void *peekSVal = NULL;
    void *dqVal = NULL;

    while(sizeQueue(in) != 0)
    {
        dqVal = dequeue(in);
        if(sizeQueue(in) != 0)
            peekQVal = peekQueue(in);

        while(sizeStack(s) > 0)
        {
            peekSVal = peekStack(s);
            if(comp(peekSVal, dqVal) >= 0)
                enqueue(out, pop(s));

            else
                break;
        }

        if((sizeQueue(in)) + 1 == 1 || comp(dqVal, peekQVal) >= 0)
            enqueue(out, dqVal);

        else
        {
            push(s, dqVal);
            pushes++;
        }
    }

    while(sizeStack(s) > 0)
    {
        enqueue(out, pop(s));
    }

    if(pushes > 0)
    {
        displayQueue(stdout, out);
        fprintf(stdout, "\n");

        temp = out;
        out = in;
        in = temp;

        passes++;
        sort(in, out, s, comp);
    }

    else
        if(passes == 0)
        {
            displayQueue(stdout, out);
            fprintf(stdout, "\n");
        }

    return;
}

//reads in integers from a file or stdin and builds the input queue
queue *readIntegers(FILE *fp, queue *q)
{
    int x  = readInt(fp);
    while(!feof(fp))
    {
        enqueue(q, newInteger(x));
        x = readInt(fp);
    }
    return q;
}

//reads in reals into a queue
queue *readReals(FILE *fp, queue *q)
{
    double x  = readReal(fp);
    while(!feof(fp))
    {
        enqueue(q, newReal(x));
        x = readReal(fp);
    }
    return q;
}

//reads in strings into a queue
queue *readStrings(FILE *fp, queue *q)
{
    char* x  = readString(fp);
    while(!feof(fp))
    {
        enqueue(q, newString(x));
        x = readString(fp);
    }
    return q;
}