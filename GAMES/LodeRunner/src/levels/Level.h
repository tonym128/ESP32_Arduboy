#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../utils/Utils.h"
#include "../utils/Enums.h"
#include "../characters/Player.h"
#include "../characters/Enemy.h"

#define ENCRYPTION_TYPE_RLE_ROW 0
#define ENCRYPTION_TYPE_RLE_COL 1
#define ENCRYPTION_TYPE_GRID 2

class Level {

  public: 

    Level() {};
        
    LevelElement getLevelData(const uint8_t x, const uint8_t y);
    void loadLevel(Player *player, Enemy enemies[]);
    void pickupGold();

    uint8_t getHeight();
    uint8_t getWidth();
    int16_t getXOffset();
    int16_t getYOffset();
    int8_t getXOffsetDelta();
    int8_t getYOffsetDelta();
    uint8_t getLevelNumber();
    uint8_t getGoldLeft();
    uint8_t getLevelLadderElementCount();
    LevelPoint getLevelLadderElement(const uint8_t index);
    LevelPoint getNextReentryPoint();
    
    void setLevelData(const uint8_t x, const uint8_t y, const LevelElement levelElement);
    void setXOffset(int16_t val);
    void setYOffset(int16_t val);
    void setXOffsetDelta(int8_t val);
    void setYOffsetDelta(int8_t val);
    void setLevelNumber(uint8_t val);
    void setGoldLeft(uint8_t val);

  private:

    static const uint8_t _width = 14;
    static const uint8_t _height = 16;

    int16_t _xOffset;
    int16_t _yOffset;
    int8_t _xOffsetDelta;
    int8_t _yOffsetDelta;
    uint8_t _levelData[_width][_height];
    uint8_t _levelNumber;
    uint8_t _goldLeft;

    uint8_t _levelLadderElementCount;
    uint8_t _reentryPointIndex;

    LevelPoint _levelLadder[18];
    LevelPoint _reentryPoint[4];

};

//--------------------------------------------------------------------------------------------------------------------------

uint8_t Level::getWidth() {
    return _width;
}

uint8_t Level::getHeight() {
    return _height;
}

int16_t Level::getXOffset() {
    return _xOffset;
}

int16_t Level::getYOffset() {
    return _yOffset;
}

int8_t Level::getXOffsetDelta() {
    return _xOffsetDelta;
}

int8_t Level::getYOffsetDelta() {
    return _yOffsetDelta;
}

uint8_t Level::getLevelNumber() {
    return _levelNumber;
}

uint8_t Level::getGoldLeft() {
    return _goldLeft;
}

uint8_t Level::getLevelLadderElementCount() {
    return _levelLadderElementCount;
}

void Level::setXOffset(int16_t val) {
    _xOffset = val;
}

void Level::setYOffset(int16_t val) {
    _yOffset = val;
}

void Level::setXOffsetDelta(int8_t val) {
    _xOffsetDelta = val;
}

void Level::setYOffsetDelta(int8_t val) {
    _yOffsetDelta = val;
}

void Level::setLevelNumber(uint8_t val) {
    _levelNumber = val;
}

void Level::setGoldLeft(uint8_t val) {
    _goldLeft = val;
}

LevelPoint Level::getLevelLadderElement(const uint8_t index) {
  return _levelLadder[index];
}

LevelPoint Level::getNextReentryPoint() {
  return _reentryPoint[_reentryPointIndex];
  _reentryPointIndex = (_reentryPointIndex == 3 ? 0 : _reentryPointIndex + 1);
}


// -----------------------------------------------------------------------------------------------
//  Load level data ..
//
void Level::loadLevel(Player *player, Enemy enemies[]) {

  uint16_t dataOffset = 0;
  uint8_t goldLeft = 0;

  const uint8_t *levelToLoad = levels[_levelNumber - LEVEL_OFFSET];
  player->setStance(PlayerStance::Running_Right1);


  // Load player starting position ..

  uint16_t  playerX = pgm_read_byte(&levelToLoad[dataOffset++]) * GRID_SIZE;
  uint16_t  playerY = pgm_read_byte(&levelToLoad[dataOffset++]) * GRID_SIZE;


  // Determine player's X Pos and level offset ..

  if (playerX < (WIDTH / 2) - 5) {

    _xOffset = 0;
    player->setX(playerX);

  }
  else {

    if (playerX >= (WIDTH / 2) - 5 && playerX <= (_width * GRID_SIZE * 2) - WIDTH) {

      player->setX((WIDTH / 2) - 5);
      _xOffset = player->getX() - playerX;


    }
    else {

      _xOffset = -153;
      player->setX(playerX + _xOffset);

    }

  }


  // Determine player's Y Pos and level offset ..

  if (playerY < (HEIGHT_LESS_TOOLBAR / 2) - 5) {

    _yOffset = 0;
    player->setY(playerY);

  }
  else {

    if (playerY >= (HEIGHT_LESS_TOOLBAR / 2) - 5 && playerY <= (_height * GRID_SIZE) - HEIGHT_LESS_TOOLBAR) {

      player->setY((HEIGHT_LESS_TOOLBAR / 2) - 5);
      _yOffset = player->getY() - playerY;

    }
    else {

      _yOffset = -(_height * GRID_SIZE) + HEIGHT_LESS_TOOLBAR;
      player->setY(playerY + _yOffset);

    }

  }


  // Load enemies ..

  uint8_t numberOfEnemies = pgm_read_byte(&levelToLoad[dataOffset++]);

  for (uint8_t x = 0; x < NUMBER_OF_ENEMIES; x++) {

    Enemy *enemy = &enemies[x];

    enemy->setId(x);
    enemy->setGoldCountdown(0);

    if (x < numberOfEnemies) {

      enemy->setX(pgm_read_byte(&levelToLoad[dataOffset++]) * GRID_SIZE);
      enemy->setY(pgm_read_byte(&levelToLoad[dataOffset++]) * GRID_SIZE);
      enemy->setEnabled(true);

    }
    else {

      enemy->setEnabled(false);

    }

  }


  // Load level ladder points ..

  _levelLadderElementCount = pgm_read_byte(&levelToLoad[dataOffset++]);

  for (uint8_t x = 0; x < _levelLadderElementCount; x++) {

    _levelLadder[x].x = pgm_read_byte(&levelToLoad[dataOffset++]);
    _levelLadder[x].y = pgm_read_byte(&levelToLoad[dataOffset++]);

  }


  // Load reentry points ..

  for (uint8_t x = 0; x < NUMBER_OF_REENTRY_POINTS; x++) {

    _reentryPoint[x].x = pgm_read_byte(&levelToLoad[dataOffset++]);
    _reentryPoint[x].y = pgm_read_byte(&levelToLoad[dataOffset++]);

  }


  // Load level data .. 

  uint8_t encryptionType = pgm_read_byte(&levelToLoad[dataOffset++]);
  
  if (encryptionType == ENCRYPTION_TYPE_GRID) {

    for (uint8_t y = 0; y < _height; y++) {

      for (uint8_t x = 0; x < _width; x++) {

        uint8_t data = pgm_read_byte(&levelToLoad[(y * _width) + x + dataOffset]);

        if (leftValue(data) == static_cast<uint8_t>(LevelElement::Gold))            { goldLeft++;}
        if (rightValue(data) == static_cast<uint8_t>(LevelElement::Gold))           { goldLeft++;}

        _levelData[x][y] = data;

      }

    }

  }
  else {
	  
    uint16_t cursor = 0;

    while (true) {

      uint8_t data = pgm_read_byte(&levelToLoad[dataOffset]);
      uint8_t block = (data & 0xE0) >> 5;
      uint8_t run = data & 0x1F;

      if (block == static_cast<uint8_t>(LevelElement::Gold))            { goldLeft = goldLeft + run;}

      if (run > 0) {

        dataOffset++;

        for (uint8_t x = 0; x < run; x++) {

          if (encryptionType == ENCRYPTION_TYPE_RLE_ROW) {
            
            uint8_t row = cursor / (_width * 2);
            uint8_t col = (cursor % (_width * 2)) / 2;

            if (cursor % 2 == 0) {
              _levelData[col][row] = (_levelData[col][row] & 0x0f) | (block << 4);
            } 
            else {
              _levelData[col][row] = (_levelData[col][row] & 0xF0) | block;
            }

          }
          else {

            uint8_t col = cursor / _height;
            uint8_t row = cursor % _height;

            if (col % 2 == 0) {
              _levelData[col / 2][row] = (_levelData[col / 2][row] & 0x0f) | (block << 4);
            } 
            else {
              _levelData[col / 2][row] = (_levelData[col / 2][row] & 0xF0) | block;
            }

          }

          cursor++;

        }

      }
      else {
      
        break;
      
      }

    }

  }

  _goldLeft = goldLeft;

}


// -----------------------------------------------------------------------------------------------
//  Get level element at position x and y ..
//
LevelElement Level::getLevelData(const uint8_t x, const uint8_t y) {

  if ((x / 2) >= _width) return LevelElement::Brick;
  if (y == 255) return LevelElement::Blank;
  if (y >= _height) return LevelElement::Solid;

  if (x % 2 == 0) {

    return static_cast<LevelElement>(_levelData[x / 2][y] >> 4); 

  }
  else {

    return static_cast<LevelElement>(_levelData[x / 2][y] & 0x0F); 

  }

  return LevelElement::Brick;
      
}


// -----------------------------------------------------------------------------------------------
//  Set level element at position x and y ..
//
void Level::setLevelData(const uint8_t x, const uint8_t y, const LevelElement levelElement) {

  if (x % 2 == 0) {

    _levelData[x / 2][y] = (_levelData[x / 2][y] & 0x0f) | (static_cast<uint8_t>(levelElement) << 4); 

  }
  else {

    _levelData[x / 2][y] = (_levelData[x / 2][y] & 0xf0) | static_cast<uint8_t>(levelElement); 
    
  }
      
}


// -----------------------------------------------------------------------------------------------
//  Update the level when the last gold is collected ..
//
void Level::pickupGold() {

  if (_goldLeft > 0) _goldLeft--;

  if (_goldLeft == 0) {

    // Update map with level ladder ..

    for (uint8_t x = 0; x < _levelLadderElementCount; x++) {

      LevelPoint lp = _levelLadder[x];

      Level::setLevelData(lp.x, lp.y, LevelElement::Ladder);

    }

  }

}