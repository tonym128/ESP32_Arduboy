#include "Plate.h"
#include "../utils/Enums.h"

enum class PlateStance : uint8_t {
  Normal,
  Normal_RHS,
  Normal_Middle,
  Incline_01,
  Incline_01_RHS,
  Incline_02,
  Incline_02_RHS,
  Incline_03,
  Incline_03_RHS,
  None
};

const uint8_t PROGMEM Offset_0[] = {

  0, 0, static_cast<uint8_t>(PlateStance::Normal), 
  0, 0, static_cast<uint8_t>(PlateStance::Normal), 
  0, 0, static_cast<uint8_t>(PlateStance::Normal), 
  0, 0, static_cast<uint8_t>(PlateStance::Normal), 

  1, 3, static_cast<uint8_t>(PlateStance::Incline_01),
  1, 3, static_cast<uint8_t>(PlateStance::Incline_01),
  1, 5, static_cast<uint8_t>(PlateStance::Incline_02),
  1, 5, static_cast<uint8_t>(PlateStance::Incline_02),
  1, 7, static_cast<uint8_t>(PlateStance::Incline_03),
  1, 7, static_cast<uint8_t>(PlateStance::Incline_03),
  1, 0, static_cast<uint8_t>(PlateStance::None),  // Signifies end fo sequence

};

const uint8_t PROGMEM Offset_1[] = {

  0, 0, static_cast<uint8_t>(PlateStance::Normal_Middle), 
  0, 0, static_cast<uint8_t>(PlateStance::Normal_Middle), 
  0, 0, static_cast<uint8_t>(PlateStance::Normal_Middle), 
  0, 0, static_cast<uint8_t>(PlateStance::Normal_Middle), 

  0, 3, static_cast<uint8_t>(PlateStance::Normal_Middle),
  0, 6, static_cast<uint8_t>(PlateStance::Normal_Middle),
  0, 9, static_cast<uint8_t>(PlateStance::Normal_Middle),
  0, 12, static_cast<uint8_t>(PlateStance::Normal_Middle),
  0, 15, static_cast<uint8_t>(PlateStance::Normal_Middle),
  0, 18, static_cast<uint8_t>(PlateStance::Normal_Middle),
  0, 20, static_cast<uint8_t>(PlateStance::Normal_Middle),
  0, 0, static_cast<uint8_t>(PlateStance::None),  // Signifies end fo sequence

};

const uint8_t PROGMEM Offset_2[] = {

  0, 0, static_cast<uint8_t>(PlateStance::Normal_RHS), 
  0, 0, static_cast<uint8_t>(PlateStance::Normal_RHS), 
  0, 0, static_cast<uint8_t>(PlateStance::Normal_RHS), 
  0, 0, static_cast<uint8_t>(PlateStance::Normal_RHS), 

  static_cast<uint8_t>(-1), 3, static_cast<uint8_t>(PlateStance::Incline_01_RHS),
  static_cast<uint8_t>(-1), 3, static_cast<uint8_t>(PlateStance::Incline_01_RHS),
  static_cast<uint8_t>(-2), 5, static_cast<uint8_t>(PlateStance::Incline_02_RHS),
  static_cast<uint8_t>(-2), 5, static_cast<uint8_t>(PlateStance::Incline_02_RHS),
  static_cast<uint8_t>(-1), 7, static_cast<uint8_t>(PlateStance::Incline_03_RHS),
  static_cast<uint8_t>(-1), 7, static_cast<uint8_t>(PlateStance::Incline_03_RHS),
  7, 13, static_cast<uint8_t>(PlateStance::Incline_02_RHS),
  7, 13, static_cast<uint8_t>(PlateStance::Incline_02_RHS),
  11, 14, static_cast<uint8_t>(PlateStance::Incline_01_RHS),
  11, 14, static_cast<uint8_t>(PlateStance::Incline_01_RHS),
  13, 16, static_cast<uint8_t>(PlateStance::Normal_RHS),
  14, 16, static_cast<uint8_t>(PlateStance::Normal_RHS),
  18, 18, static_cast<uint8_t>(PlateStance::Incline_01),
  19, 18, static_cast<uint8_t>(PlateStance::Incline_01),
  21, 20, static_cast<uint8_t>(PlateStance::Incline_02),
  21, 20, static_cast<uint8_t>(PlateStance::Incline_02),
  22, 22, static_cast<uint8_t>(PlateStance::Incline_03),
  23, 24, static_cast<uint8_t>(PlateStance::Incline_03),
  10, 0, static_cast<uint8_t>(PlateStance::None),  // Signifies end fo sequence

};

Plate::Plate() : Base() { 

  this->counter = 0;

}

uint8_t Plate::getPlateNumber() {

  return this->plateNumber;

}

void Plate::setPlateNumber(uint8_t plateNumber) {

  this->plateNumber = plateNumber;

}

void Plate::incCounter() {

  this->counter++;

  switch (this->plateNumber) {

    case 0:
      if (pgm_read_byte(&Offset_0[(this->counter * 3) + 2]) == static_cast<uint8_t>(PlateStance::None)) {
        this->counter--;
      }
      break;

    case 1:
      if (pgm_read_byte(&Offset_1[(this->counter * 3) + 2]) == static_cast<uint8_t>(PlateStance::None)) {
        this->counter--;
      }
      break;

    case 2:
      if (pgm_read_byte(&Offset_2[(this->counter * 3) + 2]) == static_cast<uint8_t>(PlateStance::None)) {
        this->counter--;
      }
      break;

  }

}

int8_t Plate::getXOffset() {

  switch (this->plateNumber) {

    case 0:
      return static_cast<int8_t>(pgm_read_byte(&Offset_0[this->counter * 3]));

    case 1:
      return static_cast<int8_t>(pgm_read_byte(&Offset_1[this->counter * 3]));

    case 2:
      return static_cast<int8_t>(pgm_read_byte(&Offset_2[this->counter * 3]));

    default: return 0;

  }

}

uint8_t Plate::getYOffset() {

  switch (this->plateNumber) {

    case 0:
      return pgm_read_byte(&Offset_0[(this->counter * 3) + 1]);

    case 1:
      return pgm_read_byte(&Offset_1[(this->counter * 3) + 1]);

    case 2:
      return pgm_read_byte(&Offset_2[(this->counter * 3) + 1]);

    default: return 0;

  }

}

uint8_t Plate::getImage() {

  switch (this->plateNumber) {

    case 0:
      return pgm_read_byte(&Offset_0[(this->counter * 3) + 2]);

    case 1:
      return pgm_read_byte(&Offset_1[(this->counter * 3) + 2]);

    case 2:
      return pgm_read_byte(&Offset_2[(this->counter * 3) + 2]);

    default: return 0;

  }

}