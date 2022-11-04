#include <mpi.h>
#include <bits/stdc++.h>
#include<omp.h>
using namespace std;

int my_mpifunc(int val[], int val_len, int no_nodes ){

    MPI_Init(NULL,NULL);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    printf("world size %d",world_size);
    int edge_per_mpi = ceil(val_len/(2.0*world_size));
    printf("edge per process %d\n",edge_per_mpi);
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
        // printf("\nValues to scatter from process %d: %d_%d, %d_%d, %d_%d, %d_%d.\n", rank, val[0], val[1], val[2], val[3], val[4], val[5], val[6], val[7]);
        printf("\nValues to scatter from process %d:",rank);
        for(int i=0; i<val_len + 2*dummy_size ;i+=2){printf(" %d-%d,",val[i],val[i+1]);}
        MPI_Scatter(val, chunk_size, MPI_INT, &sg, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    } 
    else
    {
        MPI_Scatter(NULL, chunk_size, MPI_INT, &sg, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    }

    // printf("\nProcess %d received value = %d_%d , %d_%d.", rank, sg[0],sg[1] , sg[2],sg[3]);
	   printf("\nProcess %d received value = ", rank);
	   for(int i=0; i<chunk_size ;i+=2){printf(" %d-%d,",sg[i],sg[i+1]);}

    //Apply 
    #pragma omp parallel for 
        for (int i = 0 ; i<chunk_size ; i=i+2) {
            #pragma omp critical
            {
                if(sg[i]!=-1){
                    int p = sg[i] >= sg[i+1] ? sg[i] : sg[i+1];
                    int c = sg[i] < sg[i+1] ? sg[i] : sg[i+1];
                    parent[c] = p;   
                }
            }
            }

    //print the updated value of parent array by each process 
    printf("\nupdated parent array by process %d -> ",rank);
    for (int i = 0; i < n; i++) {
		printf("%d, ",parent[i]);
	}
    
    //MPI gather parents
    int rr[n];
    MPI_Reduce(&parent, &rr, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    // MPI_Barrier(MPI_COMM_WORLD);

    if(rank == 0)
        {   
        //print the Final parent array 
        printf("\nFinal parent array in pattern : value(array index)-> ");
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
    }
    MPI_Finalize();
    return 0;
}
int main(int argc, char** argv) { 
    int no_nodes = 10;
    // int val[]={0,1 ,2,3, 3,4, 6,8, 8,9, 7,8, 6,7, 1,2};
    int val[]={0,1 ,2,3, 3,4, 5,6, 6,8, 5,7};
    int val_size = sizeof(val)/sizeof(val[0]);
    printf("val Size %d",val_size);
    int x = ceil(12.0/8.0);
    printf("::%d::", x);
    my_mpifunc(val,val_size ,no_nodes);
    return 0;
}
