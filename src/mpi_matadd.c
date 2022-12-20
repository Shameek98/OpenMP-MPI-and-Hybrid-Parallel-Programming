#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include "data.h"
#include "timer.h"

int main(int argc, char *argv[]) {
int row,col;
int total_processes;
int rank;
int elem_per_process;
data_struct *d_1;
data_struct *d_2;
double *init_a;
double *init_b;
double *c;

//struct timespec start;
//start_timer(&start);

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &total_processes);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
double *partial_a;
double *partial_b;
double *partial_c;

struct timespec start;
d_1 = get_data_struct(argv[1]);
d_2 = get_data_struct(argv[2]);
row = d_1->rows;
col = d_1->cols;
if(rank == 0){
    // //start_timer(&start);
    init_a = (double *)malloc(row*col*sizeof(double));
    init_b = (double *)malloc(row*col*sizeof(double));
    for(int i = 0; i < row; i++){
        for(int j=0;j<col;j++){
        init_a[i*col + j] = d_1->data_point[i][j];
        init_b[i*col + j] = d_2->data_point[i][j];
    }
    }
    c = (double *)malloc(row*col*sizeof(double));
    //stop_timer(&start);
}
// MPI_Barrier(MPI_COMM_WORLD);
elem_per_process = col*row/total_processes;
partial_a = (double *)malloc(elem_per_process*sizeof(double));
partial_b = (double *)malloc(elem_per_process*sizeof(double));
partial_c = (double *)malloc(elem_per_process*sizeof(double));
double local_time_comp;
start_timer(&start);
MPI_Scatter(&init_a[rank*elem_per_process], elem_per_process, MPI_DOUBLE, partial_a, elem_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
MPI_Scatter(&init_b[rank*elem_per_process], elem_per_process, MPI_DOUBLE, partial_b, elem_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
for(int i=0;i<elem_per_process;i++){
        partial_c[i] = partial_a[i] + partial_b[i];
    }

MPI_Gather(partial_c, elem_per_process, MPI_DOUBLE, &c[rank*elem_per_process], elem_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
local_time_comp  = stop_timer_mpi(&start);
double global_time_comp;
MPI_Reduce(&local_time_comp,&global_time_comp,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
free(partial_a);
free(partial_b);
free(partial_c);

if(rank == 0){
    for(int i = 0; i<row; i++){
        for(int j=0; j< col;j++)
            printf("%f ",c[i*col+j]);
        printf("\n");
    }
    fprintf(stderr, "Computation time %f ms\n", global_time_comp);
    /*free_data(d_1);
    free_data(d_2);
    free(init_a);
    free(init_b);
    free(c);*/
}
MPI_Finalize();
}
