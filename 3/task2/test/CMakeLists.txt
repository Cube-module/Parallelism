# установим минимальную требуемую версию
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# название проекта, версия и испльзуемый язык
project(test_proj VERSION 0.0.0 LANGUAGES CXX)

# установим значение переменной определяющей стандарт
set(CMAKE_CXX_STANDARD 17)
# установим обязательное соответствие стандарту
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# определим конечную цель - исполняемый файл
add_executable(sin_test sin_test.cpp)
add_executable(pow_test pow_test.cpp)
add_executable(sqrt_test sqrt_test.cpp)

# cmake -S . -B ./build 
# cmake --build build