#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include "data.h"
#define PROC 16

int main(int argc, char *argv[]) {
    int row_a,col_a;
    int row_b,col_b;
int total_processes;
int rank;
int elem_per_process;
data_struct *d_1;
data_struct *d_2;
double *init_a;
double *init_b;
double *c;

struct timespec start;
d_1 = get_data_struct(argv[1]);
d_2 = get_data_struct(argv[2]);
row_a = d_1->rows;
col_a = d_1->cols;
row_b = d_2->rows;
col_b = d_2->cols;
init_b = (double *)malloc(row_b*col_b*sizeof(double));

for(int i=0; i<row_b ; i++){
    for(int j=0; j<col_b; j++){
        init_b[i*col_b + j] = d_2->data_point[i][j];
    }
}
 /*TODO Implement Matrix Multiplication MPI here */
MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &total_processes);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
elem_per_process = col_a*row_a/PROC;
double *partial_a;
double *partial_c;
if(rank == 0){
    init_a = (double *)malloc(row_a*col_a*sizeof(double));
    for(int i = 0; i < row_a; i++){
            for(int j=0;j<col_a;j++){
            init_a[i*col_a + j] = d_1->data_point[i][j];
        }
    }
    c = (double *)malloc(row_a*col_b*sizeof(double));
}
partial_c = (double *)malloc(elem_per_process*sizeof(double));
partial_a = (double *)malloc(elem_per_process*sizeof(double));
MPI_Scatter(&init_a[rank*elem_per_process], elem_per_process, MPI_DOUBLE, partial_a, elem_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
MPI_Bcast(init_b,col_b*row_b,MPI_DOUBLE,0,MPI_COMM_WORLD);

for(int i=0;i<elem_per_process;i++){
    partial_c[i] =0;
    for (int j = 0; j < elem_per_process; j++)
    {
        /* code */
        if(rank%2 == 0)
            partial_c[i] += partial_a[j] * init_b[i + j*col_b];
        else{
            partial_c[i] += partial_a[j] * init_b[i + j*col_b + (row_b*col_b/2)];
        }
    }
    
}
printf("rank = %d\n",rank);
int from;
if(rank%2==0)
    from =rank;
else    
    from =rank-1;
MPI_Reduce(partial_c,&c[from*elem_per_process],elem_per_process,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
if(rank ==0){
    for(int i=0;i<row_a;i++){
	for(int j=0;j<col_b;j++){
	    printf("%f ", c[i*row_a+j]);
	}
	printf("\n");
}
}

// double *partial_b;
// double *partial_c;
MPI_Finalize();

return 0;

}
