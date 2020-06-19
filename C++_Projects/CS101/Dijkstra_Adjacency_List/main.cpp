///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	This program takes two input files, one containing the information for vertexes, and one containing the information for edges, and builds a wighted digraph from the 
//	information given. It then implements Dijkstra's algorithm to determine the shortest path from a user entered start and end vertex and prints the reults.
//	Course: CS_101
//	Submitted By: Stephen J Plautz
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <functional>
#include <queue>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include "edge.h"

using namespace std;

class CMyNode
{
	public:
		string name;
		CMyNode* parent;
		float delay;
		float weight; //holds the final calculated time for each search type
		vector<CMyEdge> edges;
		int neighbors;
		bool operator< (const CMyNode &other)const
		{return weight > other.weight;}
};

class CMyGraph
{
	private:
		int V = 0, E = 0;
		vector<CMyNode> nodes;
		vector<CMyNode> parents;
	public:
		CMyNode* makeNode(string n, double v)
		{
			CMyNode* temp = new CMyNode;
			temp->name = n;
			temp->parent = NULL;
			temp->delay = v;
			temp->weight = 10000000000000;
			return temp;
		}

		CMyEdge* makeEdge(CMyNode* strt, CMyNode* nd, double dist, double sp)
		{
			CMyEdge* temp = new CMyEdge;
			temp->start = strt;
			temp->end = nd;
			temp->distance = dist;
			temp->speed = sp;
			temp->neighbor = NULL;
			return temp;
		}
		CMyGraph(ifstream &file1, ifstream &file2);
		void dijkstra(string, string, int);
		CMyNode* getvertex(string);
		float getDistance(CMyNode, CMyEdge, int, string, string);
		void printpath(CMyNode, string);
};

//CMyGraph Constructor
CMyGraph :: CMyGraph(ifstream &Edges, ifstream &Nodes)
{
	string name, big, strt, nd, source, destination;
	double val, dist, sp;

	CMyNode* walker;

	while(getline(Nodes, big))
	{
		stringstream stream(big);
		stream >> name >> val;
		nodes.push_back(*makeNode(name, val));
		V++;
	}

	while(getline(Edges, big))
	{
		stringstream stream(big);
		stream >> strt >> nd >> dist >> sp;
		//traverse list to find where to put this edge O(V) time
		for(int i = 0; i < V; i++)
		{
			if(nodes[i].name == strt)
			{
				walker = &nodes[i];
				walker->edges.push_back(*makeEdge(&nodes[i], getvertex(nd), dist, sp));
				walker->neighbors++;
			}
		}
		E++;
	}

	string a, b;
	int c = 0;
	while(1)
	{
		if(c > 0)
			cout << endl;
		c++;
		cout << "Please enter a pair of nodes for a source and a destination: ";
		cin >> a >> b;
		for(int i = 0; i < 5; i++)
			dijkstra(a, b, i);
	}
}

//updates the value of all paths with the values of the evaluated shortest paths
void CMyGraph::dijkstra(string start, string end, int type)
{
	priority_queue<CMyNode> q;

	CMyNode* startVertex;
	CMyNode grab;
	CMyEdge edge;
	float distance = 0;

	for(int i = 0; i < V; i++)
	{
		//reloading for multiple passes
		nodes[i].weight = 1000000000000;
	}
	
	startVertex = getvertex(start);
	startVertex->weight = 0;

	q.push(*startVertex);
	while(!q.empty())
	{
		//grabs the head of the queue
		grab = q.top();
		q.pop();
		//get neighbors and edges of node one at a time
		for(int i = 0; i < grab.neighbors; i++)
		{
			edge = grab.edges[i];
			distance = getDistance(grab, edge, type, start, end);
			if(edge.end->weight > grab.weight + distance)
			{
				edge.end->weight = (grab.weight + distance);
				edge.end->parent = edge.start;
				q.push(*edge.end);
			}
		}
	}
	//printing the results after each call
	for(int i = 0; i < V; i++)
	{
		if(nodes[i].name == end)
		{
			if(type == 0)
				cout << endl << "Shortest distance: " << nodes[i].weight << endl;
			else if(type == 1)
				cout << endl << "Shortest delay time: " << nodes[i].weight << endl;
			else if(type == 2)
				cout << endl << "Shortest expected travel time: " << nodes[i].weight << endl;
			else if(type == 3)
				cout << endl << "Shortest expected total time: " << nodes[i].weight << endl;
			else
				cout << endl << "Fewest hops: " << nodes[i].weight << endl;
			
			printpath(nodes[i], start);	
			break;
		}	
	}
}

//prints the calculated shortest path from start to end nodes given
void CMyGraph::printpath(CMyNode end, string start)
{
	CMyNode* walk;
	vector<string> names;
	walk = &end;
	
	while(walk->parent != NULL)
	{
		names.push_back(walk->name);
		walk = walk->parent;
	}
	names.push_back(start);
	cout << "shortest path: ";
	for(int i = names.size() - 1; i >= 0; i--)
	{
		if(i != 0)
			cout << names[i] << "->";
		else
			cout << names[i];
	}
	cout << endl;
}

//functoin that returns the vertex with matching name to the search string
CMyNode* CMyGraph::getvertex(string end)
{
	CMyNode* temp = NULL;
	for(int i = 0; i < V; i++)
	{
		if(nodes[i].name == end)
		{
			temp = &nodes[i];
			break;
		}
	}
	return temp;
}

//function used to calculate the required values used in dijkstra according to the parameter specified
float CMyGraph::getDistance(CMyNode n, CMyEdge e, int type, string s, string end)
{
	if(type == 0)
		return e.distance;
	else if(type == 1)
	{
		if(n.name != s && n.name != end)
	   		return n.delay;
		else
			return 0;
	}
	else if(type == 2)
		return (e.distance/e.speed);
	else if(type == 3)
	{
		if(n.name != s && n.name != end)	
			return (n.delay + (e.distance/e.speed));
		else
			return (e.distance/e.speed);
	}  
	else
	   return 1;
}


int main(int argc, char** argv)
{
	ifstream Edges, Nodes;
	Nodes.open(argv[1]);
	Edges.open(argv[2]);
	//call to constructor
	CMyGraph(Edges, Nodes);
	return 0;
}


