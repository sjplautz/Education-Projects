OOPTS = -Wall -Wextra -g -c -std=c99
LOPTS = -Wall -g -std=c99 -O0

all: sieve2

sieve2 : $(OBJS)
	mpicc $(LOPTS) -o sieve2 $(OBJS) sieve2.c -lm

clean :
	rm -f $(OBJS) sieve2

test :
	./sieve2 500