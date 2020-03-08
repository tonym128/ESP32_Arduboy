#include "game.h"
#include "enemies.h"
#include "pickup.h"

int16_t gameOverAndStageFase;
boolean bonusVisible;
boolean nextLevelVisible;
boolean pressKeyVisible;
int leftX;
int16_t rightX;


// method implementations ////////////////////////////////////////////////////

// stateGamePlaying
// called each frame the gamestate is set to playing
void stateGamePlaying()
{
  // Update Level
  int16_t spawnTime;
  if (gameType != STATE_GAME_MAYHEM) spawnTime = 60;
  else spawnTime = 15;
  if (arduboy.everyXFrames(spawnTime * 3)) {
    spawnZombie();
  }

  // Check for Pause
  checkPause();

  // Update Objects
  updatePlayer(coolGirl);
  updateBullets();
  updateZombies();
  updateSurvivors();

  // Draw
  drawLevel();
  drawDoor();
  drawPickups();
  drawSurvivors();
  drawZombies();
  drawBullets();
  drawPlayer(coolGirl);

  drawNumbers(86, 0, FONT_SMALL, DATA_SCORE);
  drawLife(coolGirl);
  drawAmountSurvivors();
  drawCoolDown();
}

int readSurvivorData(uint8_t index)
{
  return pgm_read_byte(survivorLocation + ((int)level - 1) * 10 + (int)index) * TILE_WIDTH; //PLAYER_WIDTH
}

int readPlayerAndExitData(uint8_t index)
{
  // work
  int16_t tempLevel;
  if (gameType != STATE_GAME_MAYHEM)tempLevel = displayLevel;
  else tempLevel = level;
  return pgm_read_byte(playerAndExitLocation + ((int)tempLevel - 1) * 4 + (int)index) * TILE_WIDTH; //PLAYER_WIDTH
}

// stateGameNextLevel
// called each frame the gamestate is set to next level
void stateGamePrepareLevel()
{
  clearSurvivors();
  clearPickups();
  clearZombies();

  level++;

  if (gameType != STATE_GAME_MAYHEM) level = (level - 1) % NUM_MAPS + 1;
  else level = random(NUM_MAPS) + 1;
  displayLevel++;

  pickupsCounter = 0;
  gameOverAndStageFase = 0;
  globalCounter = 0;

  gameState = STATE_GAME_NEXT_LEVEL;
}


void nextLevelStart()
{
  leftX = -50;
  rightX = 154;
  if ((displayLevel == 1) || (gameType == STATE_GAME_CONTINUE))
  {
    bonusVisible = false;
    nextLevelVisible = true;
    pressKeyVisible = false;
    gameOverAndStageFase = 4;
  }
  else
    {
    bonusVisible = true;
    nextLevelVisible = false;
    pressKeyVisible = false;
    gameOverAndStageFase++;
  }
}

void nextLevelBonusCount()
{
  if (exitDoor.counter > 0)
  {
    exitDoor.counter--;
    scorePlayer += 5;
  }
  else
  {
    if ((displayLevel < 129) || (gameType == STATE_GAME_MAYHEM)) gameOverAndStageFase++;
    else
    {
      gameState = STATE_GAME_END;
      gameOverAndStageFase = 0;
    }
  }
}

void nextLevelWait()
{
  if (arduboy.everyXFrames(4)) globalCounter++;
  if (globalCounter > 8)
  {
    gameOverAndStageFase++;
    globalCounter = 0;
  }
}

void nextLevelSlideToMiddle()
{
  bonusVisible = false;
  nextLevelVisible = true;
  if (leftX < 21)
  {
    leftX += 4;
    rightX -= 4;
  }
  else gameOverAndStageFase++;
}


void nextLevelEnd()
{
  int16_t maxId;
  if (arduboy.everyXFrames(30)) pressKeyVisible = !pressKeyVisible;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_GAME_PLAYING;
    gameOverAndStageFase = 0;
    bonusVisible = false;
    nextLevelVisible = false;
    pressKeyVisible = false;
    leftX = -50;
    rightX = 154;

    coolGirl.x = readPlayerAndExitData(0);
    coolGirl.y = readPlayerAndExitData(1);
    setDoorPosition(readPlayerAndExitData(2), readPlayerAndExitData(3));
    swapSurvivorPool();

    if (gameType != STATE_GAME_MAYHEM)
    {
      EEPROM.write(OFFSET_VLQP_START, gameID);
      EEPROM.put(OFFSET_LEVEL, level - 1);
      EEPROM.put(OFFSET_SCORE, scorePlayer);
      EEPROM.put(OFFSET_HEALTH, coolGirl.health);
      EEPROM.write(OFFSET_VLQP_END, gameID);
      EEPROM.commit();
      maxId = (((displayLevel % TOTAL_LEVEL_AMOUNT) - 1) / NUM_MAPS) + 2;
    }
    else maxId = 5;

    for (int16_t id = 0; id < maxId; id++)
    {
      Element &surv = survivors[id];
      surv.x = readSurvivorData(2 * id);
      surv.y = readSurvivorData((2 * id) + 1);
      surv.active = true;
    }
  }
}

typedef void (*FunctionPointer) ();
const FunctionPointer PROGMEM nextLevelFases[] =
{
  nextLevelStart,
  nextLevelWait,
  nextLevelBonusCount,
  nextLevelWait,
  nextLevelSlideToMiddle,
  nextLevelWait,
  nextLevelEnd,
};

void stateGameNextLevel()
{
  ((FunctionPointer) pgm_read_dword (&nextLevelFases[gameOverAndStageFase]))();
  if (bonusVisible)
  {
    sprites.drawSelfMasked(32, 16, bonusPoints, 0);
    drawNumbers(78, 16, FONT_SMALL, DATA_TIMER);
    drawNumbers(36, 36, FONT_BIG, DATA_SCORE);
  }
  if (nextLevelVisible)
  {
    sprites.drawSelfMasked(leftX, 24, nextLevel, 0);
    if (pressKeyVisible)
    {
      if (gameType == STATE_GAME_MAYHEM)sprites.drawSelfMasked(41, 8, textMayhem, 0);
      else sprites.drawSelfMasked(43, 8, textNormal, 0);
      sprites.drawSelfMasked(37, 48, pressKey, 0);
    }
    drawNumbers(rightX, 24, FONT_BIG, DATA_LEVEL);
  }
}

// stateGameOver
// called each frame the gamestate is set to game over

void gameOverEnd()
{
  if (arduboy.everyXFrames(30)) pressKeyVisible = !pressKeyVisible;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
    gameOverAndStageFase = 0;
    pressKeyVisible = false;
  }
}


typedef void (*FunctionPointer) ();
const FunctionPointer PROGMEM gameOverFases[] =
{
  nextLevelWait,
  nextLevelWait,
  nextLevelWait,
  nextLevelWait,
  gameOverEnd,
};


void stateGameOver()
{
  ((FunctionPointer) pgm_read_dword (&gameOverFases[gameOverAndStageFase]))();
  if (gameType == STATE_GAME_MAYHEM)sprites.drawSelfMasked(41, 3, textMayhem, 0);
  else sprites.drawSelfMasked(43, 3, textNormal, 0);
  sprites.drawSelfMasked(11, 15, gameOver, 0);
  drawNumbers(35, 34, FONT_BIG, DATA_SCORE);
  if (pressKeyVisible) sprites.drawSelfMasked(37, 53, pressKey, 0);
}

void stateGameEnd()
{
  ((FunctionPointer) pgm_read_dword (&gameOverFases[gameOverAndStageFase]))();
  sprites.drawSelfMasked(22, 8, youWon, 0);
  drawNumbers(36, 32, FONT_BIG, DATA_SCORE);
  if (pressKeyVisible) sprites.drawSelfMasked(37, 56, pressKey, 0);
}

void stateGamePause()
{
  sprites.drawSelfMasked(30, 0, titleScreen02, 0);
  sprites.drawSelfMasked(92, 0, titleScreen03, 0);
  sprites.drawSelfMasked(37, 40, pause, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_GAME_PLAYING;
    sprites.drawSelfMasked(22, 32, gameOver, 0);
  }
}


void stateGameNew()
{
  level = LEVEL_TO_START_WITH - 1;
  displayLevel = level;
  initializePlayer(coolGirl);
  gameState = STATE_GAME_PREPARE_LEVEL;
}


void stateGameContinue()
{
  if ((EEPROM.read(OFFSET_VLQP_START) == gameID) && (EEPROM.read(OFFSET_VLQP_END) == gameID))
  {
    initializePlayer(coolGirl);
    EEPROM.get(OFFSET_LEVEL, level);
    displayLevel = level;
    EEPROM.get(OFFSET_SCORE, scorePlayer);
    EEPROM.get(OFFSET_HEALTH, coolGirl.health);

    gameState = STATE_GAME_PREPARE_LEVEL;
  }
  else gameState = STATE_GAME_NEW;
}

void stateGameMayhem()
{
  displayLevel = 0;
  gameState = STATE_GAME_PREPARE_LEVEL;
  initializePlayer(coolGirl);
}


void checkPause()
{
  if (arduboy.pressed(A_BUTTON + B_BUTTON)) gameState = STATE_GAME_PAUSE;
}
