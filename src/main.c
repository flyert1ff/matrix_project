#include <stdlib.h>
#include "matrix/matrix.h"
#include "output/output.h"

int main(void) {
    Matrix *A = NULL, *B = NULL, *C = NULL, *D = NULL;
    Matrix *sumAB = NULL, *Ct = NULL, *M = NULL, *result = NULL;
    int status = EXIT_FAILURE;

    // Загрузка матриц
    A = load_matrix_from_file("data/A.txt");
    B = load_matrix_from_file("data/B.txt");
    C = load_matrix_from_file("data/C.txt");
    D = load_matrix_from_file("data/D.txt");

    if (A && B && C && D) {
        sumAB = add_matrices(A, B);
        if (sumAB) {
            Ct = transpose_matrix(C);
            if (Ct) {
                M = multiply_matrices(sumAB, Ct);
                if (M) {
                    result = subtract_matrices(M, D);
                    if (result) {
                        output("Результат выражения (A+B)*Ct - D:\n");
                        print_matrix(result);
                        if (save_matrix_to_file(result, "result.txt") == 0) {
                            status = EXIT_SUCCESS;
                        } else {
                            output("Ошибка сохранения результата.\n");
                        }
                    } else {
                        output("Ошибка: вычитание M - D невозможно.\n");
                    }
                    free_matrix(M);
                } else {
                    output("Ошибка: умножение (A+B) * Ct невозможно.\n");
                }
                free_matrix(Ct);
            } else {
                output("Ошибка: транспонирование C не удалось.\n");
            }
            free_matrix(sumAB);
        } else {
            output("Ошибка: сложение A + B невозможно.\n");
        }
    } else {
        output("Ошибка загрузки матриц. Проверьте файлы в папке data/.\n");
    }

    // Освобождение основных матриц
    free_matrix(A); 
    free_matrix(B); 
    free_matrix(C); 
    free_matrix(D);
    free_matrix(result);

    return status;
}
