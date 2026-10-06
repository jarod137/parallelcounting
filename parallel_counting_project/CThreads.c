/**
 * C – Threads
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
#include <pthread.h>

int *harray;
int *local_count;
int length;

typedef struct {
    int start_index;
    int end_index;
    int thread_id;
} thread_data_t;

// Function for counting 3's in a portion of the array
// Pre: arg is a positive integer correlating to thread
// Post: Number of threes has been counted in parallel
void *count3(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int count = 0;

    // Count the occurrences of 3 in the given range
    for (int i = data->start_index; i < data->end_index; i++) {
        if (harray[i] == 3) {
            count++;
        }
    }

    // Store the result in the global array
    local_count[data->thread_id] = count;

    pthread_exit(NULL);
}

/**
 * Pre: N/A
 * Post: printed the number of 3s counted in parallel and serial
 *       and the time it took for the program to run.
 */
int main(int argc, char *argv[]) {
    int num_threads;
    struct timespec s_starttimer, s_endtimer;
    struct timespec p_starttimer, p_endtimer;
    int hcount = 0;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s array-size num-threads\n", argv[0]);
        exit(1);
    }

    length = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    // Allocate memory for the array and initialize it with random values
    harray = (int *)malloc(sizeof(int) * length);
    local_count = (int *)malloc(sizeof(int) * num_threads);

    srand(time(NULL));

    for (int i = 0; i < length; i++) {
        harray[i] = rand() % 4; // Random numbers between 0 and 3
    }

    // Serial count for comparison
    clock_gettime(CLOCK_REALTIME, &s_starttimer);

    for (int i = 0; i < length; i++) {
        if (harray[i] == 3) {
            hcount++;
        }
    }

    clock_gettime(CLOCK_REALTIME, &s_endtimer);

    printf("Serial Count: %d\n", hcount);

    // Parallel counting using threads
    clock_gettime(CLOCK_REALTIME, &p_starttimer);

    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];

    int chunk_size = length / num_threads;

    // Create threads to process portions of the array
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].start_index = i * chunk_size;
        thread_data[i].end_index =
            (i == num_threads - 1) ? length : (i + 1) * chunk_size;
        thread_data[i].thread_id = i;

        pthread_create(&threads[i], NULL, count3, (void *)&thread_data[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Sum up the results from each thread
    int pcount = 0;

    for (int i = 0; i < num_threads; i++) {
        pcount += local_count[i];
    }

    // End timing the parallel execution
    clock_gettime(CLOCK_REALTIME, &p_endtimer);

    printf("Parallel Count: %d\n", pcount);

    // Output the time taken for serial and parallel versions
    double serial_time =
        (s_endtimer.tv_sec - s_starttimer.tv_sec) +
        (s_endtimer.tv_nsec - s_starttimer.tv_nsec) / 1e9;

    double parallel_time =
        (p_endtimer.tv_sec - p_starttimer.tv_sec) +
        (p_endtimer.tv_nsec - p_starttimer.tv_nsec) / 1e9;

    printf("Time taken (serial): %f seconds\n", serial_time);
    printf("Time taken (parallel): %f seconds\n", parallel_time);

    // Clean up
    free(harray);
    free(local_count);

    return 0;
}
