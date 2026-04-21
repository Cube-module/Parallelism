#include <iostream>
#include <fstream>
#include <cmath>
#include <functional>


template<typename T>
T fun_sqrt(T arg){
    return std::sqrt(arg);
}
int main()
{
    std::ifstream fin("sqrt.txt");

    // пропускаем первую строку (x result)
    std::string a, b;
    fin >> a >> b;

    double x, res;
    double result; // для получения результата при перещете с x
    double cnt_iter = 0; // счетчик для количества итераций while
    double cnt_true = 0; // счетчик для количества правильных ответов
    while (fin >> x >> res) {
        result = fun_sqrt(x);

        if(std::abs(result - res) < 1e-4){
            cnt_true++;
        }
        cnt_iter++;
    }

    double acur = cnt_true/cnt_iter;

    std::cout << "Точность sqrt: " << acur << "\n";

    return 0;
}