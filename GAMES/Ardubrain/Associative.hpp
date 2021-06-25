#pragma once

#include "Test.hpp"

class Associative : public Test {
 public:
  Associative(Arduboy2Base& arduboy, Tinyfont& tinyfont);

  void update() final;
  void render() final;
  bool complete(uint16_t& score) final;

 private:
  void shuffle();
  bool updateMath();

 private:
  enum class Status : uint8_t {
    instructionsMemorize,
    instructionsMath,
    instructionsTest,
    showing,
    math,
    mathError,
    testing,
    testingError,
    score,
    done
  };

  Status status = Status::instructionsMemorize;

  Arduboy2Base& arduboy;
  Tinyfont& tinyfont;

  uint16_t mScore = 0;
  uint8_t options[4] = {0, 0, 0, 0};
  uint8_t correct = 0;
  bool LtoR = true;
  uint32_t millis = 0;
  uint8_t l, r, s;

  static const uint8_t pairsCount = 20;
  uint8_t pairsL[pairsCount] = {
      0,
  };
  uint8_t pairsR[pairsCount] = {
      0,
  };
};
