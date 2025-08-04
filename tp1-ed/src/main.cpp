#include <iostream>
#include <unistd.h>
#include "IndirectOrdering.hpp"

// A main recebe o nome do arquivo e inicia a aplicação
int main(int argc, char *argv[]) {
  std::string fileName;

  try {

    if (optind < argc) {
      fileName = argv[optind]; 
    } else {
      throw std::runtime_error("Erro: Nome do arquivo não especificado.");
    }

    IndirectOrdering ordering(fileName);
    ordering.startOrdering();

  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
  }
  
  return 0;
}
