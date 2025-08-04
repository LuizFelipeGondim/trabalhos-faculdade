#ifndef VECTOR_H
#define VECTOR_H

#include <stdexcept>

// Implementação de um vector criada para facilitar a codificação do programa
template <typename T>
class Vector {
private:
  T* data;     // Ponteiro para os elementos
  int size;    // Tamanho lógico
  int tamMax;  // Capacidade máxima

public:
  Vector();
  Vector(int tamMax);
  Vector(const Vector<T>& otherVector);
  Vector<T>& operator=(const Vector<T>& otherVector);

  ~Vector();

  void push(const T& value);
  T pop();
  void pop(int index);
  T* begin();
  T* end();
  const T* begin() const;
  const T* end() const;

  // Redimensiona o vetor para uma nova capacidade
  void resize(int newTamMax);
  
  // Operadores de acesso
  T& operator[](int index);
  const T& operator[](int index) const;
  const T& back() const; 

  // Métodos utilitários
  int getSize() const;
  int getTamMax() const;
};

#include "Vector.tpp"

#endif