#include <iostream>
#include <limits>
#include "MatrizDinamica.h"

// helpers para lectura robusta
bool readInt(const std::string &prompt, int &out, int maxRetries = 3) {
    for (int attempt = 0; attempt < maxRetries; ++attempt) {
        std::cout << prompt;
        if (std::cin >> out) return true;
        std::cerr << "Entrada invalida. Intentos restantes: " << (maxRetries - attempt - 1) << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return false;
}

bool readFloat(const std::string &prompt, float &out, int maxRetries = 3) {
    for (int attempt = 0; attempt < maxRetries; ++attempt) {
        std::cout << prompt;
        if (std::cin >> out) return true;
        std::cerr << "Entrada invalida. Intentos restantes: " << (maxRetries - attempt - 1) << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    return false;
}

int main() {
    try {
        std::cout << "--- Sistema de Analisis de Transformaciones Lineales ---\n";

        // Crear A 2x3 y leer valores
        std::cout << "Prueba Matriz ENTERA (Matriz A)" << std::endl;
        const int a_f = 2, a_c = 3;
        MatrizDinamica<int> A(a_f, a_c);
        std::cout << "Ingresar valores para A (2x3):" << std::endl;
        for (int i = 0; i < a_f; ++i) {
            for (int j = 0; j < a_c; ++j) {
                int v;
                if (!readInt(std::string("A[") + std::to_string(i) + "," + std::to_string(j) + "]: ", v)) {
                    std::cerr << "Fallo en lectura de A" << std::endl;
                    return 1;
                }
                A(i,j) = v;
            }
        }

        // Redimensionar A a 3x3 como en el ejemplo
        std::cout << "\n>> Redimensionando Matriz A <<" << std::endl;
        const int new_f = 3, new_c = 3;
        A.redimensionar(new_f, new_c);
        std::cout << "A redimensionada a 3x3. Datos conservados:" << std::endl;
        std::cout << A << std::endl << std::endl;

        // Leer B 3x2 (float)
        std::cout << "Prueba de Multiplicacion (Tipo FLOAT) - Matriz B" << std::endl;
        const int b_f = 3, b_c = 2;
        MatrizDinamica<float> B(b_f, b_c);
        std::cout << "Ingresar valores para B (3x2):" << std::endl;
        for (int i = 0; i < b_f; ++i) {
            for (int j = 0; j < b_c; ++j) {
                float v;
                if (!readFloat(std::string("B[") + std::to_string(i) + "," + std::to_string(j) + "]: ", v)) {
                    std::cerr << "Fallo en lectura de B" << std::endl;
                    return 1;
                }
                B(i,j) = v;
            }
        }

        // Validar dimensiones
        if (A.getColumnas() != B.getFilas()) {
            std::cerr << "Dimensiones incompatibles: A.cols=" << A.getColumnas() << " B.rows=" << B.getFilas() << std::endl;
            return 1;
        }

        // convertir A a float y multiplicar
        auto Af = MatrizDinamica<float>(A);
        auto C = MatrizDinamica<float>::multiplicar(Af, B);

        std::cout << "\nMatriz C = A * B (Resultado):" << std::endl;
        std::cout << C << std::endl << std::endl;

        std::cout << "Liberando memoria de todas las matrices..." << std::endl;
    } catch (const std::bad_alloc &ba) {
        std::cerr << "Error de memoria: " << ba.what() << std::endl;
        return 2;
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}