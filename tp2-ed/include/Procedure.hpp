#ifndef PROCEDURE_HPP
#define PROCEDURE_HPP

#include <iostream>
#include "Patient.hpp"
#include "Vector.hpp"

class Procedure {
private:
  
public:
  Vector<Patient*> units;
  time_t averageTime;
  time_t tempoEspera = 0;
  int numPacientes = 0;
  int numberUnits;

  Procedure(time_t averageTime, int numberUnits);

  int hasSpace();

};

#endif
