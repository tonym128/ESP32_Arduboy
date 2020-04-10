#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../utils/Utils.h"
#include "../utils/Enums.h"

class Enemy {

  public: 

    Enemy() {};
  
    uint8_t getId() const;
    uint16_t getX() const;
    uint16_t getY() const;
    int8_t getXDelta() const;
    int8_t getYDelta() const;
    int8_t getXFuturePosition() const;
    int8_t getYFuturePosition() const;
    bool getEnabled() const;
    uint8_t getGoldCountdown() const;
    uint8_t getDirectionCountdown() const;
    PlayerStance getStance() const;
    EscapeHole getEscapeHole() const;
    Direction getDirection() const;
    Direction getPreferredDirection() const;
    
    void setId(uint8_t val);
    void setX(uint16_t val);
    void setY(uint16_t val);
    void setXDelta(int8_t val);
    void setYDelta(int8_t val);
    void setXFuturePosition(int8_t val);
    void setYFuturePosition(int8_t val);
    void setEnabled(bool val);
    void setGoldCountdown(uint8_t val);
    void setDirectionCountdown(uint8_t val);
    void setStance(const PlayerStance &val);
    void setEscapeHole(const EscapeHole &val);
    void setDirection(const Direction &val);
    void setPreferredDirection(const Direction &val);
    void decrementDirectionCountdown(void);
    void decrementGoldCountdown(void);

  private:

  uint8_t _flags;           // bits 0 - 3 enemy id, bits 4 enabled
  uint8_t _futurePosition;  // bits 0 - 3 X, bits 4 - 7 Y
  uint16_t _x;
  uint16_t _y;
  int8_t _xDelta;
  int8_t _yDelta;
  uint8_t _goldCountdown;
  uint8_t _directionCountdown;

  PlayerStance _stance;
  EscapeHole _escapeHole;
  Direction _direction;
  Direction _preferredDirection;

};

//--------------------------------------------------------------------------------------------------------------------------

uint8_t Enemy::getId() const {
  return (_flags & 0x0f);
}

uint16_t Enemy::getX() const {
  return _x;
}

uint16_t Enemy::getY() const {
  return _y;
}

int8_t Enemy::getXDelta() const {
  return _xDelta;
}

int8_t Enemy::getYDelta() const {
  return _yDelta;
}

int8_t Enemy::getXFuturePosition() const {
  return (_futurePosition & 0x0f);
}

int8_t Enemy::getYFuturePosition() const {
  return (_futurePosition & 0xF0) >> 4;
}

bool Enemy::getEnabled() const {
  return (_flags & 0x10) == 0x10;
}

uint8_t Enemy::getGoldCountdown() const {
  return _goldCountdown;
}

uint8_t Enemy::getDirectionCountdown() const {
  return _directionCountdown;
}

PlayerStance Enemy::getStance() const {
  return _stance;
}

EscapeHole Enemy::getEscapeHole() const {
  return _escapeHole;
}

Direction Enemy::getDirection() const {
  return _direction;
}

Direction Enemy::getPreferredDirection() const {
  return _preferredDirection;
}

void Enemy::setId(uint8_t val) {
  _flags = (_flags & 0xf0) | val;
}
    
void Enemy::setX(uint16_t val) {
  _x = val;
}

void Enemy::setY(uint16_t val) {
  _y = val;
}

void Enemy::setXDelta(int8_t val) {
  _xDelta = val;
}

void Enemy::setYDelta(int8_t val) {
  _yDelta = val;
}

void Enemy::setXFuturePosition(int8_t val) {
  _futurePosition = (_futurePosition & 0xf0) | val;
}

void Enemy::setYFuturePosition(int8_t val) {
  _futurePosition = (_futurePosition & 0x0f) | (val << 4);
}

void Enemy::setEnabled(bool val) {
  _flags = (_flags | (val ? 0x10 : 0x00));
}

void Enemy::setGoldCountdown(uint8_t val) {
  _goldCountdown = val;
}

void Enemy::setDirectionCountdown(uint8_t val) {
  _directionCountdown = val;
}

void Enemy::setStance(const PlayerStance &val) {
  _stance = val;
}

void Enemy::setEscapeHole(const EscapeHole &val) {
  _escapeHole = val;
}

void Enemy::setDirection(const Direction &val) {
  _direction = val;
}

void Enemy::setPreferredDirection(const Direction &val) {
  _preferredDirection = val;
}

void Enemy::decrementDirectionCountdown(void) {
  --this->_directionCountdown;
}

void Enemy::decrementGoldCountdown(void) {
  --this->_goldCountdown;
}


