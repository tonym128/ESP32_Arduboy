#pragma once

#include "Test.hpp"

class Breathe : public Test {
 public:
  Breathe(Arduboy2Base& arduboy, Tinyfont& tinyfont) : arduboy(arduboy), tinyfont(tinyfont) {}

  void update() final;
  void render() final;
  bool complete(uint16_t& score) final;

 private:
  enum class Status : uint8_t { instructions, inhale, exhale, score, done };

  Status status = Status::instructions;

  Arduboy2Base& arduboy;
  Tinyfont& tinyfont;

  // 16 breaths 8 seconds long is a 2 minutes exercise
  const uint8_t breathsCount = 16;

  uint16_t mScore = 0;
  uint32_t millis = 0;
  uint16_t count = 0;
  uint32_t level = 0;
  uint32_t error = 0;
};
