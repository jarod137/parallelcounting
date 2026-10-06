# Jared Kaiser


all: cuda Count3s CThreads MPI OpenMP

cuda: cuda.cu
	/usr/local/cuda/bin/nvcc -o cuda -I/usr/local/cuda/include cuda.cu

Count3s: Count3s.java
	javac Count3s.java

CThreads: CThreads.c
	gcc -o CThreads CThreads.c

MPI: MPI.c
	mpicc -o mpi_count MPI.c

OpenMP: OpenMP.c
	gcc -fopenmp -o count3s_openmp OpenMP.c

clean:
	rm -f cuda
	rm -f Count3s.class
	rm -f count3s_openmp
	rm -f CThreads
	rm -f mpi_count
