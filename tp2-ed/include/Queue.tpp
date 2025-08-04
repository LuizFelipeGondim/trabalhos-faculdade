#include "Queue.hpp"

// Construtor que inicializa os atributos da fila com a capacidade máxima especificada
// e aloca memória para o armazenamento dos elementos.
template <typename T>
Queue<T>::Queue(int maxCapacity)
  : elementFront(0), size(0), capacity(maxCapacity) {
  data = new T[capacity];
}

// Destrutor que libera a memória alocada para os dados da fila.
template <typename T>
Queue<T>::~Queue() {
  delete[] data;
}

// Retorna o número de elementos atualmente na fila.
template <typename T>
int Queue<T>::getSize() const {
  return size;
}

// Verifica se a fila está vazia.
template <typename T>
bool Queue<T>::empty() const {
  return size == 0;
}

// Adiciona um novo elemento ao final da fila. Lança uma exceção se a fila estiver cheia.
template <typename T>
void Queue<T>::enqueue(T item) {
  if (size == capacity) {
    throw std::overflow_error("Queue is full!");
  }
  int position = (elementFront + size) % capacity; // Calcula a posição circular.
  data[position] = item;
  ++size;
}

// Remove e retorna o elemento na frente da fila. Lança uma exceção se a fila estiver vazia.
template <typename T>
T Queue<T>::dequeue() {
  if (empty()) {
    throw std::underflow_error("Queue is empty!");
  }
  
  T item = data[elementFront];
  elementFront = (elementFront + 1) % capacity; // Avança a frente da fila de forma circular.
  --size;
  return item;
}

// Retorna o elemento na frente da fila sem removê-lo. Lança uma exceção se a fila estiver vazia.
template <typename T>
T Queue<T>::front() const {
  if (empty()) {
    throw std::underflow_error("Queue is empty!");
  }

  return data[elementFront];
}

// Limpa a fila, redefinindo os índices e o tamanho para seus estados iniciais.
template <typename T>
void Queue<T>::clear() {
  elementFront = 0;
  size = 0;
}