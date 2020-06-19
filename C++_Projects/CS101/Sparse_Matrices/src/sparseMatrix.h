#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include<fstream>
#include<vector>

using namespace std;

struct node
{
        int row;
        int collumn;
        float value;
		node* next;

        //setting the default constructor for a node
        node (int r, int c, float v, node* n) {
		row = r; 
		collumn = c;
		value = v; 
		next = n;
	}
};

class sparseMatrix
{
	private:
		int minRowIndex;
		int maxRowIndex;
		int minColIndex;
		int maxColIndex;
		int maxRow;
		int maxCol;
		node* head;
		node* tail;
		void placeInteriorNode(int, int, float);
		void addNode(int, int, float);
		vector <node*> getRowHeads();
		vector <node*> getRow(int, vector<node*>);
		vector <node*> getCollumn(int);
		int dotProduct(vector<node*>, vector<node*>);
	public:
		friend sparseMatrix operator - (sparseMatrix &m1, sparseMatrix &m2);
		friend sparseMatrix operator * (sparseMatrix &m1, sparseMatrix &m2);
        friend sparseMatrix operator + (sparseMatrix &m1, sparseMatrix &m2);
		friend void operator << (ofstream& output, sparseMatrix& matrix);
		friend void operator >> (ifstream& input, sparseMatrix& matrix);
        sparseMatrix() {
			maxRowIndex = 0;
			minRowIndex = 0;
			minColIndex = 0;
			maxColIndex = 0;
			maxRow = 0;
			maxCol = 0;
			head = NULL;
			tail = NULL;
		}
};

#endif //SPARSEMATRIX_H
