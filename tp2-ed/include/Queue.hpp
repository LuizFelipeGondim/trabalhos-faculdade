#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include <stdexcept>

template <typename T>
class Queue {
private:
  T* data; 
  int elementFront;       
  int size;        
  int capacity;    

public:
  Queue(int maxCapacity = 5000);
  ~Queue();

  int getSize() const;
  bool empty() const;
  void enqueue(T item);
  T dequeue();
  T front() const;
  void clear();
};

#include "Queue.tpp"

#endif
