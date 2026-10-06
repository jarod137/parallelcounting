/**
 * OpenMP
 * Jared Kaiser
 * 10/05/2026
 *
 * changelog:
 *    10/05/2026:
 *         wrote first implementation
 *
 * description:
 *     This program counts the number of the number 3 randomly generated from an
 *     array that contains values between 0..3 and is done with multithreading.
 *
 *     In this implementation we are testing whether java or c with OpenMP is faster
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/**
 * Pre: an array has been created and the array and its size are passed into the function
 * Post: the amount of 3s that appear in the array are counted
 */
int count3s_serial(int *array, int length) {
    int count = 0;

    // counts the number of 3s
    for (int i = 0; i < length; i++) {
        if (array[i] == 3) {
            count++;
        }
    }

    return count;
}

/**
 * Pre: an array has been created and the array and its size are passed into the function
 * Post: the amount of 3s that appear in the array are counted
 */
int count3s_parallel(int *array, int length) {
    int count = 0;

    #pragma omp parallel shared(array, count, length)
    {
        int count_p = 0;

        #pragma omp for
        // counts the number of 3s
        for (int i = 0; i < length; i++) {
            if (array[i] == 3) {
                count_p++;
            }
        }

        #pragma omp critical
        {
            count += count_p;
        }
    }

    return count;
}

/**
 * Pre: N/A
 * Post: The time for the serial and parallel implementations of count 3s will be printed
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <array_size>\n", argv[0]);
        return 1;
    }

    int array_size = atoi(argv[1]);

    // Allocate and initialize the array with random values between 0 and 3
    int *array = (int *)malloc(array_size * sizeof(int));

    for (int i = 0; i < array_size; i++) {
        array[i] = rand() % 4;
    }

    // Timing the serial version
    double start_time_serial = omp_get_wtime();
    int count_serial = count3s_serial(array, array_size);
    double end_time_serial = omp_get_wtime();

    // Print the results for the serial version
    printf("Serial count of threes: %d\n", count_serial);
    printf("Time taken (serial): %f seconds\n",
           end_time_serial - start_time_serial);

    // Timing the parallel version
    double start_time_parallel = omp_get_wtime();
    int count_parallel = count3s_parallel(array, array_size);
    double end_time_parallel = omp_get_wtime();

    // Print the results for the parallel version
    printf("Parallel count of threes: %d\n", count_parallel);
    printf("Time taken (parallel): %f seconds\n",
           end_time_parallel - start_time_parallel);

    free(array);

    return 0;
}
