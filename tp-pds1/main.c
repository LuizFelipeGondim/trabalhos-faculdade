#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estrutura de luas
typedef struct{
  char *nome;
  int raio; 
} Lua;

//Estrutura de planetas
typedef struct {
  char *nome;
  int raio;
  int quantLuas;
  Lua *luas; //Array de luas associadas ao planeta
} Planeta;

//Estrutura de sistemas solares
typedef struct{
  int tempDescoberta;
  char *nome;
  int raio;
  int quantPlanetas;
  Planeta *planetas; //Array de planetas associados ao sistema solar
} SistemaSolar;

//Protótipos
char *lerNome();
void lerDados(SistemaSolar *sistemas, int numSistemas);
int raioMaiorPlaneta(SistemaSolar sistema);
int quantLuas(SistemaSolar sistema);
int raioMaiorLua(SistemaSolar sistema, int quantLuas);
void trocarSistema(SistemaSolar *a, SistemaSolar *b);
int ritacao(SistemaSolar *sistemas, int inicio, int fim);
void bromeroSort(SistemaSolar *sistemas, int l, int r);

int main() {

  //Leitura do número de sistemas.  
  int numSistemas;
  scanf("%d", &numSistemas);

  //Alocação da memória para os sistemas junto com a chada da função responsável por ler seus dados.
  SistemaSolar *sistemas = (SistemaSolar *)malloc(numSistemas * sizeof(SistemaSolar));
  lerDados(sistemas, numSistemas);

  //Chamada da função bromeroSort utilizando os sistemas, a posição inicial e a última como parâmetros.  
  bromeroSort(sistemas, 0, numSistemas - 1);
    
  //Mostra na tela os sistemas já ordenados em ordem decrescente de importância.  
  for (int i = numSistemas - 1; i >= 0; --i) {
    printf(" %s \n", sistemas[i].nome);
  }

  //Código é responsável pela liberação da memória alocada dinamicamente.
  for (int i = 0; i < numSistemas; ++i) {

    for (int j = 0; j < sistemas[i].quantPlanetas; ++j) {

      for (int k = 0; k < sistemas[i].planetas[j].quantLuas; ++k) {
        free(sistemas[i].planetas[j].luas[k].nome);
      }

      free(sistemas[i].planetas[j].nome);
      free(sistemas[i].planetas[j].luas);
    }

    free(sistemas[i].nome);
    free(sistemas[i].planetas);
  }
  free(sistemas);

  return 0;
}

//Função responsável por ler e alocar dinamicamente os nomes dos sistemas e o nome dos seus corpos celestes.
char *lerNome() {
  char buffer[101];
  scanf("%s", buffer);
  //Aloca espaço para a string e copiar o conteúdo.
  char *nome = (char *)malloc(strlen(buffer) + 1);
  strcpy(nome, buffer);

  return nome;
}

/*Função que lerá os dados sobre os sistemas. Primeiro é feito a leitura do sistema, seus planetas e suas luas,
sendo que para cada planeta é feito a leitura de todas as suas luas antes de ser feita a leitura de um novo planeta.*/

//Não foi feito o uso de \t nos scanfs porque, por padrão, ele ignora espaços em branco como espaço e tabulação.

/*Não adicionei as retrições de tamanho dos raios e a quantidade de sistemas, planetas e luas que está no PDF porque o 
monitor disse que são apenas condições para que o código funcione no tempo correto em qualquer computador, portanto, as
entradas já estariam formatadas com esses valores.*/

void lerDados(SistemaSolar *sistemas, int numSistemas) {

  for (int i = 0; i < numSistemas; ++i) {

    scanf("%d", &sistemas[i].tempDescoberta);
    sistemas[i].nome = lerNome();
    scanf("%d %d", &sistemas[i].raio, &sistemas[i].quantPlanetas);

    //A alocação de memória do array planetas e luas é feita logo após a leitura da quantidade de cada um.
    int quantPlanetas = sistemas[i].quantPlanetas;
    sistemas[i].planetas = (Planeta *)malloc(quantPlanetas * sizeof(Planeta));

    for (int j = 0; j < quantPlanetas; j++){

      sistemas[i].planetas[j].nome = lerNome();
      scanf("%d %d", 
        &sistemas[i].planetas[j].raio, 
        &sistemas[i].planetas[j].quantLuas
      );

      int quantLuas = sistemas[i].planetas[j].quantLuas;
      sistemas[i].planetas[j].luas = (Lua *)malloc(quantLuas * sizeof(Lua));

      for (int k = 0; k < quantLuas; k++){
        sistemas[i].planetas[j].luas[k].nome = lerNome();
        scanf("%d", &sistemas[i].planetas[j].luas[k].raio);
      }
    }
  }
}

/*Função para descobrir o maior planeta de um sistema através do seu raio, ela deve percorrer todos os 
planetas do sistema procurando o maior raio para retorná-lo, caso não haja planetas, a função retorna 0.*/ 
int raioMaiorPlaneta(SistemaSolar sistema) {

  if(!(sistema.quantPlanetas == 0)){
    int raioMaiorPlaneta = sistema.planetas[0].raio;

    for(int i = 1; i < sistema.quantPlanetas; i++){
      if (sistema.planetas[i].raio > raioMaiorPlaneta) {
        raioMaiorPlaneta = sistema.planetas[i].raio;
      }
    }
    return raioMaiorPlaneta;
  }

  return 0;
}

//Função para contar as luas de um sistema.
int quantLuas(SistemaSolar sistema) {
    int cont = 0;
    for(int i = 0; i < sistema.quantPlanetas; i++){
      cont += sistema.planetas[i].quantLuas;
    }

    return cont;
}

/*Função para descobrir a maior lua de um sistema através do seu raio, ela deve percorrer todas as luas 
do sistema procurando o maior raio lunar para retorná-lo, caso não haja luas, a função retorna 0.*/ 
int raioMaiorLua(SistemaSolar sistema, int quantLuas) {

  if(!(quantLuas == 0)){
    int raioMaiorLua = sistema.planetas[0].luas[0].raio;

    for(int i = 0; i < sistema.quantPlanetas; i++){
      for(int j = 0; j < sistema.planetas[i].quantLuas; j++){ 
        if (sistema.planetas[i].luas[j].raio > raioMaiorLua) {
          raioMaiorLua = sistema.planetas[i].luas[j].raio;
        }
      }
    }
    return raioMaiorLua;
  }

  return 0;
}

//Função auxiliar da ritação para trocar os sistemas de posição no array
void trocarSistema(SistemaSolar *a, SistemaSolar *b) {
  SistemaSolar temp = *a;
  *a = *b;
  *b = temp;
}

/*A função de ritação é responsável por rearranjar os elementos do array de sistemas de tal forma que os elementos 
de menor importância que um determinado pivô - escolhido como sendo o último elemento do array no inicío da 
operação - estejam à esquerda dele e os elementos de maior importância à direita. A função retorna a posição final 
do pivô, que então é usada para dividir o array em subarrays menores durante as chamadas recursivas do algoritmo bromeroSort.*/

int ritacao(SistemaSolar *sistemas, int inicio, int fim) {
  int pivo = fim;
  int i = inicio - 1;
  
  //Chamada de funções auxiliares para determinar o maior planeta, a quantidade de luas e a maior lua do sistema pivô.
  int raioMaiorPlanetaPivo = raioMaiorPlaneta(sistemas[pivo]);
  int quantLuasPivo = quantLuas(sistemas[pivo]);
  int raioMaiorLuaPivo = raioMaiorLua(sistemas[pivo], quantLuasPivo);

  /*Estrutura de repetição que irá pecorrer todo o array passado como parâmetro para fazer as comparação entre o elemento 
  na posição j com o pivô*/
  for (int j = inicio; j < fim; ++j) {

    /*Foi adotada a subtração como metódo de comparação porque se o resultado for negativo, eu posso inferir
    que o sistema na posição j é menos importante que o sistema pivô, tendo em vista os critérios definidos no enunciado, que são,
    em ordem de prioridade:
    1 - O sistema pivô é mais importante se tiver o raio maior que o sistema j;
    2 - O sistema pivô é mais importante se tiver mais planetas que o sistema j;
    3 - O sistema pivô é mais importante se tiver o planeta com maior raio; 
    4 - O sistema pivô é mais importante se tiver mais luas que o sistema j; 
    5 - O sistema pivô é mais importante se tiver a lua com o maior raio; 
    6 - O sistema pivô é mais importante se tiver menor tempo de descoberta que o sistema j; */

    /*Exemplo: raioSistemaJ - raioSistemaPivo, se o resultado for igual a um número negativo, significa que o sistema pivô tem o
    raio maior, logo ele é mais importante com base nos critérios adotados no enunciado.*/

    /*Dessa forma, se a comparação resultar em um número negativo, poderei incrementar a variável i - definido no início da ritação - 
    e fazer a troca de posição entre o sistema na posição i e o sistema na posição j. */ 

    //A palavra-chave "continue" é utilizada para pular o restante do código dentro do loop e avançar para a próxima iteração.

    //Comparação dos raios dos sistemas
    int comparacaoRaio = sistemas[j].raio - sistemas[pivo].raio;
      
    if (comparacaoRaio < 0) {
      ++i;
      trocarSistema(&sistemas[i], &sistemas[j]);
      continue;
    }
      
    /*Caso a comparação anterior não seja negativa, o código irá para as demais comparações. A partir da segunda comparação, é utilizado 
    a variável "comparacao", para analizar se a comparação anterior foi feita com elementos de mesma magnitude, ou seja "comparacao" 
    igual a zero - raios iguais, por exemplo -, assim, será necessário uma nova comparação com outros elementos predeterminados. Caso
    a nova comparação seja negativa, i é incrementado e a troca é realizada.*/

    /*Se não for possível atender nenhum critério de comparação - não cumpriu as condições de nenhum if - significa que o sistema da iteração
    é maior que o sistema pivô, então não haverá troca.*/

    //Comparação do número de planetas
    int comparacao = comparacaoRaio;  
    int comparacaoPlanetas = sistemas[j].quantPlanetas - sistemas[pivo].quantPlanetas;
      
    if (comparacao == 0 && comparacaoPlanetas < 0) {
      ++i;
      trocarSistema(&sistemas[i], &sistemas[j]);
      continue;
    }

    //Comparação do planeta com maior raio
    comparacao += comparacaoPlanetas;
    int raioMaiorPlanetaJ = raioMaiorPlaneta(sistemas[j]);
    int comparacaoMaiorPlaneta = raioMaiorPlanetaJ - raioMaiorPlanetaPivo;
  
    if (comparacao == 0 && comparacaoMaiorPlaneta < 0){
      ++i;
      trocarSistema(&sistemas[i], &sistemas[j]);
      continue;
    }

    //Comparação do número de luas
    comparacao += comparacaoMaiorPlaneta;
    int quantLuasJ = quantLuas(sistemas[j]);
    int comparacaoLuas = quantLuasJ - quantLuasPivo;
      
    if (comparacao == 0 && comparacaoLuas < 0) {
      ++i;
      trocarSistema(&sistemas[i], &sistemas[j]);
      continue;
    }

    //Comparação da lua com maior raio
    comparacao += comparacaoLuas;
    int raioMaiorLuaJ = raioMaiorLua(sistemas[j], quantLuasJ);
    int comparacaoMaiorLua = raioMaiorLuaJ - raioMaiorLuaPivo;
   
    if (comparacao == 0 && comparacaoMaiorLua < 0) {
      ++i;
      trocarSistema(&sistemas[i], &sistemas[j]);
      continue;
    }

    //Em relação à comparação do tempo de descoberta, a subtração é invertida pois quanto menor o tempo, mais importante o sistema é. 
      
    //Comparação do tempo de descoberta.
    comparacao += comparacaoMaiorLua;
    int comparacaoTempo = sistemas[pivo].tempDescoberta - sistemas[j].tempDescoberta;
        
    if (comparacao == 0 && comparacaoTempo < 0) {
      ++i;
      trocarSistema(&sistemas[i], &sistemas[j]);
      continue;
    }
  }

  /*Por fim, há a troca de posição do pivô de modo que os sistemas menos importantes que ele fique à sua esquerda e os mais importantes 
  à sua direita. É retornado a posição do pivô.*/
  trocarSistema(&sistemas[i + 1], &sistemas[pivo]);
  return i + 1;
}

/*O bromeroSort avaliará se o parâmetro l (início) é menor que o parâmetro r (fim), caso for verdadeiro, começará a aplicar recursivamente 
ritação e logo em seguida as sublistas à esquerda e à direita do pivô até que a lista esteja completamente ordenada.*/
void bromeroSort(SistemaSolar *sistemas, int l, int r){
  if(l < r) {
    int q = ritacao(sistemas, l, r);
    bromeroSort(sistemas, l, q - 1);
    bromeroSort(sistemas, q + 1, r);
  }
}
