/*
  Sunfire  - Space Flight Simulator/Shooter: http://www.team-arg.org/sufi-manual.html

  Arduboy version 0.1:  http://www.team-arg.org/sufi-downloads.html

  MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

  2017/2018 - Trodoss

  License: MIT : https://opensource.org/licenses/MIT
*/

//determine the game
#define GAME_ID 50

#include "globals.h"
#include "menu.h"
#include "game.h"
#include "sound_data.h"

typedef void (*FunctionPointer) ();

const FunctionPointer  mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGamePlaying,
  stateWaveComplete,
  stateGameOver
};


void inogamesetup() {
  arduboy.boot();
  arduboy.audio.begin();
  arduboy.setFrameRate(30);                                 // set the frame rate of the game at 30 fps
  arduboy.initRandomSeed();                                 // This sets the random to more random, remove this if no random is needed !
  //ATM.play(titleSong);
}


void inogameloop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer) pgm_read_dword (&mainGameLoop[gameState]))();
  arduboy.display();
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
