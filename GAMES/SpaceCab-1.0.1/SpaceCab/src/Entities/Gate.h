#pragma once
#include "../Utils/Constants.h"

struct Gate {

  private:
  
    uint8_t _xTile;
    uint8_t _yTile;
    bool _active;  

  public:

    uint8_t getXTile()                        { return _xTile; }
    uint8_t getYTile()                        { return _yTile; }
    bool isActive()                           { return _active; }
    
    void setXTile(uint8_t val)                { _xTile = val; }
    void setYTile(uint8_t val)                { _yTile = val; }
    void setActive(bool val)                  { _active = val; }

};