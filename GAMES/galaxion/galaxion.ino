//
// Galaxian for Arduboy
//

#include "Arduboy2.h"
#include "game.h"
#include <ESPboyPlaytune.h>

Arduboy2 mArduboy;
Game mGame;

ESPboyPlaytune pt(outEn);
boolean outEn() {return mArduboy.audio.enabled();}

void inogamesetup()
{
  mArduboy.begin();
  //mArduboy.beginNoLogo();
  mArduboy.initRandomSeed();
  mArduboy.setFrameRate(60);
  pt.initChannel(0);
#ifdef CAPTURE_SCREEN
  Serial.begin(115200);
#endif
}

void inogameloop()
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
void gameLogicLoop(void *)
{
  for (;;) {
    inogameloop(); 
    // ArduinoOTA.handle();
  }
}

void setup() {
  inogamesetup();
  xTaskCreatePinnedToCore(gameLogicLoop, "g", 4096, nullptr, 0, nullptr, 0);
}

void loop() {
	delay(60000);
}
