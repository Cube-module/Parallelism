#include <iostream>
#include <fstream>
#include <cmath>
#include <functional>


template<typename T>
T fun_pow(T x, T y){
    return std::pow(x, y);
}

int main()
{
    std::ifstream fin("pow.txt");

    // пропускаем первую строку (x y result)
    std::string a, b, c;
    fin >> a >> b >> c;

    double x, y, res;
    double result; // для получения результата при перещете с x y
    double cnt_iter = 0; // счетчик для количества итераций while
    double cnt_true = 0; // счетчик для количества правильных ответов
    while (fin >> x >> y >> res) {
        result = fun_pow(x, y);

        if(std::abs(result - res) < 1e-6){
            cnt_true++;
        }
        cnt_iter++;
    }

    double acur = cnt_true/cnt_iter;

    std::cout << "Точность pow: " << acur << "\n";

    return 0;
}