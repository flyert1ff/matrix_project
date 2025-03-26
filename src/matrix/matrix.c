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

Matrix* load_matrix_from_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка открытия файла");
        return NULL;
    }

    int rows, cols;
    if (fscanf(file, "%d %d", &rows, &cols) != 2 || rows <= 0 || cols <= 0) {
        fclose(file);
        fprintf(stderr, "Ошибка чтения размеров матрицы\n");
        return NULL;
    }

    Matrix* matrix = create_matrix(rows, cols);
    if (!matrix) {
        fclose(file);
        fprintf(stderr, "Ошибка выделения памяти для матрицы\n");
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fscanf(file, "%lf", &matrix->data[i][j]) != 1) {
                fprintf(stderr, "Ошибка чтения элемента матрицы [%d][%d]\n", i, j);
                free_matrix(matrix);
                fclose(file);
                return NULL;
            }
        }
    }

    fclose(file);
    return matrix;
}