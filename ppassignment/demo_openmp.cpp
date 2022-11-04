#include<bits/stdc++.h>
#include<omp.h>
// #include<mpi.h>

int main(){
    // int n = 10;
    // int sg[] = {0,1 ,2,3};
    // int parent[n];
    
	// for (int i = 0; i < n; i++) {
	// 	parent[i] = i;
	// }
	// for (int i = 0 ; i<4 ; i=i+2) {
    //     // printf("%d is connected to %d\n",x[0],x[1]);
    //     int p = sg[i] >= sg[i+1] ? sg[i] : sg[i+1];
    //     int c = sg[i] < sg[i+1] ? sg[i] : sg[i+1];
    //     // int c = x[0]<x[1]?x[0]:x[1];
    //     printf("%d is connected to %d\n",p,c);
    //     parent[c] = p;
	// }
    // for (int i = 0; i < n; i++) {
	// 	printf("%d, ",parent[i]);
	// }
    int n = 10;
    int sg[] = {0,1 ,2,3, 3,4, 5,6, 6,8, 5,7, 8,9, 7,8};
    int parent[n];
    int count = 0;
    #pragma omp parallel num_threads(n)
    {
        // printf("hello from %d\n",omp_get_thread_num());
        // count++;
        int i = omp_get_thread_num();
        parent[i] = i;
    }
    int sg_size = 16;
    #pragma omp parallel num_threads(sg_size/2)
    {   
        int i = omp_get_thread_num();
        int p = sg[i*2] >= sg[2*i+1] ? sg[2*i] : sg[2*i+1];
        int c = sg[2*i] < sg[2*i+1] ? sg[2*i] : sg[2*i+1];
        parent[c] = p;
    }
    for(int i=0;i<n;i++) 
    {
        printf("%d, ",parent[i]);
        if(parent[i]==i)
        count++;
    }    
    printf("count = %d",count)    ;
    return 0;
}