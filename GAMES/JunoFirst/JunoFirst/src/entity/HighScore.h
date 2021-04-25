#pragma once

#include <Arduboy2.h>

class HighScore {

  public: 

    HighScore() {

      _slotNumber = DO_NOT_EDIT_SLOT;
      _charIndex = 0;
      _chars[0] = 0;
      _chars[1] = 0;
      _chars[2] = 0;

    };
  

    // Properties ..

    uint8_t getSlotNumber();
    uint8_t getCharIndex();
    uint8_t getChar(uint8_t idx);

    void setSlotNumber(uint8_t val);
    void setCharIndex(uint8_t val);
    void setChar(uint8_t idx, uint8_t val);
    void setWave(uint8_t val);


    // Methods ..

    void incCharIndex();
    void decCharIndex();
    void incChar(uint8_t idx);
    void decChar(uint8_t idx);
    void disableEditting();
    void reset();

  private:
   
    uint8_t _slotNumber;
    uint8_t _charIndex;
    uint8_t _chars[3];

};


//--------------------------------------------------------------------------------------------------------------------------
// Properties ..

uint8_t HighScore::getSlotNumber() {
  return _slotNumber;
}

uint8_t HighScore::getCharIndex() {
  return _charIndex;
}

uint8_t HighScore::getChar(uint8_t idx) {
  return _chars[idx];
}

void HighScore::setSlotNumber(uint8_t val) {
  _slotNumber = val;
}

void HighScore::setCharIndex(uint8_t val) {
  _charIndex = val;
}

void HighScore::setChar(uint8_t idx, uint8_t val) {
  _chars[idx] = val;
}


//--------------------------------------------------------------------------------------------------------------------------
// Methods ..

void HighScore::incCharIndex() {
  if (_charIndex < 2) _charIndex++;
}

void HighScore::decCharIndex() {
  if (_charIndex > 0) _charIndex--;
}

void HighScore::incChar(uint8_t idx) {
  
  if (_chars[idx] == 35) {
    _chars[idx] = 0; 
  } 
  else { 
    _chars[idx]++;
  } 

}

void HighScore::decChar(uint8_t idx) {
  
  if (_chars[idx] == 0) {
    _chars[idx] = 35; 
  } 
  else { 
    _chars[idx]--;
  } 

}

void HighScore::disableEditting() {
  _slotNumber = 255;
}

void HighScore::reset() {

  _slotNumber = 255;
  _charIndex = 0;
  _chars[0] = 0;
  _chars[1] = 0;
  _chars[2] = 0;

}
