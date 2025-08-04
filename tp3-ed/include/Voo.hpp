#ifndef VOO_HPP
#define VOO_HPP

#include <iostream>
#include <string>
#include <ctime>

// Classe responsável por representar cada voo
class Voo {
private:
  std::string origem;
  std::string destino;
  float preco;
  int assentosDisponiveis;
  std::time_t dataHoraPartida;
  std::time_t dataHoraChegada;
  std::time_t duracaoVoo;
  int numeroParadas;

  // Função auxiliar para formatar data/hora
  std::string formatarHorario(std::time_t dataHora) const;

public:
  
  Voo(
    std::string origem, 
    std::string destino, 
    float preco, 
    int assentosDisponiveis, 
    std::time_t dataHoraPartida, 
    std::time_t dataHoraChegada, 
    int numeroParadas
  );

  // Exibe os dados formatados
  void mostrarVoo() const;

  float getPreco() const;
  time_t getDuracao() const;
  int getNumeroParadas() const;
};

#endif
