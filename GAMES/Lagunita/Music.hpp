#pragma once

#include <stdint.h>
#include <ArduboyTones.h>
#include "Utility.hpp"

/* Class that plays the game music. */
class Music {
 public:
  Music(Arduboy2Base& arduboy) : sound(arduboy.audio.enabled){};
  void init();
  void play();
  bool isPlaying() { return sound.playing(); };

 private:
  ArduboyTones sound;
};
