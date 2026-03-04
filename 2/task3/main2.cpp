#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <chrono>


int main(int argc, char* argv[]){

    int N = std::stoi(argv[1]);
    int potoc = std::stoi(argv[2]);

    double e = 0.000001;
    double t = 0.01;

    std::vector<double> A(N*N);
    std::vector<double> x(N);
    std::vector<double> b(N);

    // для промеж вычеслений
    std::vector<double> x_new(N);

    // A
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            if (i==j){
                A[i*N+j] = 2.0;
            }
            else{
                A[i*N+j] = 1.0;
            }
        }
    }

    // x
    for(auto it=x.begin(); it!=x.end(); it++){
        *it=0;
    }
    // b
    for(auto it=b.begin(); it!=b.end(); it++){
        *it=N+1;
    }

    double norm_r = 0;
    double norm_b = 0;

    #pragma omp parallel
    {
        auto start = std::chrono::high_resolution_clock::now();
        while (true) {

            double norm_r_loc = 0;
            double norm_b_loc = 0;

            for (int i = 0; i < N; i++) {

                double Ax_i = 0;

                for (int j = 0; j < N; j++)
                    Ax_i += A[i*N+j] * x[j];

                double r_i = Ax_i - b[i];

                norm_r_loc += r_i * r_i;
                norm_b_loc += b[i] * b[i];

                x_new[i] = x[i] - t * r_i;
            }

            #pragma omp atomic
            norm_r += norm_r_loc;

            #pragma omp atomic
            norm_b += norm_b_loc;

            #pragma omp barier

            if (sqrt(norm_r) / sqrt(norm_b) < e){
                break;
            }

            x.swap(x_new); // меняем указатели местами
        }
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration<double>(end - start).count();
        std::cout << "time: " << elapsed << "\n";
    }

    return 0;
}