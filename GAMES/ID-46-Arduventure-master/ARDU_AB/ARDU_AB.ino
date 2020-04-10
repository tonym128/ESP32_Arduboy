/*
  ARDUVENTURE: http://www.team-arg.org/ardu-manual.html

  Arduboy version 1.0.1:  http://www.team-arg.org/ardu-downloads.html

  MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html

  2017 - 2018 - JO3RI GANTOIS - GAVIN ATKIN - OLIVIER HUARD - SIEGFRIED CROES 

  Additional Level Design - Jace Atkin

  Game License: MIT : https://opensource.org/licenses/MIT

  Story, characters, sprites, tiles, design and art: copyrighted to TEAM a.r.g.
*/

//determine the game
#define GAME_ID 46

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

#include "globals.h"
#include "songs.h"
#include "menu.h"
#include "game.h"
#include "inputs.h"
#include "text.h"
#include "inventory.h"
#include "items.h"
#include "player.h"
#include "enemies.h"
#include "battles.h"


typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuContinue,
  stateMenuNew,
  stateMenuSound,
  stateMenuCredits,
  stateGamePlaying,
  stateGameInventory,
  stateGameEquip,
  stateGameStats,
  stateGameMap,
  stateGameOver,
  showSubMenuStuff,
  showSubMenuStuff,
  showSubMenuStuff,
  showSubMenuStuff,
  walkingThroughDoor,
  stateGameBattle,
  stateGameBoss,
  stateGameIntro,
  stateGameNew,
  stateGameSaveSoundEnd,
  stateGameSaveSoundEnd,
  stateGameSaveSoundEnd,
  stateGameObjects,
  stateGameShop,
  stateGameInn,
  battleGiveRewards,
  stateMenuReboot,
};

int main() {
  arduboy.mainNoUSB();
  return 0;
}

void gameLogic(void *) {
  if (!(arduboy.nextFrame())) return;
  //arduboy.fillScreen(1);
  arduboy.pollButtons();
  //arduboy.clear();
  drawTiles();
  updateEyes();
  
  ((FunctionPointer) pgm_read_dword(&mainGameLoop[gameState]))();
  checkInputs();
  if (question) drawQuestion();
  if (yesNo) drawYesNo();
  if (flashBlack) flashScreen(BLACK);     // Set in battleStart
  else if (flashWhite) flashScreen(WHITE);
  //Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
  arduboy.display();
}

void gameLogicLoop(void *)
{
  for (;;) {
    gameLogic(nullptr);
    ArduinoOTA.handle();
  }
}

void loop() {
  delay(60000);
}

void setup() {
  //WiFi.mode(WIFI_OFF);
  EEPROM.begin(100);
  arduboy.boot();
  arduboy.audio.begin();
  //ATM.play(titleSong);
  arduboy.setFrameRate(60);                                 // set the frame rate of the game at 60 fps
  xTaskCreatePinnedToCore(gameLogicLoop, "g", 4096, nullptr, 1, nullptr, 0);
}
