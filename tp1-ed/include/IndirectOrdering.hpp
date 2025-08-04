#ifndef INDIRECTORDERING_H
#define INDIRECTORDERING_H

#include <string>
#include <fstream>
#include <sstream>
#include "AuxVector.hpp"

// Estrutura Person construída baseada na entrada de dados dada
struct Person {
  std::string name;
  std::string cpf;
  std::string address;
  std::string others;
};

class IndirectOrdering {
  private:
    int amountAttributes;
    AuxVector<std::string> _attributes;
    AuxVector<Person> _persons;
    int _size;

    // Métodos auxiliares
    void swap(int& a, int& b);
    int partition(int* indexes, int l, int r, const std::string& key);
    void merge(int* indexes, int* temp, int l, int mid, int r, const std::string& key);
    bool compareBy(const Person& a, const Person& b, const std::string& key, int indexA, int indexB);
    int* createIndices();
    void print(int* indexes);

  public:
    IndirectOrdering(std::string fileName);

    // Métodos principais
    void quickSort(int* indexes, int l, int r, const std::string& key);
    void selectionSort(int* indexes, const std::string& key);
    void mergeSort(int* indexes, int* temp, int l, int r, const std::string& key);
    void startOrdering();
};

#endif