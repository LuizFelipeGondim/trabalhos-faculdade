#include "Voo.hpp"
#include <iomanip>
#include <sstream>

Voo::Voo(
  std::string origem, 
  std::string destino, 
  float preco, 
  int assentosDisponiveis, 
  std::time_t dataHoraPartida, 
  std::time_t dataHoraChegada, 
  int numeroParadas
  ) : 
  origem(origem), 
  destino(destino), 
  preco(preco), 
  assentosDisponiveis(assentosDisponiveis), 
  dataHoraPartida(dataHoraPartida), 
  dataHoraChegada(dataHoraChegada), 
  duracaoVoo(dataHoraChegada - dataHoraPartida),
  numeroParadas(numeroParadas) {}

// Transforma os segundos do time_t no formato de data esperado
std::string Voo::formatarHorario(std::time_t dataHora) const {
  std::tm* tm = std::localtime(&dataHora);
  std::stringstream ss;
  ss << std::put_time(tm, "%Y-%m-%dT%H:%M:%S");
  return ss.str();
}

// Imprime as informações de voo no formato esperado pelo trabalho
void Voo::mostrarVoo() const {
  std::cout << origem << " " << destino << " "
            << preco << " "
            << assentosDisponiveis << " "
            << formatarHorario(dataHoraPartida) << " "
            << formatarHorario(dataHoraChegada) << " "
            << numeroParadas << "\n";
}

// Retorna preco para ser usado nas comparações do insertionSort
float Voo::getPreco() const {
  return preco;
}

// Retorna duracaoVoo para ser usado nas comparações do insertionSort 
time_t Voo::getDuracao() const {
  return duracaoVoo;
}

// Retorna numeroParadas para ser usado nas comparações do insertionSort
int Voo::getNumeroParadas() const {
  return numeroParadas;
}
