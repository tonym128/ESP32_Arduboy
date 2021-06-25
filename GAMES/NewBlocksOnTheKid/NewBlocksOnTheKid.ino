#include <Arduboy2.h>
#include "particle.h"

#define boardWidth 5
#define boardHeight 10
#define partSize 6
#define boardOffset 47
#define dLandedMax 10
#define dStoppedMax 3
#define dClearedMax 20
#define dWordsMax 30
#define dPointsMax 40
#define dParticleMax 40
#define numberOfWords 26
#define numberOfBadWords 10
#define wordBufferLength 10

Arduboy2 arduboy;
BeepPin1 beep;
Particle dotsLanded[dParticleMax];

const char string_0[]  = " Nice";
const char string_1[]  = " Good";
const char string_2[]  = " Cool";
const char string_3[]  = "Sweet";
const char string_4[]  = "Great";
const char string_5[]  = "  Rad";
const char string_6[]  = " Sick";
const char string_7[]  = "Wicked";
const char string_8[]  = "  Ace";
const char string_9[]  = "Deluxe";
const char string_10[]  = " Neat";
const char string_11[]  = "Gnarly";
const char string_12[]  = " Huge";
const char string_13[]  = "  Big";
const char string_14[]  = "Strong";
const char string_15[]  = "Solid";
const char string_16[]  = "Prime";
const char string_17[]  = "Grand";
const char string_18[]  = "Swell";
const char string_19[]  = "Tight";
const char string_20[]  = "Snazzy";
const char string_21[]  = " Super";
const char string_22[]  = "Smooth";
const char string_23[]  = " Fire";
const char string_24[]  = "Groovy";
const char string_25[]  = "  Hot";

const char *const goodWords[]  = {string_0, string_1, string_2, string_3, string_4, string_5, string_6, string_7, string_8, string_9, string_10,
                                         string_11, string_12, string_13, string_14, string_15, string_16, string_17, string_18, string_19, string_20,
                                         string_21, string_22, string_23, string_24, string_25};

const char stringOver_0[]  = "BAD LUCK";
const char stringOver_1[]  = "WRECKED";
const char stringOver_2[]  = "FOOBIES";
const char stringOver_3[]  = "OUCH";
const char stringOver_4[]  = "OH NO";
const char stringOver_5[]  = "BAD TIMES";
const char stringOver_6[]  = "WASTED";
const char stringOver_7[]  = "DESTROYED";
const char stringOver_8[]  = "DAMMIT";
const char stringOver_9[]  = "NUTS";
const char stringOver_10[]  = "GAME OVER";

const char *const badWords[]  = {stringOver_0, stringOver_1, stringOver_2, stringOver_3, stringOver_4, stringOver_5, stringOver_6, stringOver_7, stringOver_8, stringOver_9, stringOver_10};

char buffer[ wordBufferLength ];                                          // make sure this is large enough for the largest string it must hold

bool moveRight, moveLeft, downButton, aButton, bButton;
bool firstLanded = 1;

uint8_t gameBoard[boardWidth][boardHeight];
uint8_t gameState = 8;  // start in special mode
uint8_t gameSpeed = 20;
uint8_t cycleCount;
uint8_t shakeState = 0;
uint8_t shakeCount = 0;
uint8_t endWord;
uint8_t endStage = 0;
uint8_t stageCount = 0;
uint8_t startStage = 0;
uint8_t currentWord = 0;
uint8_t dropCol;
uint8_t dropRow;
uint8_t dropRowSearch;
uint8_t dropCount;

uint16_t currentScore = 0;
uint16_t highScore = 0;

int16_t shakeX, shakeY;

//uint16_t bootKey = 0x7777;
//volatile uint16_t *const bootKeyPtr = (volatile uint16_t *)0x0800;

/*
void getBooted() {
  uint16_t bootKeyPtrVal = *bootKeyPtr;
  if (bootKeyPtrVal == bootKey) {
 //   power_timer0_disable();
    while (true) { delay(1);}
  }
}
*/

void inogamesetup() {
  arduboy.boot();
  arduboy.flashlight();
  arduboy.audio.begin();
  beep.begin();
  arduboy.setFrameRate(60);
  arduboy.clear();
  arduboy.display();
  gameBoard[2][0] = 2;                                                          // hard code first middle part
  //drawPowerOn();
}



void inogameloop() {
  //getBooted();

  if (!(arduboy.nextFrame()))
    return;

  beep.timer();
  arduboy.clear();

  updateDots();
  drawDropped();

  // == Button Polling Station ===

  arduboy.pollButtons();
  if ( arduboy.justPressed(RIGHT_BUTTON) ) moveRight += 1;
  if ( arduboy.justPressed(LEFT_BUTTON) )  moveLeft += 1;
  if ( arduboy.justPressed(DOWN_BUTTON) )  downButton += 1;
  if ( arduboy.justPressed(A_BUTTON) ) aButton += 1;
  if ( arduboy.justPressed(B_BUTTON) )  bButton += 1;

  // == Camera Shake Department ===

  if ( shakeState == 1) {
    if ( shakeCount > 0 ) {
      shakeCount--;
      shakeX = random(-2, 3);
      shakeY = random(-2, 3);
    } else {
      shakeState = 0;
    }
  }
  else if (shakeState == 2) {
    if ( shakeCount > 0 ) {
      shakeCount--;
      shakeY = random(-1, 2);
    } else {
      shakeState = 0;
    }
  }
  else if (shakeState == 3) {
    if ( shakeCount > 0 ) {
      shakeCount--;
      shakeY = 1;
    } else {
      shakeState = 0;
    }
  } else {
    shakeX = 0;
    shakeY = 0;
  }

  // == Game State Logic Party ===

  if ( gameState == 1 ) {
    moveCycle();
    if ( cycleCount >= gameSpeed ) {
      boardCycle();
      cycleCount = 0;
    } else {
      cycleCount++;
    }
    drawFrame();
    drawBoard();
  }
  else if ( gameState == 7 ) {
    drawFrame();
    drawBoard();
    arduboy.setCursor(34, 20);
    arduboy.print("You Win!");
    arduboy.display();
    if ( aButton || bButton ) {
      aButton, bButton = 0;
      resetGame();
    }
  }
  else if ( gameState == 8 ) {
    doGameStart();
  }
  else if ( gameState == 9 ) {
    doGameEnd();
  }

  arduboy.display();
}



void doGameStart() {
  if ( startStage == 0) {
    if (stageCount < 64 ) {
      arduboy.drawLine(63, 0, 63, stageCount);
      stageCount += 2;
    } else {
      arduboy.drawLine(63, 0, 63, 64);
      stageCount = 0;
      startStage = 1;
    }
  }
  else if ( startStage == 1) {
    if (stageCount < (boardWidth * partSize) + 3  ) {
      arduboy.drawRect(63 - (stageCount / 2), 0, stageCount , 64 );
      stageCount += 2;
    } else {
      drawFrame();
      stageCount = 0;
      gameState = 1;                                                      //ok now you can start the game
    }
  }
}


void doGameEnd() {
  drawFrame();
  drawBoard();

  if (endStage == 0) {
    beep.tone(beep.freq(440 - (stageCount * 10)), 6);
    if ( stageCount < 128 ) {

      arduboy.drawLine(0, 31, stageCount, 31);
      stageCount += 6;
    } else {
      arduboy.drawLine(0, 31, 128, 31);
      stageCount = 0;
      endStage = 1;
    }
  }
  else if ( endStage == 1) {
    if ( stageCount < 10 ) {
      arduboy.fillRect(0, 31 - stageCount, 128, stageCount * 2, 0);
      arduboy.drawLine(0, 31 + stageCount, 128, 31 + stageCount);
      arduboy.drawLine(0, 31 - stageCount, 128, 31 - stageCount);
      stageCount++;
    } else {
      arduboy.fillRect(0, 31 - 10, 128, 10 * 2, 0);
      arduboy.drawLine(0, 31 + 10, 128, 31 + 10);
      arduboy.drawLine(0, 31 - 10, 128, 31 - 10);
      stageCount = 0;
      endStage = 2;
    }
  }
  else if ( endStage == 2) {
    arduboy.fillRect(0, 22, 128, 21, 0);
    arduboy.drawLine(0, 21, 128, 21);
    arduboy.drawLine(0, 42, 128, 42);
    strcpy_P(buffer, (char *)pgm_read_dword(&(badWords[ endWord ])));
    int halfWordWidth = ((bufferLength(buffer) * 12) + 1) / 2;
    arduboy.setCursor(64 - halfWordWidth, 25);
    arduboy.setTextSize(2);
    arduboy.print(buffer);
    arduboy.print("!");
    arduboy.setTextSize(1);
    if (stageCount < 10) {                                              // wait for a while so you dont smash through the end screen
      aButton = 0;
      bButton = 0;
      stageCount++;
    } else {
      if ( aButton || bButton ) {
        aButton = 0;
        bButton = 0;
        stageCount = 10;
        endStage = 3;
      }
    }

  }
  else if ( endStage == 3) {
    if ( stageCount > 0 ) {
      arduboy.fillRect(0, 31 - stageCount, 128, stageCount * 2, 0);
      arduboy.drawLine(0, 31 + stageCount, 128, 31 + stageCount);
      arduboy.drawLine(0, 31 - stageCount, 128, 31 - stageCount);
      stageCount--;
    } else {
      arduboy.drawLine(0, 31, 128, 31);
      stageCount = 0;
      endStage = 4;
    }
  }
  else if (endStage == 4) {
    if ( stageCount < 128 ) {
      arduboy.drawLine(stageCount, 31, 128, 31);
      stageCount += 6;
    } else {
      stageCount = 0;
      endStage = 0;
      resetGame();
    }
  }
}


int bufferLength(char b[wordBufferLength]) {
  for (int i = 0; i < wordBufferLength; i++) {
    if ( b[i] == '\0') {
      return i;
    }
  }
}


void resetGame() {
  if ( currentScore > highScore ) highScore = currentScore;
  currentScore = 0;
  for ( int row = 0; row < boardHeight; row++) {
    for ( int col = 0; col < boardWidth; col++) {
      gameBoard[col][row] = 0;
    }
  }
  aButton = 0;
  bButton = 0;
  downButton = 0;
  newPart();
  gameState = 1;
}


void updateDots() {
  for ( int i = 0; i < dParticleMax; i++) {
    dotsLanded[i].update();
    if ( dotsLanded[i].render() ) {                                                             // the dot should be rendered
      int pSize = dotsLanded[i].getSize();
      if ( dotsLanded[i].getType() == 2) {
        arduboy.setCursor( dotsLanded[i].getX(), dotsLanded[i].getY() );
        strcpy_P(buffer, (char *)pgm_read_dword(&(goodWords[ dotsLanded[i].getWord() ])));
        arduboy.print(buffer);
        arduboy.print("!");
      }
      else if ( dotsLanded[i].getType() == 3) {
        arduboy.setCursor( dotsLanded[i].getX(), dotsLanded[i].getY() );
        arduboy.print( dotsLanded[i].getWord() );
      } else {
        if ( pSize == 1 ) {
          arduboy.drawPixel( dotsLanded[i].getX(), dotsLanded[i].getY(), 1);
        } else {
          arduboy.drawRect( dotsLanded[i].getX(), dotsLanded[i].getY(), pSize, pSize, 1);
        }
      }
    }
  }
}


uint8_t numberOfDigits( uint16_t value ) {
       if (value > 999999) return 7;
  else if (value > 99999) return 6;
  else if (value > 9999) return 5;
  else if (value > 999) return 4;
  else if (value > 99) return 3;
  else if (value > 9) return 2;
                else return 1;
}


void drawScore() {
  if ( highScore > currentScore) {
    arduboy.setCursor( 128 - ( numberOfDigits( highScore ) * 6) , 0);
    arduboy.print(highScore);
    arduboy.setCursor( 128 - ( numberOfDigits( currentScore ) * 6) , 8);
    if ( currentScore > 0 ) arduboy.print(currentScore);
  }
  else {
    arduboy.setCursor( 128 - ( numberOfDigits( currentScore ) * 6) , 0);
    if ( currentScore > 0 ) arduboy.print(currentScore);
  }

}


void drawFrame() {
  drawScore();
  arduboy.drawRect( boardOffset + shakeX , 0 + shakeY , boardWidth * partSize + 4, boardHeight * partSize + 4, 1);
}


void drawBoard() {
  for ( int row = 0; row < boardHeight; row++) {
    for ( int col = 0; col < boardWidth; col++) {
      if ( gameBoard[col][row] == 1 || gameBoard[col][row] == 2 ) {
        arduboy.drawRect( (col * partSize) + boardOffset + 2 + shakeX , (row * partSize) + 2 + shakeY , partSize, partSize, 1);
      }
    }
  }
}


void boardCycle() {
  for ( int row = boardHeight - 1; row > -1; row--) {
    for ( int col = boardWidth - 1; col > -1; col--) {
      if ( gameBoard[col][row] == 2 ) {                         // found the moving part
        if ( gameBoard[col][row + 2] == 1 ) {                   // about on top of something ... this seems wrong
          if ( gameBoard[col][row + 1] == 1) {                  // it's extra on top of something ... it got snuck in at the last minute
            gameBoard[col][row] = 1;
            partStopped(col, row);
          } else {
            gameBoard[col][row + 1] = 1;
            gameBoard[col][row] = 0;
            partStopped(col, row + 1);
          }
          return;
        }
        else { // not on top of something
          if ( row == boardHeight - 2 ) {                       // about to be on the ground
            if ( gameBoard[col][row + 1] == 1) {                // it's extra on top of something ... it got snuck in at the last minute
              gameBoard[col][row] = 1;
              partStopped(col, row);
            } else {
              gameBoard[col][row + 1] = 1;
              gameBoard[col][row] = 0;
              partLanded(col);
            }
          } else {
            gameBoard[col][row + 1] = 2;
            gameBoard[col][row] = 0;
          }
        }
      }
    }
  }
}


void newPart() {
  if ( currentWord > numberOfBadWords ) currentWord = 0;
  endWord = currentWord;
  currentWord++;
  gameBoard[random(boardWidth)][0] = 2;
}


void partLanded(int col) {
  if (firstLanded) {
    randomSeed( millis() );                                   // entropy!!
    firstLanded = 0;
  }
  beep.tone(beep.freq(557.330), 3);
  newPart();
  drawLanded(col, boardHeight - 1);
  shakeState = 2;
  shakeCount = 5;
  int summer = 0;
  for ( int col = 0; col < boardWidth; col++) {
    summer = summer + gameBoard[col][boardHeight - 1];
  }
  if (summer == boardWidth) {
    clearRow();
    drawPoints(20);
  }
  else {
    drawPoints(1);
  }
}


void clearRow() {
  drawCleared();
  shakeState = 1;
  shakeCount = 10;
  for ( int row = boardHeight - 1; row > 0; row--) {
    beep.tone( beep.freq(360.330 - (row * 30) ), 3 );
    for ( int col = 0; col < boardWidth; col++) {
      gameBoard[col][row] = gameBoard[col][row - 1];
    }
  }
  for ( int col = 0; col < boardWidth; col++) {
    gameBoard[col][0] = 0;
  }
}


void partStopped(int col, int row) {
  beep.tone(beep.freq(587.330 + ((10 - row) * 30)), 3);
  drawStopped(col, row);
  shakeState = 3;
  shakeCount = 1;
  if ( row == 0 ) {                                             // its at the top
    gameState = 9;
  } else {                                                      // its not at the top
    drawPoints(10 - row);
    newPart();
  }
}


void drawDropped() {
  if (dropCount > 0) {
    dropCount--;
    bool dropBump;
    dropBump = random(2);
    arduboy.drawLine( (dropCol * partSize) + boardOffset + 4 + dropBump, (dropRow * partSize) , (dropCol * partSize) + boardOffset + 4 + dropBump, ((dropRowSearch - 1)*partSize) , 1);
  }
}


void moveDown() {
  for ( int row = 0; row < boardHeight; row++) {
    for ( int col = 0; col < boardWidth; col++) {
      if ( gameBoard[col][row] == 2 ) {
        dropCol = col;
        dropRow = row;
        for ( int rowSearch = row; rowSearch < boardHeight; rowSearch++) {
          if ( gameBoard[col][rowSearch] == 1 ) {
            gameBoard[col][rowSearch - 1] = 1;
            gameBoard[col][row] = 0;
            dropRowSearch = rowSearch;
            dropCount = 1;
            drawDropSmoke();
            partStopped(col, rowSearch - 1);
            return;
          }
        }
        gameBoard[col][boardHeight - 1] = 1;           // shoulda returned if it found something, so guess not slam that puppy down
        gameBoard[col][row] = 0;
        dropRowSearch = boardHeight;
        dropCount = 2;
        drawDropSmoke();
        partLanded(col);
        return;
      }
    }
  }
}


void moveCycle() {
  
  if ( aButton ) {
    aButton = 0;
    moveDown();
  }
  
  if ( bButton ) {
    bButton = 0;
    moveDown();
  }

  if ( downButton) {
    downButton = 0;
    moveDown();
  }

  for ( int row = 0; row < boardHeight; row++) {
    for ( int col = 0; col < boardWidth; col++) {
      if ( gameBoard[col][row] == 2 ) {
        if ( moveRight ) {
          moveRight = 0;
          if ( col < boardWidth - 1 ) {                                 // not at the right edge of the board
            if ( gameBoard[col + 1][row] == 0 ) {                      // nothing to the right
              gameBoard[col + 1][row] = 2;
              gameBoard[col][row] = 0;
              return;
            }
          }
        }
        if ( moveLeft ) {
          moveLeft = 0;
          if ( col > 0 ) {                                            // not at the left edge of the board
            if ( gameBoard[col - 1][row] == 0 ) {                     // nothing to the left
              gameBoard[col - 1][row] = 2;
              gameBoard[col][row] = 0;
              return;
            }
          }
        }
      }
    }
  }
  
}


// -=-=- Welcome to the Particle Zone -=-=-


void drawStopped(int col, int row) {
  for ( int i = 0; i < dStoppedMax; i++) {
    dotsLanded[i].setX( (col * partSize) + boardOffset + random(partSize + 4) );
    dotsLanded[i].setY( (row * partSize) + partSize );
    dotsLanded[i].setVelX( random(-2, 3) );
    dotsLanded[i].setVelY( random(2));
    dotsLanded[i].setCounter( random(10, 15) );
    dotsLanded[i].setSize( random(1, 3) );
    dotsLanded[i].setType( 1 );
  }
}

void drawLanded(int col, int row) {
  for ( int i = 0; i < dLandedMax; i++) {
    dotsLanded[i].setX( (col * partSize) + boardOffset + random(partSize + 4) );
    dotsLanded[i].setY( (row * partSize) + random(partSize + 4) );
    dotsLanded[i].setVelX( random(-3, 4) );
    dotsLanded[i].setVelY( random(4));
    dotsLanded[i].setCounter( random(10, 46) );
    dotsLanded[i].setSize( random(1, 4) );
    dotsLanded[i].setType( 1 );
  }
}

void drawCleared() {
  drawGoodWords();
  for ( int i = 0; i < dClearedMax; i++) {
    dotsLanded[i].setX( boardOffset + random(boardWidth * partSize) );
    dotsLanded[i].setY( ((boardHeight - 1)*partSize) + random(partSize + 2) );
    dotsLanded[i].setVelX( random(-7, 8) );
    dotsLanded[i].setVelY( random(7));
    dotsLanded[i].setCounter( random(10, 66) );
    dotsLanded[i].setSize( random(1, 5) );
    dotsLanded[i].setType( 0 );
  }
}

void drawDropSmoke() {
  for ( int i = dLandedMax; i < dLandedMax + 4; i++) {
    dotsLanded[i].setX( (dropCol * partSize) + boardOffset + 2 + random(partSize) );
    dotsLanded[i].setY( random( (dropRow * partSize) , ((dropRowSearch - 1)*partSize) ) );
    dotsLanded[i].setVelX( 0 );
    dotsLanded[i].setVelY( random(2));
    dotsLanded[i].setCounter( random(5, 10) );
    dotsLanded[i].setSize( 1 );
    dotsLanded[i].setType( 1 );
  }
}

void drawGoodWords() {
  for ( int i = dClearedMax; i < dWordsMax; i++) {
    if ( !dotsLanded[i].render() ) {
      dotsLanded[i].setX( random(0, 4) );
      dotsLanded[i].setY( 63 );
      dotsLanded[i].setVelX( 0 );
      dotsLanded[i].setVelY( 3.5 );
      dotsLanded[i].setCounter( 70 );
      dotsLanded[i].setSize( 1 );
      dotsLanded[i].setType( 2 );
      dotsLanded[i].setWord( random(numberOfWords) );
      return;                                               // we have done what we set out to do
    }
  }
}

void drawPoints(int points) {
  currentScore += points;
  uint8_t scoreCount = 25;
  if ( highScore > currentScore) scoreCount = 20;
  for ( int i = dWordsMax; i < dPointsMax; i++) {
    if ( !dotsLanded[i].render() ) {
      int numOffset = 6;
      if ( points > 9 ) numOffset = 12;
      dotsLanded[i].setX( random(boardOffset - numOffset) + boardOffset + (boardWidth * partSize) + 4);
      dotsLanded[i].setY( 63 );
      dotsLanded[i].setVelX( 0 );
      dotsLanded[i].setVelY( 3.5 );
      dotsLanded[i].setCounter( scoreCount );
      dotsLanded[i].setSize( 1 );
      dotsLanded[i].setType( 3 );
      dotsLanded[i].setWord( points );
      return;                                             // we have done what we set out to do
    }
  }
}

void drawPowerOn() {
  for ( int i = 0; i < dParticleMax; i++) {
    int b = random(1, 5);
    dotsLanded[i].setX( 3 + random( 5 ) );
    dotsLanded[i].setY( 2 );
    dotsLanded[i].setVelX( random(-3, 5) + ((5 - b) * 3) );
    dotsLanded[i].setVelY( random(-3, 2));
    dotsLanded[i].setCounter( random(10, 40) );
    dotsLanded[i].setSize( b );
    dotsLanded[i].setType( 0 );
  }
}


// end of file copyright Kevin Bates 2020
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
