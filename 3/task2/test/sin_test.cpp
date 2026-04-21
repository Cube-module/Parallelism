#include <iostream>
#include <fstream>
#include <cmath>
#include <functional>


template<typename T>
T fun_sin(T arg){
    return std::sin(arg);
}

int main()
{
    std::ifstream fin("sin.txt");

    // пропускаем первую строку (x result)
    std::string a, b;
    fin >> a >> b;

    double x, res;
    double result; // для получения результата при перещете с x
    double cnt_iter = 0; // счетчик для количества итераций while
    double cnt_true = 0; // счетчик для количества правильных ответов
    while (fin >> x >> res) {
        result = fun_sin(x);

        if(std::abs(result - res) < 1e-6){
            cnt_true++;
        }
        cnt_iter++;
    }

    double acur = cnt_true/cnt_iter;

    std::cout << "Точность sin: " << acur << "\n";

    return 0;
}