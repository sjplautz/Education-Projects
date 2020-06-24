OOPTS = -Wall -Wextra -g -c -std=c99
LOPTS = -Wall -g -std=c99 -O0

all: project2 project2.c

project2 : $(OBJS)
	gcc $(LOPTS) -o project2 $(OBJS) project2.c

clean :
	rm -f $(OBJS) project2

test :
	./project2 600
	./project2 600 10
	./project2 600 20
	./project2 600 30
	./project2 600 40