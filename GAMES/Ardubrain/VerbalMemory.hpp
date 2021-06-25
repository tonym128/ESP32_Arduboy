#pragma once

#include "Test.hpp"

class VerbalMemory : public Test {
 public:
  VerbalMemory(Arduboy2Base& arduboy, Tinyfont& tinyfont)
      : arduboy(arduboy), tinyfont(tinyfont) {}

  void update() final;
  void render() final;
  bool complete(uint16_t& score) final;

 private:
  enum class Status : uint8_t { instructions, running, score, done };

  Status status = Status::instructions;

  Arduboy2Base& arduboy;
  Tinyfont& tinyfont;

  uint16_t mScore = 0;
  uint16_t millis = 0;

  // 16 random words.
  static const uint8_t wordsCount = 16;  // TODO : increase it while playing?
  uint8_t words[wordsCount];
  uint8_t wordsHits[wordsCount];

  uint8_t currentWord;
};
