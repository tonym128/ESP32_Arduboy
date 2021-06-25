#pragma once

#include "../Utils/Arduboy2Ext.h"

struct Slot {

  private:
   
    uint8_t char0;
    uint8_t char1;
    uint8_t char2;
    uint16_t score;

  public: 

    Slot() {};
  
    uint8_t getChar0()            { return this->char0; }
    uint8_t getChar1()            { return this->char1; }
    uint8_t getChar2()            { return this->char2; }
    uint16_t getScore()           { return this->score; }

    void setChar0(uint8_t val)    { this->char0 = val; }
    void setChar1(uint8_t val)    { this->char1 = val; }
    void setChar2(uint8_t val)    { this->char2 = val; }
    void setScore(uint16_t val)   { this->score = val; }

};