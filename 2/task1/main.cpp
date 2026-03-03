#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

int main(int argc, char* argv[]){
    int N = std::stoi(argv[1]); // размер масива
    int k = std::stoi(argv[2]); // число потоков

    omp_set_num_threads(k); // количество потоков k

    std::vector<double> A(N*N); // матрица
    std::vector<double> x(N); // вектор
    std::vector<double> y(N); // результат

    auto start = std::chrono::high_resolution_clock::now();

    // инициализирую матрицу и вектор
    #pragma omp parallel for schedule(static)
    for(int i=0; i<N; i++){
        x[i] = 2.0;
        for(int j=0; j<N; j++){
            A[i*N+j] = 1.0;
        }
    }
    
    // перемножение матрицы на вектор
    #pragma omp parallel for schedule(static)
    for(int i=0; i<N; i++){
        y[i] = 0.0;
        for(int j=0; j<N; j++){
            y[i] += A[i*N+j] * x[j];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    double elapsed = std::chrono::duration<double>(end - start).count();

    std::cout << "Time: " << elapsed << "\n";

    // ускорение
    double T1 = (35.5907 + 37.8646)/2;
    double S = T1/elapsed;

    std::cout << "S: " << S << "\n";

    return 0;
}