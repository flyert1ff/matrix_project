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
void print_matrix(const Matrix* matrix) {
    if (!matrix) {
        printf("Матрица не существует!\n");
        return;
    }

    printf("Матрица %dx%d:\n", matrix->rows, matrix->cols);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%.3lf ", matrix->data[i][j]);  // Красивый формат вывода
        }
        printf("\n");
    }
}
int save_matrix_to_file(const Matrix* matrix, const char* filename) {
    if (!matrix) {
        fprintf(stderr, "Ошибка: матрица не существует!\n");
        return -1;
    }

    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Ошибка открытия файла");
        return -1;
    }

    // Записываем размеры матрицы
    fprintf(file, "%d %d\n", matrix->rows, matrix->cols);

    // Записываем элементы матрицы
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            fprintf(file, "%.2lf ", matrix->data[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}
Matrix* copy_matrix(const Matrix* source) {
    if (!source) {
        fprintf(stderr, "Ошибка: исходная матрица не существует!\n");
        return NULL;
    }

    Matrix* copy = create_matrix(source->rows, source->cols);
    if (!copy) {
        fprintf(stderr, "Ошибка выделения памяти для копии матрицы\n");
        return NULL;
    }

    for (int i = 0; i < source->rows; i++) {
        for (int j = 0; j < source->cols; j++) {
            copy->data[i][j] = source->data[i][j];
        }
    }

    return copy;
}
Matrix* add_matrices(const Matrix* A, const Matrix* B) {
    if (!A || !B) {
        fprintf(stderr, "Ошибка: одна из матриц не существует!\n");
        return NULL;
    }

    if (A->rows != B->rows || A->cols != B->cols) {
        fprintf(stderr, "Ошибка: размерности матриц не совпадают!\n");
        return NULL;
    }

    Matrix* result = create_matrix(A->rows, A->cols);
    if (!result) {
        fprintf(stderr, "Ошибка выделения памяти для суммы матриц\n");
        return NULL;
    }

    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->cols; j++) {
            result->data[i][j] = A->data[i][j] + B->data[i][j];
        }
    }

    return result;
}
Matrix* multiply_matrices(const Matrix* A, const Matrix* B) {
    if (!A || !B) {
        fprintf(stderr, "Ошибка: одна из матриц не существует!\n");
        return NULL;
    }

    if (A->cols != B->rows) {
        fprintf(stderr, "Ошибка: количество столбцов A не равно количеству строк B!\n");
        return NULL;
    }

    Matrix* C = create_matrix(A->rows, B->cols);
    if (!C) {
        fprintf(stderr, "Ошибка выделения памяти для произведения матриц\n");
        return NULL;
    }

    for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->cols; j++) {
            C->data[i][j] = 0;
            for (int k = 0; k < A->cols; k++) {
                C->data[i][j] += A->data[i][k] * B->data[k][j];
            }
        }
    }

    return C;
}
Matrix* transpose_matrix(const Matrix* matrix) {
    if (!matrix) {
        fprintf(stderr, "Ошибка: матрица не существует!\n");
        return NULL;
    }

    Matrix* transposed = create_matrix(matrix->cols, matrix->rows);
    if (!transposed) {
        fprintf(stderr, "Ошибка выделения памяти для транспонированной матрицы\n");
        return NULL;
    }

    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            transposed->data[j][i] = matrix->data[i][j];
        }
    }

    return transposed;
}
MATRIX_TYPE determinant(const Matrix* matrix) {
    if (!matrix) {
        fprintf(stderr, "Ошибка: матрица не существует!\n");
        return 0;
    }

    if (matrix->rows != matrix->cols) {
        fprintf(stderr, "Ошибка: матрица не квадратная!\n");
        return 0;
    }

    int n = matrix->rows;

    // Определитель 1×1 матрицы
    if (n == 1) {
        return matrix->data[0][0];
    }

    // Определитель 2×2 матрицы
    if (n == 2) {
        return matrix->data[0][0] * matrix->data[1][1] - matrix->data[0][1] * matrix->data[1][0];
    }

    MATRIX_TYPE det = 0;

    // Рекурсивное разложение по первой строке
    for (int j = 0; j < n; j++) {
        // Создаём подматрицу (n-1) × (n-1)
        Matrix* minor = create_matrix(n - 1, n - 1);
        if (!minor) {
            fprintf(stderr, "Ошибка выделения памяти для миноров\n");
            return 0;
        }

        for (int i = 1; i < n; i++) {
            int colIndex = 0;
            for (int k = 0; k < n; k++) {
                if (k == j) continue;
                minor->data[i - 1][colIndex] = matrix->data[i][k];
                colIndex++;
            }
        }

        // Определитель по формуле (-1)^j * A[0][j] * det(A_{0j})
        det += (j % 2 == 0 ? 1 : -1) * matrix->data[0][j] * determinant(minor);
        
        free_matrix(minor);
    }

    return det;
}

