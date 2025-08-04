#include <iostream>
#include <unistd.h>
#include "Sistema.hpp"

// Captura o nome do arquivo de entrada na linha de comando e inicia a aplicação
int main(int argc, char *argv[]) {
  std::string fileName;

  try {

    if (optind < argc) {
      fileName = argv[optind]; 
    } else {
      throw std::runtime_error("Erro: Nome do arquivo não especificado.");
    }
    
    Sistema sistema(fileName);
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
  }
  
  return 0;
}