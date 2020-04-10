#pragma once

#include "../Utils/Arduboy2Ext.h"
#include "../Utils/Constants.h"
#include "Levels.h"
#include "Fuel.h"
#include "Gate.h"

#include "FixedPoints.h"
#include "FixedPointsCommon.h"



template<typename T>
inline T * ProgmemCopy(T & object, T const * const progmem)
{
	return reinterpret_cast<T *>(memcpy_P(&object, progmem, sizeof(T)));
}

struct Level {

  public:
 
    SQ15x16 xOffset = 0;
    SQ15x16 yOffset = 0;

  private:
 
    uint8_t _number;
    uint16_t _height = 0;
    uint16_t _width = 0;
    uint16_t _heightInTiles = 0;
    uint16_t _widthInTiles = 0;
    uint8_t _levelData[256];
    uint8_t _faresRequired;
    uint8_t _levelNameOffset;
    uint8_t _numberOfPositions;
    uint8_t _levelGateLeft;
    uint8_t _levelGateRight;
    uint16_t _fuelMax = 0;
    uint16_t _internalGateInterval = 0;             // Period of time internal gate is open.
    uint16_t _internalGateCounter = 0;              // Gate open countdown.
    GateMode _internalGateMode = GateMode::Closed;

    Fuel _fuel0;
    Fuel _fuel1;
    Fuel _fuel2;

    Gate _gate0;
    Gate _gate1;
    Gate _gate2;
    Gate _gate3;
    Gate _gate4;
    Gate _gate5;
    Gate _gate6;
    Gate _gate7;
    Gate _gate8;
    Gate _gate9;

    Fuel *_fuels[FUEL_TILES_MAX] = { &_fuel0, &_fuel1, &_fuel2 };
    Gate *_gates[GATE_TILES_MAX] = { &_gate0, &_gate1, &_gate2, &_gate3, &_gate4, &_gate5, &_gate6, &_gate7, &_gate8, &_gate9 };


  public:

    Level() {
      
      for (uint8_t x = 0 ; x < GATE_TILES_MAX; x++) {
        _gates[x]->setActive(false);
      }

    };

    int16_t getXOffsetDisplay()               const { return xOffset.getInteger(); }
    int16_t getYOffsetDisplay()               const { return yOffset.getInteger(); }

    uint8_t getLevelNumber()                  const { return _number; }
    uint16_t getHeight()                      const { return _height; }
    uint16_t getWidth()                       const { return _width; }
    uint16_t getHeightInTiles()               const { return _heightInTiles; }
    uint16_t getWidthInTiles()                const { return _widthInTiles; }
    uint16_t getFaresRequired()               const { return _faresRequired; }
    uint8_t getLevelNameOffset()              const { return _levelNameOffset; }
    uint8_t getNumberOfCustomerPositions()    const { return _numberOfPositions; }
    uint16_t getFuelMax()                     const { return _fuelMax; }

    Fuel * getFuel(uint8_t idx)               const { return _fuels[idx]; }

    void setLevelNumber(uint8_t val)          { _number = val; }
    void setHeight(uint16_t val)              { _height = val; }
    void setWidth(uint16_t val)               { _width = val; }
    void setHeightInTiles(uint16_t val)       { _heightInTiles = val; _height = val * TILE_SIZE; }
    void setWidthInTiles(uint16_t val)        { _widthInTiles = val; _width = val * TILE_SIZE; }

    void setFuel(uint8_t idx, Fuel * fuel)    { _fuels[idx] = fuel; }

    Fuel * getFuel(uint8_t xTile, uint8_t yTile) {

      for (uint8_t i = 0; i < FUEL_TILES_MAX; i++) {

        Fuel *fuel = _fuels[i];

        if (fuel->getXTile() == xTile && fuel->getYTile() == yTile) {
          return fuel;
        }

      }

      return nullptr;

    }

    uint8_t getLevelData(uint8_t x, uint8_t y) {

      uint8_t tile = getTile(x, y);


      // If we have gate tiles and they are not the level (y != 0) check to  
      // see if the tiles need to be rendered vertically ot horizontally ..

      if (tile == GATE1 && y > 0 && (getTile(x, y - 1) == GATE1 || getTile(x, y + 1) == GATE1)) {
        return GATE2;
      }

      if (tile == LEVE1) {
        return (_internalGateMode == GateMode::Closed ? LEVE1 : LEVE2);
      }

      return tile;

    }

    void openLevelGates() {

      for (uint8_t x = _levelGateLeft; x <= _levelGateRight; x++) {

        if (x % 2 == 0) {
          _levelData[x / 2] = (_levelData[x / 2] & 0x0F);
        }
        else {
          _levelData[x / 2] = (_levelData[x / 2] & 0xF0);
        }

      }

    }

    void decInternalGateCounter() {

      if (_internalGateCounter > 0) {

        _internalGateCounter--;

        if (_internalGateCounter == 0) {

          changeInternalGate(GateMode::Closed);

        }

      }

    }

    void changeInternalGate(GateMode mode) {

      for (uint8_t i = 0; i < GATE_TILES_MAX; i++) {

        Gate *gate = _gates[i];

        if (gate->isActive()) {

          if (gate->getXTile() % 2 == 0) {
            _levelData[((gate->getYTile() * _widthInTiles) + gate->getXTile()) / 2] = (_levelData[((gate->getYTile() * _widthInTiles) + gate->getXTile()) / 2] & 0x0F) | (mode == GateMode::Closed ? GATE1 << 4 : EMPTY);
          }
          else {
            _levelData[((gate->getYTile() * _widthInTiles) + gate->getXTile()) / 2] = (_levelData[((gate->getYTile() * _widthInTiles) + gate->getXTile()) / 2] & 0xF0) | (mode == GateMode::Closed ? GATE1 : EMPTY);
          }

        }

      }

      _internalGateMode = mode;
      _internalGateCounter = (mode == GateMode::Open ? _internalGateInterval : 0);

    }

    void reset(uint8_t levelNumber) { 
     
      _number = levelNumber; 
      _internalGateMode = GateMode::Closed;

      LevelDefinition levelDefinition;
      ProgmemCopy(levelDefinition, &levelInit[levelNumber]);

      _widthInTiles = levelDefinition.levelWidth;
      _heightInTiles = levelDefinition.levelHeight;
      _width = levelDefinition.levelWidth * TILE_SIZE;
      _height = levelDefinition.levelHeight * TILE_SIZE;

      xOffset = static_cast<SQ15x16>(levelDefinition.levelXOffset);
      yOffset = static_cast<SQ15x16>(levelDefinition.levelYOffset);
      _faresRequired = levelDefinition.faresRequired;
      _levelNameOffset = levelDefinition.levelNameOffset;
      _numberOfPositions = levelDefinition.numberOfPositions;
      _levelGateLeft = levelDefinition.levelGateLeft;
      _levelGateRight = levelDefinition.levelGateRight;
      _fuelMax = levelDefinition.playerInitialFuel;
      _internalGateInterval = levelDefinition.internalGateInterval;


      // Clear fuel locations ..

      for (uint8_t y = 0; y < FUEL_TILES_MAX; y++) {

        Fuel *fuel = _fuels[y];
        fuel->setXTile(0);
        fuel->setYTile(0);
        fuel->setFuelLeft(0);

      }

      // Read map data ..
      {
  
        const uint8_t *levelMap = levelMaps[_number];
        uint8_t fuelIdx = 0;
        uint8_t gateIdx = 0;
        uint16_t dataOffset = 0;
        uint16_t cursor = 0;

        while (true) {

          uint8_t data = pgm_read_byte(&levelMap[dataOffset]);
          uint8_t tile = (data & 0xF0) >> 4;
          uint8_t run = data & 0x0F;

          if (run > 0) {

            dataOffset++;

            for (uint8_t x = 0; x < run; x++) {
              
              if (cursor % 2 == 0) {
                _levelData[cursor / 2] = (_levelData[cursor / 2] & 0x0f) | (tile << 4);
              }
              else {
                _levelData[cursor / 2] = (_levelData[cursor / 2] & 0xF0) | tile;
              }

              switch (tile) {

                case FUEL1:
                  updateFuelDetails(cursor % _widthInTiles, cursor / _widthInTiles, fuelIdx);
                  fuelIdx++;
                  break;

                case GATE1:
                  if (cursor / 2 > _widthInTiles) {
                    updateGateDetails(cursor % _widthInTiles, cursor / _widthInTiles, gateIdx);
                    gateIdx++;
                  }
                  break;

              }

              cursor++;

            }

          }
          else {
          
            break;
          
          }

        }

      }

    }

    void updateFuelDetails(uint8_t x, uint8_t y, uint8_t index) {

      Fuel *fuel = _fuels[index];
      fuel->setXTile(x);
      fuel->setYTile(y);
      fuel->setFuelLeft(random(FUEL_MIN, FUEL_MAX));
 
    }

    void updateGateDetails(uint8_t x, uint8_t y, uint8_t index) {

      Gate *gate = _gates[index];
      gate->setXTile(x);
      gate->setYTile(y);
      gate->setActive(true);
 
    }

  private:

    uint8_t getTile(uint8_t x, uint8_t y) {

      uint8_t tile = EMPTY;

      if (y > _heightInTiles) return BRICK;

      if ((x % 2) == 0) {
        tile = (_levelData[((y * _widthInTiles) + x) / 2] >> 4);
      }
      else {
        tile = (_levelData[((y * _widthInTiles) + x) / 2] & 0x0f);
      }

      return tile;

    }

};