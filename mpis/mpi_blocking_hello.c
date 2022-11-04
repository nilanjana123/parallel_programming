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
    if (rank == 0) {
        number = 10;
        MPI_Isend(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD , request);
    } 
    // if rank 1 receive data 
    else if (rank == 1) {
        MPI_Irecv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
                 request);
        printf("Before MPI_wait statement : Process 1 received %d (random garbage value)from process 0\n",
            number);
        MPI_Wait(request , MPI_STATUS_IGNORE);
        printf("After MPI_WAIT statement : Process 1 received %d from process 0\n",
            number);
    }
    // finalize the MPI environment . 
    MPI_Finalize();
    return 0;
}
