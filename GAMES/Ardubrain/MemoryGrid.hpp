#pragma once

#include "Test.hpp"

class MemoryGrid : public Test {
 public:
  MemoryGrid(Arduboy2Base& arduboy, Tinyfont& tinyfont)
      : arduboy(arduboy), tinyfont(tinyfont) {}

  void update() final;
  void render() final;
  bool complete(uint16_t& score) final;

 private:
  enum class Status : uint8_t { instructions, showing, solving, score, done };

  Status status = Status::instructions;

  Arduboy2Base& arduboy;
  Tinyfont& tinyfont;

  uint16_t mScore = 0;
  uint16_t millis = 0;
  uint16_t moves = 0;
  static const uint8_t rows = 4;
  static const uint8_t cols = 8;
  uint8_t matrix[rows * cols] = {};
  uint8_t solution[rows * cols] = {};
  uint8_t cursor_x = 0;
  uint8_t cursor_y = 0;
  uint8_t difficulty = 1;

 private:
  void drawGrid(uint8_t mat[]);
  void createTest();
};
