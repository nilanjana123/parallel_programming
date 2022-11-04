// // C++ program to print DFS traversal from
// // a given vertex in a given graph
// #include <bits/stdc++.h>
// using namespace std;

// // Graph class represents a directed graph
// // using adjacency list representation
// class Graph {
// public:
// 	map<int, bool> visited;
// 	map<int, list<int> > adj;

// 	// function to add an edge to graph
// 	void addEdge(int v, int w);

// 	// DFS traversal of the vertices
// 	// reachable from v
// 	void DFS(int v);
// };

// void Graph::addEdge(int v, int w)
// {
// 	adj[v].push_back(w); // Add w to v’s list.
// }

// void Graph::DFS(int v)
// {
// 	// Mark the current node as visited and
// 	// print it
// 	visited[v] = true;
// 	cout << v << " ";

// 	// Recur for all the vertices adjacent
// 	// to this vertex
// 	list<int>::iterator i;
// 	for (i = adj[v].begin(); i != adj[v].end(); ++i)
// 		if (!visited[*i])
// 			DFS(*i);
// }

// // Driver code
// int main()
// {
// 	// Create a graph given in the above diagram
// 	Graph g;
// 	g.addEdge(0, 1);
// 	g.addEdge(0, 2);
// 	g.addEdge(1, 2);
// 	g.addEdge(2, 0);
// 	g.addEdge(2, 3);
// 	g.addEdge(3, 3);

// 	cout << "Following is Depth First Traversal"
// 			" (starting from vertex 2) \n";
// 	g.DFS(2);

// 	return 0;
// }

// // improved by Vishnudev C
#include<stdio.h>
 

int G[10][10],visited[10],n;    //n is no of vertices and graph is sorted in array G[10][10]

int DFS(int i)
{
    int j;
    printf("\n%d",i);
        visited[i]=1;
    for(j=0;j<n;j++)
        if(!visited[j]&&G[i][j]==1)
                DFS(j);
    return 0;
}

int main()
{
    int edge_list[]={0,1 ,2,3, 3,4};
    int e = sizeof(edge_list)/sizeof(edge_list[0]);
    for(int i=0;i<e;i+=2){
        int x,y = edge_list[i],edge_list[i+1];
        G[x][y] = 1;
        G[y][x] = 1;
    }

    for (int i = 0; i < 10; i++)
    {
        for(int j = 0; j< 10 ;j++){
            printf("%d ",G[i][j]);
        } printf("\n");
    }
    
    //visited is initialized to zero
   for(int i=0;i<n;i++)
        visited[i]=0;
    DFS(0);
}