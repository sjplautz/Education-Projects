///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	CS 101
//	PROJECT 3	This program reads in two txt files via an overloaded input operator, puts their contents into two sroted linked lists, and then performs user 
//			specified arithmetic operations on the linked lists in the form of matrixes. It then outputs the reulting matrix into a txt file.
//	STEPHEN PLAUTZ
//	MAR 4, 2016
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include"sparseMatrix.h"

using namespace std;

int main(int argc, char **argv) 
{
	if(argc < 5){
		cerr << "Inusfficient arguments provided, exiting\n";
	}	

	//declaring three linked lists structures that are objects of type "matrix"
	sparseMatrix m1;
	sparseMatrix m2;
	sparseMatrix m;

	//opening input sparse matrixes
	ifstream infile1(argv[2]), infile2(argv[3]);

	//opening the output file
	ofstream outstream1("output/m1.txt");
	ofstream outstream2("output/m2.txt");
	ofstream outstream3(argv[4]);

	//at this point the input operator is overloaded to input the contents of these files into the linked lists
	infile1 >> m1;
	infile2 >> m2;
	outstream1 << m1;
	outstream1.close();
	outstream2 << m2;
	outstream2.close();

	//closing the files
	infile1.close(); infile2.close();

	//performing the different operations based off of user input
	string op = (argv[1]);
	if (op == "add") m = (m1 + m2);
	else if (op == "sub") m = (m1 - m2);
	else if (op == "mul") m = (m1 * m2);
	else 
	{
		cerr << "Operation not supported, exiting\n";
		return 1;
	}

	//using overlaoded ouput operator to read in results of operation into linked list m
	outstream3 << m;
	outstream3.close();
	
	return 0;
}
