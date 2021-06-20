#ifndef GAME_H
#define GAME_H

#include "global.h"

#define TRENCH_BACKGROUND_FRAMES 2

byte trenchBackgroundFrame = 0;

// TODO: No magic numbers
void drawTrench() {
  if(currentDistanceFromExaustPort == 0) {
    Sprites::drawOverwrite(-16 - xDisplacement / 3,
                         -14 - yDisplacement / 3,
                         TrenchBackgroundSprites, 
                         3); 
  } else {
     Sprites::drawOverwrite(-16 - xDisplacement / 3,
                           -14 - yDisplacement / 3,
                           TrenchBackgroundSprites, 
                           trenchBackgroundFrame); 
                    
    if (ab.everyXFrames(5)) {
      (trenchBackgroundFrame == TRENCH_BACKGROUND_FRAMES) ? trenchBackgroundFrame = 0 : trenchBackgroundFrame += 1;
    }
  }
}

void drawCockpit() {
  //TODO: No Magic Numbers
  int8_t extraDisplacement = 0;
  if(shipShakeCounter > 0) {
    extraDisplacement = (shipShakeCounter % 4 == 0) ? 2 : -2;
    shipShakeCounter--;
  }
  
  Sprites::drawPlusMask((-16 + xDisplacement / 2) + extraDisplacement,
                        (-14 + yDisplacement / 2) + extraDisplacement * -1, cockpit, 0);
                        
  ab.setCursor((50 + xDisplacement / 2) + extraDisplacement,
               (50 + yDisplacement / 2) + extraDisplacement * -1);
  ab.print("0");         
  if(currentDistanceFromExaustPort < 1000) ab.print("0");
  if(currentDistanceFromExaustPort < 100) ab.print("0");
  if(currentDistanceFromExaustPort < 10) ab.print("0");
  
  ab.print(currentDistanceFromExaustPort);
}

void drawCrosshair() {
  Sprites::drawPlusMask(crosshairX, crosshairY, crosshair, 0);
}

void decreaseDistance() {
    if (ab.everyXFrames(1) && currentDistanceFromExaustPort > 0) {
    currentDistanceFromExaustPort = currentDistanceFromExaustPort - 1;
  }
}

void calculateDisplacement() {
  xDisplacement = crosshairX - DEFAULT_CROSSHAIR_X;
  yDisplacement = crosshairY - DEFAULT_CROSSHAIR_Y;
}

void drawTies() {
  for(byte i = 0; i < TIE_MAX_COUNT; i++) {
    if(!tieAlive[i] && tieExplosionCooldown[i] > 0) {
      tieExplosionCooldown[i]--;
      Sprites::drawPlusMask(tieX[i] - xDisplacement / 4, tieY[i] - yDisplacement / 4, tie, 8);
    } else if(tieAlive[i]) {
      Sprites::drawPlusMask(tieX[i] - xDisplacement / 4, tieY[i] - yDisplacement / 4, tie, tieFrame[i]);
    }
  }
}

void checkForTieRespawn() {
  if(ab.everyXFrames(60) && currentDistanceFromExaustPort > 0) {
    for(byte i = 0; i < TIE_MAX_COUNT; i++) {
      if(!tieEnabled[i]) break;
      if(!tieAlive[i]) {
        if(tieRespawnTime[i] == 0) {
          Serial.println("isnt alive");
          tieRespawnTime[i] = random(1, 4);
          tieAlive[i] = true;
          tieX[i] = random(0, 100);
          tieY[i] = random(0, 36);
          tieFrame[i] = 0;
        } else {
          tieRespawnTime[i] -= 1;
        }
      }
    }
  }
}

void advanceTies() {
  if(ab.everyXFrames(tieFrameSpeed)) {
    for(byte i = 0; i < TIE_MAX_COUNT; i++) { 
      if(tieAlive[i]) {
        if(tieFrame[i] == TIE_FRAMES) {
//          beep.tone(beep.freq(323.251), 5);
          tone(D3,323.251, 5);
          tieAlive[i] = false;
          shipShakeCounter = SHIP_SHAKE_COOLDOWN;
          hull -= 1;
        } else {
          tieFrame[i] += 1;
        }
      }
    }
  }
}

void checkDeath() {
  if(hull == 0 ) {
    lives--;
    explosionBlinkCounter = EXPLOSION_BLINK_FRAMES;
    gameState = STATE_DEATH;
  }
}

void checkFinalShot() {
  if(currentDistanceFromExaustPort == 0) {
    if(lastShotCooldown == 0) {
      hull = 0;
    }
    lastShotCooldown--;
  }
}

void checkShoot() {
  if(shootCooldownCounter > 0) {
    shootCooldownCounter--;
  }
  if(shooting) {
    //beep.tone(beep.freq(587.330), 5);
    tone(D3, 587.330, 5);
    
    byte crosshairXCenter = crosshairX + 5;
    byte crosshairYCenter = crosshairY + 5;

    if(currentDistanceFromExaustPort == 0) {
      if(crosshairXCenter >= 58 && 
             crosshairXCenter <= 70 &&
             crosshairYCenter >= 26 && 
             crosshairYCenter <= 38) {
        gameState = STATE_WIN;       
      }
    }

    for(byte i = 0; i < TIE_MAX_COUNT; i++) { 
      if(tieAlive[i]) {
          byte tieXCollisionStart = tieX[i] - (xDisplacement / 4) + 2;
          byte tieYCollisionStart = tieY[i] - (yDisplacement / 4) + 2;
          byte tieXCollisionEnd = (tieX[i] - (xDisplacement / 4)) + 26;
          byte tieYCollisionEnd = (tieY[i] - (yDisplacement / 4)) + 30;
          if(crosshairXCenter >= tieXCollisionStart && 
             crosshairXCenter <= tieXCollisionEnd &&
             crosshairYCenter >= tieYCollisionStart && 
             crosshairYCenter <= tieYCollisionEnd) {
            tieAlive[i] = false;
            defeatedTies++;
            tieExplosionCooldown[i] = EXPLOSION_COOLDOWN;
          }
       }
     }
    blinkCounter = BLINK_FRAMES;
    shooting = false;
  }
}

void drawBlink() {
  if(blinkCounter > 0) {
    if(blinkCounter % 2) {
      ab.fillRect(0, 0, 128, 64);
    }
    blinkCounter--;
  }
}

void drawHull() {
  ab.setCursor(36,0);
  ab.print("hull:");
  ab.print((hull * 100)/3);
  ab.print("%");
}

void checkTieBehavior() {
  if(currentDistanceFromExaustPort == DISTANCE_ENABLE_TIE2 && !tieEnabled[1]) {
    tieEnabled[1] = true;
  } else if(currentDistanceFromExaustPort == DISTANCE_ENABLE_SPEED) {
    tieFrameSpeed = TIE_FRAME_SPEED_FAST;
  } else if(currentDistanceFromExaustPort == DISTANCE_ENABLE_MAX_SPEED) {
    tieFrameSpeed = TIE_FRAME_SPEED_FASTEST;
  }
}


void stateGame() {
  checkTieBehavior();
  checkShoot();
  checkFinalShot();
  checkDeath();
  calculateDisplacement();
  advanceTies();
  checkForTieRespawn();
  drawTrench();
  drawTies();
  drawBlink();
  drawCockpit();
  drawCrosshair();
  drawHull();
  decreaseDistance();
}

void statePause() {
  ab.setCursor(44, 32);
  ab.println("-PAUSE-");
}

void stateDeath() {
  if(explosionBlinkCounter == 0) {
    if (lives == 0) {
      gameState = STATE_LOSE;
    } else {
      gameState = STATE_WARMUP;
    }
  } else {
    if(explosionBlinkCounter % 8) {
      ab.fillRect(0, 0, 128, 64);
    }
      explosionBlinkCounter--;
    }
}

#endif
