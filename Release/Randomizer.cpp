#include "Randomizer.h"

Randomizer::Randomizer() {
  srand(time(NULL));
  randomInteger = 20;
}

Randomizer::~Randomizer() {}

int Randomizer::generateRandom(int min, int max) {
  randomInteger = min + ((int)rand() % max);
  return 0;
}
