#include <iostream>
#include <vector>
#include <cmath>

void matrix_mult(const std::vector<double> &A, std::vector<double> &x, std::vector<double> &prom, const std::vector<double> &b, int t, int N){

    for(int i=0; i<N; i++){

        for(int j=0; j<N; j++){
            prom[i] += A[i*N+j] * x[j];
        }
        x[i] = prom[i] - ((prom[i] - b[i]) * (t)); // xn+1
    }
}

bool evklid(const std::vector<double> &A, const std::vector<double> &x, std::vector<double> &prom, std::vector<double> &prom2,const std::vector<double> &b, int N, int e){

    double sum1 =0;
    double sum2 =0;

    for (int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            prom[i] += A[i*N+j] * x[j];
        }
        prom2[i] = prom[i] - b[i];
        sum1 += std::pow(prom2[i], 2);
        sum2 += std::pow(b[i], 2);
    }
    sum1 = std::sqrt(sum1);
    sum2 = std::sqrt(sum2);

    if (e > sum1/sum2){
        return true;
    }
    return false;
}

int main(int argc, char* argv[]){

    int N = std::stoi(argv[1]);
    int potoc = std::stoi(argv[2]);

    double e = 0.000001;
    double t = 0.01;

    std::vector<double> A(N*N);
    std::vector<double> x(N);
    std::vector<double> b(N);

    // для промеж вычеслений
    std::vector<double> prom(N);
    std::vector<double> prom2(N);

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


    matrix_mult(A, x, prom, b, t, N);



}