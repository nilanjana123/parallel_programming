#include <mpi.h>
#include <bits/stdc++.h>
#include<omp.h>
using namespace std;

int my_mpifunc(int val[], int val_len, int no_nodes, int testcase_no ){

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int edge_per_mpi = ceil(val_len/(2.0*world_size));
    int chunk_size = edge_per_mpi*2;
    int sg[chunk_size];
    int n = no_nodes;
    int count = 0;
    int parent[n] ;
    for (int i = 0; i < n; i++) {
		parent[i] = i;
	}
    int dummy_size = world_size * edge_per_mpi - (val_len/2);
    for(int i=0; i<dummy_size; i++){
        val[val_len + 2*i] = -1;
        val[val_len + 2*i +1] = -1;
    }
    // if rank is 0 or master process , send data 
    if (rank == 0) {
        printf("Testcase %d : \n" , testcase_no);
        for(int i=0;i<15;i++)printf("*"); printf("\n");
    
        printf("edge per process %d\n",edge_per_mpi);
        printf("Values to scatter from process %d:",rank);
        for(int i=0; i<val_len + 2*dummy_size ;i+=2){printf(" %d-%d,",val[i],val[i+1]);}
        printf("\n");
        MPI_Scatter(val, chunk_size, MPI_INT, &sg, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    } 
    else
    {
        MPI_Scatter(NULL, chunk_size, MPI_INT, &sg, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    }

	   printf("\nProcess %d received value = ", rank);
	   for(int i=0; i<chunk_size ;i+=2){printf(" %d-%d,",sg[i],sg[i+1]);}

    //Apply 
    int pn1 = -1 , pn2 = -1, n1=-1, n2=-1;
    #pragma omp parallel for private(n1,n2,pn1,pn2)
        for (int i = 0 ; i<chunk_size ; i=i+2) {
                if(sg[i]!=-1){
                    int n1 = sg[i] >= sg[i+1] ? sg[i] : sg[i+1];
                    int n2 = sg[i] < sg[i+1] ? sg[i] : sg[i+1];
                    // parent[n2] = n1; 
                    int pn1 = parent[n1];
                    int pn2 = parent[n2];
                    
                    if (pn1 >= pn2){
                        parent[pn2] = pn1;
                        parent[n2] = pn1;
                    }
                    if (pn1 < pn2){
                        parent[pn1] = pn2;
                        parent[n1] = pn2;
                    }
            }
            }

    //print the updated value of parent array by each process 
    printf("\nupdated parent array by process %d -> ",rank);
    for (int i = 0; i < n; i++) {
		printf("%d, ",parent[i]);
	}
    
    //MPI gather parents
    int rr[world_size*n];
    // MPI_Reduce(&parent, &rr, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0){
        MPI_Gather(&parent, n, MPI_INT, rr, n, MPI_INT, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Gather(&parent, n, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
    }
    int final_Parent[n];
    for(int i=0;i<n;i++)final_Parent[i]=i;
    MPI_Reduce(&parent, &final_Parent, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0){
        for(int i=0 ;i<world_size ;i++){
            for(int j=0; j<n ;j++){
                int prev = rr[j];
                int curr = rr[n*i+j];
                if(curr > prev){
                    for (int k = 0; k<n ; k++){
                        if (rr[k] == prev) rr[k] = curr;
                    }
                    // final_Parent[prev] = curr;
                    // final_Parent[j] = curr;
                    // rr[j] = curr;
                }
                else{
                    for(int k=0; k<n;k++){
                        if(rr[k] == curr) rr[k] = prev;
                    }
                }
            }
        }
        for (int i=0;i<n;i++)final_Parent[i]=rr[i];
        printf("\n\nFinal parent array in root(node) pattern : ");
        for(int i=0;i<n ;i++){
            if (final_Parent[i] == i){
                printf("[localRoot %d(%d)], ", final_Parent[i],i);
                count++;
            }
            else{
                printf("%d(%d), ",final_Parent[i],i);
            }
        }
        printf("\n\nNo of connected components : %d\n", count);
        for(int i=0;i<100;i++)printf("_"); printf("\n\n");
        printf("\n");
         
    }
    return 0;
}
int main(int argc, char** argv) { 
    int no_nodes = 0;
    int edge_list_size = 0;
    int testcase_no = 0;

    MPI_Init(NULL,NULL);

    //Testcase1
    testcase_no =1;
    int edge_list_1[]={0,1, 2,0, 3,4};
    edge_list_size = sizeof(edge_list_1)/sizeof(edge_list_1[0]);
    no_nodes = 5;
    my_mpifunc(edge_list_1, edge_list_size, no_nodes, testcase_no);
    
    //Testcase 2
    testcase_no = 2;
    int edge_list_2[]={0,1 ,2,3, 3,4};
    edge_list_size = sizeof(edge_list_2)/sizeof(edge_list_2[0]);
    no_nodes = 5;
    my_mpifunc(edge_list_2, edge_list_size, no_nodes, testcase_no);
    
    //Testcase 3
    testcase_no = 3;
    int edge_list_3[]={0,1 ,2,3, 3,4, 4,1};
    edge_list_size = sizeof(edge_list_3)/sizeof(edge_list_3[0]);
    no_nodes = 5;
    my_mpifunc(edge_list_3, edge_list_size, no_nodes, testcase_no);
    
    //Testcase 4
    testcase_no = 4;
    int edge_list_4[]={0,1 ,2,3, 3,4, 5,6, 6,8, 5,7};
    edge_list_size = sizeof(edge_list_4)/sizeof(edge_list_4[0]);
    no_nodes = 10;
    my_mpifunc(edge_list_4, edge_list_size, no_nodes, testcase_no);
    
    //Testcase 5
    testcase_no = 5;
    int edge_list_5[]={0,1 ,2,3, 3,4, 5,6, 6,8, 5,7, 8,9, 1,4};
    edge_list_size = sizeof(edge_list_5)/sizeof(edge_list_5[0]);
    no_nodes = 10;
    my_mpifunc(edge_list_5, edge_list_size, no_nodes, testcase_no);
    
    //Testcase 6
    testcase_no = 6;
    int edge_list_6[]={0,1 ,2,3, 3,4, 5,6, 6,8, 5,7, 6,4, 4,9};
    edge_list_size = sizeof(edge_list_6)/sizeof(edge_list_6[0]);
    no_nodes = 10;
    my_mpifunc(edge_list_6, edge_list_size, no_nodes, testcase_no);
    
    //Testcase 7
    testcase_no = 7;
    int edge_list_7[]={ 0,9, 1,9, 1,2, 8,9, 3,0, 5,8, 8,6, 4,7, 6,4};
    edge_list_size = sizeof(edge_list_7)/sizeof(edge_list_7[0]);
    no_nodes = 10;
    my_mpifunc(edge_list_7, edge_list_size, no_nodes, testcase_no);
    
    //Testcase 8
    testcase_no = 8;
    int edge_list_8[]={ 0,9, 1,9, 1,2, 3,0, 5,8, 8,6};
    edge_list_size = sizeof(edge_list_8)/sizeof(edge_list_8[0]);
    no_nodes = 10;
    my_mpifunc(edge_list_8, edge_list_size, no_nodes, testcase_no);

    MPI_Finalize();

    return 0;
}
