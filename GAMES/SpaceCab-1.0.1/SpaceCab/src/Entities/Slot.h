#pragma once

#include "../Utils/Arduboy2Ext.h"

struct Slot {

  private:
   
    uint8_t _char0;
    uint8_t _char1;
    uint8_t _char2;
    uint16_t _score;

  public: 

    Slot() {};
  
    uint8_t getChar0()            { return _char0; }
    uint8_t getChar1()            { return _char1; }
    uint8_t getChar2()            { return _char2; }
    uint16_t getScore()           { return _score; }

    void setChar0(uint8_t val)    { _char0 = val; }
    void setChar1(uint8_t val)    { _char1 = val; }
    void setChar2(uint8_t val)    { _char2 = val; }
    void setScore(uint16_t val)   { _score = val; }

};