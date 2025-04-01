#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include "../../include/config.h"

typedef struct 
{
    int rows;
    int cols;
    MATRIX_TYPE **data;

}Matrix;

Matrix *create_matrix(int rows, int cols);
void free_matrix(Matrix *matrix);
Matrix *load_matrix_from_file(const char *filename);
void print_matrix(const Matrix *matrix);
int save_matrix_to_file(const Matrix *matrix, const char *filename);
Matrix *copy_matrix(const Matrix *source);
Matrix *add_matrices(const Matrix *A, const Matrix *B);
<<<<<<< HEAD
Matrix *subtract_matrices(const Matrix *A, const Matrix *B);
=======
Matrix* subtract_matrices(const Matrix* A, const Matrix* B);
>>>>>>> 16acea9 (написан файл main.c)
Matrix *multiply_matrices(const Matrix *A, const Matrix *B);
Matrix *transpose_matrix(const Matrix *matrix);
MATRIX_TYPE determinant(const Matrix *matrix);

#endif
