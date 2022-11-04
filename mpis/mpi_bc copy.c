#include <stdio.h>
#include <stdlib.h>

__device__ int fun(int tid){
    if(tid==10)printf("tid=10 %d, %d, %d\n" , blockIdx.x, blockDim.x, threadIdx.x);
    if(tid==10)printf("tid=10 %d, %d, %d\n" , blockIdx.x, blockDim.x, threadIdx.x);
    return 1;
}

__global__ void matadd(int *a ,int *b int *c){
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    c[tid] = a[tid] + b[tid];
    int val = fun(tid);
}

int main(){
    int *a , *b, *c; //variable for host memory
    int *d_a, *d_b, *d_c;  // variable for device memory

    //memory allocation of a , b, c on host memory
    a= (int *)malloc(sizeof(int) * 1024);
    b= (int *)malloc(sizeof(int) * 1024);
    c= (int *)malloc(sizeof(int) * 1024);
    
    //allocate memory on device memory

    if(cudaMalloc( (void ** )&d_a , sizeof(int) * 1024) != cudaSuccess ) printf("error in allocating d_a .");
    if(cudaMalloc( (void ** )&d_a , sizeof(int) * 1024) != cudaSuccess ) printf("error in allocating d_b .");
    if(cudaMalloc( (void ** )&d_a , sizeof(int) * 1024) != cudaSuccess ) printf("error in allocating d_c .");

    //initialze host array a and b
    for (int i=0 ; i< 1024 ; i++)
        a[i] = b[i] = i;

    //copy data from host to device 
    if(cudaMemcpy(d_a , a, sizeof(int) * 1024, cudaMemcpyHostToDevice) != cudaSuccess) printf("Memcpy failed d_a");
    if(cudaMemcpy(d_b , b, sizeof(int) * 1024, cudaMemcpyHostToDevice) != cudaSuccess) printf("Memcpy failed d_b");

    //matrix add
    matadd<<<2,512>>>(d_a,d_b,d_C);

    
    if(cudaMemcpy(c , d_c, sizeof(int) * 1024, cudaMemcpyDeviceToHost) != cudaSuccess) printf("Memcpy failed d_c");

    for(int i=0 ; i<1024 ; i++)
    if(c[i] != 2*i ) printf("Error in result\n" );

    printf("i= %d c[i] = %d", 1023, c[1023]);

    return 0;
}