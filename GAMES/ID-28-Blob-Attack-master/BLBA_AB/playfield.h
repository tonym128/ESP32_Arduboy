#ifndef PLAYFIELD_H
#define PLAYFIELD_H

#include <Arduino.h>
#include "globals.h"

#define PLAYFIELD_WIDTH          8         // Playfield width in blobs
#define PLAYFIELD_HEIGHT         10        // Playfield height in blobs
#define PLAYFIELD_ZERO_X         2         // zero x-position of the playfield in pixel position on the screen
#define PLAYFIELD_ZERO_Y         3         // zero x-position of the playfield in pixel position on the screen
#define BLOB_FREE                0         // an empty place in the playfield

#define BLOB_PIXELS              6
#define TILES_IN_BLOBS           3      // number of vertical or horizontal tiles in blobsgrid

#define BLOB_CURRENT             0
#define BLOB_NEXT                2
#define BLOB_WAITING             4

#define NO_FLAG_ON_FIELD         0
#define FLAG_ON_FIELD            1

#define ELF_NORMAL               0
#define ELF_THUMBSUP             1
#define ELF_STRESSED             2
#define ELF_PAUSED               3
#define ELF_NOD                  4

boolean giveExtraScore;
boolean canMoveBlobsDown;
boolean showCombo;
boolean showSpeedUp;
boolean showNod;
boolean showPop;
boolean showChain;
byte elfState;

unsigned long extraScoreForChain;

int field[PLAYFIELD_WIDTH][PLAYFIELD_HEIGHT];
int fieldFlags[PLAYFIELD_WIDTH][PLAYFIELD_HEIGHT];

int randomBlobPit[6];
int blobNumbers;
byte blobFrame;

const unsigned char PROGMEM elfStressedBodySequenceY[] = {34, 32, 33};
const unsigned char PROGMEM elfStressedHeadSequenceY[] = {0, 4, 2};
const unsigned char PROGMEM elfStressedWandSequenceX[] = {51, 51, 50};
const unsigned char PROGMEM elfStressedWandSequenceY[] = {26, 24, 26};

const unsigned char PROGMEM elfPausedBodySequenceY[] = {38, 37, 37, 37, 37, 37, 37, 38, 40, 41, 42, 42, 42, 42, 42, 42, 42, 42, 41, 40, 39,};
const unsigned char PROGMEM elfPausedHeadSequenceX[] = {50, 50, 50, 50, 50, 50, 50, 50, 51, 51, 52, 52, 52, 52, 52, 52, 52, 51, 51, 50, 50,};
const unsigned char PROGMEM elfPausedHeadSequenceY[] = { 4, 4, 4, 4, 4, 4, 4, 4, 3, 1, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4,};
const unsigned char PROGMEM elfPausedWandSequenceX[] = {100, 100, 100, 100, 100, 100, 100, 101, 100, 99, 98, 97, 97, 97, 97, 97, 97, 97, 97, 98, 99, 100,};
const unsigned char PROGMEM elfPausedWandSequenceY[] = { 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 2, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3,};
const unsigned char PROGMEM elfPausedMouthSequence[] = {0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 4, 3, 2, 2, 5};

const unsigned char PROGMEM thumbsUpSequence[] = {0, 0, 0, 1, 2, 3, 3, 3, 3, 3};
const unsigned char PROGMEM nodSequence[] = {0, 0, 0, 1, 2, 3 };
const unsigned char PROGMEM popSequence[] = {0, 1, 2, 3 };

const unsigned char PROGMEM elfNormalEyesSequence[] = {0, 1, 2, 3, 4, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};

const unsigned char PROGMEM speedSequenceY[] = {0, 6, 12, 14, 16, 14, 16, 16};
const unsigned char PROGMEM upSequenceY[] = {48, 42, 36, 34, 32, 34, 32, 32};
const unsigned char PROGMEM chainSequenceY[] = {32, 32, 32, 32, 34, 32, 16, 0};

byte elfStressedFrame;
byte elfPausedFrame;
byte elfNormalFrame;
byte thumbsUpFrame;
byte speedUpFrame;
byte nodFrame;
byte popFrame;
byte chain;
byte chainFrame;
int currentBlobs[] =
{
  // array for current blob
  0, 0, 0,
  0, 0, 0,
  0, 0, 0,
};

int blobsXY[2];          // X and Y coordinates for the small 3x3 blob grid

// playfield
//-----------

void initPlayfield()
{
  for (int x = 0; x < PLAYFIELD_WIDTH; x++)
  {
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
    {
      field[x][y] = BLOB_FREE;
    }
  }
}

void removeFlag()
{
  for (int x = 0; x < PLAYFIELD_WIDTH; x++)
  {
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
    {
      fieldFlags[x][y] = NO_FLAG_ON_FIELD;
    }
  }
}


boolean isTileFree(int array_x, int array_y)
{
  if (field [array_x][array_y] == BLOB_FREE) return true;
  else return false;
}


boolean isOnlyOneBlob()
{
  byte temp = 0;
  for (int i = 0; i < 9; i++)
  {
    if (currentBlobs[i] != 0)temp++;
  }
  if (temp < 2) return true;
  else return false;
}


void storeBlob(int array_x, int array_y)
{
  int draw_pointer = 0;
  for (int y = array_y; y < array_y + TILES_IN_BLOBS; y++)
  {
    for (int x = array_x; x < array_x + TILES_IN_BLOBS; x++)
    {
      if (currentBlobs[draw_pointer] != 0) field[x][y] = currentBlobs[draw_pointer];
      draw_pointer++;
    }
  }
}

void storeOneBlob(int array_x, int array_y)
{
  // if the blob is not on the floor
  if ((array_y) < PLAYFIELD_HEIGHT - 2)
  {

    for (int x = 0; x < TILES_IN_BLOBS; x++)
    {
      if ((!isTileFree(array_x + x, array_y + 2)) && (!isOnlyOneBlob()) && (currentBlobs[3 + x] != 0))
      {
        field [array_x + x][array_y + 1] = currentBlobs[3 + x];
        currentBlobs[3 + x] = 0;
      }
    }
  }
}

void drawField()
{
  for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
  {
    for (int x = 0 ; x < PLAYFIELD_WIDTH; x++)
    {
      // draw every tile in the playfield
      //if (field [x][y] != BLOB_FREE) sprites.drawOverwrite((x * BLOB_PIXELS) + PLAYFIELD_ZERO_X, (y * BLOB_PIXELS) + PLAYFIELD_ZERO_Y, animatedBlobs, 0);
      sprites.drawSelfMasked((x * BLOB_PIXELS) + PLAYFIELD_ZERO_X, (y * BLOB_PIXELS) + PLAYFIELD_ZERO_Y, animatedBlobs, (4 * field[x][y]) + blobFrame);
    }
  }
}


boolean aboveIsSameBlob(int array_x, int array_y)
{
  if ((array_y - 1 > 0) && (field [array_x][array_y] == field [array_x][array_y - 1])) return true;
  else return false;
}

boolean underIsSameBlob(int array_x, int array_y)
{
  if ((array_y + 1 < PLAYFIELD_HEIGHT) && (field [array_x][array_y] == field [array_x][array_y + 1])) return true;
  else return false;
}

boolean rightIsSameBlob(int array_x, int array_y)
{
  if ((array_x + 1 < PLAYFIELD_WIDTH ) && (field [array_x][array_y] == field [array_x + 1][array_y])) return true;
  else return false;
}

boolean leftIsSameBlob(int array_x, int array_y)
{
  if ((array_x > 0 ) && (field [array_x][array_y] == field [array_x - 1][array_y])) return true;
  else return false;
}


boolean isMovePossible (int array_x, int array_y)
{
  // checks collision with blocks already stored in the playfield
  // check if the 3x3 tiles of a blob with the correct area in the playfield provide by draw_x and draw_y
  int draw_pointer = 0;
  for (int y = array_y; y < array_y + TILES_IN_BLOBS; y++)
  {
    for (int x = array_x; x < array_x + TILES_IN_BLOBS; x++)
    {
      // check if the block is outside the limits of the playfield
      if (x < 0 || x > PLAYFIELD_WIDTH - 1 || y > PLAYFIELD_HEIGHT - 1)
      {
        byte temp = currentBlobs[draw_pointer];
        if ( temp != 0) return false;
      }

      // check if the block has collided with tile already stored in the playfield array
      byte temp = currentBlobs[draw_pointer];
      if ((temp != 0) && (isTileFree(x, y) == false)) return false;
      draw_pointer++;
    }
  }
  return true;
}


boolean isOneBlobDropPossible(int array_x, int array_y)
{
  // checks 1 blob collision with blobs already stored in the playfield
  if ((currentBlobs[1] == 0) && (currentBlobs[7] == 0))
  {
    for (byte temp = 3; temp < 6; temp++)
    {
      if ((currentBlobs[temp] != 0) && (isTileFree(array_x, array_y) == false)) return true;
    }
  }
  else return false;
}



void moveBlobsDown()
{
  canMoveBlobsDown = false;
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (byte row = PLAYFIELD_HEIGHT - 1; row > 0; row--)
    {
      if (isTileFree(column, row))
      {
        if (!isTileFree(column, row - 1))
        {
          field [column][row] = field [column][row - 1];
          field [column][row - 1] = BLOB_FREE;
          drawField();
          canMoveBlobsDown = true;
        }
      }
    }
  }
}

void removeGroups()
{
  if (!showPop)
  {
    for (int x = 0; x < PLAYFIELD_WIDTH; x++)
    {
      for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
      {
        if (fieldFlags[x][y] == FLAG_ON_FIELD)
        {
          giveExtraScore = true;
          field[x][y] = BLOB_FREE;
          scorePlayer += 50;
        }
      }
    }
    if (giveExtraScore == true)
    {
      scorePlayer += extraScoreForChain;
      extraScoreForChain += 500;
      chain++;
      showNod = true;
      if (chain > 1)
      {
        showChain = true;
        showCombo = true;
      }
      sound.tone(440, 100);
      delay(100);
      sound.tone(1047, 200);
    }
    giveExtraScore = false;
    removeFlag();
  }
}


void fourInPack()
{
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (byte row = PLAYFIELD_HEIGHT - 1; row > 0; row--)
    {
      if (!isTileFree(column, row))
      {
        if (aboveIsSameBlob(column, row) && rightIsSameBlob(column, row) && aboveIsSameBlob(column + 1, row))
        {
          // XX
          // XX
          fieldFlags[column][row] = FLAG_ON_FIELD;
          fieldFlags[column][row - 1] = FLAG_ON_FIELD;
          fieldFlags[column + 1][row] = FLAG_ON_FIELD;
          fieldFlags[column + 1][row - 1] = FLAG_ON_FIELD;
          showPop = true;
        }
        if (rightIsSameBlob(column, row) && aboveIsSameBlob(column + 1, row) && rightIsSameBlob(column + 1, row - 1))
        {
          //  XX
          // XX
          fieldFlags[column][row] = FLAG_ON_FIELD;
          fieldFlags[column + 1][row] = FLAG_ON_FIELD;
          fieldFlags[column + 1][row - 1] = FLAG_ON_FIELD;
          fieldFlags[column + 2][row - 1] = FLAG_ON_FIELD;
          showPop = true;
        }
        if (rightIsSameBlob(column, row) && underIsSameBlob(column + 1, row) && rightIsSameBlob(column + 1, row + 1))
        {
          // XX
          //  XX
          fieldFlags[column][row] = FLAG_ON_FIELD;
          fieldFlags[column + 1][row] = FLAG_ON_FIELD;
          fieldFlags[column + 1][row + 1] = FLAG_ON_FIELD;
          fieldFlags[column + 2][row + 1] = FLAG_ON_FIELD;
          showPop = true;
        }
        if (aboveIsSameBlob(column, row) && rightIsSameBlob(column, row - 1) && aboveIsSameBlob(column + 1, row - 1))
        {
          //  X
          // XX
          // X
          fieldFlags[column][row] = FLAG_ON_FIELD;
          fieldFlags[column][row - 1] = FLAG_ON_FIELD;
          fieldFlags[column + 1][row - 1] = FLAG_ON_FIELD;
          fieldFlags[column + 1][row - 2] = FLAG_ON_FIELD;
          showPop = true;
        }
        if (aboveIsSameBlob(column, row) && leftIsSameBlob(column, row - 1) && aboveIsSameBlob(column - 1, row - 1))
        {
          // X
          // XX
          //  X
          fieldFlags[column][row] = FLAG_ON_FIELD;
          fieldFlags[column][row - 1] = FLAG_ON_FIELD;
          fieldFlags[column - 1][row - 1] = FLAG_ON_FIELD;
          fieldFlags[column - 1][row - 2] = FLAG_ON_FIELD;
          showPop = true;
        }
      }
    }
  }
}


void fourInColumn()
{
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (byte row = PLAYFIELD_HEIGHT - 1; row > 0; row--)
    {
      if (!isTileFree(column, row))
      {

        if (aboveIsSameBlob(column, row) && aboveIsSameBlob(column, row - 1))
        {
          // X
          // X
          // X
          if (aboveIsSameBlob(column, row - 2))
          {
            // X
            // X
            // X
            // X
            fieldFlags[column][row] = FLAG_ON_FIELD;
            fieldFlags[column][row - 1] = FLAG_ON_FIELD;
            fieldFlags[column][row - 2] = FLAG_ON_FIELD;
            fieldFlags[column][row - 3] = FLAG_ON_FIELD;
            showPop = true;
          }
          for (byte temp = 0; temp < 3; temp++)
          {
            if (rightIsSameBlob(column, row - temp))
            {
              // X        // X        // XX
              // X        // XX       // X
              // XX       // X        // X
              fieldFlags[column][row] = FLAG_ON_FIELD;
              fieldFlags[column][row - 1] = FLAG_ON_FIELD;
              fieldFlags[column][row - 2] = FLAG_ON_FIELD;
              fieldFlags[column + 1][row - temp] = FLAG_ON_FIELD;
              showPop = true;
            }
          }
          for (byte temp = 0; temp < 3; temp++)
          {
            if (leftIsSameBlob(column, row - temp))
            {
              //  X       //  X       // XX
              //  X       // XX       //  X
              // XX       //  X       //  X
              fieldFlags[column][row] = FLAG_ON_FIELD;
              fieldFlags[column][row - 1] = FLAG_ON_FIELD;
              fieldFlags[column][row - 2] = FLAG_ON_FIELD;
              fieldFlags[column - 1][row - temp] = FLAG_ON_FIELD;
              showPop = true;
            }
          }
        }
      }
    }
  }
}


void fourInRow()
{
  //check if 4 or more blobs are equal in the same row
  for (byte column = 0; column < PLAYFIELD_WIDTH; column++)
  {
    for (byte row = PLAYFIELD_HEIGHT - 1; row > 0; row--)
    {
      if (!isTileFree(column, row))
      {
        if (rightIsSameBlob(column, row) && rightIsSameBlob(column + 1, row))
        {
          // XXX
          if (rightIsSameBlob(column + 2, row))
          {
            // XXXX
            fieldFlags[column][row] = FLAG_ON_FIELD;
            fieldFlags[column + 1][row] = FLAG_ON_FIELD;
            fieldFlags[column + 2][row] = FLAG_ON_FIELD;
            fieldFlags[column + 3][row] = FLAG_ON_FIELD;
            showPop = true;
          }
          for (byte temp = 0; temp < 3; temp++)
          {
            if (aboveIsSameBlob(column + temp, row))
            {
              // X        //  X       //   X
              // XXX      // XXX      // XXX
              fieldFlags[column][row] = FLAG_ON_FIELD;
              fieldFlags[column + 1][row] = FLAG_ON_FIELD;
              fieldFlags[column + 2][row] = FLAG_ON_FIELD;
              fieldFlags[column + temp][row - 1] = FLAG_ON_FIELD;
              showPop = true;
            }
          }
          for (byte temp = 0; temp < 3; temp++)
          {
            if (underIsSameBlob(column + temp, row))
            {
              // XXX      // XXX      // XXX
              // X        //  X       //   X
              fieldFlags[column][row] = FLAG_ON_FIELD;
              fieldFlags[column + 1][row] = FLAG_ON_FIELD;
              fieldFlags[column + 2][row] = FLAG_ON_FIELD;
              fieldFlags[column + temp][row + 1] = FLAG_ON_FIELD;
              showPop = true;
            }
          }
        }
      }
    }
  }
}


void clearBlob(int array_x, int array_y)
{
  field [array_x][array_y] = BLOB_FREE;
}

void moveBlobsRight()
{
  if (!isOnlyOneBlob() && isMovePossible(blobsXY[0] + 1, blobsXY[1])) blobsXY[0]++;
}


void moveBlobsLeft()
{
  if (!isOnlyOneBlob() && isMovePossible(blobsXY[0] - 1, blobsXY[1])) blobsXY[0]--;
}


void rotateBlobsRight()
{
  if (!isOnlyOneBlob())
  {
    byte temp = currentBlobs[1];
    currentBlobs[1] = currentBlobs[3];
    currentBlobs[3] = currentBlobs[7];
    currentBlobs[7] = currentBlobs[5];
    currentBlobs[5] = temp;
    sound.tone(330, 25);
  }
  if (!isMovePossible(blobsXY[0], blobsXY[1]))
  {
    byte temp = currentBlobs[1];
    currentBlobs[1] = currentBlobs[5];
    currentBlobs[5] = currentBlobs[7];
    currentBlobs[7] = currentBlobs[3];
    currentBlobs[3] = temp;
  }
}

void rotateBlobsLeft()
{
  if (!isOnlyOneBlob())
  {
    byte temp = currentBlobs[1];
    currentBlobs[1] = currentBlobs[5];
    currentBlobs[5] = currentBlobs[7];
    currentBlobs[7] = currentBlobs[3];
    currentBlobs[3] = temp;
    sound.tone(330, 25);
  }
  if (!isMovePossible(blobsXY[0], blobsXY[1]))
  {
    byte temp = currentBlobs[1];
    currentBlobs[1] = currentBlobs[3];
    currentBlobs[3] = currentBlobs[7];
    currentBlobs[7] = currentBlobs[5];
    currentBlobs[5] = temp;
  }
}


void fillBlobPit()
{
  for (byte x = 0; x < 6; x++)
  {
    randomBlobPit[x] = random(1, 4);
  }
}

void drawBlobs (int draw_x, int draw_y, int which_blobs)
{
  switch (which_blobs)
  {
    case BLOB_CURRENT:
      {
        int draw_pointer = 0;
        for (int y = draw_y; y < draw_y + 18; y = y + BLOB_PIXELS)
        {
          for (int x = draw_x; x < draw_x + 18; x = x + BLOB_PIXELS)
          {
            int temp = currentBlobs[draw_pointer];
            if (temp > 0)
            {
              sprites.drawSelfMasked(x, y, animatedBlobs, (4 * temp) + blobFrame);
            }
            draw_pointer++;
          }
        }

        break;
      }
    case BLOB_NEXT:
      sprites.drawSelfMasked(draw_x, draw_y, blobsBig, randomBlobPit[0]);
      sprites.drawSelfMasked(draw_x, draw_y + (2 * BLOB_PIXELS), blobsBig, randomBlobPit[1]);
      break;
    case BLOB_WAITING:
      sprites.drawSelfMasked(draw_x, draw_y, blobsSmall, randomBlobPit[2]);
      sprites.drawSelfMasked(draw_x, draw_y + BLOB_PIXELS, blobsSmall, randomBlobPit[3]);
      break;

  }
}

void createCurrentBlobs ()
{
  blobsXY[0] = 2;     //player X
  blobsXY[1] = 0;     //player Y
  for (byte i = 0; i < 9; i++)
  {
    currentBlobs[i] = 0;
  }
  currentBlobs[1] = randomBlobPit[0];
  currentBlobs[4] = randomBlobPit[1];

  for (byte i = 0; i < 4; i++)
  {
    randomBlobPit[i] = randomBlobPit[i + 2];
  }

  randomBlobPit[4] = random(1, 6);
  randomBlobPit[5] = random(1, 6);
}

boolean isRowThreeFree()
{
  for (byte array_x = 0; array_x < 8; array_x++)
  {
    if (field [array_x][3] == BLOB_FREE) return true;
    else return false;
  }

}

void checkIfBlobsAreGettingToHigh()
{
  for (byte array_x = 0; array_x < 8; array_x++)
  {
    if (field [array_x][4] == BLOB_FREE)
    {
      if (showCombo) elfState = ELF_THUMBSUP;
      else if (showNod) elfState = ELF_NOD;
      else elfState = ELF_NORMAL;
    }
    else
    {
      elfState = ELF_STRESSED;
      break;
    }
  }
}


void drawNextBlobs()
{
  sprites.drawPlusMask(54, 2, nextBlobs_plus_mask, 0);
}

void drawNextAndWaitingBlobs()
{
  drawBlobs((PLAYFIELD_WIDTH * BLOB_PIXELS) + 8, 4, BLOB_NEXT);
  drawBlobs((PLAYFIELD_WIDTH * BLOB_PIXELS) + 22, 10, BLOB_WAITING);
}

void drawCurrentBlobs()
{
  drawBlobs((blobsXY[0]*BLOB_PIXELS) + PLAYFIELD_ZERO_X, (blobsXY[1]*BLOB_PIXELS) + PLAYFIELD_ZERO_Y, BLOB_CURRENT);
}

void scoreDraw(int scoreX, int scoreY)
{
  sprites.drawErase(scoreX - 3, scoreY - 2, scoreBackground, 0);
  char buf[10];
  ltoa(scorePlayer, buf, 10);
  char charLen = strlen(buf);
  char pad = 10 - charLen;

  //draw 0 padding
  for (byte i = 0; i < pad; i++)
  {
    sprites.drawSelfMasked(scoreX + (4 * i), scoreY, numbersSmall, 0);
  }

  for (byte i = 0; i < charLen; i++)
  {
    char digit = buf[i];
    byte j;
    if (digit <= 48)
    {
      digit = 0;
    }
    else {
      digit -= 48;
      if (digit > 9) digit = 0;
    }

    for (byte z = 0; z < 10; z++)
    {
      if (digit == z) j = z;
    }
    sprites.drawSelfMasked(scoreX + (pad * 4) + (4 * i), scoreY, numbersSmall, digit);
  }
}

void drawNormalElf()
{
  sprites.drawSelfMasked(51, 0, elfNormal, 0);
  sprites.drawSelfMasked(79, 24, elfNormalEyes, pgm_read_byte(&elfNormalEyesSequence[elfPausedFrame]));
}

void drawStressedElf()
{
  sprites.drawPlusMask(58, pgm_read_byte(&elfStressedBodySequenceY[elfStressedFrame]), elfStressBody_plus_mask, 0);
  sprites.drawPlusMask(61, - pgm_read_byte(&elfStressedHeadSequenceY[elfStressedFrame]), elfStressHead_plus_mask, 0);
  sprites.drawPlusMask(pgm_read_byte(&elfStressedWandSequenceX[elfStressedFrame]), pgm_read_byte(&elfStressedWandSequenceY[elfStressedFrame]), elfStressWand_plus_mask, 0);
}

void drawPausedElf()
{
  sprites.drawPlusMask(51, pgm_read_byte(&elfPausedBodySequenceY[elfPausedFrame]), elfPauseBody_plus_mask, 0);
  sprites.drawPlusMask(pgm_read_byte(&elfPausedHeadSequenceX[elfPausedFrame]), - pgm_read_byte(&elfPausedHeadSequenceY[elfPausedFrame]), elfPauseHead_plus_mask, 0);
  sprites.drawPlusMask(pgm_read_byte(&elfPausedWandSequenceX[elfPausedFrame]), pgm_read_byte(&elfPausedWandSequenceY[elfPausedFrame]), elfPauseWand_plus_mask, 0);
  sprites.drawErase(pgm_read_byte(&elfPausedHeadSequenceX[elfPausedFrame]) + 31, - pgm_read_byte(&elfPausedHeadSequenceY[elfPausedFrame]) + 43, elfPauseMouth, pgm_read_byte(&elfPausedMouthSequence[elfPausedFrame]));
}

void drawThumbsUpElf()
{
  sprites.drawSelfMasked(51, 0, elfThumbsUp, pgm_read_byte(&thumbsUpSequence[thumbsUpFrame]));
}

void drawNodElf()
{
  sprites.drawSelfMasked(51, 0, elfNod, pgm_read_byte(&nodSequence[nodFrame]));
}

void drawDitherBackground()
{
  for (byte y = 0; y < 8; y++)
  {
    for (byte x = 0; x < 77; x += 2)
    {
      sprites.drawSelfMasked(51 + x, 8 * y, elfBackground, 0);
    }
  }
}

void drawSpeedUp()
{
  sprites.drawPlusMask(2, pgm_read_byte(&speedSequenceY[speedUpFrame]), speed_plus_mask, 0);
  sprites.drawPlusMask(16, pgm_read_byte(&upSequenceY[speedUpFrame]), up_plus_mask, 0);
}

void drawPops()
{
  for (int x = 0; x < PLAYFIELD_WIDTH; x++)
  {
    for (int y = 0; y < PLAYFIELD_HEIGHT; y++)
    {
      if (fieldFlags[x][y] == FLAG_ON_FIELD)
      {
        sprites.drawPlusMask((x * BLOB_PIXELS) + PLAYFIELD_ZERO_X, (y * BLOB_PIXELS) + PLAYFIELD_ZERO_Y, blobPop_plus_mask, pgm_read_byte(&popSequence[popFrame]));
      }
    }
  }
}

void drawChain()
{
  sprites.drawPlusMask(2, pgm_read_byte(&chainSequenceY[chainFrame]), chain_plus_mask, 0);
}

void updatePops()
{
  if (arduboy.everyXFrames(6)) popFrame++;
  if (popFrame > 3)
  {
    popFrame = 0;
    showPop = false;
    while (canMoveBlobsDown)
    {
      removeGroups();
      moveBlobsDown();
      fourInPack();
      fourInColumn();
      fourInRow();
    }
    canMoveBlobsDown = true;
  }
}

void updateSpeedUp()
{
  if (arduboy.everyXFrames(6)) speedUpFrame++;
  if (speedUpFrame > 7)
  {
    speedUpFrame = 0;
    showSpeedUp = false;
  }
}

void updateChain()
{
  if (arduboy.everyXFrames(6)) chainFrame++;
  if (chainFrame > 7)
  {
    chainFrame = 0;
    showChain = false;
  }
}

void updateStage()
{
  if (gameState != STATE_GAME_PAUSE) checkIfBlobsAreGettingToHigh();
  if (showNod) elfState = ELF_NOD;
  if (showCombo) elfState = ELF_THUMBSUP;
  if (arduboy.everyXFrames(20)) blobFrame = (++blobFrame) % 4;
  if (arduboy.everyXFrames(6)) elfPausedFrame = (++elfPausedFrame) % 21;
  switch (elfState)
  {
    case ELF_NORMAL:
      drawNormalElf();
      break;
    case ELF_THUMBSUP:
      drawThumbsUpElf();
      if (arduboy.everyXFrames(10)) thumbsUpFrame++;
      if (thumbsUpFrame > 9)
      {
        thumbsUpFrame = 0;
        showCombo = false;
        showNod = false;
      }
      break;
    case ELF_STRESSED:
      if (arduboy.everyXFrames(6))elfStressedFrame = (++elfStressedFrame) % 3;
      drawDitherBackground();
      drawStressedElf();
      break;
    case ELF_PAUSED:
      drawDitherBackground();
      drawPausedElf();
      break;
    case ELF_NOD:
      drawNodElf();
      if (arduboy.everyXFrames(6)) nodFrame++;
      if (nodFrame > 5)
      {
        nodFrame = 0;
        showNod = false;
        showCombo = false;
      }
  }
  arduboy.drawRect(0, 0, 51, 64, WHITE);
  drawNextBlobs();


  switch (elfState)
  {
    case ELF_PAUSED:
      arduboy.fillRect(56, 4, 10, 22, WHITE);
      arduboy.fillRect(70, 10, 5, 11, WHITE);
      sprites.drawErase(56, 13, pause, 0);
      break;
    case ELF_NOD:
    case ELF_NORMAL:
    case ELF_STRESSED:
    case ELF_THUMBSUP:
      drawField();
      if (showPop)
      {
        drawPops();
        updatePops();
      }
      drawCurrentBlobs();
      drawNextAndWaitingBlobs();
      break;
  }

  arduboy.fillRect(53, 52, 47, 9, WHITE);
  scoreDraw(57, 54);

  if (showSpeedUp)
  {
    drawSpeedUp();
    updateSpeedUp();
  }
  if (showChain)
  {
    drawChain();
    updateChain();
  }
}

void testSpeed()
{
  if (scorePlayer < 5000) currentSpeed = SPEED_STATE_START;
  else if (scorePlayer < 10000) currentSpeed = SPEED_STATE_ONE;
  else if (scorePlayer < 15000) currentSpeed = SPEED_STATE_TWO;
  else if (scorePlayer < 20000) currentSpeed = SPEED_STATE_THREE;
  else if (scorePlayer < 25000) currentSpeed = SPEED_STATE_FOUR;
  else if (scorePlayer < 50000) currentSpeed = SPEED_STATE_FIVE;
  else if (scorePlayer < 100000) currentSpeed = SPEED_STATE_SIX;
  else currentSpeed = SPEED_STATE_SEVEN;

  if (previousSpeed != currentSpeed)
  {
    showSpeedUp = true;
    previousSpeed = currentSpeed;
  }
}


void deletePossibleBlobs()
{
  while (canMoveBlobsDown)
  {
    updateStage();
    fourInPack();
    fourInColumn();
    fourInRow();
    removeGroups();
    moveBlobsDown();
  }
  canMoveBlobsDown = true;
}

void dropBlobs()
{
  if (isOneBlobDropPossible(blobsXY[0], blobsXY[1] + 1)) storeOneBlob(blobsXY[0], blobsXY[1]);
  //move down is no longer possible because the field is full, the game is over
  if ((blobsXY[1] == 0) && !isTileFree(blobsXY[0] + 1, 0)) gameState = STATE_GAME_OVER;
  if (isMovePossible(blobsXY[0], blobsXY[1] + 1))
  {
    blobsXY[1]++;
    sound.tone(104, 10);
    extraScoreForChain = 0;
    chain = 0;
  }
  else if (gameState != STATE_GAME_OVER)
  {
    storeBlob(blobsXY[0], blobsXY[1]);
    scorePlayer += 10;
    deletePossibleBlobs();
    createCurrentBlobs();
  }
}

#endif
