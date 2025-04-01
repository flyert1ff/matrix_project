#include <stdlib.h>
#include "matrix/matrix.h"
#include "output/output.h"

int main(void) {
    Matrix *A = NULL, *B = NULL, *C = NULL, *D = NULL;
    Matrix *sumAB = NULL, *Ct = NULL, *M = NULL, *result = NULL;

    A = load_matrix_from_file("data/A.txt");
    B = load_matrix_from_file("data/B.txt");
    C = load_matrix_from_file("data/C.txt");
    D = load_matrix_from_file("data/D.txt");

    if (!A || !B || !C || !D) {
        output("Ошибка загрузки матриц. Проверьте файлы в папке data/.\n");
        free_matrix(A); free_matrix(B); free_matrix(C); free_matrix(D);
        return EXIT_FAILURE;
    }

    sumAB = add_matrices(A, B);
    if (!sumAB) {
        output("Ошибка: сложение A + B невозможно.\n");
        free_matrix(A); free_matrix(B); free_matrix(C); free_matrix(D);
        return EXIT_FAILURE;
    }

    Ct = transpose_matrix(C);
    if (!Ct) {
        output("Ошибка: транспонирование C не удалось.\n");
        free_matrix(A); free_matrix(B); free_matrix(C); free_matrix(D);
        free_matrix(sumAB);
        return EXIT_FAILURE;
    }

    M = multiply_matrices(sumAB, Ct);
    if (!M) {
        output("Ошибка: умножение (A+B) * Ct невозможно.\n");
        free_matrix(A); free_matrix(B); free_matrix(C); free_matrix(D);
        free_matrix(sumAB); free_matrix(Ct);
        return EXIT_FAILURE;
    }

    result = subtract_matrices(M, D);
    if (!result) {
        output("Ошибка: вычитание M - D невозможно.\n");
        free_matrix(A); free_matrix(B); free_matrix(C); free_matrix(D);
        free_matrix(sumAB); free_matrix(Ct); free_matrix(M);
        return EXIT_FAILURE;
    }

    output("Результат выражения (A+B)*Ct - D:\n");
    print_matrix(result);

    if (save_matrix_to_file(result, "result.txt") != 0) {
        output("Ошибка сохранения результата.\n");
    }

    free_matrix(A); free_matrix(B); free_matrix(C); free_matrix(D);
    free_matrix(sumAB); free_matrix(Ct); free_matrix(M); free_matrix(result);

    return EXIT_SUCCESS;
}
