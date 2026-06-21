cmake_minimum_required(VERSION 3.20)
project(ec8_designPatterns)

set(CMAKE_CXX_STANDARD 20)

# Incluye la carpeta donde están tus headers
include_directories(include)

# El ejecutable de demostración principal
add_executable(ec8_designPatterns src/main.cpp)

# Habilitar el motor de pruebas de CMake (CTest)
enable_testing()

# El ejecutable secundario que solo corre los asserts
add_executable(test_expenses tests/test_expenses.cpp)
add_test(NAME ExpenseTests COMMAND test_expenses)