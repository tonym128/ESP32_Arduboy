#pragma once

#include "Test.hpp"

class Maze : public Test {
 public:
  Maze(Arduboy2Base& arduboy, Tinyfont& tinyfont) : arduboy(arduboy), tinyfont(tinyfont) {}

  void update() final;
  void render() final;
  bool complete(uint16_t& score) final;

 private:
  bool solveable();

 private:
  enum class Status : uint8_t { instructions, showing, navigating, score, done };

  Status status = Status::instructions;

  Arduboy2Base& arduboy;
  Tinyfont& tinyfont;

  uint16_t mScore = 0;
  uint16_t millis = 0;
  uint16_t moves = 0;

  bool maze[8][16] = {};
  uint8_t posX;
  uint8_t posY;
  uint8_t goalX;
  uint8_t goalY;
};
