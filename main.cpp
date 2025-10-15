#include <iostream>
#include "MatrizDinamica.h"

int main() {
    try {
        std::cout << "Prueba Matriz ENTERA (int)" << std::endl;
        MatrizDinamica<int> A(2,3);
        // llenar A
        A(0,0) = 1; A(0,1) = 2; A(0,2) = 3;
        A(1,0) = 4; A(1,1) = 5; A(1,2) = 6;

        std::cout << "Redimensionar A a 3x3" << std::endl;
        A.redimensionar(3,3);

        std::cout << "Prueba Matriz FLOAT (float)" << std::endl;
        MatrizDinamica<float> B(3,2);
        B(0,0) = 1.5f; B(0,1) = 0.5f;
        B(1,0) = 2.0f; B(1,1) = 1.0f;
        B(2,0) = 1.0f; B(2,1) = 2.5f;

    std::cout << "Matriz A (int, redimensionada a 3x3):" << std::endl;
    std::cout << A << std::endl << std::endl;

    std::cout << "Matriz B (float):" << std::endl;
    std::cout << B << std::endl << std::endl;

    auto Af = MatrizDinamica<float>::multiplicar(MatrizDinamica<float>(A), B);
    std::cout << "Matriz C = A * B (float):" << std::endl;
    std::cout << Af << std::endl << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}