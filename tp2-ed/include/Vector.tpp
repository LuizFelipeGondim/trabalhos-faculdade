#include "Vector.hpp"
#include <iostream>

// Construtor padrão
template <typename T>
Vector<T>::Vector() : data(nullptr), size(0), tamMax(0) {}

// Construtor com capacidade inicial
template <typename T>
Vector<T>::Vector(int tamMax)
  : data(new T[tamMax]), size(0), tamMax(tamMax) {}

// Construtor de cópia
template <typename T>
Vector<T>::Vector(const Vector<T>& otherVector)
    : data(new T[otherVector.tamMax]), size(otherVector.size), tamMax(otherVector.tamMax) {
  for (int i = 0; i < size; ++i) {
    data[i] = otherVector.data[i];
  }
}

// Operador de atribuição 
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& otherVector) {
  if (this != &otherVector) { 
    delete[] data;
    tamMax = otherVector.tamMax;
    size = otherVector.size;
    data = new T[tamMax];

    for (int i = 0; i < size; ++i) {
      data[i] = otherVector.data[i];
    }
  }
  return *this;
}

// Destrutor
template <typename T>
Vector<T>::~Vector() {
  delete[] data;
}

// Redimensiona o vetor para uma nova capacidade
template <typename T>
void Vector<T>::resize(int newTamMax) {
  T* newData = new T[newTamMax];
  
  for (int i = 0; i < size; ++i) {
    newData[i] = data[i];
  }
  
  delete[] data;
  data = newData;
  tamMax = newTamMax;
}

// Adiciona um elemento ao final
template <typename T>
void Vector<T>::push(const T& value) {
  if (size == tamMax) {
    int newTamMax = (tamMax == 0) ? 1 : tamMax * 2;
    resize(newTamMax);
  }

  data[size++] = value;
}

// Remove o último elemento do vetor e retorna o ponteiro removido
template <typename T>
T Vector<T>::pop() {
  if (size == 0) {
    throw std::underflow_error("O vetor está vazio, não é possível remover elementos.");
  }
  
  T removedElement = data[size - 1];
  --size;

  return removedElement;
}

// Remove o elemento do vetor correspondente ao índice informado
template <typename T>
void Vector<T>::pop(int index) {
  if (index < 0 || index >= size) {
    throw std::out_of_range("Índice fora do intervalo!");
  }

  for (int i = index; i < size - 1; ++i) {
    data[i] = data[i + 1];
  }
  --size;
}

// Métodos begin e end
template <typename T>
T* Vector<T>::begin() { return data; }

template <typename T>
T* Vector<T>::end() { return data + size; }

template <typename T>
const T* Vector<T>::begin() const { return data; }

template <typename T>
const T* Vector<T>::end() const { return data + size; }

// Operador de acesso (não constante)
template <typename T>
T& Vector<T>::operator[](int index) {
  if (index < 0 || index >= size) {
    throw std::out_of_range("índice fora do intervalo");
  }

  return data[index];
}

// Operador de acesso (constante)
template <typename T>
const T& Vector<T>::operator[](int index) const {
  if (index < 0 || index >= size) {
    throw std::out_of_range("índice fora do intervalo");
  }

  return data[index];
}

// Retorna o último elemento
template <typename T>
const T& Vector<T>::back() const {
  return data[size - 1];
}

// Retorna o tamanho lógico
template <typename T>
int Vector<T>::getSize() const {
  return size;
}

// Retorna a capacidade
template <typename T>
int Vector<T>::getTamMax() const {
  return tamMax;
}