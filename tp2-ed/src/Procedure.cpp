#include "Procedure.hpp"

Procedure::Procedure(time_t averageTime, int numberUnits) : 
  averageTime(averageTime),
  numberUnits(numberUnits) { 
    units.resize(numberUnits);
  }

// Verifica se tem espaço no procedimento
// Se tiver, retorna a quantidade de espaços livres
int Procedure::hasSpace() {
  if (numberUnits - units.getSize() > 0)
    return numberUnits - units.getSize();

  return 0;
}