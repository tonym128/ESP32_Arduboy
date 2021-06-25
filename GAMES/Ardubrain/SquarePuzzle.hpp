#pragma once

#include "Test.hpp"

class SquarePuzzle : public Test {
 public:
  SquarePuzzle(Arduboy2Base& arduboy, Tinyfont& tinyfont)
      : arduboy(arduboy), tinyfont(tinyfont) {}

  void update() final;
  void render() final;
  bool complete(uint16_t& score) final;

 private:
  enum class Status : uint8_t { instructions, solving, score, done };

  Status status = Status::instructions;

  Arduboy2Base& arduboy;
  Tinyfont& tinyfont;

  uint16_t mScore = 0;
  uint16_t millis = 0;
  uint16_t moves = 0;
  static const uint8_t cells = 4;
  uint8_t matrix[cells * cells] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};
  uint8_t hole_x = 3;
  uint8_t hole_y = 3;

 private:
  void hN();
  void hS();
  void hW();
  void hE();
};
