BIN = ./bin
LIBS = ./lib
OBJS = project1.o dgemm0.o dgemm1.o dgemm2.o dgemm3.o 
EXES = project1.exe
OOPTS = gcc -Wall -Wextra -g -c -std=c99
LOPTS = gcc -Wall -g -std=c99 -O0

.PHONY: all
all: project1.exe 

project1.exe : $(OBJS)
	$(LOPTS) -o $(BIN)/project1.exe $(OBJS) 

project1.o : ./src/project1.c ./src/dgemm0.c ./src/dgemm1.c ./src/dgemm2.c ./src/dgemm3.c
	$(OOPTS) ./src/project1.c

dgemm0.o : ./src/dgemm0.c $(LIBS)/dgemm0.h
	$(OOPTS) ./src/dgemm0.c

dgemm1.o : ./src/dgemm1.c $(LIBS)/dgemm1.h
	$(OOPTS) ./src/dgemm1.c

dgemm2.o : ./src/dgemm2.c $(LIBS)/dgemm2.h
	$(OOPTS) ./src/dgemm2.c

dgemm3.o : ./src/dgemm3.c $(LIBS)/dgemm3.h
	$(OOPTS) ./src/dgemm3.c

.PHONY: clean
clean :
	rm -f $(OBJS) $(BIN)/$(EXES)

.PHONY: test
test :
	 $(BIN)/$(EXES) 64
	 $(BIN)/$(EXES) 128
	 $(BIN)/$(EXES) 256
	 $(BIN)/$(EXES) 512
	 $(BIN)/$(EXES) 1024
	 $(BIN)/$(EXES) 2048
