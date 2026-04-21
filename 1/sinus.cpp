#include <iostream>
#include <vector>
#include <typeinfo>
#include <cmath>
#include <numbers>

#define PI 3.14159265358979323846

#ifdef USE_FLOAT
    using Type = float;
#else
    using Type = double;
#endif


int main(){
    int N = 10000000;
    std::vector<Type> Arr(N);
    
    if(typeid(Type)==typeid(float)){
        for(int i=0; i<N; i++){
            Arr[i] = std::sin(2*PI*i/N);
        }
    }
    else{
        for(int i=0; i<N; i++){
            Arr[i] = std::sin(2*PI*i/N);
        }
    }

    Type sum = 0;

    for(int i=0; i<N; i++){
        sum+=Arr[i];
    }

    std::cout << sum << "\n";
}