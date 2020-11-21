/*----------------------------------
               JumpBoy


    Created by:takaiwa@gmail.com
    Game Copyright (C) 2016 takaiwa@gmail.com

  ----------------------------------*/
#include <Arduboy2.h>
#include <ArduboyTones.h>

#include "Bitmaps.h"

#define NOTE_A1  110  //spring
#define NOTE_C2  131  //miss spring item(-)
#define NOTE_G2  196
#define NOTE_C3  262  // add coins item
#define NOTE_G3  392  //coin
#define NOTE_C4  523  //heart item,spring item(+),flag item


#define MODE_TITLE  1
#define MODE_START  2
#define MODE_RUNNING  3
#define MODE_GAMECLEAR  4
#define MODE_GAMEOVER  5

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

uint8_t       pMode = MODE_TITLE;

unsigned long lTime;
const byte    FPS = 1000 / 30;

unsigned long pWait = 0;
bool          pWaitFlg = false;

uint8_t  gray = 0;
float    frame = 0;
float    animSpeed = 0.2;
uint8_t  totalFrames = 4;

int      pX = 20;
int      pY = 0;
float  pSpeed = 0;
float  pASpeed = 2;
float  savepASpeed = 2;

int      prevPX = 20;
int      prevPY = 0;
int      pDir = 270;

int      pYY = 1;

uint8_t  pDirectionY = 1;
uint8_t  pSpeedYInit = 1;
uint8_t  pSpeedY = pSpeedYInit;

uint8_t  boundY = 10;
uint8_t  boundH = 10;

const uint8_t MAXSPD = 4;
const uint8_t MAXLIVES = 12;

uint8_t  lives = 3;
uint8_t  level = 1;

int stage = 1;
int record = 0;

int concurrent_coin_max = 1;

/*-------------------------------
    levelStart
  ------------------------------*/
void levelStart(int lvl)
{
  stage = 1;
  pASpeed = 2;
  savepASpeed = 2;

  arduboy.clear();

  if (lvl == 1) {
    stage = 1;
    lives = 3;
    initSpeedScroll();
    arduboy.drawSlowXYBitmap(17, 10, bStart, 96, 48, 1);
  }

  arduboy.display();

  delay(2000);

  initSpring();
  
  initCoin(concurrent_coin_max,false);
  
  initPowerUp();

  level = lvl;
  pY = 24;

  pMode = MODE_RUNNING;
}


/*-------------------------------
    miss
  ------------------------------*/
void miss()
{
  pSpeed = 0;
  sound.tone(NOTE_C2, 160); 
  initPowerUp();
  lives--;
  if (lives <= 0) {
    delay(3000);
    pMode = MODE_TITLE;
  }
}


/*-------------------------------
    stageClear
  ------------------------------*/
void stageClear()
{
  record = stage;

//  pASpeed++;
  pASpeed+=0.5;
  speedupScroll();

  if (pASpeed > 4) {
    pASpeed = 2;
  }

  savepASpeed = pASpeed;
  stage++;

  initSpring();
  initCoin(concurrent_coin_max,false);

  arduboy.clear();
  arduboy.drawSlowXYBitmap(17, 10, bClear, 96, 48, 1);
  arduboy.display();

  delay(2000);
}


/*--------------------------
     Draw Header
  -------------------------*/
void drawHeader()
{
  arduboy.fillRect(0, 0, 127, 10, 1);
  for (int i = 0; i < lives; i++) {
    arduboy.drawBitmap(3 + (i * 10) , 1 , bHeart, 8, 8, 0);
  }
  arduboy.setCursor(72, 1);
  arduboy.write(' ');
  arduboy.write('C');
  arduboy.write('O');
  arduboy.write('I');
  arduboy.write('N');
  arduboy.write(' ');
  arduboy.print(getCoinQty());
  arduboy.write(' ');
}


/*-------------------------
    Draw Player
  ------------------------ */
void drawPlayer()
{
    bool is_miss = false;
    if (pY >= 46) {
      collisionSpring(is_miss);
  
      if (!is_miss) {
        pDir = 270;
        pYY = -1 * pASpeed;
      } else {
        //Serial.println("miss...");
        pY = 12;
  
        // check gameover
        miss();
        
        pWait = millis() + 1000 * 3;
        savepASpeed = pASpeed;
        pASpeed = 0;
        pWaitFlg = true;
  
        //Serial.println(pWait);
        return;
      }
    }
    if (pY <= 12) {
      pDir == 90;
      pYY = 1 * pASpeed;
    }
    pY += pYY;

  arduboy.drawSlowXYBitmap(pX , pY, bMan, 8, 8, 1);
}


/*-------------------------
    displayTitle
  ------------------------ */
void displayTitle()
{
  int flash = 0;
  
  while (true) {
    delay( 30 );
    arduboy.clear();

    arduboy.setCursor(36, 1);
    arduboy.print("JUMP BOY");
    
    arduboy.drawSlowXYBitmap(14, 12, bTitle, 104, 36, 1);

    flash++;
    flash %= 50;

    if (record > 0) {
      arduboy.setCursor(30, 44);
      arduboy.print(" record: ");
      arduboy.print(record);
      arduboy.print(" ");         
    }

    if (flash < 25) {
      arduboy.setCursor(30, 54);
      arduboy.print("Press A or B");
    }

    arduboy.display();

    if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
      pMode = MODE_START;
      break;
    }
  }
}


/*-------------------------
    setup
  ------------------------ */
void setup()
{
  //Serial.print("Entering Setup");
  arduboy.begin();
  arduboy.audio.on();

  displayTitle();
  lTime = millis();
  arduboy.initRandomSeed();
}


/*-------------------------
    LOOP
  ------------------------ */
void loop()
{ 
  if (pMode == MODE_TITLE) {
    displayTitle();
    return;
  }

  if (pMode == MODE_START) {
    levelStart(1);
    return;
  }
  
  if (pWaitFlg) {
    if (millis() > pWait) {
      //Serial.println("wait brake....");
      pWaitFlg = false;
      return;
    } else {
      //Serial.print("waiting....\n");
      //Serial.println(millis());
      return;
    }
  }

  //Serial.print("Loop");
 
  if (millis() > lTime + FPS) {
    if (pSpeed == 0) {
      frame = 0;
    }

    prevPX = pX;
    prevPY = pY;
    pSpeed = 0;

    if (pASpeed == 0) {
      if (arduboy.buttonsState()) {
        pASpeed = 1;
        pSpeed = pASpeed;
      }
    } else {
      if (arduboy.pressed(UP_BUTTON)) {
        pDir = 90;
        pASpeed = savepASpeed;
        pSpeed = pASpeed;
      } else if (arduboy.pressed(DOWN_BUTTON)) {
        pDir = 270;
        pASpeed = savepASpeed;
        pSpeed = pASpeed;
      } else if (arduboy.pressed(LEFT_BUTTON)) {
        pDir = 180;
        pASpeed = savepASpeed;
        pSpeed = pASpeed;
      } else if (arduboy.pressed(RIGHT_BUTTON)) {
        pDir = 0;
        pASpeed = savepASpeed;
        pSpeed = pASpeed;
      }
    }

    if (pSpeed > 0) {
      switch (pDir) {
        case 180:
          pX -= pSpeed;
          break;
        case 0:
          pX += pSpeed;
          break;
      }
    }

    if (pY > 56 - boundH) {
      pY = 56 - boundH;
    }

    if (pY < boundY) {
      pY = boundY;
    }

    if (pX > 120) {
      pX = 120;
    }

    if (pX < 8) {
      pX = 8;
    }


    if ( prevPX != pX || prevPY != pY ) {
      frame += animSpeed * pASpeed;
    }
    if (frame > 4) {
      frame = 0;
    }

    movePowerUp();
    moveCoin();
    moveSpring();

    arduboy.clear();
    lTime = millis();

    drawHeader();
    drawSpring();

    drawPlayer();
    drawPowerUp();
    drawCoin();

    /*----------------------------
        Drawing - Display
      ---------------------------*/
    arduboy.display();
  }
}
