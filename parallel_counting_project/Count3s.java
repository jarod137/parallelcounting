/**
 * Java
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

import java.util.Random;

public class Count3s implements Runnable {

    private static int count = 0; // Shared count for threes
    private static Object lock = new Object(); // Lock for synchronized access
    private static int[] array; // The array to count in

    private int startIndex; // Start index for the thread
    private int elements; // Number of elements to process

    /**
     * Pre: N/A
     * Post: The time for the serial and parallel implementations of count 3s will be printed
     */
    public static void main(String[] args) {

        // Check for command line arguments
        if (args.length != 2) {
            System.out.println("Usage: java CountThrees <array_size> <number_of_threads>");
            return;
        }

        int arrayLength = Integer.parseInt(args[0]);
        int maxThreads = Integer.parseInt(args[1]);

        // Ensure the number of threads does not exceed the array size
        if (maxThreads > arrayLength) {
            System.out.println("Number of threads cannot exceed array size.");
            return;
        }

        // Initialize the array
        array = new int[arrayLength];
        Random random = new Random();

        // Initialize the elements in the array with random values
        for (int i = 0; i < array.length; i++) {
            array[i] = random.nextInt(4); // Random values from 0 to 3
        }

        // Timing and counting serially
        double startTimeSerial = System.nanoTime();
        int serialCount = count3sSerial();
        double endTimeSerial = System.nanoTime();

        System.out.println("Serial count of threes: " + serialCount);
        System.out.println("Time taken (serial): " +
                (endTimeSerial - startTimeSerial) / 1_000_000_000.0 + " seconds");

        // Create and run the threads
        Thread[] threads = new Thread[maxThreads];
        int lengthPerThread = arrayLength / maxThreads;

        double startTimeParallel = System.nanoTime();

        for (int i = 0; i < maxThreads; i++) {
            // Create thread instances with start index and number of elements
            threads[i] = new Thread(new Count3s(i * lengthPerThread, lengthPerThread));
            threads[i].start();
        }

        // Wait for all threads to finish
        for (int i = 0; i < maxThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        double endTimeParallel = System.nanoTime();

        System.out.println("Parallel count of threes: " + count);
        System.out.println("Time taken (parallel): " +
                (endTimeParallel - startTimeParallel) / 1_000_000_000.0 + " seconds");
    }

    // Constructor for CountThrees
    public Count3s(int start, int elem) {
        this.startIndex = start;
        this.elements = elem;
    }

    // Method to count threes in serial
    private static int count3sSerial() {
        int serialCount = 0;

        for (int i = 0; i < array.length; i++) {
            if (array[i] == 3) {
                serialCount++;
            }
        }

        return serialCount;
    }

    @Override
    public void run() {
        int myCount = 0;

        // Count the number of threes in this segment
        for (int i = 0; i < elements; i++) {
            if (array[startIndex + i] == 3) {
                myCount++;
            }
        }

        // Synchronize access to the shared count
        synchronized (lock) {
            count += myCount;
        }
    }
}
