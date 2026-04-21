#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>

void mult(int N, int k, int t, std::vector<double>& A, std::vector<double>& b, std::vector<double>& res){

    // распределяем индексы между k потоков
    int start = t * N / k;
    int end = (t+1) * N / k;

    // перемножаем
    for(int i=start; i<end; i++){
        for(int j=0; j<N; j++){
            res[i] += A[i*N+j] * b[j];
        }
    }
}

int main(int argc, char* argv[]){
    int N = std::stoi(argv[1]);
    int k = std::stoi(argv[2]);

    std::ofstream fout("results.txt", std::ios::app);

    std::vector<double> A(N*N);
    std::vector<double> b(N);
    std::vector<double> res(N);

    double time = 0;

    // инициализация нулями
    for(int i=0; i<N; i++){
        b[i] = 0;
        res[i] = 0;
        for(int j=0; j<N; j++){
            A[i*N+j] = 0;
        }
    }

    // распараллеливание
    for(int i = 0; i < 50; i++){

        std::vector<std::thread> thread;
        auto start = std::chrono::high_resolution_clock::now();

        for(int t = 0; t < k; t++){
            thread.emplace_back(mult, N, k, t, std::ref(A), std::ref(b), std::ref(res));
        }

        for(int t = 0; t < k; t++){
            thread[t].join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        time += std::chrono::duration<double>(end - start).count();

    }

    double T = time / 50;

    fout << N << " " << k << " " << T << "\n";
    std::cout << N << " " << k << " " << T << "\n";
    return 0;

}