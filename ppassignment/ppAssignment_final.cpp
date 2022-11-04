#include <mpi.h>
#include <stdio.h>
#include<omp.h>

int main(int argc, char** argv) { 
    //initialize MPI
    MPI_Init(NULL,NULL);

    // get rank of the process .
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunk_size = 4;
    int sg[chunk_size];
    int n = 10;
    int count = 0;
    int parent[n] ;
    for (int i = 0; i < n; i++) {
		parent[i] = i;
	}
    
    // if rank is 0 or master process , send data 
    if (rank == 0) { 
        int buffer[16] = {0,1 ,2,3, 3,4, 5,6, 6,8, 5,7, 8,9, 7,8};
        printf("\nValues to scatter from process %d: %d_%d, %d_%d, %d_%d, %d_%d.\n", rank, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);
        MPI_Scatter(buffer, chunk_size, MPI_INT, &sg, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    } 
    else
    {
        MPI_Scatter(NULL, chunk_size, MPI_INT, &sg, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    }

    printf("\nProcess %d received value = %d_%d , %d_%d.", rank, sg[0],sg[1] , sg[2],sg[3]);
	
    //Apply 
    #pragma omp parallel for 
        for (int i = 0 ; i<chunk_size ; i=i+2) {
            int p = sg[i] >= sg[i+1] ? sg[i] : sg[i+1];
            int c = sg[i] < sg[i+1] ? sg[i] : sg[i+1];
            parent[c] = p;
	        }

    //print the updated value of parent array by each process 
    printf("\nupdated parent array by process %d -> ",rank);
    for (int i = 0; i < n; i++) {
		printf("%d, ",parent[i]);
	}
    
    //MPI gather parents
    int rr[n];
    MPI_Reduce(&parent, &rr, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if(rank == 0)
        {   
        //print the Final parent array 
        printf("\nFinal parent array %d -> ",rank);
        int count=0;
        for(int i=0;i<n ;i++){
            printf("%d, ",parent[i]);
            if(rr[i] == i){
                count++;
            }
        }
        printf("\n\nNo of connected components : %d\n", count);
    }
    MPI_Finalize();
    return 0;
}
