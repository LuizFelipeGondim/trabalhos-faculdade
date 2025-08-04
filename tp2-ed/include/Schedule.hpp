#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include <iostream>
#include <ctime>
#include <iomanip>
#include <fstream>
#include "Queue.hpp"
#include "Events.hpp"
#include "Vector.hpp"
#include "Patient.hpp"
#include "Procedure.hpp"


class Schedule {
private:
  int numberPatients = 0;
  time_t esperaMediaPaciente = 0;
  time_t esperaMediaUnidade = 0;
  time_t clock;
  Events events;
  Vector<Procedure*> procedures;
  Vector<Patient*> patients;

  // Fila de triagem
  Queue<Patient*> triageQueue;

  // Filas de atendimento
  Queue<Patient*> patientCareMildQueue;
  Queue<Patient*> patientCareModerateQueue;
  Queue<Patient*> patientCareSevereQueue;

  // Filas de pós atendimento
  Queue<Patient*> serviceMildQueue;
  Queue<Patient*> serviceModerateQueue;
  Queue<Patient*> serviceSevereQueue;
  
  // Funções auxiliares
  void handleTriageQueue();
  void handlePatientCareQueue();
  void handleHospitalMeasuresQueue();
  void handleLaboratoryTestsQueue();
  void handleImagingTestsQueue();
  void handleMedicalSuppliesQueue();
  Patient* dequeuePatient(int state);
  void startSimulation();

public:

  Schedule(std::string fileName);
  ~Schedule();

  void estatisticas();
  void showStatistics();
};

#endif
