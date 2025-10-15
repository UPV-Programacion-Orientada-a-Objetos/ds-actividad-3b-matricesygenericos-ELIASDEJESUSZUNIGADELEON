#include <iostream>
#include "MatrizDinamica.h"

int main() {
    try {
        std::cout << "--- Sistema de Analisis de Transformaciones Lineales ---\n";

        // Leer matriz A (int)
        std::cout << "Prueba Matriz ENTERA (Matriz A)" << std::endl;
        int a_f, a_c;
        std::cout << "Ingresar filas y columnas para A (ej: 2 3): ";
        if (!(std::cin >> a_f >> a_c)) {
            std::cerr << "Entrada invalida" << std::endl;
            return 1;
        }
        MatrizDinamica<int> A(a_f, a_c);
        std::cout << "Ingresar valores para A (" << a_f << "x" << a_c << "):" << std::endl;
        for (int i = 0; i < a_f; ++i) {
            for (int j = 0; j < a_c; ++j) {
                std::cout << "A[" << i << "," << j << "]: ";
                if (!(std::cin >> A(i,j))) {
                    std::cerr << "Entrada invalida" << std::endl;
                    return 1;
                }
            }
        }

        // Redimensionar A segun ejemplo (opcional)
        std::cout << "\n>> Redimensionando Matriz A <<" << std::endl;
        int new_f = a_f + 1; // ejemplo: aumentar una fila
        int new_c = std::max(a_c, a_c); // mantener columnas
        A.redimensionar(new_f, new_c);
        std::cout << "A redimensionada a " << new_f << "x" << new_c << ". Datos conservados:" << std::endl;
        std::cout << A << std::endl << std::endl;

        // Leer matriz B (float)
        std::cout << "Prueba Multiplicacion (Tipo FLOAT) - Matriz B" << std::endl;
        int b_f, b_c;
        std::cout << "Ingresar filas y columnas para B (ej: 3 2): ";
        if (!(std::cin >> b_f >> b_c)) {
            std::cerr << "Entrada invalida" << std::endl;
            return 1;
        }
        MatrizDinamica<float> B(b_f, b_c);
        std::cout << "Ingresar valores para B (" << b_f << "x" << b_c << "):" << std::endl;
        for (int i = 0; i < b_f; ++i) {
            for (int j = 0; j < b_c; ++j) {
                std::cout << "B[" << i << "," << j << "]: ";
                if (!(std::cin >> B(i,j))) {
                    std::cerr << "Entrada invalida" << std::endl;
                    return 1;
                }
            }
        }

        // Validar dimensiones antes de multiplicar
        if (A.getColumnas() != B.getFilas()) {
            std::cerr << "Dimensiones incompatibles para multiplicacion: A.cols=" << A.getColumnas()
                      << " B.rows=" << B.getFilas() << std::endl;
            return 1;
        }

        // convertir A a float y multiplicar
        auto Af = MatrizDinamica<float>(A);
        auto C = MatrizDinamica<float>::multiplicar(Af, B);

        std::cout << "\nMatriz C = A * B (Resultado " << C.getFilas() << "x" << C.getColumnas() << "):" << std::endl;
        std::cout << C << std::endl << std::endl;

        std::cout << "Liberando memoria de todas las matrices..." << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}