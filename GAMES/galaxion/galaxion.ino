//
// Galaxian for Arduboy
//

#include "Arduboy2.h"
#include "ArduboyTones.h"
#include "game.h"

Arduboy2 mArduboy;
ArduboyTones sound(mArduboy.audio.enabled);

Game mGame;

void setup()
{
  //mArduboy.begin();
  mArduboy.begin();
  mArduboy.initRandomSeed();
  mArduboy.setFrameRate(60);

#ifdef CAPTURE_SCREEN
  Serial.begin(115200);
#endif
}

void loop()
{
  if (!(mArduboy.nextFrame())) {
    return;
  }

  mArduboy.clear();
  mGame.check_input();
  mGame.do_state();
  mGame.draw();
  mArduboy.display();
}
