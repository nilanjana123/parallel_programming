#include <mpi.h>
#include <stdio.h>
#include<omp.h>
int sum=0;
int main(int argc, char** argv) { 
	/*
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();*/
    
    MPI_Init(NULL,NULL);
    int rank;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
// int world_size;
// MPI_Comm_size(MPI_COMM_WORLD, &world_size);
int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count,
                  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
printf("hello from %d",rank);
int number;
if (rank == 0) {
    number = 10;
    MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
} 
else if (rank == 1) {
    MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process 1 received number %d from process 0\n",
           number);
}
MPI_Finalize();
    return 0;
}
