#pragma once

#include <Arduboy2.h>
#include "Enums.h"

class Name {

  public: 

    Name();
  

    // Properties ..

    uint8_t getCharIndex();
    char getChar(uint8_t idx);
    void setCharIndex(uint8_t val);
    void setChar(uint8_t idx, uint8_t val);
    void setChars(char val[NAME_LENGTH]);


    // Methods ..

    void incCharIndex();
    void decCharIndex();
    void incChar(uint8_t idx);
    void decChar(uint8_t idx);

    void clear();
    void clear(uint16_t startingLocation);
    void retrieve(uint16_t startingLocation);
    void save(uint16_t startingLocation);
    char* getString();

    void setStartLoc(uint16_t eeprom_start_loc);

  private:
   
    uint8_t charIndex;
    char chars[NAME_LENGTH + 1];
    char newChars[10];

    uint16_t eeprom_start_loc;

};
