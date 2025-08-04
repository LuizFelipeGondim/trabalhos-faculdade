#include <stdio.h>
#include <stdlib.h>
#define TAM 50

typedef struct {
  int elementX;
  int elementY;
} Relation;

typedef struct {
  int closureSize;
  Relation closureRelations[TAM * TAM];
} ClosureParams;

void prepareClosure(ClosureParams* closure, int numRelations, Relation* relations);
void showRelations(int numRelations, Relation* relations);
int isReflexive(int numElements, int* setA, int numRelations, Relation* relations, ClosureParams* reflexiveClosure);
int isIrreflexive(int numElements, int* setA, int numRelations, Relation* relations);
int isSymmetric(int numRelations, Relation* relations, ClosureParams* symmetricClosure);
int isAntiSymmetric(int numRelations, Relation* relations);
int isTransitive(int numRelations, Relation* relations);
void getTransitiveClosure(ClosureParams* transitiveClosure);

int main() {
  Relation relations[TAM];
  ClosureParams symmetricClosure = {0, {}};
  ClosureParams reflexiveClosure = {0, {}};
  ClosureParams transitiveClosure = {0, {}};

  int numElements, numRelations = 0;
  scanf("%d", &numElements);

  int setA[numElements];

  for (int i = 0; i < numElements; i++)
    scanf("%d", &setA[i]);

  int x, y;
  while (scanf("%d %d", &x, &y) != EOF) {
    relations[numRelations].elementX = x;
    relations[numRelations].elementY = y;
    numRelations++;
  }

  printf("Propriedades\n");
  
  int reflexive = isReflexive(numElements, setA, numRelations, relations, &reflexiveClosure);
  int irreflexive = isIrreflexive(numElements, setA, numRelations, relations);
  int symmetric = isSymmetric(numRelations, relations, &symmetricClosure);
  int antiSymmetric = isAntiSymmetric(numRelations, relations);
  int assymetric = irreflexive && antiSymmetric;

  printf("5. Assimetrica: %c\n", (assymetric ? 'V' : 'F'));

  int transitive = isTransitive(numRelations, relations);
  int equivalence = reflexive && symmetric && transitive;
  int partialOrder = reflexive && antiSymmetric && transitive;

  printf("\nRelacao de equivalencia: %c\n", (equivalence ? 'V' : 'F'));
  printf("Relacao de ordem parcial: %c\n", (partialOrder ? 'V' : 'F'));

  printf("\nFecho reflexivo da relacao:");
  if (reflexive) {
    printf(" R");
  } else {
    printf("\n");
    prepareClosure(&reflexiveClosure, numRelations, relations);
    showRelations(reflexiveClosure.closureSize, reflexiveClosure.closureRelations);
  }

  printf("\nFecho simetrico da relacao:");
  if (symmetric) {
    printf(" R");
  } else {
    printf("\n");
    prepareClosure(&symmetricClosure, numRelations, relations);
    showRelations(symmetricClosure.closureSize, symmetricClosure.closureRelations);
  }

  printf("\nFecho transitivo da relacao:");
  if (transitive) {
    printf(" R");
  } else {
    printf("\n");
    prepareClosure(&transitiveClosure, numRelations, relations);
    getTransitiveClosure(&transitiveClosure);
    showRelations(transitiveClosure.closureSize, transitiveClosure.closureRelations);
  }

  printf("\n\n");
  return 0;
}

int findRelation(int numRelations, Relation* relations, int elementX, int elementY) {
  for (int i = 0; i < numRelations; i++) {
    if (relations[i].elementX == elementX && relations[i].elementY == elementY) {
      return 1;
    }
  }

  return 0;
}

int organizeRelations(const void *a, const void *b) {
  Relation *relationA = (Relation *)a;
  Relation *relationB = (Relation *)b;

  if (relationA->elementX != relationB->elementX) {
    return relationA->elementX - relationB->elementX;
  } else {
    return relationA->elementY - relationB->elementY;
  }
}

void prepareClosure(ClosureParams* closure, int numRelations, Relation* relations) {
  int index = closure->closureSize;

  for (int i = 0; i < numRelations; i++) {
    closure->closureRelations[index] = relations[i];
    index++;
  }

  qsort(closure->closureRelations, index, sizeof(Relation), organizeRelations);
  closure->closureSize = index;
}

void showRelations(int numRelations, Relation* relations) {
  printf("(%d, %d)", relations[0].elementX, relations[0].elementY);

  for (int i = 1; i < numRelations; i++)
    printf(", (%d, %d)", relations[i].elementX, relations[i].elementY);
}

int isReflexive(int numElements, int* setA, int numRelations, Relation* relations, ClosureParams* reflexiveClosure) {
  int reflexive = 1; 
  int relationFound;
  Relation reflexiveRelations[numElements];
  int index = 0;

  for (int i = 0; i < numElements; i++) {
    int element = setA[i];

    relationFound = findRelation(numRelations, relations, element, element);

    if (!relationFound) {
      reflexive = 0;

      reflexiveRelations[index].elementX = element;
      reflexiveRelations[index].elementY = element;
      index++;
    }
  }

  printf("1. Reflexiva: %c\n", reflexive ? 'V' : 'F');
  if (!reflexive) {
    showRelations(index, reflexiveRelations);
    printf("\n");

    for (int i = 0; i < index; i++) {
      reflexiveClosure->closureRelations[i].elementX = reflexiveRelations[i].elementX;
      reflexiveClosure->closureRelations[i].elementY = reflexiveRelations[i].elementY;
    }
    reflexiveClosure->closureSize = index;
  }

  return reflexive;
}

int isIrreflexive(int numElements, int* setA, int numRelations, Relation* relations) {
  int irreflexive = 1; 
  int relationFound;
  Relation irreflexiveRelations[numElements];
  int index = 0;

  for (int i = 0; i < numElements; i++) {
    int element = setA[i];

    relationFound = findRelation(numRelations, relations, element, element);

    if (relationFound) {
      irreflexive = 0;

      irreflexiveRelations[index].elementX = element;
      irreflexiveRelations[index].elementY = element;
      index++;
    }
  }

  printf("2. Irreflexiva: %c\n", irreflexive ? 'V' : 'F');
  if (!irreflexive) {
    showRelations(index, irreflexiveRelations);
    printf("\n");
  }

  return irreflexive;
}

int isSymmetric(int numRelations, Relation* relations, ClosureParams* symmetricClosure) {
  int symmetric = 1; 
  int relationFound;
  Relation symmetricRelations[TAM];
  int index = 0;

  for (int i = 0; i < numRelations; i++) {
    int elementX = relations[i].elementX;
    int elementY = relations[i].elementY;

    relationFound = findRelation(numRelations, relations, elementY, elementX);

    if (!relationFound) {
      symmetric = 0;

      symmetricRelations[index].elementX = elementY;
      symmetricRelations[index].elementY = elementX;
      index++;
    }
  }

  qsort(symmetricRelations, index, sizeof(Relation), organizeRelations);

  printf("3. Simetrica: %c\n", symmetric ? 'V' : 'F');
  if (!symmetric) {
    showRelations(index, symmetricRelations);
    printf("\n");

    for (int i = 0; i < index; i++) {
      symmetricClosure->closureRelations[i].elementX = symmetricRelations[i].elementX;
      symmetricClosure->closureRelations[i].elementY = symmetricRelations[i].elementY;
    }

    symmetricClosure->closureSize = index;
  }

  return symmetric;
}

int isAntiSymmetric(int numRelations, Relation* relations) {
  int antisymmetric = 1;
  int relationFound, relationAnalyzed;
  Relation antiSymmetricRelations[TAM];
  int index = 0;

  for (int j = 0; j < numRelations; j++){
    
    int elementX = relations[j].elementX;
    int elementY = relations[j].elementY;

    if (elementX != elementY) {
      relationFound = findRelation(numRelations, relations, elementY, elementX);
      relationAnalyzed = findRelation(index, antiSymmetricRelations, elementY, elementX);

      if (relationFound && !(relationAnalyzed)) {
        antisymmetric = 0;

        antiSymmetricRelations[index].elementX = elementX;
        antiSymmetricRelations[index].elementY = elementY;
        index++;
      }
    }
  }

  qsort(antiSymmetricRelations, index, sizeof(Relation), organizeRelations);

  printf("4. Anti-simetrica: %c\n", antisymmetric ? 'V' : 'F');
  if (!antisymmetric) {
    printf("((%d, %d), (%d, %d))", 
          antiSymmetricRelations[0].elementX,
          antiSymmetricRelations[0].elementY,
          antiSymmetricRelations[0].elementY,
          antiSymmetricRelations[0].elementX);

    for (int i = 1; i < index; i++)
      printf(", ((%d, %d), (%d, %d))", 
            antiSymmetricRelations[i].elementX,
            antiSymmetricRelations[i].elementY,
            antiSymmetricRelations[i].elementY,
            antiSymmetricRelations[i].elementX);

    printf("\n");
  }

  return antisymmetric;
}

int isTransitive(int numRelations, Relation* relations) {
  int transitive = 1;
  int relationFound, relationAnalyzed;
  Relation transitiveRelations[TAM * 2];
  int index = 0;

  for (int i = 0; i < numRelations; i++) {
    for (int j = 0; j < numRelations; j++) {
      if (relations[i].elementY == relations[j].elementX) {
        int elementX = relations[i].elementX;
        int elementY = relations[j].elementY;

        relationFound = findRelation(numRelations, relations, elementX, elementY);
        relationAnalyzed = findRelation(index, transitiveRelations, elementX, elementY);

        if ((!relationFound) && (!relationAnalyzed)) {
          transitive = 0;

          transitiveRelations[index].elementX = elementX;
          transitiveRelations[index].elementY = elementY;
          index++;
        }
      }
    }
  }

  qsort(transitiveRelations, index, sizeof(Relation), organizeRelations);

  printf("6. Transitiva: %c\n", transitive ? 'V' : 'F');
  if (!transitive) {
    showRelations(index, transitiveRelations);
    printf("\n");
  }

  return transitive;
}

void getTransitiveClosure(ClosureParams* transitiveClosure) {
  int index = transitiveClosure->closureSize;
  int relationFound;
  int addedNewRelations = 1;

  while (addedNewRelations) {
    addedNewRelations = 0;
    for (int i = 0; i < index; i++) {
      for (int j = 0; j < index; j++) {
        if (transitiveClosure->closureRelations[i].elementY == transitiveClosure->closureRelations[j].elementX) {
          int elementX = transitiveClosure->closureRelations[i].elementX;
          int elementY = transitiveClosure->closureRelations[j].elementY;
          relationFound = findRelation(index, transitiveClosure->closureRelations, elementX, elementY);
          
          if (!relationFound) {
            transitiveClosure->closureRelations[index].elementX = elementX;
            transitiveClosure->closureRelations[index].elementY = elementY;
            index++;
            addedNewRelations = 1;
          }
        }
      }
    }
  }

  transitiveClosure->closureSize = index;
  qsort(transitiveClosure->closureRelations, index, sizeof(Relation), organizeRelations);
}