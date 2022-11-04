#include <mpi.h>
#include <stdio.h>
#include<omp.h>
int main(int argc, char** argv) { 
    //initialize MPI
    MPI_Request request[2];
	MPI_Init(NULL,NULL);

    // get rank of the process .
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // if rank is 0 or master process , send data 
    int number;
    int root = 2;
    if(rank == root)
    {
        number = 10;
        printf("[MPI process %d] root sends value %d.\n", rank, number);
    }
    MPI_Bcast(&number, 1, MPI_INT, root, MPI_COMM_WORLD);
    if(rank != root)
    {
        printf("[MPI process %d] received value %d.\n", rank, number);
    }

    // finalize the MPI environment . 
    MPI_Finalize();
    return 0;
}
