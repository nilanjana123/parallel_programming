#include <mpi.h>
#include <stdio.h>
#include<omp.h>
//     vector<int> e = {0,1 ,2,3, 3,4, 5,6, 6,8, 5,7, 8,9, 7,8};

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
    int processed2 = number[0] * number[1] + rank ;
    if (rank == 3){
        int buffer[4];
        MPI_Gather(&processed1, 1, MPI_INT, buffer, 1, MPI_INT, 3, MPI_COMM_WORLD);
        printf("Gather : Values collected on process %d: %d, %d, %d, %d.\n", rank, buffer[0], buffer[1], buffer[2], buffer[3]);
    }
    else
    {
        MPI_Gather(&processed1, 1, MPI_INT, NULL, 0, MPI_INT, 3, MPI_COMM_WORLD);
    }

    int buffer[4];
    MPI_Allgather(&processed2, 1, MPI_INT, buffer, 1, MPI_INT, MPI_COMM_WORLD);
    printf("ALLGATHER : Values collected on process %d: %d, %d, %d, %d.\n", rank, buffer[0], buffer[1], buffer[2] ,buffer[3]);
    // finalize the MPI environment . 
    MPI_Finalize();
    return 0;
}
