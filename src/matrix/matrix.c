#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

Matrix *create_matrix(int rows, int cols) {
    int flag = 0;  

    if (rows <= 0 || cols <= 0) flag = 1;

    Matrix *matrix = NULL;

    if (!flag) {
        matrix = (Matrix *)malloc(sizeof(Matrix));
        if (!matrix) flag = 2;
    }

    if (!flag) {
        matrix->data = (MATRIX_TYPE **)malloc(rows * sizeof(MATRIX_TYPE *));
        if (!matrix->data) flag = 3;
    }

    if (!flag) {
        matrix->rows = rows;
        matrix->cols = cols;

        for (int i = 0; i < rows; i++) {
            matrix->data[i] = (MATRIX_TYPE *)calloc(cols, sizeof(MATRIX_TYPE));
            if (!matrix->data[i]) {
                flag = 4;

                for(int j = 0; j<i; j++)
                    free(matrix->data[j]); 
                free(matrix->data);
                matrix->data=NULL;
                free(matrix);
                matrix=NULL;
                i = rows;
            }
        }
    }

    switch (flag) {
        case 1:
            fprintf(stderr, "Ошибка: некорректные размеры матрицы\n");
            break;
        case 2:
            fprintf(stderr, "Ошибка выделения памяти (структура)\n");
            break;
        case 3:
            fprintf(stderr, "Ошибка выделения памяти (указатели)\n");
            free(matrix);
            break;
        case 4:
            fprintf(stderr, "Ошибка выделения памяти (данные)\n");
            break;
        default:
            return matrix;
    }

    return NULL; 
}


void free_matrix(Matrix *matrix){
    if(!matrix){
        fprintf(stderr, "Ошибка: передан NULL-указатель в free_matrix\n");
        return;
    }
    for(int i=0; i<matrix->rows;i++)
        free(matrix->data[i]);

    free(matrix->data);
    free(matrix);
}

Matrix* load_matrix_from_file(const char* filename) {
    int flag = 0;
    FILE* file = NULL;
    Matrix *matrix = NULL;

    file = fopen(filename, "r");
    if (!file) flag = 1;
    else {
        int rows, cols;
        if (fscanf(file, "%d %d", &rows, &cols) != 2 || rows <= 0 || cols <= 0) flag = 2;
        else{
            matrix = create_matrix(rows, cols);
            if (!matrix) flag = 3;
            else{
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        if (fscanf(file, "%lf", &matrix->data[i][j]) != 1) {
                            flag = 4;
                            i = rows;
                            j = cols;
                        }
                    }
                }
                if(flag == 0){
                    double extra_data;
                    if(fscanf(file, "%lf", &extra_data)!= EOF)
                        fprintf(stderr, "Предупреждение: файл содержит лишние данные\n");
                }
            }
        }
        fclose(file);
    }
    switch (flag) {
        case 1:
            perror("Ошибка открытия файла");
            return NULL;
        case 2:
            fprintf(stderr, "Ошибка чтения размеров матрицы\n");
            return NULL;
        case 3:
            fprintf(stderr, "Ошибка выделения памяти\n");
            return NULL;
        case 4:
            fprintf(stderr, "Ошибка чтения данных матрицы\n");
            free_matrix(matrix);
            matrix = NULL;
            return NULL;
        default:
        return matrix;
    }
}
void print_matrix(const Matrix* matrix) {
    if (!matrix) {
        printf("Матрица не существует!\n");
        return;
    }

    printf("Матрица %dx%d:\n", matrix->rows, matrix->cols);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++)
            printf("%.3lf ", matrix->data[i][j]); 
        printf("\n");
    }
}
int save_matrix_to_file(const Matrix* matrix, const char* filename) {
    int flag = 0;
    if (!matrix) flag = 1;

    FILE* file = fopen(filename, "w");
    if (!file) flag = 2;

    // Записываем размеры матрицы
    fprintf(file, "%d %d\n", matrix->rows, matrix->cols);

    // Записываем элементы матрицы
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++)
            fprintf(file, "%.2lf ", matrix->data[i][j]);
        fprintf(file, "\n");
    }

    fclose(file);
    switch (flag)
    {
    case 1:
        fprintf(stderr, "Ошибка: матрица не существует!\n");
        return -1;
    case 2:
        perror("Ошибка открытия файла");
        return -1;
    default:
        return 0;
    }
    
}
Matrix* copy_matrix(const Matrix* source) {
    int flag = 0;
    if (!source) flag = 1;

    Matrix* copy = create_matrix(source->rows, source->cols);
    if (!copy) flag = 2;

    for (int i = 0; i < source->rows; i++) {
        for (int j = 0; j < source->cols; j++) 
            copy->data[i][j] = source->data[i][j];
    }
    switch (flag)
    {
    case 1:
        fprintf(stderr, "Ошибка: исходная матрица не существует!\n");
        return NULL;
    case 2:
        fprintf(stderr, "Ошибка выделения памяти для копии матрицы\n");
        return NULL;
    default:
        return copy;
    }
    
}
Matrix* add_matrices(const Matrix* A, const Matrix* B) {
    int flag = 0;
    if (!A || !B) flag = 1;
    if (A->cols != B->rows) flag = 4;

    if (A->rows != B->rows || A->cols != B->cols) flag = 2;

    Matrix* result = create_matrix(A->rows, A->cols);
    if (!result) flag = 3;

    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->cols; j++) 
            result->data[i][j] = A->data[i][j] + B->data[i][j];
        
    }
    switch (flag)
    {
    case 1:
        fprintf(stderr, "Ошибка: одна из матриц не существует!\n");
        return NULL;
    case 2:
        fprintf(stderr, "Ошибка: размерности матриц не совпадают!\n");
        return NULL;
    case 3:
        fprintf(stderr, "Ошибка выделения памяти для суммы матриц\n");
        return NULL;
    case 4:
        fprintf(stderr, "Ошибка: количество столбцов A не равно количеству строк B!\n");
        return NULL;
    default: return result;
    }
    
}
Matrix* multiply_matrices(const Matrix* A, const Matrix* B) {
    int flag = 0;
    if (!A || !B) flag = 1;

    if (A->cols != B->rows) flag = 2;
    

    Matrix* C = create_matrix(A->rows, B->cols);
    if (!C) flag = 3;
    

    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->cols; j++) {
            C->data[i][j] = 0;
            for (int k = 0; k < A->cols; k++) 
                C->data[i][j] += A->data[i][k] * B->data[k][j];
            
        }
    }
    switch (flag)
    {
    case 1:
        fprintf(stderr, "Ошибка: одна из матриц не существует!\n");
        return NULL;
    case 2:
        fprintf(stderr, "Ошибка: количество столбцов A не равно количеству строк B!\n");
        return NULL;
    case 3:
        fprintf(stderr, "Ошибка выделения памяти для произведения матриц\n");
        return NULL;
    default: return C;
    }
    
}
Matrix* transpose_matrix(const Matrix* matrix) {
    int flag = 0;
    if (!matrix) flag = 1;

    Matrix* transposed = create_matrix(matrix->cols, matrix->rows);
    if (!transposed) flag = 2;

    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) 
            transposed->data[j][i] = matrix->data[i][j];
        
    }
    switch (flag)
    {
    case 1:
        fprintf(stderr, "Ошибка: матрица не существует!\n");
        return NULL;
    case 2:
        fprintf(stderr, "Ошибка выделения памяти для транспонированной матрицы\n");
        return NULL;
    default: return transposed;
    }
    
}
MATRIX_TYPE determinant(const Matrix* matrix) {
    int flag = 0;
    if (!matrix) flag = 1;

    if (flag == 0 && matrix->rows != matrix->cols) flag = 2;

    int n = (flag == 0) ? matrix->rows : 0;

    // Базовые случаи
    if (flag == 0 && n == 1) return matrix->data[0][0];
    if (flag == 0 && n == 2) return matrix->data[0][0] * matrix->data[1][1] - 
                                   matrix->data[0][1] * matrix->data[1][0];

    MATRIX_TYPE det = 0;

    for (int j = 0; flag == 0 && j < n; j++) {
        Matrix* minor = create_matrix(n - 1, n - 1);
        if (!minor) {
            flag = 3;
            j = n; 
        } else {
            for (int i = 1; i < n; i++) {
                int colIndex = 0;
                for (int k = 0; k < n; k++) {
                    if (k != j) {
                        minor->data[i - 1][colIndex++] = matrix->data[i][k];
                    }
                }
            }

            MATRIX_TYPE minorDet = determinant(minor);
            free_matrix(minor);
            det += (j % 2 == 0 ? 1 : -1) * matrix->data[0][j] * minorDet;
        }
    }

    switch (flag) {
        case 1:
            fprintf(stderr, "Ошибка: матрица не существует!\n");
            return 0;
        case 2:
            fprintf(stderr, "Ошибка: матрица не квадратная!\n");
            return 0;
        case 3:
            fprintf(stderr, "Ошибка выделения памяти для миноров\n");
            return 0;
        default:
            return det;
    }
}


