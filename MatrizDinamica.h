// MatrizDinamica.h
#ifndef MATRIZDINAMICA_H
#define MATRIZDINAMICA_H

#include <iostream>
#include <stdexcept>
#include <algorithm> // for std::min

template <typename T>
class MatrizDinamica {
private:
    T **datos;
    int filas;
    int columnas;

    // Allocate f x c matrix. Strong exception safety: no leaks if allocation fails.
    void allocate(int f, int c) {
        if (f <= 0 || c <= 0) {
            datos = nullptr;
            filas = columnas = 0;
            return;
        }
        T **tmp = nullptr;
        try {
            tmp = new T*[f];
        } catch (...) {
            // propagate bad_alloc
            throw;
        }

        int allocatedRows = 0;
        try {
            for (int i = 0; i < f; ++i) {
                tmp[i] = new T[c];
                // initialize to zero
                for (int j = 0; j < c; ++j) tmp[i][j] = T();
                ++allocatedRows;
            }
        } catch (...) {
            // cleanup partially allocated rows
            for (int k = 0; k < allocatedRows; ++k) delete[] tmp[k];
            delete[] tmp;
            throw;
        }

        datos = tmp;
        filas = f;
        columnas = c;
    }

    void deallocate() {
        if (!datos) return;
        for (int i = 0; i < filas; ++i) {
            delete[] datos[i];
        }
        delete[] datos;
        datos = nullptr;
        filas = columnas = 0;
    }

public:
    //constructor
    MatrizDinamica(int f = 0, int c = 0) : datos(nullptr), filas(0), columnas(0) {
        if (f > 0 && c > 0) {
            try {
                allocate(f, c);
            } catch (...) {
                // leave object in valid empty state
                datos = nullptr; filas = columnas = 0;
                throw;
            }
        }
    }

    //destructor
    ~MatrizDinamica() {
        deallocate();
    }

    //copy constructor
    MatrizDinamica(const MatrizDinamica &other) : datos(nullptr), filas(0), columnas(0) {
        if (other.datos) {
            try {
                allocate(other.filas, other.columnas);
                for (int i = 0; i < filas; ++i)
                    for (int j = 0; j < columnas; ++j)
                        datos[i][j] = other.datos[i][j];
            } catch (...) {
                datos = nullptr; filas = columnas = 0;
                throw;
            }
        }
    }

    //converting constructor from MatrizDinamica<U> to MatrizDinamica<T>
    template <typename U>
    MatrizDinamica(const MatrizDinamica<U> &other) : datos(nullptr), filas(0), columnas(0) {
        if (other.getFilas() > 0 && other.getColumnas() > 0) {
            try {
                allocate(other.getFilas(), other.getColumnas());
                for (int i = 0; i < filas; ++i)
                    for (int j = 0; j < columnas; ++j)
                        datos[i][j] = static_cast<T>(other(i,j));
            } catch (...) {
                datos = nullptr; filas = columnas = 0;
                throw;
            }
        }
    }

    //move constructor
    MatrizDinamica(MatrizDinamica &&other) noexcept : datos(other.datos), filas(other.filas), columnas(other.columnas) {
        other.datos = nullptr;
        other.filas = other.columnas = 0;
    }

    //copy assignment
    MatrizDinamica& operator=(const MatrizDinamica &other) {
        if (this == &other) return *this;
        // allocate new storage then swap
        MatrizDinamica tmp;
        if (other.datos) {
            try {
                tmp.allocate(other.filas, other.columnas);
                for (int i = 0; i < other.filas; ++i)
                    for (int j = 0; j < other.columnas; ++j)
                        tmp.datos[i][j] = other.datos[i][j];
            } catch (...) {
                // leave this unchanged and rethrow
                throw;
            }
        }
        swap(tmp);
        return *this;
    }

    //move assignment
    MatrizDinamica& operator=(MatrizDinamica &&other) noexcept {
        if (this == &other) return *this;
        deallocate();
        datos = other.datos;
        filas = other.filas;
        columnas = other.columnas;
        other.datos = nullptr;
        other.filas = other.columnas = 0;
        return *this;
    }

    void swap(MatrizDinamica &other) noexcept {
        std::swap(datos, other.datos);
        std::swap(filas, other.filas);
        std::swap(columnas, other.columnas);
    }

    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }

    //acceso con comprobacion minima
    T& operator()(int i, int j) {
        if (!datos || i < 0 || i >= filas || j < 0 || j >= columnas)
            throw std::out_of_range("indice fuera de rango");
        return datos[i][j];
    }

    const T& operator()(int i, int j) const {
        if (!datos || i < 0 || i >= filas || j < 0 || j >= columnas)
            throw std::out_of_range("indice fuera de rango");
        return datos[i][j];
    }

    // print en formato tabla
    void print(std::ostream &os = std::cout) const {
        if (!datos) {
            os << "[]";
            return;
        }
        for (int i = 0; i < filas; ++i) {
            os << "| ";
            for (int j = 0; j < columnas; ++j) {
                os << datos[i][j];
                if (j + 1 < columnas) os << " | ";
            }
            os << " |";
            if (i + 1 < filas) os << '\n';
        }
    }


// (operator<< se define despues del cierre de la clase)
    //redimensionar conservando datos validos
    void redimensionar(int nuevaF, int nuevaC) {
        if (nuevaF <= 0 || nuevaC <= 0) {
            // libera y deja en 0x0
            deallocate();
            return;
        }

        T **nuevo = nullptr;
        try {
            nuevo = new T*[nuevaF];
            for (int i = 0; i < nuevaF; ++i) {
                nuevo[i] = new T[nuevaC];
                for (int j = 0; j < nuevaC; ++j) nuevo[i][j] = T();
            }
        } catch (...) {
            // cleanup if partially allocated
            if (nuevo) {
                for (int k = 0; k < nuevaF; ++k) {
                    if (nuevo[k]) delete[] nuevo[k];
                }
                delete[] nuevo;
            }
            throw;
        }

        // copiar datos
        int minF = std::min(filas, nuevaF);
        int minC = std::min(columnas, nuevaC);
        for (int i = 0; i < minF; ++i)
            for (int j = 0; j < minC; ++j)
                nuevo[i][j] = datos[i][j];

        // liberar antiguo
        deallocate();

        // asignar nuevo
        datos = nuevo;
        filas = nuevaF;
        columnas = nuevaC;
    }

    //multiplicacion statica: C = A * B
    static MatrizDinamica multiplicar(const MatrizDinamica &A, const MatrizDinamica &B) {
        if (A.columnas != B.filas)
            throw std::invalid_argument("dimensiones incompatibles para multiplicacion");
        MatrizDinamica C(A.filas, B.columnas);
        for (int i = 0; i < C.filas; ++i) {
            for (int j = 0; j < C.columnas; ++j) {
                T sum = T();
                for (int k = 0; k < A.columnas; ++k) {
                    sum += A.datos[i][k] * B.datos[k][j];
                }
                C.datos[i][j] = sum;
            }
        }
        return C;
    }
};

    // sobrecarga de operator<< para impresion facil (fuera de la clase)
    template <typename T>
    std::ostream& operator<<(std::ostream &os, const MatrizDinamica<T> &m) {
        m.print(os);
        return os;
    }

    #endif