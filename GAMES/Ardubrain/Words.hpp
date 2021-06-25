#pragma once

#include <Arduboy2.h>
//#include <avr/io.h>
#include <pgmspace.h>
#include <stdint.h>

class Words {
 private:
  Words() = default;
  const __FlashStringHelper* word() const;

 public:
  static const __FlashStringHelper* word(const uint8_t id);

 public:
  /* 700 bytes of data. */
  const char _word[7];
  static const uint8_t wordsCount = 100;
};
