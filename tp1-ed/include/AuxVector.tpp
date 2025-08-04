#include "AuxVector.hpp"
#include <iostream>

// Construtor padrão
template <typename T>
AuxVector<T>::AuxVector() : _data(nullptr), _size(0), _tamMax(0) {}

// Construtor com capacidade inicial
template <typename T>
AuxVector<T>::AuxVector(int tamMax)
  : _data(new T[tamMax]), _size(0), _tamMax(tamMax) {}

// Construtor de cópia
template <typename T>
AuxVector<T>::AuxVector(const AuxVector<T>& otherVector)
    : _data(new T[otherVector._tamMax]), _size(otherVector._size), _tamMax(otherVector._tamMax) {
  for (int i = 0; i < _size; ++i) {
    _data[i] = otherVector._data[i];
  }
}

// Operador de atribuição 
template <typename T>
AuxVector<T>& AuxVector<T>::operator=(const AuxVector<T>& otherVector) {
  if (this != &otherVector) { 
    delete[] _data;
    _tamMax = otherVector._tamMax;
    _size = otherVector._size;
    _data = new T[_tamMax];

    for (int i = 0; i < _size; ++i) {
      _data[i] = otherVector._data[i];
    }
  }
  return *this;
}

// Destrutor
template <typename T>
AuxVector<T>::~AuxVector() {
  delete[] _data;
}

// Redimensiona o vetor para uma nova capacidade
template <typename T>
void AuxVector<T>::resize(int tamMax) {
  T* newData = new T[tamMax];
  
  for (int i = 0; i < _size; ++i) {
    newData[i] = _data[i];
  }
  
  delete[] _data;
  _data = newData;
  _tamMax = tamMax;
}

// Adiciona um elemento ao final
template <typename T>
void AuxVector<T>::push(const T& value) {
  if (_size == _tamMax) {
    int newTamMax = (_tamMax == 0) ? 1 : _tamMax * 2;
    resize(newTamMax);
  }

  _data[_size++] = value;
}

// Métodos begin e end
template <typename T>
T* AuxVector<T>::begin() { return _data; }

template <typename T>
T* AuxVector<T>::end() { return _data + _size; }

template <typename T>
const T* AuxVector<T>::begin() const { return _data; }

template <typename T>
const T* AuxVector<T>::end() const { return _data + _size; }

// Operador de acesso (não constante)
template <typename T>
T& AuxVector<T>::operator[](int index) {
  if (index < 0 || index >= _size) {
    throw std::out_of_range("índice fora do intervalo");
  }

  return _data[index];
}

// Operador de acesso (constante)
template <typename T>
const T& AuxVector<T>::operator[](int index) const {
  if (index < 0 || index >= _size) {
    throw std::out_of_range("índice fora do intervalo");
  }

  return _data[index];
}

// Retorna o tamanho lógico
template <typename T>
int AuxVector<T>::getSize() const {
  return _size;
}

// Retorna a capacidade
template <typename T>
int AuxVector<T>::getTamMax() const {
  return _tamMax;
}
