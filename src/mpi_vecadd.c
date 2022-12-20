#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include "data.h"
#include "timer.h"

#define PROC 16

int main(int argc, char *argv[]) {
int row;
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
d_1 = get_data_struct(argv[1]);
d_2 = get_data_struct(argv[2]);
row = d_1->rows;
init_a = (double *)malloc(row*sizeof(double));
    init_b = (double *)malloc(row*sizeof(double));
    for(int i = 0; i < row; i++){
        init_a[i] = d_1->data_point[i][0];
        init_b[i] = d_2->data_point[i][0];
    }
    c = (double *)malloc(row*sizeof(double));

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &total_processes);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
double *partial_a;
double *partial_b;
double *partial_c;

struct timespec start;

if(rank == 0){
    //start_timer(&start);
    
    //stop_timer(&start);
}
// MPI_Barrier(MPI_COMM_WORLD);
elem_per_process = row/total_processes;
partial_a = (double *)malloc(elem_per_process*sizeof(double));
partial_b = (double *)malloc(elem_per_process*sizeof(double));
partial_c = (double *)malloc(elem_per_process*sizeof(double));
double local_time;
start_timer(&start);
MPI_Scatter(&init_a[rank*elem_per_process], elem_per_process, MPI_DOUBLE, partial_a, elem_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
MPI_Scatter(&init_b[rank*elem_per_process], elem_per_process, MPI_DOUBLE, partial_b, elem_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
for(int i=0;i<elem_per_process;i++){
       partial_c[i] = partial_a[i] + partial_b[i];
    }
MPI_Gather(partial_c, elem_per_process, MPI_DOUBLE, &c[rank*elem_per_process], elem_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);
local_time  = stop_timer_mpi(&start);
double global_time;
MPI_Reduce(&local_time,&global_time,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
free(partial_a);
free(partial_b);
free(partial_c);

if(rank == 0){
    for(int i = (elem_per_process*total_processes); i<row;i++){
            c[i] = init_a[i] + init_b[i];
    }
    for(int i = 0; i<row; i++){
        printf("%f\n",c[i]);
    }
    fprintf(stderr, "Computation time %f ms\n", global_time);
    free_data(d_1);
    free_data(d_2);
    free(c);
}
MPI_Finalize();
//free_data(d_1);
//free_data(d_2);
//stop_timer(&start);

//fprintf(stderr, "time taken\n");
}
