/*
 Trench Run
 Copyright (C) 2019 Lucas Cardinali
 All rights reserved.
*/


#include "global.h"
#include "assets.h"
#include "intro.h"
#include "input.h"
#include "menu.h"
#include "game.h"


typedef void (*FunctionPointer) ();

const FunctionPointer  mainGameLoop[] = {
  stateIntroText,
  stateIntroCrawl,
  stateMenu,
  stateWarmup,
  stateGame,
  statePause,
  stateDeath,
  stateWin,
  stateLose,
};

void inogamesetup() {
  ab.begin();
  //beep.begin();
  //tunestunes.initChannel(PIN_SPEAKER_1);
  //tunes.toneMutesScore(true);
  ab.initRandomSeed();
  ab.setFrameRate(FPS);
}

void inogameloop() {
  if (!(ab.nextFrame())) return;
  //beep.timer();
  ab.pollButtons();
  ((FunctionPointer) pgm_read_dword(&mainGameLoop[gameState]))();
  checkInput();
  ab.display(true);
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
