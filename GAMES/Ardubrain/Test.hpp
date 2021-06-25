#pragma once

#include <Arduboy2.h>
#include "Tinyfont.hpp"

class Test {
 public:
  virtual ~Test(){};
  virtual void update() = 0;
  virtual void render() = 0;
  virtual bool complete(uint16_t& score) = 0;
};
