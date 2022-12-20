#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "data.h"
#include "timer.h"

int main(int argc, char **argv)
{
    if(argc != 3){
        printf("ERROR: Please specify only 2 files.\n");
        exit(EXIT_FAILURE);
    }
        
    struct timespec start;
    start_timer(&start);

    data_struct *d_1 = get_data_struct(argv[1]);
    data_struct *d_2 = get_data_struct(argv[2]);

    double **mem = (double**) malloc((d_1->rows) * sizeof(double*));
    for (int a=0;a<(d_1->rows);a++){
        mem[a] = (double*) malloc((d_2->cols) * sizeof(double));
    }
    
    stop_timer(&start);
    fprintf(stderr, " (reading input)\n");

    if(d_1->cols != d_2->rows){
        printf("ERROR: Matrix dimension mismatch.\n");
        exit(EXIT_FAILURE);
    }

    start_timer(&start);
    for(int i=0; i<(d_1->rows); i++){
        for(int j=0; j<d_2->cols;j++){
            mem[i][j] = 0;
            for(int k=0;k<(d_1->cols);k++){
                mem[i][j] += d_1->data_point[i][k] * d_2->data_point[k][j];
            }
        }
    }
    stop_timer(&start);
    fprintf(stderr, " (calculating answer)\n");
    
    start_timer(&start);
    /* TODO: Print output */
    for(int i=0;i<d_1->rows;i++){
        for(int j=0;j<d_2->cols;j++){
            printf("%f ", mem[i][j]);
        }
	printf("\n");
    }
    stop_timer(&start);
    fprintf(stderr, " (printing output)\n");  
    for(int i=0;i<d_1->rows;i++){
	free(mem[i]);
    }
    free(mem);
    free_data(d_1);
    free_data(d_2);
}
