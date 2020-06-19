#include <iostream>
#include <vector>
#include <fstream>
#include "sparseMatrix.h"

using namespace std;

//adds a new node to the matrix linked list, skips entries with a value of 0 
//resolves collisions by adding values of original and new node
void sparseMatrix::addNode(int r, int c, float v){
	if(v == 0)
		return;
	if(this->head == NULL){
		this->head = this->tail = new node(r, c, v, NULL);
		this->minRowIndex = this->maxRowIndex = r;
		this->minColIndex = this->maxColIndex = c;
		//updated to record dimensions of represented matrix
		this->maxRow = r;
		this->maxCol = c;
	}
	else{
		//if the node has either the smallest row value, or an equal to min row value and the new 
		//smallest relative collumn value it is the new head
		if(r < this->minRowIndex || (r == this->minRowIndex && c < this->minColIndex)){
			node* tempNode = new node(r, c, v, this->head);
			this->head = tempNode;
			this->minRowIndex = r;
			this->minColIndex = c;
		}
		//if the node has either the largest row value, or an equal to max row value and the new 
		//largest relative collumn value it is the new tail
		else if(r > this->maxRowIndex || (r == this->maxRowIndex && c > this->maxColIndex)){	
			node* tempNode = new node(r, c, v, NULL);
			this->tail->next = tempNode;
			this->tail = tempNode;
			this->maxRowIndex = r;
			this->maxColIndex = c;
			//updated to record dimensions of represented matrix
			this->maxRow = r;
			if(this->maxCol < c)
				this->maxCol = c;
		}
		//otherwise it is an interior node and we must place it in collumn order as well
		else
			placeInteriorNode(r, c, v);
	}
}

//adds a node to the matrix linked list, keeping intact collumn order as well as row order
void sparseMatrix::placeInteriorNode(int r, int c, float v){
	node* iterator = this->head;
	node* prev = iterator;
	
	while(iterator != NULL){

		//if this condition is met it is the first entry in a row
		if(prev->row < r && iterator->row > r){
			prev->next = new node(r, c, v, iterator);
			//updated to record dimensions of represented matrix
			if(r > this->maxRow)
				this->maxRow = r;
			return;
		}

		//if there are other entries in the row
		if(iterator->row == r){	
			//iterate until the current collumn entry is greater than new, or the row has changed
			while((iterator->row == r) && (iterator->collumn < c)){
				prev = iterator;
				iterator = iterator->next;
			}
			
			//there is an existing node in this index, perform addition by default
			if(iterator->row == r && iterator->collumn == c)
				iterator->value += v;
				
			//if this is the first node for it's index then create it
			else{
				prev->next = new node(r, c, v, iterator);
				//updated to record dimensions of represented matrix
				if(c > this->maxCol)
					this->maxCol = c;
			}
			return;
		}

		prev = iterator;
		iterator = iterator->next;
	}
}

//returns a vector containing an index of all row heads in a matrix
//inserts nodes in indices corresponding to their row values
vector<node*> sparseMatrix::getRowHeads(){
	vector<node*> rowHeads;
	node* iterator = this->head;
	int lastrow = -1;
	vector<node*>::iterator it = rowHeads.begin();
	while(iterator != NULL){
		if(iterator->row > lastrow)
			rowHeads.insert(it + iterator->row, iterator);
		lastrow = iterator->row;
		iterator = iterator->next;
	}
	return rowHeads;
}

//returns a vector containign an idex of all nodes in a certain row of a matrix
//inserts nodes in indices corresponding to their row values
vector<node*> sparseMatrix::getRow(int r, vector<node*> rowHeads){
	vector<node*> row;
	node* iterator = rowHeads[r];
	vector<node*>::iterator it = rowHeads.begin();
	while(iterator->row == r){
		row.insert(it + iterator->row, iterator);
		iterator = iterator->next;
	}
	return row;
}

//returns a vector containing an index of all nodes in a certain collumn of a matrix
//inserts nodes in indices corresponding to their collumn values
vector<node*> sparseMatrix::getCollumn(int c){
	vector<node*> col;
	node* iterator = this->head;
	vector<node*>::iterator it = col.begin();
	while(iterator != NULL){
		if(iterator->collumn == c)
			col.insert(it + iterator->row, iterator);
		iterator = iterator->next;
	}
	return col;
}

//returns the dot product of two vectors
int sparseMatrix::dotProduct(vector<node*> r, vector<node*> c){
	if(r.size() == 0 || c.size() == 0)
		return 0;

	//checks to see which dimension is more populated 
	bool rowsLarger = true;
	if(r.size() < c.size())
		rowsLarger = false;

	//all values with dimesnions not in other vector aremultiplied by zero and thus need not be considered
	int rowVal, colVal, product, sum = 0;
	if(rowsLarger){
		for(long unsigned int i = 0; i < c.size(); i++){
			rowVal = r[i]->row;
			colVal = c[i]->collumn;
			product = rowVal * colVal;
			sum += product;
			product = 0;
		}
	}
	else{
		for(long unsigned int i = 0; i < r.size(); i++){
			rowVal = r[i]->row;
			colVal = c[i]->collumn;
			product = rowVal * colVal;
			sum += product;
			product = 0;
		}
	}
	return product;
}
	
//performs matrix addition on 2 matrices, and returns a new resulting matrix
sparseMatrix operator + (sparseMatrix &m1, sparseMatrix &m2){
	sparseMatrix result; 
	node* iterator1 = m1.head;
	node* iterator2 = m2.head;

	while(iterator1 != NULL){
		result.addNode(iterator1->row, iterator1->collumn, iterator1->value);
		iterator1 = iterator1->next;
	}
	while(iterator2 != NULL){
		result.addNode(iterator2->row, iterator2->collumn, iterator2->value);
		iterator2 = iterator2->next;
	}

	return result;
}

//performs matrix subtraction on 2 matrices, and returns a new resulting matrix
sparseMatrix operator - (sparseMatrix &m1, sparseMatrix &m2){
	sparseMatrix result; 
	node* iterator1 = m1.head;
	node* iterator2 = m2.head;
	
	while(iterator1 != NULL){
		result.addNode(iterator1->row, iterator1->collumn, iterator1->value);
		iterator1 = iterator1->next;
	}
	while(iterator2 != NULL){
		result.addNode(iterator2->row, iterator2->collumn, (-1.0 * iterator2->value));
		iterator2 = iterator2->next;
	}

	return result;
}

//pperforms matrix multiplaction on 2 matrices, returning the resulting matrix
sparseMatrix operator * (sparseMatrix &m1, sparseMatrix &m2){
	sparseMatrix result; 
	//used as a cache of row heads to increase efficiency of extracting a row
	vector <node*> m1Heads = m1.getRowHeads();
	vector <node*> m1Row, m2Col;
	int sum = 0;

	for(int i = 0; i < m1.maxRow; i++){
		m1Row = m1.getRow(i, m1Heads);
		for(int j = 0; j < m2.maxCol; j++){
			m2Col = m2.getCollumn(j);
			sum = m1.dotProduct(m1Row, m2Col);
			result.addNode(i, j, sum);
		}
	}
			
	return result;
}

//prints out a matrix to the provided output file
void operator << (ofstream &outfile, sparseMatrix &matrix){
	node* iterator = matrix.head;

	while (iterator != NULL){
		outfile << iterator->row << " " << iterator->collumn << " " << iterator->value << '\n';
		iterator = iterator->next;
	}
	outfile << "0 0 0";
}

//reads in the contents of a file into a linked list
void operator >> (ifstream &infile, sparseMatrix &matrix){
	int row;
	int collumn;
	float value;

	while(true){
		infile >> row;
		infile >> collumn;
		infile >> value;

		//end of file is marked by a line of 0 0 0
		if(row == 0 && collumn == 0 && value == 0)
			return;

		matrix.addNode(row, collumn, value);
	}	
}
