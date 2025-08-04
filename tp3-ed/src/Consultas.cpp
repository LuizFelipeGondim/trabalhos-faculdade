#include "Consultas.hpp"

// Construtor responsável por pegar a consulta e separá-la em atributo, comparador e valor
// Para cada par atributo/valor, temos um elemento no vetor de consultas
Consulta::Consulta(std::string input) {
  // Expressão regular para capturar (atributo, comparador, valor)
  std::regex regexPattern(R"((\w+)(==|<=|>=|<|>)(\d+(\.\d+)?|\w+))");
  std::smatch match;

  std::string temp = input;
  while (std::regex_search(temp, match, regexPattern)) {
    consultas.push({match[1], match[2], match[3]});
    temp = match.suffix().str(); // Continua a busca no restante da string
  }
}

// Método que recebe as árvores de índices e delega a consulta para uma árvore especifica
// conforme o tipo do atributo. Ela recupera todos os índices dos voos que atendem os requisitos
Vector<int> Consulta::consultarVoos(
    ArvoreIndices<std::string>& arvoreIndicesOrigem,
    ArvoreIndices<std::string>& arvoreIndicesDestino,
    ArvoreIndices<float>& arvoreIndicesPreco,
    ArvoreIndices<int>& arvoreIndicesAssentos,
    ArvoreIndices<time_t>& arvoreIndicesPartida,
    ArvoreIndices<time_t>& arvoreIndicesChegada,
    ArvoreIndices<time_t>& arvoreIndicesDuracao,
    ArvoreIndices<int>& arvoreIndicesParadas
  ) {
  
  Vector<int> indicesFiltrados, aux;

  for (int i = 0; i < consultas.getSize(); i++) {

    if (consultas[i].atributo == "org") {
      aux = arvoreIndicesOrigem.buscar(consultas[i].valor, consultas[i].comparador);

    } else if (consultas[i].atributo == "dst") {
      aux = arvoreIndicesDestino.buscar(consultas[i].valor, consultas[i].comparador);

    } else if (consultas[i].atributo == "prc") {
      float preco = std::stof(consultas[i].valor);
      aux = arvoreIndicesPreco.buscar(preco, consultas[i].comparador);

    } else if (consultas[i].atributo == "sea") {
      int assentos = stoi(consultas[i].valor);
      aux = arvoreIndicesAssentos.buscar(assentos, consultas[i].comparador);

    } else if (consultas[i].atributo == "dep") {
      time_t partida = static_cast<time_t>(std::stoll(consultas[i].valor));
      aux = arvoreIndicesPartida.buscar(partida, consultas[i].comparador);

    } else if (consultas[i].atributo == "arr") {
      time_t chegada = static_cast<time_t>(std::stoll(consultas[i].valor));
      aux = arvoreIndicesChegada.buscar(chegada, consultas[i].comparador);

    } else if (consultas[i].atributo == "sto") {
      int paradas = stoi(consultas[i].valor);
      aux = arvoreIndicesParadas.buscar(paradas, consultas[i].comparador);

    } else if (consultas[i].atributo == "dur") {
      time_t duracao = static_cast<time_t>(std::stoll(consultas[i].valor));
      aux = arvoreIndicesDuracao.buscar(duracao, consultas[i].comparador);
    }

    // Ordena indices para ser usado no método de interseção
    aux.sort(); 

    if (indicesFiltrados.getSize() == 0) {
      indicesFiltrados = aux;
    } else {
      indicesFiltrados = intersecao(indicesFiltrados, aux);
    }
    
  }
  return indicesFiltrados;
}

// Método que gera um vetor que armazena os índices comuns aos dois vetores passados como parâmetro
Vector<int> Consulta::intersecao(Vector<int> esq, Vector<int> dir) {
  Vector<int> indicesInterseccao;
  
  // Assumimos que ambos os vetores estão ordenados
  int i = 0, j = 0;
  while (i < esq.getSize() && j < dir.getSize()) {
    if (esq[i] < dir[j]) {
      ++i;
    } else if (esq[i] > dir[j]) {
      ++j;
    } else {
      // Se os elementos forem iguais, adicionamos à interseção
      indicesInterseccao.push(esq[i]);
      ++i;
      ++j;
    }
  }

  return indicesInterseccao;
}