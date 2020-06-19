#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <cctype>
#include <queue>

using namespace std;

struct node
{
	char data;
	node* left;
	node* right;
};

//node constructor
node* newNode(int data)
{
	node *temp = new node;
	temp->data = data;
	temp->left = temp->right = NULL;
	return (temp);
}

int determine(ifstream &order1, ifstream &order2);

node* buildTree0(string, string, int*, int, int, int);
node* buildTree1(char arr1[], char arr2[], int, int, int);
int search(char arr1[], int, int, char);
char* extractData(char arr1[], char arr2[], int, int);

void decode(string, node*, ofstream &message);
void printInorder(node*, ofstream &order3);
void printLevelorder(node*, ofstream &order4);
void printPreorder(node*, ofstream &order3);
void printPostorder(node*, ofstream &order4);

int main(int argc, char** argv)
{
	int c = 0;
	int size = 0;
	string o1string;
	string o2string;

	//step 1: read in files
	ifstream order1, order2, cipherin; //(either preorder and postorder OR inorder and levelorder)
	order1.open(argv[1]);
	order2.open(argv[2]);
	cipherin.open(argv[3]);

	string line, ciphertext;

	//initializing ciphertext
	while(getline(cipherin, line))
	{
		ciphertext += line;
	}

	ofstream order3, order4, message;
	order3.open(argv[4]);  //(first type of not given order traversal)
	order4.open(argv[5]); //(second type of not given order traversal)
	message.open(argv[6]); //(deciphered output of ciphertext)


	//step 2: determine what type of traversal order input file 1 and 2 are
	c = determine(order1, order2);

	//step 3: using this information build a huffman tree (labels at leaf nodes are letters, all others can be ignored)
	getline(order1, o1string);
	getline(order2, o2string);
	size = o1string.size();

	if(c == 0)
	{
		//build tree function based off preorder and postorder
		int preIndex = 0;
		node* root = buildTree0(o1string, o2string, &preIndex, 0, size - 1, size);
		//step 4: perform the remaining two tree traversals that were not provided on the constructed huffman tree
		printInorder(root, order3);
		printLevelorder(root, order4);
		//step 5: decode the ciphertext into plain text by using the huffman tree, (0 for left, 1 for right)
		decode(ciphertext, root, message);
	}
	else
	{
		//initializing the traversals as char arrays
		char inorder[size];
		char levelorder[size];
		for(int i = 0; i < size; i++)
		{
			inorder[i] = o1string[i];
			levelorder[i] = o2string[i];
		}

		//build tree function based off inorder and levelorder
		node* root = buildTree1(inorder, levelorder, 0, size - 1, size);
		//step 4: perform the remaining two tree traversals that were not provided on the constructed huffman tree
		printPreorder(root, order3);
		printPostorder(root, order4);
		//step 5: decode the ciphertext into plain text by using the huffman tree, (0 for left, 1 for right)
		decode(ciphertext, root, message);
	}

	return 0;
}

//determines what type of sorting order to use
int determine(ifstream &order1, ifstream &order2)
{
	char first, last;
	string temp1, temp2;

	getline(order1, temp1);
	first = temp1[0];	
	order1.seekg(0);

	getline(order2, temp2);
	order2.seekg(0);
	
	//should give last the last letter of order2 file
	last = temp2[(temp2.size()-1)];

	//with preorder and postorder, the first letter of preorder will always match the last letter of porstorder
	if(first == last)
		return 0;
	else
		return 1;
}

//a recursive function that creates a huffman tree given a preoder and postorder traversal
// preIndex is used to keep track of index in o1string.
// l is low index and h is high index for the current subarray in o2string
node* buildTree0(string preorder, string postorder, int* preIndex, int l, int h, int size)
{
	// Base case
	if (*preIndex >= size || l > h)
		return NULL;

	// The first node in preorder traversal is root. So take the node at
	// preIndex from preorder and make it root, and increment preIndex
	node* root = newNode(preorder[*preIndex]);
	++*preIndex;

	// If the current subarry has only one element, no need to recur
	if (l == h)
		return root;

	// Search the next element of pre[] in post[]
	int i;
	for (i = l; i <= h; ++i)
		if (preorder[*preIndex] == postorder[i])
			break;

	// Use the index of element found in postorder to divide postorder array in
	// two parts. Left subtree and right subtree
	if (i <= h)
	{
		root->left = buildTree0(preorder, postorder, preIndex, l, i, size);
		root->right = buildTree0(preorder, postorder, preIndex, i + 1, h, size);
	}
	return root;
}

//Function to find index of value in inorder[start...end]
int search(char inorder[], int strt, int end, char value)
{
	for (int i = strt; i <= end; i++)
		if (inorder[i] == value)
			return i;

	//in the case that the letter was not found in inorder
	return -1;
}

// sizeLVL is size of levelorder, size1 is size of inorder (or portion of inorder) and size1 < sizeLVL. This
// function extracts chars from levelorder which are present in inorder.
char* extractData(char inorder[], char levelorder[], int size1, int sizeLVL)
{
	char* newlevelstring = new char[size1];
	unsigned int j = 0;
	for (int i = 0; i < sizeLVL; i++){
		if (search(inorder, 0, size1 - 1, levelorder[i]) != -1)
			newlevelstring[j] = levelorder[i], j++;
	}

	return newlevelstring;
}

//builds a tree given an inorder and levelorder 
node* buildTree1(char inorder[], char levelorder[], int inStrt, int inEnd, int sizeIN)
{
	if(inStrt > inEnd)
		return NULL;

	// The first node in level order traversal is root
	node*root = newNode(levelorder[0]);

	// base case If this node has no children then return
	if (inStrt == inEnd)
		return root;

	// find the index of this character in Inorder traversal
	int inIndex = search(inorder, inStrt, inEnd, root->data);

	// Extract left subtree chars from level order traversal
	char* llevel  = extractData(inorder, levelorder, inIndex, sizeIN);

	// Extract right subtree chars from level order traversal
	// first argument is telling it to only grab inorder array from inIndex + 1 on
	char* rlevel  = extractData(inorder + inIndex + 1, levelorder, sizeIN-inIndex-1, sizeIN);

	//construct left and right subtrees
	root->left = buildTree1(inorder, llevel, inStrt, inIndex-1, sizeIN);
	root->right = buildTree1(inorder, rlevel, inIndex+1, inEnd, sizeIN);

	delete [] rlevel;
	delete [] llevel;

	return root;
}

//prints inorder traversal of the tree to an output file
void printInorder(node* root, ofstream &order3)
{
	if (root == NULL)
		return;
	printInorder(root->left, order3);
	order3 << root->data;
	printInorder(root->right, order3);
}

//prints levelorder traversal of the tree to an output file
void printLevelorder(node* root, ofstream &order4)
{
	if(root == NULL) return;
	queue<node*> Q;
	//enters the address of the current root being inspected into the queue
	Q.push(root);
	//while there is at least on node in the queue
	while(!Q.empty())
	{
		node* current = Q.front();
		order4 << current->data;
		if(current->left != NULL)
			Q.push(current->left);
		if(current->right != NULL)
			Q.push(current->right);
		//removes element at the front of the queue
		Q.pop();
	}
}

//prints preorder traversal of the tree to an output file
void printPreorder(node* root, ofstream &order3)
{
	if (root)
	{
		order3 << root->data;
		printPreorder(root->left, order3);
		printPreorder(root->right, order3);
	}
}

//prints postorder traversal of the tree to an output file
void printPostorder(node* root, ofstream &order4)
{
	if(root != NULL)
	{
		printPostorder(root->left, order4);
		printPostorder(root->right, order4);
		order4 << root->data;
	}
}

//decodes the ciphertext by using binary traversal of huffman tree
void decode(string ciphertext, node* root, ofstream &message)
{
	string plain = "";
	while (ciphertext.length() > 0)
	{
		node *current = root;
		while (current->left != NULL)
		{
			if (ciphertext[0]=='0')
				current = current->left;
			if (ciphertext[0]=='1')
				current = current->right;
			ciphertext.erase(0,1);
		}
		plain = plain + current->data;
	}
	message << plain;
}
