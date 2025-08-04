#include "Patient.hpp"

Patient::Patient(
  int id,
  int discharged,
  time_t admission,
  int urgency,
  int hospitalMeasures,
  int laboratoryTests,
  int imagingTests,
  int medicalSupplies
) : id(id),
    discharged(discharged),
    admission(admission),
    urgency(urgency),
    hospitalMeasures(hospitalMeasures),
    laboratoryTests(laboratoryTests),
    imagingTests(imagingTests),
    medicalSupplies(medicalSupplies),
    waitingTime(0),
    serviceTime(0),
    state(1) {}

// Imprime as informações do paciente conforme foi solicitado no enunciado do trabalho
void Patient::print() {

  time_t totalTimeInSeconds = waitingTime + serviceTime;  
  time_t dischargeTime = admission + totalTimeInSeconds;  
  
  char admissionDate[100];
  strftime(admissionDate, sizeof(admissionDate), "%a %b %e %H:%M:%S %Y", localtime(&admission));

  char dischargeDate[100];
  strftime(dischargeDate, sizeof(dischargeDate), "%a %b %e %H:%M:%S %Y", localtime(&dischargeTime));

  std::cout << id;
  std::cout << " " << admissionDate;
  std::cout << " " << dischargeDate;
  std::cout << " " << std::fixed << std::setprecision(2) << totalTimeInSeconds / 3600.0;
  std::cout << " " << std::fixed << std::setprecision(2) << serviceTime / 3600.0;
  std::cout << " " << std::fixed << std::setprecision(2) << waitingTime / 3600.0 << std::endl;
}