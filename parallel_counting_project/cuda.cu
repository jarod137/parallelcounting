/**
 * CUDA
 * Jared Kaiser
 * 10/05/2026
 *
 * changelog:
 *     10/05/2026:
 *         wrote first implementation
 *
 * description:
 *     This program counts the number of the number 3 randomly generated from an
 *     array that contains values between 0..3 and is done with multithreading
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <cuda_runtime.h>

/**
 * Pre: darray is not empty
 * Post: The number of 3s has been counted for each thread
 */
__global__ void count3(int *darray, int *dsum, int asize) {
    int myindex = threadIdx.x + blockIdx.x * blockDim.x;
    int count = 0;

    if (myindex < asize) {
        if (darray[myindex] == 3)
            count = 1; // Count occurrences of 3
    }

    // Each block will store its partial sum in dsum[blockIdx.x]
    __shared__ int shared_sum[256];

    shared_sum[threadIdx.x] = count;
    __syncthreads();

    // Sum within block
    for (int i = blockDim.x / 2; i > 0; i /= 2) {
        if (threadIdx.x < i) {
            shared_sum[threadIdx.x] += shared_sum[threadIdx.x + i];
        }
        __syncthreads();
    }

    // The result of the block is saved in global memory
    if (threadIdx.x == 0) {
        atomicAdd(dsum, shared_sum[0]);
    }
}

/**
 * Pre: N/A
 * Post: The time taken for parallel and serial are taken as well
 *       as the counts are printed
 */
int main(int argc, char *argv[]) {
    int i;
    int *gpucount;
    int pcount; // parallel count
    int length;
    int t;
    int *dsum;
    int *darray;

    struct timespec s_starttimer;
    struct timespec s_endtimer;
    struct timespec p_starttimer;
    struct timespec p_endtimer;

    int hcount = 0;
    int numblocks;
    cudaError_t err;

    switch (argc) {
        case 4:
            length = atoi(argv[1]);
            numblocks = atoi(argv[2]);
            t = atoi(argv[3]);
            break;

        default:
            fprintf(stderr, "Usage: %s array-size num-blocks num-threads\n", argv[0]);
            exit(1);
    }

    printf("Running program on array size : %d and CUDA threads %d\n", length, t);

    int *harray;
    harray = (int *)malloc(sizeof(int) * length);

    srand(time(NULL));

    for (i = 0; i < length; i++) {
        harray[i] = rand() % 4;
    }

    // Serial count
    clock_gettime(CLOCK_REALTIME, &s_starttimer);

    for (i = 0; i < length; i++) {
        if (harray[i] == 3)
            hcount++;
    }

    clock_gettime(CLOCK_REALTIME, &s_endtimer);

    // Parallel count
    clock_gettime(CLOCK_REALTIME, &p_starttimer);

    // Allocate memory on the GPU
    cudaMalloc((void **)&darray, sizeof(int) * length);
    cudaMalloc((void **)&dsum, sizeof(int));
    cudaMalloc((void **)&gpucount, sizeof(int));

    // Initialize dsum to 0
    cudaMemset(dsum, 0, sizeof(int));

    // Copy data from host to device
    err = cudaMemcpy(darray, harray, sizeof(int) * length, cudaMemcpyHostToDevice);

    if (err != cudaSuccess) {
        fprintf(stderr,
                "Failed to copy data vector from host to device (error code %s)!\n",
                cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Launch the count3 kernel
    int threadsPerBlock = 256;
    int blocksPerGrid = (length + threadsPerBlock - 1) / threadsPerBlock;

    count3<<<blocksPerGrid, threadsPerBlock>>>(darray, dsum, length);

    err = cudaGetLastError();

    if (err != cudaSuccess) {
        fprintf(stderr, "CUDA error: %s\n", cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // Copy the result from device to host
    cudaMemcpy(&pcount, dsum, sizeof(int), cudaMemcpyDeviceToHost);

    // Measure the parallel time
    clock_gettime(CLOCK_REALTIME, &p_endtimer);

    // Calculate execution time
    double serial_time =
        (s_endtimer.tv_sec - s_starttimer.tv_sec) +
        (s_endtimer.tv_nsec - s_starttimer.tv_nsec) / 1e9;

    double parallel_time =
        (p_endtimer.tv_sec - p_starttimer.tv_sec) +
        (p_endtimer.tv_nsec - p_starttimer.tv_nsec) / 1e9;

    // Print results
    printf("Time taken (serial): %f seconds\n", serial_time);
    printf("Time taken (parallel): %f seconds\n", parallel_time);
    printf("Serial Count: %d\t\tParallel Count: %d\n", hcount, pcount);

    // Clean up
    free(harray);
    cudaFree(darray);
    cudaFree(dsum);
    cudaFree(gpucount);

    return 0;
}
