# OpenMP-MPI-and-Hybrid-Parallel-Programming
Leveraged OpenMP, MPI to parallelize basic linear algebra algorithms. Used various techniques to parallelize matrix operations using shared memory model(OpenMP) and message passing interface(MPI) and evaluated timing statistics.

Linear algebra algorithms:
- Vector Addition(1xN matrix addition)
- Matrix Addition
- Matrix Multiplication

How to generate matrices for computation?
gen_matrix.py is a matrix generator script which generates matrices with seed values. 

Sample Command for matrix generation:\
python gen_matrix.py 406 3 0 > vec_8_1_a.txt\
python gen_matrix.py 506 3 0 > vec_8_1_b.txt

This will generate 2 matrices with 8x1 elements(2<sup>3</sup> rows and 2<sup>0</sup> columns). 406/506 are seed values for the 2 matrices.

Steps:\
"make serial(/omp/mpi)" to compile the code\
"make gen_matrix" to generate test data.\
"make run_serial(/omp/mpi)" to run the code.
