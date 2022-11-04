#include <mpi.h>
#include <stdio.h>
#include<omp.h>
//     vector<int> e = {0,1 ,2,3, 3,4, 5,6, 6,8, 5,7, 8,9, 7,8};
/*
int update_parent(int mno , int sg[] , int n){
    printf("update function from %d machine\n", mno);
        int parent[n];
	for (int i = 0; i < n; i++) {
		parent[i] = i;
	}
	for (int i = 0 ; i<n*2 ; i=i+2) {
        // printf("%d is connected to %d\n",x[0],x[1]);
        int p = sg[i] >= sg[i+1] ? sg[i] : sg[i+1];
        int c = sg[i] < sg[i+1] ? sg[i] : sg[i+1];
        // int c = x[0]<x[1]?x[0]:x[1];
        printf("%d is connected to %d\n",p,c);
        parent[c] = p;
	}

    for (int i = 0; i < n; i++) {
		printf("%d, ",parent[i]);
	}
    return 0;
}*/

int main(int argc, char** argv) { 
    //initialize MPI
    MPI_Init(NULL,NULL);

    // get rank of the process .
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // if rank is 0 or master process , send data 
    int chunk_size = 4;
    int sg[chunk_size];
    int n = 10;
    int parent[n] ;
    for (int i = 0; i < n; i++) {
		parent[i] = i;
	}
    
    if (rank == 0) { 
        int buffer[16] = {0,1 ,2,3, 3,4, 5,6, 6,8, 5,7, 8,9, 7,8};
        printf("Values to scatter from process %d: %d, %d, %d, %d, %d, %d, %d, %d.\n", rank, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);
        MPI_Scatter(buffer, chunk_size, MPI_INT, &sg, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    } 
    else
    {
        MPI_Scatter(NULL, chunk_size, MPI_INT, &sg, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    }
    printf("\nProcess %d received value = %d %d %d %d.\n", rank, sg[0],sg[1] , sg[2],sg[3]);
	
    int count = 0;
	int tno = 0 ,p,c;
    int sg_size = 16;
    int i = 0;
    
    #pragma omp parallel for 
        for (int i = 0 ; i<chunk_size ; i=i+2) {
            // printf("%d is connected to %d\n",x[0],x[1]);
            int p = sg[i] >= sg[i+1] ? sg[i] : sg[i+1];
            int c = sg[i] < sg[i+1] ? sg[i] : sg[i+1];
            // int c = x[0]<x[1]?x[0]:x[1];
            printf("%d is connected to %d\n",p,c);
            parent[c] = p;
	        }
    
    // #pragma omp parallel num_threads(sg_size/2) private(tno,p,c) shared(sg)
    // {   
    //     #pragma omp critical
    //     {
    //         tno = omp_get_thread_num();
    //         p = sg[tno*2] > sg[2*tno+1] ? sg[2*tno] : sg[2*tno+1];
    //         c = sg[2*tno] < sg[2*tno+1] ? sg[2*tno] : sg[2*tno+1];
    //         parent[c] = p;
    //     } 
        
    // }
    //MPI gather parents
    int rr[n];
     for (int i = 0; i < n; i++) {
		printf("%d_[%d]",rank,parent[i]);
	}
    printf("\n");
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Reduce(&parent, &rr, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if(rank == 0)
    {   int count=0;
        // printf("The reduction result is \n\n%d %d %d %d %d %d %d %d %d %d.\n", rr[0],rr[1],rr[2],rr[3],rr[4],rr[5],rr[6],rr[7],rr[8],rr[9]);
        for(int i=0;i<n ;i++){
            if(rr[i] == i){
                count++;
            }
        }
        printf("No of connected components : %d\n", count);
    }
    MPI_Finalize();
    return 0;
}
