#ifndef SISTEMA_HPP
#define SISTEMA_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept> 
#include "Vector.hpp"
#include "Voo.hpp"
#include "ArvoreIndices.hpp"
#include "Consultas.hpp"

// Classe responsável por ler os voos do arquivo de entrada, processá-los e imprimir o resultado das consultas
class Sistema {
private:
  int quantidadeRegistros;
  
  Vector<Voo*> voos;
  ArvoreIndices<std::string> arvoreIndicesOrigem;
  ArvoreIndices<std::string> arvoreIndicesDestino;
  ArvoreIndices<float> arvoreIndicesPreco;
  ArvoreIndices<int> arvoreIndicesAssentos;
  ArvoreIndices<time_t> arvoreIndicesPartida;
  ArvoreIndices<time_t> arvoreIndicesChegada;
  ArvoreIndices<time_t> arvoreIndicesDuracao;
  ArvoreIndices<int> arvoreIndicesParadas;

  bool compararVoos(const Voo& voo1, const Voo& voo2, const std::string& trigrama);
  void ordenarIndices(Vector<int>& indices, const std::string& trigrama);
  std::time_t parseDataHora(const std::string& data, const std::string& hora);
  
public:
  Sistema(const std::string& nomeArquivo);
  ~Sistema();

  void lerVoos(std::ifstream& arquivo);
  void fazerConsultas(std::ifstream& arquivo);
  void mostrarVoos(Vector<int> indicesConsulta, int tam) const;
};

#endif
