#include "Sistema.hpp"

// Efetua a leitura dos voos e faz as consultas
Sistema::Sistema(const std::string& nomeArquivo) {
  std::ifstream arquivo(nomeArquivo);

  if (!arquivo.is_open())
    throw std::runtime_error("Erro ao abrir o arquivo");

  lerVoos(arquivo);
  fazerConsultas(arquivo);
}

Sistema::~Sistema() {
  for (auto voo : voos)
    delete voo;
}

// Função para fazer a conversão manual de data/hora para time_t
std::time_t Sistema::parseDataHora(const std::string& data, const std::string& hora) {
  std::tm tm = {}; 
  std::stringstream dataStream(data);

  char separador; // Para ignorar os separadores '-'
  dataStream >> tm.tm_year >> separador >> tm.tm_mon >> separador >> tm.tm_mday;

  tm.tm_year -= 1900;
  tm.tm_mon -= 1;

  std::stringstream horaStream(hora);
  horaStream >> tm.tm_hour >> separador >> tm.tm_min >> separador >> tm.tm_sec;

  if (dataStream.fail() || horaStream.fail())
    throw std::runtime_error("Erro ao converter data/hora");

  return std::mktime(&tm); // Converte para time_t
}

// Faz a leitura dos voos no arquivo de entrada e os salva no vetor voos, ainda preenche as árvores de índices 
void Sistema::lerVoos(std::ifstream& arquivo) {
  std::string linha;
  quantidadeRegistros = 0;

  // Ler a primeira linha para obter a quantidade de registros
  if (std::getline(arquivo, linha))
    quantidadeRegistros = std::stoi(linha);

  // Ler os voos com base na quantidade informada
  for (int i = 0; i < quantidadeRegistros; ++i) {
    if (!std::getline(arquivo, linha)) break;

    std::stringstream ss(linha);
    std::string origem, destino, precoStr, assentosStr, partidaStr, chegadaStr, paradasStr;

    ss >> origem >> destino >> precoStr >> assentosStr >> partidaStr >> chegadaStr >> paradasStr;

    float preco = std::stof(precoStr);
    int assentos = std::stoi(assentosStr);
    int paradas = std::stoi(paradasStr);
    std::time_t partida = parseDataHora(partidaStr.substr(0, 10), partidaStr.substr(11, 8));
    std::time_t chegada = parseDataHora(chegadaStr.substr(0, 10), chegadaStr.substr(11, 8)); 

    // Adiciona o voo ao vetor
    voos.push(new Voo(origem, destino, preco, assentos, partida, chegada, paradas));

    // Preenche as árvores de indices com os respectivos atributos e indices do voo
    arvoreIndicesOrigem.inserir(origem, i);
    arvoreIndicesDestino.inserir(destino, i);
    arvoreIndicesPreco.inserir(preco, i);
    arvoreIndicesAssentos.inserir(assentos, i);
    arvoreIndicesPartida.inserir(partida, i);
    arvoreIndicesChegada.inserir(chegada, i);
    arvoreIndicesDuracao.inserir(chegada - partida, i);
    arvoreIndicesParadas.inserir(paradas, i);
  }
}

// Método responsável pela leitura das consultas, processamentos das mesmas e impressão do resultado
void Sistema::fazerConsultas(std::ifstream& arquivo) {
  Vector<int> indicesConsulta;
  std::string linha;
  int quantidadeConsultas = 0;

  // Ler a primeira linha para obter a quantidade de consultas
  if (std::getline(arquivo, linha))
    quantidadeConsultas = std::stoi(linha);

  // Ler os voos com base na quantidade informada
  for (int i = 0; i < quantidadeConsultas; ++i) {
    if (!std::getline(arquivo, linha)) break;

    std::stringstream ss(linha);
    std::string quant, trigrama, stringConsulta;

    ss >> quant >> trigrama >> stringConsulta;
    int quantidadeRespostas = std::stoi(quant);

    Consulta consulta(stringConsulta);
    
    indicesConsulta = consulta.consultarVoos(
                        arvoreIndicesOrigem,
                        arvoreIndicesDestino,
                        arvoreIndicesPreco,
                        arvoreIndicesAssentos,
                        arvoreIndicesPartida,
                        arvoreIndicesChegada,
                        arvoreIndicesDuracao,
                        arvoreIndicesParadas
                      );
    
    // Ordena índices para impressão
    ordenarIndices(indicesConsulta, trigrama);

    std::cout << quant << " " << trigrama << " " << stringConsulta << std::endl;
    mostrarVoos(indicesConsulta, quantidadeRespostas);
  }
}

// Imprime os voos conforme os índices resultantes da consulta e na quantidade informada
void Sistema::mostrarVoos(Vector<int> indicesConsulta, int tam) const {
  for (int i = 0; i < tam; i++)
    voos[indicesConsulta[i]]->mostrarVoo();
}

// Compara os voos seguindo a ordem do trigrama informado na consulta
bool Sistema::compararVoos(const Voo& voo1, const Voo& voo2, const std::string& trigrama) {
  for (char criterio : trigrama) {
    if (criterio == 'p') {
      if (voo1.getPreco() != voo2.getPreco()) 
        return voo1.getPreco() < voo2.getPreco();
    } 
    else if (criterio == 'd') {
      if (voo1.getDuracao() != voo2.getDuracao()) 
        return voo1.getDuracao() < voo2.getDuracao();
    } 
    else if (criterio == 's') {
      if (voo1.getNumeroParadas() != voo2.getNumeroParadas()) 
        return voo1.getNumeroParadas() < voo2.getNumeroParadas();
    }
  }
  return false; // Mantém a ordem original se todos os critérios forem iguais
}

// Ordena os índices utilizando o insertionSort
void Sistema::ordenarIndices(Vector<int>& indices, const std::string& trigrama) {
  for (int i = 1; i < indices.getSize(); i++) {
    int chave = indices[i];
    int j = i - 1;

    while (j >= 0 && compararVoos(*voos[chave], *voos[indices[j]], trigrama)) {
      indices[j + 1] = indices[j];
      j--;
    }
    indices[j + 1] = chave;
  }
}