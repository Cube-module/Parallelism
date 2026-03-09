#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <chrono>


int main(int argc, char* argv[]){
    auto start1 = std::chrono::high_resolution_clock::now();

    int N = std::stoi(argv[1]);

    double e = 0.000001;
    double t = 0.01;

    std::vector<double> A(N*N);
    std::vector<double> x(N);
    std::vector<double> b(N);

    std::vector<double> times(16);

    // для промеж вычеслений
    std::vector<double> x_new(N);

    int iter = 0;
    int max_iter = 1000;
    double time = 0;

    for(int potoc=1; potoc<17; potoc++){

        omp_set_num_threads(potoc);

        for (int itr=0; itr<50; itr++){

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

            auto start = std::chrono::high_resolution_clock::now();
            #pragma omp parallel
            {
                #pragma omp single
                iter = 0;

                while (iter < max_iter) {

                    double norm_r_loc = 0;
                    double norm_b_loc = 0;

                    #pragma omp for
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

                    bool stop = false;
                    #pragma omp single
                    {
                        if (sqrt(norm_r) / sqrt(norm_b) < e)
                            stop = true;
                    }

                    #pragma omp barrier
                    if(stop){
                        break;
                    }

                    #pragma omp single
                    norm_r = 0;

                    #pragma omp single
                    norm_b = 0;

                    #pragma omp single
                    x.swap(x_new); // меняем указатели местами

                    #pragma omp single
                    iter++;
                }
            }

            auto end = std::chrono::high_resolution_clock::now();
            time += std::chrono::duration<double>(end - start).count();        
            
        }
        times[potoc-1] = time/50;
        time = 0;

    }
    std::cout << "------" << "#pragma omp parallel" << "------" << "\n"<< "\n";

    double T1 = times[0]; 
    std::cout << "T1: " << T1 << "\n";

    for(int i=1; i<16; i++){
        double T = times[i];
        double S = T1 / T;
        double E = S / (i+1);
        std::cout << "------" << i+1 << " Potocs" << "------" << "\n";
        std::cout << "T: " << T << "\n";
        std::cout << "S: " << S << "\n";
        std::cout << "E: " << E << "\n";
    }
    
    auto end1 = std::chrono::high_resolution_clock::now();
    auto time1 = std::chrono::duration<double>(end1 - start1).count();
    std::cout << "ALL Time: " << time1 << "\n";
       
    return 0;
}