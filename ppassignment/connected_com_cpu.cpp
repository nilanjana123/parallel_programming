#include <bits/stdc++.h>
using namespace std;

int conn_com(int n, vector<vector<int> >& edges){
    int con_no = 0;
    int parent[n];
	for (int i = 0; i < n; i++) {
		parent[i] = i;
	}
	for (auto x : edges) {
        // printf("%d is connected to %d\n",x[0],x[1]);
        int p = x[0]>=x[1]?x[0]:x[1];
        int c = x[0]<x[1]?x[0]:x[1];
        printf("%d is connected to %d\n",p,c);
        parent[c] = p;
	}
    for (int i = 0; i < n; i++) {
		printf("%d, ",parent[i]);
	}
	
    for (int i = 0; i < n; i++) {
        if (parent[i]==i)
		    con_no++;
	}
	
    return con_no;
}
int main()
{
	int n = 10;
	vector<int> e1 = { 0, 1 };
	vector<int> e2 = { 2, 3 };
	vector<int> e3 = { 3, 4 };
    vector<int> e4 = { 5, 6 };
	vector<int> e5 = { 6, 8 };
	vector<int> e6 = { 5, 7 };
	vector<int> e7 = { 8, 9 };
	vector<int> e8 = { 7, 8 };
    vector<vector<int> > e;
	e.push_back(e1);
	e.push_back(e2);
	e.push_back(e3);
	e.push_back(e4);
	e.push_back(e5);
	e.push_back(e6);
	e.push_back(e7);
	e.push_back(e8);
	int a = conn_com(n, e);
	cout << "total no. of connected components are: " << a
		<< endl;
	return 0;
}


/*
int conn_com(int n, vector<vector<int> >& edges){
    int con_no = 0;
    int parent[n];
	for (int i = 0; i < n; i++) {
		parent[i] = i;
	}
	for (auto x : edges) {
        // printf("%d is connected to %d\n",x[0],x[1]);
        int p = x[0]>=x[1]?x[0]:x[1];
        int c = x[0]<x[1]?x[0]:x[1];
        printf("%d is connected to %d\n",p,c);
        parent[c] = p;
	}
    for (int i = 0; i < n; i++) {
		printf("%d, ",parent[i]);
	}
	
    for (int i = 0; i < n; i++) {
        if (parent[i]==i)
		    con_no++;
	}
	
    return con_no;
}
vector<vector<int> > Graph_generator(){
	int n = 10;
	vector<int> e1 = { 0, 1 };
	vector<int> e2 = { 2, 3 };
	vector<int> e3 = { 3, 4 };
    vector<int> e4 = { 5, 6 };
	vector<int> e5 = { 6, 8 };
	vector<int> e6 = { 5, 7 };
	vector<int> e7 = { 8, 9 };
	vector<int> e8 = { 7, 8 };
    vector<vector<int> > e;
	e.push_back(e1);
	e.push_back(e2);
	e.push_back(e3);
	e.push_back(e4);
	e.push_back(e5);
	e.push_back(e6);
	e.push_back(e7);
	e.push_back(e8);
	return e;
}
*/