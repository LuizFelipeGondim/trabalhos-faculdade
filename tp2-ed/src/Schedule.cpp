#include "Schedule.hpp"

// Efetua a leitura dos dados
Schedule::Schedule(std::string fileName) {
  std::ifstream file(fileName);

  if (!file.is_open()) {
    throw std::runtime_error("Erro: Arquivo não encontrado.");
  }

  double averageTime; 
  int numberUnits;

  for (int i = 0; i < 6; i++) {
    file >> averageTime >> numberUnits;
    time_t averageTimeInSeconds = static_cast<time_t>(averageTime * 3600);
    procedures.push(new Procedure(averageTimeInSeconds, numberUnits));
  }

  file >> numberPatients; 

  for (int i = 0; i < numberPatients; ++i) {
    int id, discharged, year, month, day;
    float hour;
    int urgency, hospitalMeasures, laboratoryTests, imagingTests, medicalSupplies;

    file >> id >> discharged >> year >> month >> day >> hour >> urgency
         >> hospitalMeasures >> laboratoryTests >> imagingTests >> medicalSupplies;

    // Calcula o time_t baseado nos valores lidos
    std::tm tm = {};
    tm.tm_year = year - 1900; 
    tm.tm_mon = month - 1;    
    tm.tm_mday = day;
    tm.tm_hour = static_cast<int>(hour);
    tm.tm_min = static_cast<int>((hour - tm.tm_hour) * 60);

    time_t timestamp = std::mktime(&tm);

    patients.push(new Patient(
      id, discharged, timestamp, urgency,
      hospitalMeasures, laboratoryTests, imagingTests, medicalSupplies
    ));
  }

  file.close();
}

Schedule::~Schedule() {
  int proceduresSize = procedures.getSize();
  for (int i = 0; i < proceduresSize; i++)
    delete procedures[i];

  for (int i = 0; i < numberPatients; i++)
    delete patients[i];
}

// Inicia a simulação do escalonador adicionando um evento pra cada paciente
// Depois ele vai analisando os estados do paciente e as situações das filas
// Dependendo desses fatores, um novo evento é adicionado no escalonador
// A simulação acaba quando não houver mais eventos a serem analisados ou as filas estiverem vazias
void Schedule::startSimulation() {
  bool finishSimulation = false;
  int hasSpace;

  if (patients.getSize() == 0)
    finishSimulation = true;

  // Inicializa eventos com os pacientes
  for (int i = 0; i < patients.getSize(); i++) {
    events.push(Event(
      patients[i]->id,
      patients[i]->admission
    ));
  }

  while (!finishSimulation) {
    // Atualiza o relógio para o próximo evento
    Event event = events.top();
    clock = event.timestamp;

    // Seleciona paciente do evento
    Patient* patient = nullptr;
    for (int i = 0; i < patients.getSize(); i++) {
      if (event.id == patients[i]->id) {
        patient = patients[i];
        break;
      }
    }

    // Adiciona paciente na fila de triagem
    if (patient->state == 1) {
      patient->state++;
      patient->triageTime = clock;
      triageQueue.enqueue(patient);
    }
    
    // Verifica se algum paciente já terminou a triagem.
    // Se terminou, ele é mandado para a fila de espera do atendimento.
    // O tempo de entrada na fila também é calculado
    if (patient->state == 3) {

      patient->state++;
      patient->patientCareTime = clock;

      if (patient->urgency == 0) {
        patientCareMildQueue.enqueue(patient);
      } else if (patient->urgency == 1) {
        patientCareModerateQueue.enqueue(patient);
      } else if (patient->urgency == 2) {
        patientCareSevereQueue.enqueue(patient);
      } 

      // Remove paciente da unidade de triagem
      for (int i = 0; i < procedures[0]->units.getSize(); i++)
        if (patient->id == procedures[0]->units[i]->id)
          procedures[0]->units.pop(i);
    }

    // Verifica se o paciente recebeu alta, caso não, ele é encaminhado para sua fila
    // O tempo de entrada na fila também é calculado
    if (patient->state == 5) {
      
      // Remove paciente da unidade de atendimento
      for (int i = 0; i < procedures[1]->units.getSize(); i++)
        if (patient->id == procedures[1]->units[i]->id)
          procedures[1]->units.pop(i);

      if (patient->discharged) {
        patient->state = 14;
      } else if (patient->hospitalMeasures > 0) { // Verifica se paciente pode pular estado
        patient->state++;
        patient->hospitalMeasuresTime = clock;

        if (patient->urgency == 0) {
          serviceMildQueue.enqueue(patient);
        } else if (patient->urgency == 1) {
          serviceModerateQueue.enqueue(patient);
        } else if (patient->urgency == 2) {
          serviceSevereQueue.enqueue(patient);
        }
      } else {
        patient->state = 7;
      }
    }

    // Encaminha e calcula entrada do paciente na fila de serviços, com estado 7
    if (patient->state == 7) {
      // Remove paciente da unidade de medidas hospitalares
      for (int i = 0; i < procedures[2]->units.getSize(); i++)
        if (patient->id == procedures[2]->units[i]->id)
          procedures[2]->units.pop(i);

      if (patient->laboratoryTests > 0) { // Verifica se paciente pode pular estado
        patient->state++;
        patient->laboratoryTestsTime = clock;

        if (patient->urgency == 0) {
          serviceMildQueue.enqueue(patient);
        } else if (patient->urgency == 1) {
          serviceModerateQueue.enqueue(patient);
        } else if (patient->urgency == 2) {
          serviceSevereQueue.enqueue(patient);
        }
      } else {
        patient->state = 9;
      }
    }

    // Encaminha e calcula entrada do paciente na fila de serviços, com estado 9
    if (patient->state == 9) {
      // Remove paciente da unidade de testes laboratoriais
      for (int i = 0; i < procedures[3]->units.getSize(); i++)
        if (patient->id == procedures[3]->units[i]->id)
          procedures[3]->units.pop(i);

      if (patient->imagingTests > 0) { // Verifica se paciente pode pular estado
        patient->state++;
        patient->imagingTestsTime = clock;

        if (patient->urgency == 0) {
          serviceMildQueue.enqueue(patient);
        } else if (patient->urgency == 1) {
          serviceModerateQueue.enqueue(patient);
        } else if (patient->urgency == 2) {
          serviceSevereQueue.enqueue(patient);
        }
      } else {
        patient->state = 11;
      }
    }

    // Encaminha e calcula entrada do paciente na fila de serviços, com estado 12
    if (patient->state == 11) {
      // Remove paciente da unidade de testes de imagem
      for (int i = 0; i < procedures[4]->units.getSize(); i++)
        if (patient->id == procedures[4]->units[i]->id)
          procedures[4]->units.pop(i);

      if (patient->medicalSupplies > 0) { // Verifica se paciente pode pular estado
        patient->state++;
        patient->medicalSuppliesTime = clock;

        if (patient->urgency == 0) {
          serviceMildQueue.enqueue(patient);
        } else if (patient->urgency == 1) {
          serviceModerateQueue.enqueue(patient);
        } else if (patient->urgency == 2) {
          serviceSevereQueue.enqueue(patient);
        }
      } else {
        patient->state = 13;
      }
    }

    // Alta hospitalar
    if (patient->state == 13) {
      // Remove paciente da unidade de instrumentos/medicamentos
      for (int i = 0; i < procedures[5]->units.getSize(); i++)
        if (patient->id == procedures[5]->units[i]->id)
          procedures[5]->units.pop(i);

      patient->state++;
    }  

    // Verifica se tem espaço no serviço de triagem, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[0]->hasSpace();
    if (hasSpace > 0)
      for (int i = 0; i < hasSpace; i++)
        if (!triageQueue.empty())
          handleTriageQueue();
    
    // Verifica se tem espaço no serviço de atendimento, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[1]->hasSpace();
    if (hasSpace > 0)
      for (int i = 0; i < hasSpace; i++)
        handlePatientCareQueue();
      
    // Verifica se tem espaço no serviço de medidas hospitalares, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[2]->hasSpace();
    if (hasSpace > 0)
      for (int i = 0; i < hasSpace; i++)
        handleHospitalMeasuresQueue();

    // Verifica se tem espaço no serviço de testes laboratoriais, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[3]->hasSpace();
    if (hasSpace > 0)
      for (int i = 0; i < hasSpace; i++) 
        handleLaboratoryTestsQueue();

    // Verifica se tem espaço no serviço de testes de imagem, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[4]->hasSpace();
    if (hasSpace > 0)
      for (int i = 0; i < hasSpace; i++)
        handleImagingTestsQueue();

    // Verifica se tem espaço no serviço de suprimentos médicos, se tiver, envia o primeiro da fila. 
    hasSpace = procedures[5]->hasSpace();
    if (hasSpace > 0)
      for (int i = 0; i < hasSpace; i++)
        handleMedicalSuppliesQueue();

    // Remove evento atual do min heap
    events.pop();

    // Verifica se ainda há eventos e se há alguem em alguma fila
    if (events.empty()) {
      finishSimulation = true;

      if (!triageQueue.empty()) {
        hasSpace = procedures[0]->hasSpace();
        for (int i = 0; i < hasSpace; i++)
          handleTriageQueue();

        finishSimulation = false;
      } 
      
      if (
        !patientCareMildQueue.empty() || 
        !patientCareModerateQueue.empty() || 
        !patientCareSevereQueue.empty()
      ) {
        hasSpace = procedures[1]->hasSpace();
        for (int i = 0; i < hasSpace; i++)
          handlePatientCareQueue();

        finishSimulation = false;
      } 
      
      if (
        !serviceMildQueue.empty() || 
        !serviceModerateQueue.empty() || 
        !serviceSevereQueue.empty()
      ) {
        hasSpace = procedures[2]->hasSpace();
        for (int i = 0; i < hasSpace; i++)
          handleHospitalMeasuresQueue();

        hasSpace = procedures[3]->hasSpace();
        for (int i = 0; i < hasSpace; i++)
          handleLaboratoryTestsQueue();

        hasSpace = procedures[4]->hasSpace();
        for (int i = 0; i < hasSpace; i++)
          handleImagingTestsQueue();

        hasSpace = procedures[5]->hasSpace();
        for (int i = 0; i < hasSpace; i++)
          handleMedicalSuppliesQueue();

        finishSimulation = false;
      } 

    }
  }
}

// Verifica se a fila de triagem está vazia.
// Se estiver, o paciente é retirado da fila para atualizar suas estatísticas e 
// ser inserido na unidade de triagem. Depois um novo evento é inserido
void Schedule::handleTriageQueue() {
  Patient* patientTriage = triageQueue.dequeue();

  patientTriage->waitingTime += clock - patientTriage->triageTime;
  patientTriage->serviceTime += procedures[0]->averageTime;
  patientTriage->state++;
  procedures[0]->units.push(patientTriage);

  events.push(Event(
    patientTriage->id,         
    clock + procedures[0]->averageTime 
  ));
}

// Verifica se as filas de atendimento estão vazias
// Se estiver, o paciente é retirado da fila para atualizar suas estatísticas e 
// ser inserido na unidade de atendimento. Depois um novo evento é inserido
void Schedule::handlePatientCareQueue() {
  Patient* patientCare = nullptr;
  
  if (!patientCareSevereQueue.empty()) {
    patientCare = patientCareSevereQueue.dequeue();
  } else if (!patientCareModerateQueue.empty()) {
    patientCare = patientCareModerateQueue.dequeue();
  } else if (!patientCareMildQueue.empty()) {
    patientCare = patientCareMildQueue.dequeue();
  }

  if (patientCare != nullptr) {
    patientCare->waitingTime += clock - patientCare->patientCareTime;
    patientCare->serviceTime += procedures[1]->averageTime;
    patientCare->state++;
    procedures[1]->units.push(patientCare);

    events.push(Event(
      patientCare->id,       
      clock + procedures[1]->averageTime 
    ));
  }
}

// Verifica se as filas de serviço estão vazias e o estado do paciente é igual a 6
// Se for, ele é retirado da fila para atualizar suas estatísticas e ser inserido 
// na unidade de medidas hospitalares. Depois um novo evento é inserido
void Schedule::handleHospitalMeasuresQueue() {
  Patient* patientService = dequeuePatient(6);

  if (patientService != nullptr) {
    patientService->waitingTime += clock - patientService->hospitalMeasuresTime;
    patientService->serviceTime += patientService->hospitalMeasures * procedures[2]->averageTime;
    patientService->state++;
    procedures[2]->units.push(patientService);

    events.push(Event(
      patientService->id,       
       clock + (patientService->hospitalMeasures * procedures[2]->averageTime)
    ));
  }
} 

// Verifica se as filas de serviço estão vazias e o estado do paciente é igual a 8
// Se for, ele é retirado da fila para atualizar suas estatísticas e depois um novo evento é inserido
void Schedule::handleLaboratoryTestsQueue() {
  Patient* patientService = dequeuePatient(8);

  if (patientService != nullptr) {
    patientService->waitingTime += clock - patientService->laboratoryTestsTime;
    patientService->serviceTime += patientService->laboratoryTests * procedures[3]->averageTime;
    patientService->state++;
    procedures[3]->units.push(patientService);

    events.push(Event(
      patientService->id,        
      clock + (patientService->laboratoryTests * procedures[3]->averageTime)  
    ));
  }
} 

// Verifica se as filas de serviço estão vazias e o estado do paciente é igual a 10
// Se for, ele é retirado da fila para atualizar suas estatísticas e depois um novo evento é inserido
void Schedule::handleImagingTestsQueue() {
  Patient* patientService = dequeuePatient(10);

  if (patientService != nullptr) {
    patientService->waitingTime += clock - patientService->imagingTestsTime;
    patientService->serviceTime += patientService->imagingTests * procedures[4]->averageTime;
    patientService->state++;
    procedures[4]->units.push(patientService);

    events.push(Event(
      patientService->id,         
      clock + (patientService->imagingTests * procedures[4]->averageTime)  
    ));
  }
} 

// Verifica se as filas de serviço estão vazias e o estado do paciente é igual a 12
// Se for, ele é retirado da fila para atualizar suas estatísticas e depois um novo evento é inserido
void Schedule::handleMedicalSuppliesQueue() {
  Patient* patientService = dequeuePatient(12);

  if (patientService != nullptr) {
    patientService->waitingTime += clock - patientService->medicalSuppliesTime;
    patientService->serviceTime += patientService->medicalSupplies * procedures[5]->averageTime;
    patientService->state++;
    procedures[5]->units.push(patientService);

    events.push(Event(
      patientService->id,           
      clock + (patientService->medicalSupplies * procedures[5]->averageTime)
    ));
  }
}

// Verifica se há pacientes nas filas de pós atendimento em determinado estado
// Se não tiver, em nenhum lugar da fila e em nenhuma fila, é retornado nullptr
Patient* Schedule::dequeuePatient(int state) {
  Patient* patientService = nullptr;

  if (!serviceSevereQueue.empty()) {
    size_t queueSize = serviceSevereQueue.getSize();
    for (size_t i = 0; i < queueSize; ++i) {
      patientService = serviceSevereQueue.front();

      if (patientService->state == state) {
        serviceSevereQueue.dequeue();
        return patientService;
      }

      serviceSevereQueue.enqueue(patientService);
      serviceSevereQueue.dequeue();  
    }
    patientService = nullptr;
  }

  if (!serviceModerateQueue.empty() && patientService == nullptr) {
    size_t queueSize = serviceModerateQueue.getSize();
    for (size_t i = 0; i < queueSize; ++i) {
      patientService = serviceModerateQueue.front();

      if (patientService->state == state) {
        serviceModerateQueue.dequeue();
        return patientService;
      }

      serviceModerateQueue.enqueue(patientService);
      serviceModerateQueue.dequeue();
    }
    patientService = nullptr;
  }

  if (!serviceMildQueue.empty() && patientService == nullptr) {
    size_t queueSize = serviceMildQueue.getSize();
    for (size_t i = 0; i < queueSize; ++i) {
      patientService = serviceMildQueue.front();

      if (patientService->state == state) {
        serviceMildQueue.dequeue();
        return patientService;
      }

      serviceMildQueue.enqueue(patientService);
      serviceMildQueue.dequeue();
    }
    patientService = nullptr;
  }

  return patientService;
}

// Mostra as estatísticas conforme solicitado no enunciado do trabalho
void Schedule::showStatistics() {
  startSimulation();
  //for (int i = 0; i < numberPatients; i++) 
    //patients[i]->print();
}

void Schedule::estatisticas() {
  for (int i = 0; i < numberPatients; i++) 
    esperaMediaPaciente += patients[i]->waitingTime;
  
  for (int i = 0; i < 6; i++)
    procedures[i]->tempoEspera;
}   