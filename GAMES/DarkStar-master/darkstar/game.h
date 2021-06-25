#include <Arduboy2.h>
#include <Sprites.h>
#include "globals.h"
#include "print.h"

void moveTo(int8_t rowOffset, int8_t colOffset)
{
  uint8_t row = player -> row + rowOffset;
  uint8_t col = player -> col + colOffset;
  if (row >= levelHeight || (col >= levelWidth)) return; // stop at level boundaries

  uint8_t pos = row * levelWidth + col;
  if ((levelMap[pos] == ttWall) || // stop at walls
     ((levelMap[pos] == ttCollectible) && (player == &blocker)) || // blocker stops at collectibles too
     (pos == blocker.pos) || (pos == ball.pos)) return; // stop when encountering blocker/ball
  //move to new position
  player -> pos = pos;
  player -> row = row;
  player -> col = col;
  player -> h = colOffset;
  player -> v = rowOffset;
  player -> count = tilesWidth - 1;
  ++levelMoves;
  levelState = lsMoving;
}

void switchControl()
{
  control.x = player -> x << 8; // use 8.8 fixed point
  control.y = player -> y << 8;
  int8_t h = blocker.x - ball.x;   // get distances
  int8_t v = blocker.y - ball.y;
  int8_t a = abs(h);
  int8_t b = abs(v);
  if (a > b) // horizontal distance is further
  {
    control.h = 1 << 8;
    if (h < 0) control.h = -1 << 8;
    control.v = (v << 8) / a;
    control.count = a;
  }
  else // vertical distance is further
  {
    control.v = 1 << 8;
    if (v < 0) control.v = -1 << 8;
    control.h = (h << 8) / b;
    control.count = b;
  }
  if (player == &ball)
    player = &blocker;         // switch to blocker
  else
  {
    control.h = -control.h; // flip directions when moving towards ball
    control.v = -control.v;
    player = &ball;         //  switch to ball
  }
  control.frame = 0;
  levelState = lsSwitchControl;
}

void loadLevel()
{
  levelState = lsIdle;
  levelRetry = false;
  levelMoves = 0;
  levelCollectibles = 0;
  uint8_t *s = (uint8_t *)(levelData + levelNumber * (2 + levelWidth / 4 * levelHeight)); //point to end of level data
  uint8_t *t = levelMap + levelWidth * levelHeight;
  for (int i = 0; i < levelWidth * levelHeight; i++)
  {
    uint8_t data;
    if ((i & 3) == 0) data = pgm_read_byte(--s);
    uint8_t tile = data & 3;
    *--t = tile;
    if (tile == ttCollectible) levelCollectibles++;
    data >>= 2;
  }
  blocker.pos = pgm_read_byte(--s);
  ball.pos = pgm_read_byte(--s);
  ball.row = ball.pos / levelWidth;
  ball.col = ball.pos % levelWidth;
  ball.x   = ball.col * (tilesWidth - 1);
  ball.y   = ball.row * (tilesHeight - 1);
  blocker.row = blocker.pos / levelWidth;
  blocker.col = blocker.pos % levelWidth;
  blocker.x   = blocker.col * (tilesWidth - 1);
  blocker.y   = blocker.row * (tilesHeight - 1);
  player = &ball;
  gameState = gsPlayLevel;
}

void playLevel()
{
  if (levelState == lsIdle)
  {
    if (gameMoves + levelMoves >= maxMoves)
      gameState = gsGameOver;
    else if (arduboy.justPressed(A_BUTTON)) gameState = gsShowMenu;
    else if (arduboy.justPressed(B_BUTTON)) switchControl();
    else if (arduboy.pressed(UP_BUTTON))    moveTo(-1, 0);
    else if (arduboy.pressed(DOWN_BUTTON))  moveTo( 1, 0);
    else if (arduboy.pressed(LEFT_BUTTON))  moveTo( 0,-1);
    else if (arduboy.pressed(RIGHT_BUTTON)) moveTo( 0, 1);
  }
  else if (levelState == lsMoving)
  {
    if (player -> count)
    {
      player -> x += player -> h;
      player -> y += player -> v;
      --player -> count;
    }
    else
    {
      levelState = lsIdle;
      if (levelMap[player-> pos] == ttCollectible)
      {
        levelMap[player-> pos] = ttSpace;
        --levelCollectibles;
        //set collected sound
      }
      if (levelCollectibles) //keep moving in same direction as long as there are collectibles
      {
        moveTo(player -> v, player -> h);
        if(levelState == lsMoving) --levelMoves;
      }
      else
      {
        //set completed sound
        levelState = lsCompleted;
      }
    }
  }
  else if (  levelState == lsSwitchControl) // move control sprite
  {
    if (control.count)
    {
      control.x += control.h;
      control.y += control.v;
      --control.count;
    }
    else if (control.h || control.v) // show control sprite stationary for
    {
       control.h = 0;
       control.v = 0;
       control.count = 15;
    }
    else
    {
      levelState = lsIdle;
    }
  }
  else if (levelState == lsCompleted)
  {
    if (!arduboy.buttonsState())
      gameState = gsLevelCompleted;
  }
  if (arduboy.everyXFrames(5)) blocker.frame = ++blocker.frame % 6;
  if (arduboy.everyXFrames(5)) control.frame = ++control.frame % 3;
}

void drawLives()
{
  for (uint8_t lives = 0; lives < levelRetries ; lives++)
  Sprites::drawPlusMask(2 + lives * (hudNumbersWidth - 1), -1, hudNumbers, 10);
}

void drawLevelNumber()
{
  uint8_t n = levelNumber;
  for (int8_t i = 1; i <= 2; i++)
  {
    Sprites::drawPlusMask(WIDTH - 2 - i * (hudNumbersWidth - 1), HEIGHT - hudNumbersHeight, hudNumbers, n % 10);
    n /= 10;
  }
}

void drawMoves()
{
  uint8_t n = levelMoves;
  for (uint8_t i = 1; i <= 3; i++)
  {
    Sprites::drawPlusMask(WIDTH - 3 - i * (hudNumbersWidth - 1), -1, hudNumbers, n % 10);
    n /= 10;
  }
}

void drawLevel()
{
  // draw levelMap
  for (uint8_t row = 0; row < levelHeight; row++)
    for (uint8_t col = 0; col < levelWidth; col++)
    {
      uint8_t c = levelMap[row * levelWidth + col];
      Sprites::drawSelfMasked(camera.x + col * (tilesWidth - 1), camera.y + row * (tilesHeight - 1), tiles, c);
    }
  // draw sprites
  Sprites::drawPlusMask(camera.x + blocker.x - 1, camera.y + blocker.y - 1, blockerSprite, blocker.frame);
  Sprites::drawPlusMask(camera.x + ball.x, camera.y + ball.y , ballSprite, ball.frame);
  if (levelState == lsSwitchControl) Sprites::drawPlusMask(camera.x + (control.x >> 8) - 2, camera.y + (control.y >> 8) - 5, joystickSprite, control.frame);
  // draw hud
  drawLives();
  drawMoves();
  drawLevelNumber();
}

void drawFrame()
{
  arduboy.sBuffer[0] = 0xFE; // draw top left corner
  arduboy.sBuffer[1] = 0x03;
  arduboy.sBuffer[WIDTH-1] = 0xFE; // draw top right corner
  arduboy.sBuffer[WIDTH-2] = 0x03;
  arduboy.sBuffer[(HEIGHT/8 - 1) * WIDTH + 0] = 0x7F; // draw bottom left corner
  arduboy.sBuffer[(HEIGHT/8 - 1) * WIDTH + 1] = 0xC0;
  arduboy.sBuffer[(HEIGHT/8) * WIDTH - 1] = 0x7F; // draw bottom right corner
  arduboy.sBuffer[(HEIGHT/8) * WIDTH - 2] = 0xC0;
  for (uint8_t i=2; i < (WIDTH-2); i++) // draw horizontal lines
  {
    arduboy.sBuffer[i] = 0x01;
    arduboy.sBuffer[(HEIGHT/8 - 1) * WIDTH + i] = 0x80;
  }
  for (uint8_t i=1; i < (HEIGHT / 8 - 1); i++) // draw vertical lines
  {
    arduboy.sBuffer[i * WIDTH] = 0xFF;
    arduboy.sBuffer[i * WIDTH + WIDTH - 1] = 0xFF;
  }
}

void resetGame()
{
  gameNextState = false;
  gameMoves = 0;
  gameState = gsTitle;
}

void title()
{
  //handle rising star
  uint8_t y = HEIGHT - darkstarSpriteHeight; // final y position
  if (gameMoves < darkstarSpriteHeight)
  {
    y = HEIGHT - gameMoves;
  }
  Sprites::drawSelfMasked((WIDTH - darkstarSpriteWidth) / 2, y , darkstarSprite, 0);
  //handle scrolling title
  uint8_t x = WIDTH - darkSpriteWidth;
  if (gameMoves < introTitleDelay)
  {
    x = WIDTH; //keep title offscreen
  }
  else if (gameMoves < (introTitleDelay + darkSpriteWidth / 2))
  {
    x = WIDTH - (gameMoves - introTitleDelay) * 2; // scroll right to left
  }
  Sprites::drawPlusMask(x, 8 , darkSprite, 0);
  Sprites::drawPlusMask(((WIDTH - starSpriteWidth)/2) - x, 32 , starSprite, 0);
  //handle flashing button
  if (gameMoves >= (introTitleDelay + (darkSpriteWidth/2)))
  {
    if (arduboy.buttonsState())
      gameNextState = true;
    else if (gameNextState)
    {
      levelNumber = 1;
      levelRetries = maxRetries;
      resetGame();
      loadLevel();
    }
  }
  if (gameMoves >= (introButtonOnDelay + (darkSpriteWidth/2)))
  {
    print_str(anyButtonText);

    if (gameMoves >= (introButtonOffDelay + (darkSpriteWidth/2)))
    {
      gameMoves = introLoopDelay + (darkSpriteWidth/2);
    }
  }
 if (!gameNextState && gameState == gsTitle) ++gameMoves;
}

void levelCompleted()
{
  drawFrame();
  print_str(levelCompletedText);
  print_num(87, 6, levelNumber, 2);
  print_num(53, 28, levelMoves, 3);
  print_num(30, 39, maxMoves - gameMoves - levelMoves, 4);
  uint8_t btn = arduboy.buttonsState();
  if (levelRetries)
  {
    print_str(retryLevelText);
    if (btn & A_BUTTON)
    {
      levelRetry = true;
      gameNextState = true;
    }
  }
  if (btn & B_BUTTON)
    gameNextState = true;
  if ((btn == 0) && gameNextState)
  {
    gameNextState = false;
    if (levelRetry)
      --levelRetries;
    else
    {
      gameMoves += levelMoves;
      ++levelNumber;
    }
    loadLevel();
  }
}

void missionCompleted()
{
  drawFrame();
  print_str(missionCompletedText);
  print_num(58, 50, gameMoves + levelMoves, 4);
  if (arduboy.buttonsState() & (A_BUTTON | B_BUTTON))
    gameNextState = true;
  else if (gameNextState)
    resetGame();
}

void gameOver()
{
  drawFrame();
  print_str(gameOverText);
  if (arduboy.buttonsState() & (A_BUTTON | B_BUTTON))
    gameNextState = true;
  else if (gameNextState)
    resetGame();
}

void showMenu()
{
  uint8_t option = gameOption;
  if (arduboy.justPressed(UP_BUTTON)) --option;
  if (arduboy.justPressed(DOWN_BUTTON)) ++option;
  drawFrame();
  if (levelRetries)
  {
    option %= 4;
    print_str(menuText);
    Sprites::drawPlusMask(pgm_read_byte(&menuPos[option][0]), pgm_read_byte(&menuPos[option][1]) , ballSprite, 0);
  }
  else
  {
    option %= 2;
    print_str(shortMenuText);
    Sprites::drawPlusMask(pgm_read_byte(&shortMenuPos[option][0]), pgm_read_byte(&shortMenuPos[option][1]) , ballSprite, 0);
  }
  if (!arduboy.buttonsState())
    gameNextState = true;
  if (gameNextState && arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameNextState = false;
    if (option == 0) //continue playing
      gameState = gsPlayLevel;
    else if (levelRetries && option != 3) //retry or next level
    {
      --levelRetries;
      if (option == 2) //next level
      {
        ++levelNumber;
        gameMoves += nextLevelPenalty;
      }
      loadLevel();
    }
    else
      resetGame();
    option = 0;
  }
  gameOption = option;
}
