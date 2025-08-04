#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <iostream>
#include <iomanip>
#include <string>

class Patient {
private:

public:
  int id;
  int discharged;
  time_t admission;
  int urgency;
  int hospitalMeasures;
  int laboratoryTests;
  int imagingTests;
  int medicalSupplies;

  time_t triageTime;
  time_t patientCareTime;
  time_t hospitalMeasuresTime;
  time_t laboratoryTestsTime;
  time_t imagingTestsTime;
  time_t medicalSuppliesTime;

  time_t waitingTime;
  time_t serviceTime;
  
  int state;

  Patient(
    int id = 0,
    int discharged = 0,
    time_t admission = 0,
    int urgency = 0,
    int hospitalMeasures = 0,
    int laboratoryTests = 0,
    int imagingTests = 0,
    int medicalSupplies = 0
  );

  void print();
};

#endif
