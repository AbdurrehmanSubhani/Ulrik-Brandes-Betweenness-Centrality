// Algo_Project_Part3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <iomanip>
#include <string>
#include <fstream>
#define S 6
using namespace std;

int main(){

	int** Graph = new int*[5];
	int Size = 0;

	ifstream read;
	read.open("facebook-combined.txt");

	if (read) {

		read.seekg(-1, ios::end);
		read.seekg(-5, ios::cur);

		// As the graph edges are sorted from 0 - Max no of nodes
		string Max_Size;
		getline(read, Max_Size,',');

		Size = stoi(Max_Size) + 1;

		cout << Size << endl;
		delete[] Graph;
		Graph = new int* [Size];

		for (int i = 0; i < Size; i++)
			Graph[i] = new int[Size];

		for (int i = 0; i < Size; i++)
			for (int j = 0; j < Size; j++)
				Graph[i][j] = 0;

		read.seekg(0, ios::beg);

		int count = 0;
		while (count < 28) {
			string Vertice1, Vertice2;
			getline(read, Vertice1, ',');
			getline(read, Vertice2);
			if (Vertice1 == "" && Vertice2 == "")
				break;

			int Node1 = stoi(Vertice1);
			int Node2 = stoi(Vertice2);

			//cout << "Node1: " << Node1<<" "<< "Node2: " << Node2 << endl;

			Graph[Node1][Node2] = 1;
			count++;
		}

		for (int i = 0; i < Size; i++) {
			for (int j = 0; j < Size;j++)
				cout << Graph[i][j] << " ";
			cout << endl;
		}

		read.close();
	}
	else {
		cout << "Error reading file" << endl;

	}

	double* Betweennes_Centrality= new double[Size];
	
	for (int i = 0; i < Size; i++)
		Betweennes_Centrality[i] = 0.0;

	for (int s = 0 ;  s < Size   ; s++) {
		
		
		stack<int> Stack;
		vector< vector<int> > Predecessors(Size);
		int* sigma= new int[Size];
		int* distance = new int[Size];

		for (int i = 0; i < Size; i++) {
			sigma[i] = 0; distance[i] = -1;
		}

		sigma[s] = 1; 
		distance[s] = 0;

		queue<int> Queue;
		Queue.push(s);

		while (!Queue.empty()) {
			int v = Queue.front();
			Queue.pop();
			Stack.push(v);

			for (int Neighbour = 0 ; Neighbour < Size; Neighbour++) {
				if (Graph[v][Neighbour] == 1) {
					
					if (distance[Neighbour] < 0) {
						Queue.push(Neighbour);
						distance[Neighbour] = distance[v] + 1;
					}

					if (distance[Neighbour] == (distance[v] + 1)) {
						sigma[Neighbour] = sigma[Neighbour] + sigma[v];
						Predecessors[Neighbour].push_back(v);
					}

				}
				else
					continue;
			}
		}

		double* Pair_Dependecy= new double[Size];
		
		for (int i = 0; i < Size; i++)
			Pair_Dependecy[i] = 0.0;

		while (!Stack.empty()) {
			int W = Stack.top();
			Stack.pop();

			for (int i = 0 , V = 0; i<Predecessors[W].size() ; i++) {
				V = Predecessors[W][i];
				Pair_Dependecy[V] = Pair_Dependecy[V] + (sigma[V] * 1.0 / sigma[W]) * (1 + Pair_Dependecy[W]) ;
			}
			if (W != s) {
				Betweennes_Centrality[W] = Betweennes_Centrality[W] + Pair_Dependecy[W];
			}
		}

	}

	for (int i = 0; i < Size; i++)
		cout << "i: "<<i+ 1<<" "<<Betweennes_Centrality[i] / 2 << endl;

	fstream Write;
	Write.open("Betweenness_Centrality.txt", ios::out);

	for (int i = 0; i < Size; i++)
		Write << "Node " << i << " : " << Betweennes_Centrality[i] / 2 << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
