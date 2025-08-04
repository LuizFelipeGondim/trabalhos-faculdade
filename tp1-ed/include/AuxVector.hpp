#ifndef AUXVECTOR_H
#define AUXVECTOR_H

#include <stdexcept>

// Implementação de um vector criada para facilitar a codificação do programa
template <typename T>
class AuxVector {
  private:
    T* _data;     // Ponteiro para os elementos
    int _size;    // Tamanho lógico
    int _tamMax;  // Capacidade máxima

  public:
    AuxVector();
    AuxVector(int tamMax);
    AuxVector(const AuxVector<T>& otherVector);
    AuxVector<T>& operator=(const AuxVector<T>& otherVector);

    ~AuxVector();

    void push(const T& value);
    T* begin();
    T* end();
    const T* begin() const;
    const T* end() const;

    // Redimensiona o vetor para uma nova capacidade
    void resize(int tamMax);
    
    // Operadores de acesso
    T& operator[](int index);
    const T& operator[](int index) const;

    // Métodos utilitários
    int getSize() const;
    int getTamMax() const;
};

#include "AuxVector.tpp"

#endif
