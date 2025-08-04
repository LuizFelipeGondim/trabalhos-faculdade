#include <iostream>
#include <unistd.h>
#include "Schedule.hpp"

int main(int argc, char *argv[]) {
  std::string fileName;

  try {

    if (optind < argc) {
      fileName = argv[optind]; 
    } else {
      throw std::runtime_error("Erro: Nome do arquivo não especificado.");
    }
    Schedule schedule(fileName);
    schedule.showStatistics();
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
  }
  
  return 0;
}