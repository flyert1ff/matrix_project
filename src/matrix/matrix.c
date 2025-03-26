#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

Matrix *create_matrix(int rows, int cols)
{
    if(rows <= 0 || cols <= 0){
        printf("Empty matrix");
        return NULL;
    }
    Matrix *matrix = (Matrix*) malloc(sizeof(Matrix));
    if(!matrix){
        printf("allocation error - 1");
        return NULL;
    }
    matrix->cols = cols;
    matrix->rows = rows;
    matrix->data = (MATRIX_TYPE**)malloc(rows * sizeof(MATRIX_TYPE*));

    if(!matrix->data){
        printf("allocation error - 2");
        return NULL;
    }

    for(int i = 0; i<rows; i++){
        matrix->data[i] = (MATRIX_TYPE*)calloc(cols, sizeof(MATRIX_TYPE));
        if(!matrix->data[i]){
            for(int j = 0; j<i; j++)
                free(matrix->data[j]);
            free(matrix->data);
            free(matrix);
            printf("allocation error - 3");
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(Matrix *matrix){
    if(!matrix){
        printf("free error");
        return;
    }
    for(int i=0; i<matrix->rows;i++)
        free(matrix->data[i]);

    free(matrix->data);
    free(matrix);
}