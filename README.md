# Parallel Counting Performance Comparison

A comparative performance study of different approaches to parallelizing a simple counting problem. This project counts the number of occurrences of the value `3` in a randomly generated array and compares the execution time of several implementations across Java and C-based parallel programming models.

The project was created to investigate how different parallelization techniques perform when solving the same computational problem.

## Overview

Each implementation generates an array containing randomly generated values and counts how many elements are equal to `3`.

The same basic problem is implemented using several approaches:

* **Java Threads**
* **POSIX Threads (pthreads)**
* **OpenMP**
* **MPI**
* **CUDA**
* **Serial execution** for comparison

The primary goal is to compare the performance of Java-based and C-based approaches and examine the effects of different parallel programming models.

## Implementations

| Implementation        | Language   | Parallel Model  |
| --------------------- | ---------- | --------------- |
| `Count3s.java`        | Java       | Java Threads    |
| `CThreads.c`          | C          | POSIX Threads   |
| `OpenMP.c`            | C          | OpenMP          |
| `MPI.c`               | C          | MPI             |
| `cuda.cu`             | CUDA C/C++ | CUDA            |
| Serial implementation | C/Java     | Single-threaded |

Each implementation performs the same fundamental task so that their execution times can be compared.

## Project Structure

```text
.
├── src/
│   ├── Count3s.java
│   ├── CThreads.c
│   ├── OpenMP.c
│   ├── MPI.c
│   └── cuda.cu
├── build/
├── Makefile
└── README.md
```

## Requirements

Depending on which implementation you want to run, you may need:

* Java JDK
* GCC
* POSIX Threads
* OpenMP
* MPI implementation such as OpenMPI or MPICH
* NVIDIA CUDA Toolkit and `nvcc` for the CUDA implementation
* A CUDA-capable NVIDIA GPU for GPU execution

Not every implementation requires all of these dependencies.

## Building

The project includes a `Makefile` to simplify compilation.

```bash
make
```

Compiled programs are placed in the `build/` directory.

To remove compiled executables:

```bash
make clean
```

### Individual Implementations

#### Java

Compile the Java implementation with:

```bash
javac src/Count3s.java -d build
```

Run it with:

```bash
java -cp build Count3s
```

#### POSIX Threads

Compile with:

```bash
gcc -O3 -pthread src/CThreads.c -o build/cthreads
```

Run with:

```bash
./build/cthreads
```

#### OpenMP

Compile with:

```bash
gcc -O3 -fopenmp src/OpenMP.c -o build/openmp
```

Run with:

```bash
./build/openmp
```

#### MPI

Compile with:

```bash
mpicc -O3 src/MPI.c -o build/mpi
```

Run with:

```bash
mpirun -np 4 ./build/mpi
```

The number after `-np` controls the number of MPI processes.

#### CUDA

The CUDA implementation requires the NVIDIA CUDA Toolkit and `nvcc`.

Compile with:

```bash
nvcc -O3 src/cuda.cu -o build/cuda
```

Run with:

```bash
./build/cuda
```

> **Note:** The CUDA implementation cannot be compiled unless `nvcc` is installed and available in the system's PATH.

## How It Works

The programs follow the same general process:

1. Generate an array of random values.
2. Search the array for elements equal to `3`.
3. Count the matching elements.
4. Report the resulting count.
5. Measure the execution time.

Parallel implementations divide the work among multiple threads, processes, or GPU threads.

Conceptually, the operation is:

```text
Array:
[1, 3, 0, 2, 3, 1, 3, 0]

Result:
Number of 3s = 3
```

The computation itself is intentionally simple. This allows the project to focus on the performance characteristics and overhead associated with each parallel programming model.

## Performance Comparison

The implementations can be compared using measurements such as:

* Execution time
* Parallel speedup
* Number of threads/processes
* Input size
* Scalability
* Parallelization overhead

For a given input size, the speedup of a parallel implementation can be calculated as:

```text
Speedup = Serial Execution Time / Parallel Execution Time
```

A higher speedup indicates that the parallel implementation completed the computation faster relative to the serial version.

## What This Project Demonstrates

This project provides a practical comparison of several approaches to parallel programming.

In particular, it demonstrates:

* Java multithreading
* POSIX thread programming
* OpenMP parallel programming
* MPI distributed-memory programming
* CUDA GPU programming
* Parallel workload distribution
* Synchronization and shared data
* Performance measurement and benchmarking
* Comparing different programming models for the same algorithm

Because the underlying algorithm is simple, differences in performance can help illustrate the overhead and characteristics of each parallel programming approach.

## Expected Results

The fastest implementation is not necessarily the same for every system or input size.

Performance can depend on:

* CPU architecture
* Number of CPU cores
* GPU hardware
* Number of threads/processes
* Array size
* Compiler optimizations
* Thread/process creation overhead
* Memory bandwidth
* Operating system and runtime environment

For relatively small inputs, the overhead of creating and managing parallel workers may outweigh the benefits of parallel execution. Larger inputs generally provide more work for parallel implementations to distribute.

## Correctness

Performance comparisons are only meaningful if each implementation produces the same result.

Each implementation should therefore be checked to ensure that the number of occurrences of `3` is consistent with the other implementations when operating on equivalent input.

The serial implementation can serve as a baseline for validating the parallel implementations.

## Purpose

The project was developed as an exploration of parallel programming and performance.

Rather than focusing on a complex algorithm, the project uses a simple computational task to make it easier to observe how different parallel technologies approach the same problem.

The comparison is particularly useful for examining the tradeoffs between:

**Java Threads → POSIX Threads → OpenMP → MPI → CUDA**

and determining when each approach is appropriate.

## Future Improvements

Potential improvements include:

* Automated benchmarking across multiple array sizes
* Running multiple trials and calculating average execution times
* Generating performance graphs
* Comparing different thread counts
* Testing different MPI process counts
* Measuring CUDA execution across different GPU configurations
* Calculating parallel efficiency
* Separating data-generation time from computation time
* Improving statistical analysis of benchmark results
* Comparing additional parallel programming models

## Author

**Jared Kaiser**

Bachelor of Science in Computer Science
New Mexico State University
