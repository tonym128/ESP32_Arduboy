#pragma once

#include "../Utils/Arduboy2Ext.h"
#include "../Utils/Constants.h"
#include "../Utils/WrappedValue.h"
#include "../Utils/ClampedValue.h"

using ScoreChar = WrappedValue<uint8_t, 0, 25>;
using CharIndex = ClampedValue<uint8_t, 0, 2>;

struct HighScore {

  private:
   
    uint8_t _slotNumber;
    CharIndex _charIndex;
    ScoreChar _chars[3];

  public: 

    HighScore() {

      _slotNumber = DO_NOT_EDIT_SLOT;
      _charIndex = 0;
      _chars[0] = 0;
      _chars[1] = 0;
      _chars[2] = 0;

    };

    uint8_t getSlotNumber()                    { return _slotNumber; }
    uint8_t getCharIndex()                     { return _charIndex; }
    uint8_t getChar(uint8_t idx)               { return _chars[idx]; }

    void setSlotNumber(uint8_t val)            { _slotNumber = val; }
    void setCharIndex(uint8_t val)             { _charIndex = val; }
    void setChar(uint8_t idx, uint8_t val)     { _chars[idx] = val; }


    void incCharIndex() {
      ++_charIndex;
    }

    void decCharIndex() {
      --_charIndex;
    }

    void incChar(uint8_t idx) {      
      ++_chars[idx]; 
    }

    void decChar(uint8_t idx) {
      --_chars[idx];
    }

    void disableEditting() {
      _slotNumber = 255;
    }

    void reset() {

      _slotNumber = DO_NOT_EDIT_SLOT;
      _charIndex = 0;
      _chars[0] = 0;
      _chars[1] = 0;
      _chars[2] = 0;

    }

};
