#ifndef MENU_H
#define MENU_H

#include "assets.h"

#define WARMUP_FRAME_COUNT 300

void stateMenu() {
  Sprites::drawOverwrite(24, 0, StarWarsLogo, 0);
  ab.setCursor(34, 42);
  ab.print(strcpy_P(tBuffer, (char*)pgm_read_dword(&(menuText[0]))));
  ab.setCursor(20, 56);
  ab.print(strcpy_P(tBuffer, (char*)pgm_read_dword(&(menuText[1]))));
}

void stateWarmup() {
  ab.setCursor(8, 10);
  ab.setTextSize(2);
  ab.print("GET READY");
  ab.setTextSize(1);
  ab.setCursor(10, 40);
  ab.print("Remaining Lifes:");
  ab.print(lives);
  if (ab.everyXFrames(255))
  {
    resetLevel();
    gameState = STATE_GAME;
  }
}

int calculateScore(int starting) {
  int score = starting;
  score += lives * 500;
  score += hull * 100;
  score += defeatedTies * 10;
  return score;
}

void stateWin() {
  ab.setCursor(0, 0);
  ab.println("Congratulations!");
  ab.println("");
  ab.println("You've dealt a");
  ab.println("crippling blow to the ");
  ab.println("empire!");
  ab.println("");
  ab.print("SCORE: ");
  ab.print(calculateScore(1000));
  
  ab.setCursor(18, 56);
  ab.println("-Press a Button-");
}

void stateLose() {
   int score = 0;
  score += lives * 500;
  score += hull * 100;
  score += defeatedTies * 10;
  ab.setCursor(0, 0);
  ab.println("Hope is lost!");
  ab.println("");
  ab.println("With you dies the");
  ab.println("last hope for a ");
  ab.println("liberated galaxy...");
  ab.println("");
  ab.print("SCORE: ");
  ab.print(calculateScore(0));
  ab.setCursor(18, 56);
  ab.println("-Press a Button-");
}

#endif
