#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <fstream>


// возвращает индекс для псевдо 2 мерного масива(одномерного) просто чтобы не мучаться
int idx(int i, int j, int N){
    return i*N+j;
}

// функция заполнения краев матрицыв
void initBorders(std::vector<double> &A, int N){
    // начальные значения по углам
    double topLeft = 10.0;
    double topRight = 20.0;
    double bottomRight = 30.0;
    double bottomLeft = 20.0;

    // переменные которые не нужно пересчитывать
    double top_r_l = topRight - topLeft;
    double bottom_r_l = bottomRight - bottomLeft;
    double left_bottom_top = bottomLeft - topLeft;
    double right_bottom_top = bottomRight - topRight;

    // заполнением верхнего и нижнего краёв матрицы
    for(int j=0; j<N; j++){
        // t от 0 до 1: позиция вдоль стороны слева направо
        double t = double(j) / (N-1);

        // заполняем плавным переход от лева к права по краям (права горячее)
        A[idx(0, j, N)] = topLeft + t * (top_r_l);              // верх
        A[idx(N - 1, j, N)] = bottomLeft + t * (bottom_r_l); // низ
    }

    // заполнением левого и правого краёв матрицы
    for(int i=0; i<N; i++){
        // t от 0 до 1: позиция вдоль стороны сверху вниз
        double t = double(i) / (N-1);

        A[idx(i,0,N)] = topLeft + t * (left_bottom_top);
        A[idx(i,N-1,N)] = topRight + t * (right_bottom_top);
    }
}

// вычисляем и записываем в матрицу результаты
double makeIteration(std::vector<double>& A, std::vector<double>& B, int N) {
    double error = 0.0;

    for (int i = 1; i < N - 1; i++) {
        for (int j = 1; j < N - 1; j++) {
            int id = idx(i, j, N);

            // среднее арифметическое температуры от соседей
            B[id] = 0.25 * (
                A[idx(i - 1, j, N)] +
                A[idx(i + 1, j, N)] +
                A[idx(i, j - 1, N)] +
                A[idx(i, j + 1, N)]
            );

            // максимальная разница тепмператур между старым и новым значением
            error = std::max(error, std::abs(B[id] - A[id]));
        }
    }

    return error;
}

// функция вывода матрицы (печать)
void printMatrix(const std::vector<double>& A, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << A[idx(i, j, N)] << " ";
        }
        std::cout << "\n";
    }
}


int main(int argc, char** argv) {

    if (argc != 4) {
        std::cout << "incorrect input\n";
        std::cout << "Usage: ./main N eps maxIter\n";
        return 1;
    }

    int N = std::stoi(argv[1]);
    double eps = std::stod(argv[2]);
    int maxIter = std::stoi(argv[3]);

    // главная матрица
    std::vector<double> A(N * N, 0.0);
    // матрица посредник для коректного изменения температуры
    std::vector<double> B(N * N, 0.0);

    initBorders(A, N);
    initBorders(B, N);

    double error = 0.0;
    int iter = 0;

    // НАЧАЛО
    auto start = std::chrono::high_resolution_clock::now();
    do {
        error = makeIteration(A, B, N);
        std::swap(A, B);
        iter++;
    } while (error > eps && iter < maxIter);
    // КОНЕЦ
    auto end = std::chrono::high_resolution_clock::now();

    // ВРЕМЯ РАБОТЫ
    double time = std::chrono::duration<double>(end - start).count();

    std::ofstream out("results.txt", std::ios::app);

    out << "Grid size: " << N << "\n";
    out << "Time: " << time << " sec\n";
    out << "Iterations: " << iter << "\n";
    out << "Error: " << error << "\n";
    out << "--------------------------\n";

    // printMatrix(A, N);

    return 0;
}