#pragma once
#include "../Utils/Constants.h"

struct Customer {

  private:
  
    uint8_t _frame;
    uint8_t _fare;
    uint8_t _startingPosition;   // Index in starting location positions;  Used to ensure new customers are not launched in the same location as an old customer was dropped at.
    uint8_t _xTile;
    uint8_t _yTile;
    uint8_t _destinationPosition;  
    uint8_t _xDestinationTile;
    uint8_t _yDestinationTile;
    CustomerStatus _status;
    Direction _walkingDirection;
    int8_t _xWalkingOffset;

  public:

    uint8_t getFrame()                          { return _frame; }
    uint8_t getFare()                           { return _fare; }
    uint16_t getX()                             { return _xTile * TILE_SIZE; }
    uint16_t getY()                             { return _yTile * TILE_SIZE; }
    uint8_t getXTile()                          { return _xTile; }
    uint8_t getYTile()                          { return _yTile; }
    uint8_t getXDestinationTile()               { return _xDestinationTile; }
    uint8_t getYDestinationTile()               { return _yDestinationTile; }
    uint8_t getStartingPosition()               { return _startingPosition; }
    uint8_t getDestinationPosition()            { return _destinationPosition; }
    CustomerStatus getStatus()                  { return _status; }
    Direction getWalkingDirection()             { return _walkingDirection; }
    int8_t getXWalkingOffset()                  { return _xWalkingOffset; }
    
    void setFrame(uint8_t val)                  { _frame = val; }
    void setFare(uint8_t val)                   { _fare = val; }
    void setXTile(uint8_t val)                  { _xTile = val; }
    void setYTile(uint8_t val)                  { _yTile = val; }
    void setXDestinationTile(uint8_t val)       { _xDestinationTile = val; }
    void setYDestinationTile(uint8_t val)       { _yDestinationTile = val; }
    void setStartingPosition(uint8_t val)       { _startingPosition = val; }
    void setDestinationPosition(uint8_t val)    { _destinationPosition = val; }
    void setStatus(CustomerStatus val)          { _status = val; }
    void setWalkingDirection(Direction val)     { _walkingDirection = val; }
    void setXWalkingOffset(int8_t val)          { _xWalkingOffset = val; }

    void incFrame() {
      _frame = (_frame == 0 ? 1 : 0);
    }

    void incXWalkingOffset() {
      _xWalkingOffset++;
    }

    void decXWalkingOffset() {
      _xWalkingOffset--;
    }

    void decFare() {
      if (_fare > FARE_MIN ) --_fare;
    }

};