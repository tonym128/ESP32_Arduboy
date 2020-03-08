#pragma once
#include "../Utils/Constants.h"

struct Fuel {

  private:
  
    uint8_t _xTile;
    uint8_t _yTile;
    uint8_t _fuelLeft;  
    uint8_t _fuelLeft_Init;  

  public:

    uint8_t getXTile()                        { return _xTile; }
    uint8_t getYTile()                        { return _yTile; }
    uint8_t getFuelLeft()                     { return _fuelLeft; }
    
    void setXTile(uint8_t val)                { _xTile = val; }
    void setYTile(uint8_t val)                { _yTile = val; }
    void setFuelLeft(uint8_t val)             { _fuelLeft = val; _fuelLeft_Init = val; }

    void decFuel() {

      _fuelLeft = (_fuelLeft == 0 ? 0 : _fuelLeft - 1);

    }

    uint8_t getFuelLeftPerCent() {

      uint8_t fuelLeft = (_fuelLeft * 4) / _fuelLeft_Init;
      return (fuelLeft >= 4 ? 3 : fuelLeft);

    }

};