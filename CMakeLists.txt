cmake_minimum_required(VERSION 3.12)
project(Semga67 VERSION 1.0 LANGUAGES CXX)

# Устанавливаем стандарт C++ (например, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Автоматически находим все файлы .cpp в папке src
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Подключаем папку include, чтобы hpp-файлы были видны через #include "file.hpp"
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
