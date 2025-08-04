#ifndef CONSULTA_HPP
#define CONSULTA_HPP

#include <string>
#include <sstream>
#include <regex>
#include "Vector.hpp"
#include "ArvoreIndices.hpp"

struct ParametrosConsulta {
  std::string atributo;
  std::string comparador;
  std::string valor;
};

// Classe que trata as consultas 
class Consulta {
  private:
    Vector<ParametrosConsulta> consultas;
    Vector<int> intersecao(Vector<int> esq, Vector<int> dir);

  public:
    Consulta();
    Consulta(std::string consulta);

    Vector<int> consultarVoos(
      ArvoreIndices<std::string>& arvoreIndicesOrigem,
      ArvoreIndices<std::string>& arvoreIndicesDestino,
      ArvoreIndices<float>& arvoreIndicesPreco,
      ArvoreIndices<int>& arvoreIndicesAssentos,
      ArvoreIndices<time_t>& arvoreIndicesPartida,
      ArvoreIndices<time_t>& arvoreIndicesChegada,
      ArvoreIndices<time_t>& arvoreIndicesDuracao,
      ArvoreIndices<int>& arvoreIndicesParadas
    );
};

#endif