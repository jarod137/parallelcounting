/**
 * MPI
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
#include <mpi.h>

/**
 * Pre: N/A
 * Post: Number of 3s has been counted in both parallel and serial
 */
int main(int argc, char *argv[]) {
    int rank, size;
    int *harray;
    int *local_array;
    int local_count = 0, total_count = 0;
    int length, numthreads;
    int local_size;
    int i;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 4) {
        if (rank == 0) {
            fprintf(stderr,
                    "Usage: %s array-size num-blocks num-threads\n",
                    argv[0]);
        }

        MPI_Finalize();
        exit(1);
    }

    length = atoi(argv[1]);
    numthreads = atoi(argv[2]);

    local_size = length / size;

    // Allocate memory for the entire array on rank 0, and for the local portions
    if (rank == 0) {
        harray = (int *)malloc(sizeof(int) * length);

        srand(time(NULL));

        for (i = 0; i < length; i++) {
            harray[i] = rand() % 4; // Random numbers between 0 and 3
        }
    }

    // Allocate memory for local arrays
    local_array = (int *)malloc(sizeof(int) * local_size);

    // Scatter the array to all processes
    MPI_Scatter(harray, local_size, MPI_INT,
                local_array, local_size, MPI_INT,
                0, MPI_COMM_WORLD);

    // Start timing the parallel execution
    double parallel_start_time = MPI_Wtime();

    // Count the occurrences of 3 in the local portion of the array
    for (i = 0; i < local_size; i++) {
        if (local_array[i] == 3) {
            local_count++;
        }
    }

    // Perform the reduction to sum the counts from all processes
    MPI_Reduce(&local_count, &total_count, 1, MPI_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

    // End timing the parallel execution
    double parallel_end_time = MPI_Wtime();

    // Only rank 0 will have the final count
    if (rank == 0) {
        printf("Parallel Count: %d\n", total_count);
        printf("Time taken (parallel): %f seconds\n",
               parallel_end_time - parallel_start_time);
    }

    // Serial count for comparison
    if (rank == 0) {
        double serial_start_time = MPI_Wtime();
        int hcount = 0;

        for (i = 0; i < length; i++) {
            if (harray[i] == 3) {
                hcount++;
            }
        }

        double serial_end_time = MPI_Wtime();

        printf("Serial Count: %d\n", hcount);
        printf("Time taken (serial): %f seconds\n",
               serial_end_time - serial_start_time);
    }

    // Clean up and finalize
    if (rank == 0) {
        free(harray);
    }

    free(local_array);

    MPI_Finalize();

    return 0;
}
