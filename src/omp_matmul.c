#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "data.h"
#include <omp.h>
#include "timer.h"

int main(int argc, char **argv)
{
    if(argc != 4){
        printf("Usage: <num_thread> <vec_a> <vec_b>.\n");
        exit(EXIT_FAILURE);
    }

    // int numThreads = omp_get_max_threads();
    // convinient for use in partitioning
    int numThreads = atoi(argv[1]);
    omp_set_num_threads(numThreads);
        
    struct timespec start;
    start_timer(&start);

    data_struct *d_1 = get_data_struct(argv[2]);
    data_struct *d_2 = get_data_struct(argv[3]);

    double **mem = calloc((d_1->rows), sizeof(double*));
    for (int a=0;a<(d_1->rows);a++){
        mem[a] = calloc((d_2->cols),sizeof(double));
    }
    stop_timer(&start);
    fprintf(stderr, " (reading input)\n");

    if(d_1->cols != d_2->rows){
        printf("ERROR: Matrix dimension mismatch.\n");
        exit(EXIT_FAILURE);
    }

    start_timer(&start);
    /* TODO: Implement Matrix Multiplication  openMP here */
    #pragma omp parallel for schedule(guided, 2)
    for(int i=0; i<(d_1->rows); i++){
	#pragma unroll(2)
	for(int j=0; j<d_2->cols;j++){
            //mem[i][j] = 0;
            for(int k=0;k<(d_1->cols);k++){
                mem[i][j] += d_1->data_point[i][k] * d_2->data_point[k][j];
            }
        }
    }
    stop_timer(&start);
    fprintf(stderr, " (calculating answer)\n");
    
    start_timer(&start);
    /* TODO: Print output */
    for(int i=0; i<(d_1->rows); i++){
        for(int j=0;j<(d_2->cols); j++){
            printf("%f ",mem[i][j]);
        }
	printf("\n");
    }
    stop_timer(&start);
    fprintf(stderr, " (printing output)\n");

    for (int a=0;a<(d_1->rows);a++){
       free(mem[a]);
    }
    free(mem);
    free_data(d_1);
    free_data(d_2);
}
