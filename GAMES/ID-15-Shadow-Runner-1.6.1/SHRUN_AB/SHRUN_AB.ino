/*
  SHADOW RUNNER: http://www.team-arg.org/shrun-manual.html

  Arduboy version 1.6 : http://www.team-arg.org/more-about.html

  MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

  2015 - JO3RI - Trodoss

  Special thanks to Dreamer3 for helping with the conversion from Gamby

  Game License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID 15

#include "globals.h"
#include "runner.h"
#include "menu.h"
#include "game.h"
#include "items.h"
#include "playfield.h"


typedef void (*FunctionPointer) ();

const FunctionPointer  mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGameInitLevel,
  stateGamePlaying,
  stateGamePause,
  stateGameOver,
};

void setup () {
  arduboy.begin();
  arduboy.setFrameRate(60);
  arduboy.initRandomSeed();
}

void inogameloop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer) pgm_read_dword (&mainGameLoop[gameState]))();
  arduboy.display();
  delay(1);
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
