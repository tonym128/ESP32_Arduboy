#pragma once

#include <Arduboy2.h>

class Slot {

  public: 

    Slot() {};
  

    // Properties ..

    uint8_t getChar0();
    uint8_t getChar1();
    uint8_t getChar2();
    uint8_t getWave();
    uint32_t getScore();

    void setChar0(uint8_t val);
    void setChar1(uint8_t val);
    void setChar2(uint8_t val);
    void setWave(uint8_t val);
    void setScore(uint32_t val);


  private:
   
    uint8_t _char0;
    uint8_t _char1;
    uint8_t _char2;
    uint8_t _wave;
    uint32_t _score;

};


//--------------------------------------------------------------------------------------------------------------------------
// Properties ..

uint8_t Slot::getChar0() {
  return _char0;
}

uint8_t Slot::getChar1() {
  return _char1;
}

uint8_t Slot::getChar2() {
  return _char2;
}

uint8_t Slot::getWave() {
  return _wave;
}

uint32_t Slot::getScore() {
  return _score;
}

void Slot::setChar0(uint8_t val) {
  _char0 = val;
}

void Slot::setChar1(uint8_t val) {
  _char1 = val;
}

void Slot::setChar2(uint8_t val) {
  _char2 = val;
}

void Slot::setWave(uint8_t val) {
  _wave = val;
}

void Slot::setScore(uint32_t val) {
  _score = val;
}
