#pragma once

#include "Test.hpp"

class Series : public Test {
 public:
  Series(Arduboy2Base& arduboy, Tinyfont& tinyfont) : arduboy(arduboy), tinyfont(tinyfont) {}

  void update() final;
  void render() final;
  bool complete(uint16_t& score) final;

 private:
  void shuffle();
  void renderSeries(uint8_t limit);

 private:
  enum class Status : uint8_t { instructions, showing, testing, score, done };

  Status status = Status::instructions;

  Arduboy2Base& arduboy;
  Tinyfont& tinyfont;

  static const uint8_t seriesLen = 32;
  uint16_t mScore = 0;
  uint8_t options[4] = {0, 0, 0, 0};
  uint8_t series[seriesLen];
  uint8_t current = 0;
  uint8_t correct = 0;
  uint16_t millis = 0;
};
