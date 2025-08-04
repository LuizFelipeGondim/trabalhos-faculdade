#include "IndirectOrdering.hpp"

// Efetua leitura dos dados passados no arquivo de entrada  
IndirectOrdering::IndirectOrdering(std::string fileName) {
  std::ifstream file(fileName);

  if (!file.is_open()) {
    throw std::runtime_error("Erro: Arquivo " + fileName + " não encontrado.");
  }

  std::string line;

  std::getline(file, line);
  amountAttributes = std::stoi(line);

  for (int i = 0; i < amountAttributes; i++) {
    std::getline(file, line);
    _attributes.push(line);
  }

  // Evita redifinição do tamanho do vetor a cada linha
  std::getline(file, line);
  int tamMax = std::stoi(line);
  _persons.resize(tamMax);

  while (std::getline(file, line)) {
    std::stringstream lineStream(line);
    std::string name, cpf, address, others;

    // Dividir a linha pelos campos (separados por vírgula)
    std::getline(lineStream, name, ',');
    std::getline(lineStream, cpf, ',');
    std::getline(lineStream, address, ',');
    std::getline(lineStream, others, ',');

    // Adicionar ao vetor de _persons
    _persons.push({name, cpf, address, others});
  }

  _size = _persons.getSize();

  file.close();
}

void IndirectOrdering::swap(int& a, int& b) {
  int temp = a;
  a = b;
  b = temp;
}

void IndirectOrdering::quickSort(int* indexes, int l, int r, const std::string& key) {
  if (l < r) {

    int pivotIndex = partition(indexes, l, r, key);

    quickSort(indexes, l, pivotIndex - 1, key);
    quickSort(indexes, pivotIndex + 1, r, key);
  }
}

// Partição para o quicksort com mediana de três
int IndirectOrdering::partition(int* indexes, int l, int r, const std::string& key) {
  int mid = l + (r - l) / 2;

  // Determina a mediana de três
  if (compareBy(_persons[indexes[mid]], _persons[indexes[l]], key, indexes[mid], indexes[l]))
    swap(indexes[mid], indexes[l]);
  if (compareBy(_persons[indexes[r]], _persons[indexes[l]], key, indexes[r], indexes[l]))
    swap(indexes[r], indexes[l]);
  if (compareBy(_persons[indexes[r]], _persons[indexes[mid]], key, indexes[r], indexes[mid]))
    swap(indexes[r], indexes[mid]);

  int pivotIndex = mid;
  int pivotValue = indexes[pivotIndex];

  // Coloca o pivô na posição final
  swap(indexes[pivotIndex], indexes[r]);
  
  int i = l - 1;

  for (int j = l; j <= r - 1; ++j) {
    if (compareBy(_persons[indexes[j]], _persons[pivotValue], key, indexes[j], indexes[pivotValue])) {
      i++;
      swap(indexes[i], indexes[j]);
    }
  }

  swap(indexes[i + 1], indexes[r]);
  return i + 1;
}

// Método de ordenação simples escolhido  
void IndirectOrdering::selectionSort(int* indexes, const std::string& key) {
  for (int i = 0; i < _size - 1; ++i) {
    int minIndex = i;

    for (int j = i + 1; j < _size; ++j)
      if (compareBy(_persons[indexes[j]], _persons[indexes[minIndex]], key, indexes[j], indexes[minIndex]))
        minIndex = j;

    swap(indexes[i], indexes[minIndex]);
  }
}

// Método criado para comparar a ordem dos dados com base na chave
bool IndirectOrdering::compareBy(
    const Person& a, 
    const Person& b, 
    const std::string& key,
    int indexA,
    int indexB
  ) {

  // Há uma dupla verificação para garantir a estabilidade dos algoritmos de ordenações estáveis (SeletionSort)  
  if (key == "name") {
    if (a.name != b.name) 
      return a.name < b.name;

  } else if (key == "cpf") {
    if (a.cpf != b.cpf) 
      return a.cpf < b.cpf;

  } else if (key == "address") {
    if (a.address != b.address) 
      return a.address < b.address;
  }
  
  return indexA < indexB;
}

// Método que cria um array de índices do tamanho do vetor de dados
int* IndirectOrdering::createIndices() {
  int* indexes = new int[_size];

  for (int i = 0; i < _size; ++i) 
    indexes[i] = i;

  return indexes;
}

void IndirectOrdering::mergeSort(int* indexes, int* temp, int l, int r, const std::string& key) {
  if (l < r) {
    int mid = l + (r - l) / 2;

    mergeSort(indexes, temp, l, mid, key);
    mergeSort(indexes, temp, mid + 1, r, key);

    merge(indexes, temp, l, mid, r, key);
  }
}

void IndirectOrdering::merge(int* indexes, int* temp, int l, int mid, int r, const std::string& key) {
  int i = l;     
  int j = mid + 1;
  int k = l;

  // Combina os elementos das duas metades
  while (i <= mid && j <= r) {
    if (compareBy(_persons[indexes[i]], _persons[indexes[j]], key, indexes[i], indexes[j])) {
      temp[k++] = indexes[i++];
    } else {
      temp[k++] = indexes[j++];
    }
  }

  // Copia os elementos restantes da metade esquerda (se houver)
  while (i <= mid)
    temp[k++] = indexes[i++];

  // Copia os elementos restantes da metade direita (se houver)
  while (j <= r)
    temp[k++] = indexes[j++];

  // Copia os elementos do array temporário de volta para o array original
  for (int m = l; m <= r; ++m)
    indexes[m] = temp[m];
}

// Método responsável por iniciar as ordenações
void IndirectOrdering::startOrdering() {

  //SelectionSort
  int* indexes = createIndices();
  selectionSort(indexes, "name");
  print(indexes);
  delete[] indexes;

  indexes = createIndices();
  selectionSort(indexes, "cpf");
  print(indexes);
  delete[] indexes;

  indexes = createIndices();
  selectionSort(indexes, "address");
  print(indexes);
  delete[] indexes;

  //QuickSort
  indexes = createIndices();
  quickSort(indexes, 0, _size - 1, "name");
  print(indexes);
  delete[] indexes;

  indexes = createIndices();
  quickSort(indexes, 0, _size - 1, "cpf");
  print(indexes);
  delete[] indexes;

  indexes = createIndices();
  quickSort(indexes, 0, _size - 1, "address");
  print(indexes);
  delete[] indexes;

  //MergeSort
  indexes = createIndices();
  int* tempName = new int[_size];
  mergeSort(indexes, tempName, 0, _size - 1, "name");
  delete[] tempName;
  print(indexes);
  delete[] indexes;

  indexes = createIndices();
  int* tempCpf = new int[_size];
  mergeSort(indexes, tempCpf, 0, _size - 1, "cpf");
  delete[] tempCpf;
  print(indexes);
  delete[] indexes;

  indexes = createIndices();
  int* tempAddress = new int[_size];
  mergeSort(indexes, tempAddress, 0, _size - 1, "address");
  delete[] tempAddress;
  print(indexes);
  delete[] indexes;
}

// Printa o código no terminal assim como foi designado pelo enunciado
void IndirectOrdering::print(int* indexes) {
  int size = _persons.getSize();

  std::cout << amountAttributes << std::endl;
  for (int i = 0; i < amountAttributes; i++){
    std::cout << _attributes[i] << std::endl; 
  }

  std::cout << size << std::endl;
  for (int i = 0; i < size; ++i) {
    const auto& person = _persons[indexes[i]];
    std::cout << person.name << ","
              << person.cpf << "," 
              << person.address << "," 
              << person.others 
              << std::endl;
  }
}
