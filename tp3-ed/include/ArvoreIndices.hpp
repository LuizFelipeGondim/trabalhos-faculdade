#ifndef ARVOREINDICES_HPP
#define ARVOREINDICES_HPP

#include "Vector.hpp"

// Arvore AVL que armazena um determinado tipo de atributo do voo
// Em cada nó, um atributo está associado a um vetor de indices com os voos que possuem esse atributo
template <typename T>
class ArvoreIndices {
private:

  struct No {
    T chave;
    Vector<int> indices; // Vetor com índices dos voos que possuem essa chave
    No* esquerda;
    No* direita;
    int altura;

    No(const T& chave) : chave(chave), esquerda(0), direita(0), altura(1) {}
  };

  No* raiz;

  void destruirArvore(No* no);
  int obterAltura(No* no);
  int fatorBalanceamento(No* no);
  No* rotacaoDireita(No* y);
  No* rotacaoEsquerda(No* x);
  No* balancear(No* no);
  No* inserir(No* no, const T& chave, int indiceVoo);

  void buscarRecursivo(
    No* no, 
    const T& valor, 
    const std::string& comparador, 
    Vector<int>& indicesEncontrados
  ); 

public:
  ArvoreIndices();
  ~ArvoreIndices();

  void inserir(const T& chave, int indiceVoo);
  Vector<int> buscar(T valor, std::string comparador);
};

#include "ArvoreIndices.tpp" 

#endif
