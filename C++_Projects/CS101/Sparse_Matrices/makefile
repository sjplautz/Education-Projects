BIN = ./bin
SRC = ./src
DATA = ./input
OUTPUT = ./results
SRCS = $(wildcard ./src/*.cpp)
OBJS = main.o sparseMatrix.o
EXES = project3.exe
OBJOPTS = g++ -g -c -Wall -std=c++11
EXEOPTS = g++ -g -Wall -std=c++11

.PHONY: all
all: project3.exe

project3.exe: $(OBJS)
	$(EXEOPTS) -o $(BIN)/project3.exe $(OBJS)

main.o: $(SRC)/main.cpp sparseMatrix.o
	$(OBJOPTS) $(SRC)/main.cpp

sparseMatrix.o: $(SRC)/sparseMatrix.cpp $(SRC)/sparseMatrix.h
	$(OBJOPTS) $(SRC)/sparseMatrix.cpp

.PHONY: clean
clean:
	rm -f $(OBJS) $(BIN)/$(EXES) $(OUTPUT)/*

.PHONY: testAdd
testAdd: $(BIN)/$(EXES)
	$(BIN)/$(EXES) add $(DATA)/matrix1.txt $(DATA)/matrix2.txt $(OUTPUT)/addition.txt

.PHONY: testSub
testSub: $(BIN)/$(EXES)
	$(BIN)/$(EXES) sub $(DATA)/matrix1.txt $(DATA)/matrix2.txt $(OUTPUT)/subtraction.txt

.PHONY: testMult
testMult: $(BIN)/$(EXES)
	$(BIN)/$(EXES) mult $(DATA)/matrix1.txt $(DATA)/matrix2.txt $(OUTPUT)/multiplication.txt
