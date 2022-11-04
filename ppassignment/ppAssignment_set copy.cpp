#include <mpi.h>
#include <bits/stdc++.h>
#include<omp.h>
using namespace std;

int my_mpifunc(int val[], int val_len, int no_nodes, int testcase_no ){

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // printf("world size %d",world_size);
    int edge_per_mpi = ceil(val_len/(2.0*world_size));
    // printf("edge per process %d\n",edge_per_mpi);
    int chunk_size = edge_per_mpi*2;
    int sg[chunk_size];
    int n = no_nodes;
    int count = 0;
    int parent[n] ;
    int node_rank[n];
    for (int i = 0; i < n; i++) {
		parent[i] = i;
        node_rank[i] = 0;
	}
    int dummy_size = world_size * edge_per_mpi - (val_len/2);
    for(int i=0; i<dummy_size; i++){
        val[val_len + 2*i] = -1;
        val[val_len + 2*i +1] = -1;
    }
    int cc = no_nodes;
    // if rank is 0 or master process , send data 
    if (rank == 0) {
        printf("Testcase %d : \n" , testcase_no);
        for(int i=0;i<15;i++)printf("*"); printf("\n");
    
        // printf("world size %d\n",world_size);
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
    int p = -1 , c = -1;
    #pragma omp parallel for private(p,c)
        for (int i = 0 ; i<chunk_size ; i=i+2) {
                if(sg[i]!=-1){
                    int p = sg[i] >= sg[i+1] ? sg[i] : sg[i+1];
                    int c = sg[i] < sg[i+1] ? sg[i] : sg[i+1];
                    while (parent[p] != p) {
                        p = parent[p];
                    }
                    while (parent[c] != c) {
                        c = parent[c];
                    }
                    // parent[c] = p;
                    // if (pi == pj) {
                    //     return 0;
                    // }
                    if (p!=c){
                        if (node_rank[p] > node_rank[c]) {
                            parent[c] = p;
                        }
                        else if (node_rank[p] < node_rank[c]) {
                            parent[p] = c;
                        }
                        else {
                        parent[c] = p;
                        node_rank[p]++;
                        }
                        #pragma omp critical
                        {
                            cc--;
                        }
                        // cc--;
                    }
                    
            }
            }

    //print the updated value of parent array by each process 
    printf("\nupdated parent array by process %d -> ",rank);
    for (int i = 0; i < n; i++) {
		printf("%d, ",parent[i]);
	}
    printf("\n\nNo of connected components : %d\n", cc);
        for(int i=0;i<50;i++)printf("_"); printf("\n\n");

    /*
    //MPI gather parents
    int rr[n];
    MPI_Reduce(&parent, &rr, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    // MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0)
        {   
        //print the Final parent array 
        printf("\n\nFinal parent array in pattern : value(array index)-> ");
        int count=0;
        for(int i=0;i<n ;i++){
            if(rr[i] == i){
                printf("[local root %d(%d)], ",rr[i],i);
                count++;
            }
            else{
            printf("%d(%d), ",rr[i],i);
            }
        }
        printf("\n\nNo of connected components : %d\n", count);
        for(int i=0;i<50;i++)printf("_"); printf("\n\n");

    }*/
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
    /*
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
    int edge_list_5[]={0,1 ,2,3, 3,4, 5,6, 6,8, 5,7};
    edge_list_size = sizeof(edge_list_5)/sizeof(edge_list_5[0]);
    no_nodes = 10;
    my_mpifunc(edge_list_5, edge_list_size, no_nodes, testcase_no);
    
    //Testcase 6
    testcase_no = 6;
    int edge_list_6[]={0,1 ,2,3, 3,4, 5,6, 6,8, 5,7};
    edge_list_size = sizeof(edge_list_6)/sizeof(edge_list_6[0]);
    no_nodes = 10;
    my_mpifunc(edge_list_6, edge_list_size, no_nodes, testcase_no);
    
    //Testcase 7
    testcase_no = 7;
    int edge_list_7[]={0,1 ,2,3, 3,4, 5,6, 6,8, 5,7};
    edge_list_size = sizeof(edge_list_7)/sizeof(edge_list_7[0]);
    no_nodes = 10;
    my_mpifunc(edge_list_7, edge_list_size, no_nodes, testcase_no);
    
    //Testcase 8
    testcase_no = 8;
    int edge_list_8[]={0,1 ,2,3, 3,4, 5,6, 6,8, 5,7};
    edge_list_size = sizeof(edge_list_8)/sizeof(edge_list_8[0]);
    no_nodes = 10;
    my_mpifunc(edge_list_8, edge_list_size, no_nodes, testcase_no);
    */
    MPI_Finalize();

    return 0;
}
