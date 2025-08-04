#ifndef EVENTS_HPP
#define EVENTS_HPP

#include "Vector.hpp"
#include <stdexcept>

struct Event {
  int id;
  time_t timestamp;

  Event(int id, time_t h) : id(id), timestamp(h){}

  Event() : id(0), timestamp(0){}
};

class Events {
private:
  Vector<Event> heap;

  void heapifyDown(int index);
  void heapifyUp(int index);
  bool compareEvents(const Event& a, const Event& b);
  void swap(int a, int b);

public:

  void push(const Event& event);
  Event top();
  void pop();
  bool empty() const;
  int size() const;
};

#endif