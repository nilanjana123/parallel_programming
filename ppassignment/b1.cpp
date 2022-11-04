#include <bits/stdc++.h>
using namespace std;

int parent[10];
int r[10];

int find1(int i) {
    while (parent[i] != i) {
        i = parent[i];
    } 
    return i;
}

int Union(int i, int j) {
    int pi = find1(i);
    int pj = find1(j);
    
    if (pi == pj) {
        return 0;
    }
    
    if (r[pi] > r[pj]) {
        parent[pj] = pi;
    }
    else if (r[pi] < r[pj]) {
        parent[pi] = pj;
    }
    else {
        parent[pj] = pi;
        r[pi]++;
    }
    // group--;
    return 0;
}
int conn_com(int n, vector<vector<int> >& edges){
    int con_no = 0;
	for (int i = 0; i < n; i++) {
		parent[i] = i;
	}
	for (auto x : edges) {
        Union(x[0],x[1]);
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

