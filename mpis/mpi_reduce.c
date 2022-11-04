#include <mpi.h>
#include <stdio.h>
#include<omp.h>
int main(int argc, char** argv) { 
    //initialize MPI
    MPI_Init(NULL,NULL);

    // get rank of the process .
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // if rank is 0 or master process , send data 
    int number[2];
    if (rank == 0) { 
        int buffer[8] = { 0, 1 , 2 , 3 , 4 , 5 , 6 , 7 };
        printf("Values to scatter from process %d: %d, %d, %d, %d, %d, %d, %d, %d.\n", rank, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);
        MPI_Scatter(buffer, 2, MPI_INT, &number, 2, MPI_INT, 0, MPI_COMM_WORLD);
    } 
    else
    {
        MPI_Scatter(NULL, 2, MPI_INT, &number, 2, MPI_INT, 0, MPI_COMM_WORLD);
    }
    printf("Process %d received value = %d and %d.\n", rank, number[0],number[1]);
    
    int processed1 = (number[0] + number[1]) * rank ;
    int processed2 = (number[0] * number[1]) + rank ;
    int reduction_result1 = 0;
    MPI_Reduce(&processed1, &reduction_result1, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
 
    if(rank == 0)
    {
        printf("The reduction result is %d.\n", reduction_result1);
    }
    int reduction_result2 = 0;
    MPI_Allreduce(&processed2, &reduction_result2, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
 
    printf("[MPI Process %d] The result is %d.\n", rank, reduction_result2);
 // finalize the MPI environment . 
    MPI_Finalize();
    return 0;
}
