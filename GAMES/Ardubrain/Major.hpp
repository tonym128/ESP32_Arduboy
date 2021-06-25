#pragma once

#include "Test.hpp"

class Major : public Test {
 public:
  Major(Arduboy2Base& arduboy, Tinyfont& tinyfont) : arduboy(arduboy), tinyfont(tinyfont) {}

  void update() final;
  void render() final;
  bool complete(uint16_t& score) final;

 private:
  void shuffle();

 private:
  enum class Status : uint8_t { instructions, testing, score, done };

  Status status = Status::instructions;

  Arduboy2Base& arduboy;
  Tinyfont& tinyfont;

  uint16_t mScore = 0;
  uint8_t options[4] = {0, 0, 0, 0};
  uint8_t correct = 0;
  bool wordToNumber = true;
  uint16_t millis = 0;
};
