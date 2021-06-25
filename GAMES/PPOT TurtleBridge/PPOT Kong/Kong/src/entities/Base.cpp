#include "Dinner.h"

#include "../utils/Enums.h"
#include "../map/Coordinates.h"

#define LEVER_ON 140

Base::Base() { }

uint8_t Base::getCounter() {

  return this->counter;

}

void Base::setCounter(uint8_t counter) {

  this->counter = counter;

}
