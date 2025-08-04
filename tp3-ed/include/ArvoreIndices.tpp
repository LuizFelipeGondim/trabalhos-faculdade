#include "ArvoreIndices.hpp"

template <typename T>
ArvoreIndices<T>::ArvoreIndices() : raiz(0) {}

template <typename T>
ArvoreIndices<T>::~ArvoreIndices() {
  destruirArvore(raiz);
}

// Método padrão de uma árvore AVL
template <typename T>
void ArvoreIndices<T>::destruirArvore(No* no) {
  if (no) {
    destruirArvore(no->esquerda);
    destruirArvore(no->direita);
    delete no;
  }
}

// Método padrão de uma árvore AVL
template <typename T>
int ArvoreIndices<T>::obterAltura(No* no) {
  return no ? no->altura : 0;
}

// Método padrão de uma árvore AVL
template <typename T>
int ArvoreIndices<T>::fatorBalanceamento(No* no) {
  return no ? obterAltura(no->esquerda) - obterAltura(no->direita) : 0;
}

// Método padrão de uma árvore AVL
template <typename T>
typename ArvoreIndices<T>::No* ArvoreIndices<T>::rotacaoDireita(No* y) {
  No* x = y->esquerda;
  No* T2 = x->direita;

  x->direita = y;
  y->esquerda = T2;

  y->altura = (obterAltura(y->esquerda) > obterAltura(y->direita) ? obterAltura(y->esquerda) : obterAltura(y->direita)) + 1;
  x->altura = (obterAltura(x->esquerda) > obterAltura(x->direita) ? obterAltura(x->esquerda) : obterAltura(x->direita)) + 1;

  return x;
}

// Método padrão de uma árvore AVL
template <typename T>
typename ArvoreIndices<T>::No* ArvoreIndices<T>::rotacaoEsquerda(No* x) {
  No* y = x->direita;
  No* T2 = y->esquerda;

  y->esquerda = x;
  x->direita = T2;

  x->altura = (obterAltura(x->esquerda) > obterAltura(x->direita) ? obterAltura(x->esquerda) : obterAltura(x->direita)) + 1;
  y->altura = (obterAltura(y->esquerda) > obterAltura(y->direita) ? obterAltura(y->esquerda) : obterAltura(y->direita)) + 1;

  return y;
}

// Método padrão de uma árvore AVL
template <typename T>
typename ArvoreIndices<T>::No* ArvoreIndices<T>::balancear(No* no) {
  int balance = fatorBalanceamento(no);

  if (balance > 1 && fatorBalanceamento(no->esquerda) >= 0)
    return rotacaoDireita(no);

  if (balance > 1 && fatorBalanceamento(no->esquerda) < 0) {
    no->esquerda = rotacaoEsquerda(no->esquerda);
    return rotacaoDireita(no);
  }

  if (balance < -1 && fatorBalanceamento(no->direita) <= 0)
    return rotacaoEsquerda(no);

  if (balance < -1 && fatorBalanceamento(no->direita) > 0) {
    no->direita = rotacaoDireita(no->direita);
    return rotacaoEsquerda(no);
  }

  return no;
}

// Método padrão de uma árvore AVL
template <typename T>
typename ArvoreIndices<T>::No* ArvoreIndices<T>::inserir(No* no, const T& chave, int indiceVoo) {
  if (!no) {
    No* novoNo = new No(chave);
    novoNo->indices.push(indiceVoo);
    return novoNo;
  }

  if (chave < no->chave) {
    no->esquerda = inserir(no->esquerda, chave, indiceVoo);
  } else if (chave > no->chave) {
    no->direita = inserir(no->direita, chave, indiceVoo);
  } else {
    no->indices.push(indiceVoo);
    return no;
  }

  no->altura = (obterAltura(no->esquerda) > obterAltura(no->direita) ? obterAltura(no->esquerda) : obterAltura(no->direita)) + 1;

  return balancear(no);
}

// Método padrão de uma árvore AVL
template <typename T>
void ArvoreIndices<T>::inserir(const T& chave, int indiceVoo) {
  raiz = inserir(raiz, chave, indiceVoo);
}

// Método que retorna os índices dos voos que atendem à consulta
template <typename T>
Vector<int> ArvoreIndices<T>::buscar(T valor, std::string comparador) {
  Vector<int> indicesEncontrados;

  // Iniciar a busca recursiva a partir da raiz
  buscarRecursivo(raiz, valor, comparador, indicesEncontrados);

  return indicesEncontrados;
}

// Busca recursiva para fazer as consultas ao longo da árvore
template <typename T>
void ArvoreIndices<T>::buscarRecursivo(
  No* no, 
  const T& valor, 
  const std::string& comparador, 
  Vector<int>& indicesEncontrados
) {
  if (!no) return; // Se o nó for nulo, retorne

  bool resultado = false;
  if (comparador == "==") {
    resultado = (no->chave == valor); // Comparação de igualdade
  } else if (comparador == "<=") {
    resultado = (no->chave <= valor); // Comparação menor ou igual
  } else if (comparador == ">=") {
    resultado = (no->chave >= valor); // Comparação maior ou igual
  } else if (comparador == "<") {
    resultado = (no->chave < valor); // Comparação menor
  } else if (comparador == ">") {
    resultado = (no->chave > valor); // Comparação maior
  }

  // Se o nó atender ao critério, adicionar seus índices ao vetor
  if (resultado)
    for (int i = 0; i < no->indices.getSize(); ++i)
      indicesEncontrados.push(no->indices[i]);

  // Continuar a busca na subárvore esquerda e direita
  buscarRecursivo(no->esquerda, valor, comparador, indicesEncontrados);
  buscarRecursivo(no->direita, valor, comparador, indicesEncontrados);
}
