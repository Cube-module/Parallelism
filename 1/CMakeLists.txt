cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# название проекта, версия и используемый язык
project(sinus_hw_1 VERSION 0.0.0 LANGUAGES CXX)

# установим значение переменной определяющей стандарт
set(CMAKE_CXX_STANDARD 17)
# установим обязательное соответствие стандарту
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# определим конечную цель - исполняемый файл
add_executable(sinus sinus.cpp)

option(USE_FLOAT "Type vector" OFF)

if (USE_FLOAT)
    target_compile_definitions(sinus PRIVATE USE_FLOAT)
endif()