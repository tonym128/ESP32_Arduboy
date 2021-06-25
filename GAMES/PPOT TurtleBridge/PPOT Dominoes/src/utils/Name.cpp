#include "Name.h"
#include "Enums.h"

Name::Name() {
  
  this->charIndex = 0;
  this->chars[NAME_LENGTH]= 0;
  
  for (uint8_t x = 0; x < NAME_LENGTH; x++) {
    this->chars[x] = ' ';
  }

};
  


//--------------------------------------------------------------------------------------------------------------------------
// Properties ..

uint8_t Name::getCharIndex() {
  return this->charIndex;
}

char Name::getChar(uint8_t idx) {
  return this->chars[idx];
}

void Name::setCharIndex(uint8_t val) {
  this->charIndex = val;
}

void Name::setChar(uint8_t idx, uint8_t val) {
  this->chars[idx] = val;
}

void Name::setChars(char val[NAME_LENGTH]) {
  this->charIndex = 0;
  memcpy(this->chars, val, NAME_LENGTH);
}

void Name::setStartLoc(uint16_t eeprom_start_loc) {
  this->eeprom_start_loc = eeprom_start_loc;
  this->retrieve(eeprom_start_loc);
}

//--------------------------------------------------------------------------------------------------------------------------
// Methods ..

void Name::incCharIndex() {
  if (this->charIndex < NAME_LENGTH - 1) this->charIndex++;
}

void Name::decCharIndex() {
  if (this->charIndex > 0) this->charIndex--;
}

void Name::incChar(uint8_t idx) {
  
  switch (this->chars[idx]) {

    case ASCII_SPACE:
      this->chars[idx] = ASCII_CAPITAL_A;
      break;

    case ASCII_CAPITAL_A ... ASCII_CAPITAL_Y:
      this->chars[idx]++;
      break;

    case ASCII_CAPITAL_Z:
      this->chars[idx] = ASCII_LOWER_A;
      break;

    case ASCII_LOWER_A ... ASCII_LOWER_Y:
      this->chars[idx]++;
      break;

    case ASCII_LOWER_Z:
      this->chars[idx] = ASCII_SPACE;
      break;

    default:
      this->chars[idx] = ASCII_CAPITAL_A;
      break;
      
  }

  save(this->eeprom_start_loc);

}

void Name::decChar(uint8_t idx) {

  switch (this->chars[idx]) {

    case ASCII_SPACE:
      this->chars[idx] = ASCII_LOWER_Z;
      break;

    case ASCII_CAPITAL_A:
      this->chars[idx] = ASCII_SPACE;
      break;

    case ASCII_CAPITAL_B ... ASCII_CAPITAL_Z:
      this->chars[idx]--;
      break;

    case ASCII_LOWER_A:
      this->chars[idx] = ASCII_CAPITAL_Z;
      break;

    case ASCII_LOWER_B ... ASCII_LOWER_Z:
      this->chars[idx]--;
      break;

    default:
      this->chars[idx] = ASCII_LOWER_Z;
      break;
    
  }

  save(this->eeprom_start_loc);

}

void Name::clear() {

  this->charIndex = 0;
      
  for (uint8_t x = 0; x < NAME_LENGTH; x++) {
    
    this->chars[x] = ASCII_SPACE;

  }

}

void Name::clear(uint16_t startingLocation) {

  this->charIndex = 0;
      
  for (uint8_t x = 0; x < NAME_LENGTH; x++) {
    
    this->chars[x] = ASCII_SPACE;

  }

  Name::save(startingLocation);

}

void Name::retrieve(uint16_t startingLocation) {

  this->charIndex = 0;
      
  for (uint8_t x = 0; x < NAME_LENGTH; x++) {
    
    this->chars[x] = EEPROM.read(startingLocation + x);

  }

  // Has it been initialised ?  If not clear it ..

  if (this->chars[1] != ASCII_SPACE    &&                     
     (this->chars[1] < ASCII_CAPITAL_A || this->chars[1] > ASCII_CAPITAL_Z) &&     
     (this->chars[1] < ASCII_LOWER_A   || this->chars[1] > ASCII_LOWER_Z)       
     ) { clear(startingLocation); }

}

void Name::save(uint16_t startingLocation) {

  for (uint8_t x = 0; x < NAME_LENGTH; x++) {
    
    EEPROM.write(startingLocation + x, getChar(x));
    EEPROM.commit();

  }

}

char* Name::getString() {

  for (uint8_t x = 0; x < NAME_LENGTH ; x++) {
    newChars[x] = this->chars[x];
  }

  for (uint8_t x = 8; x > 0 ; x--) {

    if (newChars[x] == ASCII_SPACE) {
      newChars[x] = 0;
    }
    else break;
  }

  return newChars;

}

