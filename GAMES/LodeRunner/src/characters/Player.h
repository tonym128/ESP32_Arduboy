#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../utils/Utils.h"
#include "../utils/Enums.h"

class Player {

  public: 

    Player() {};
  
    uint8_t getX();
    uint8_t getY();
    int8_t getXDelta();
    int8_t getYDelta();
    uint16_t getScore();
    uint8_t getMen();
    PlayerStance getStance();
    GameState getNextState();
    
    void setX(uint8_t val);
    void setY(uint8_t val);
    void setXDelta(int8_t val);
    void setYDelta(int8_t val);
    void setScore(uint16_t val);
    void setMen(uint8_t val);
    void setStance(const PlayerStance val);
    void setNextState(const GameState val);
    void incrementMen();

  private:

    uint8_t _x;
    uint8_t _y;
    int8_t _xDelta;
    int8_t _yDelta;
    uint16_t _score;
    uint8_t _men;
    PlayerStance _stance;
    GameState _nextState;

};

//--------------------------------------------------------------------------------------------------------------------------

uint8_t Player::getX() {
  return _x;
}

uint8_t Player::getY() {
  return _y;
}

int8_t Player::getXDelta() {
  return _xDelta;
}

int8_t Player::getYDelta() {
  return _yDelta;
}

uint16_t Player::getScore() {
  return _score;
}

uint8_t Player::getMen() {
  return _men;
}

PlayerStance Player::getStance() {
  return _stance;
}

GameState Player::getNextState() {
  return _nextState;
}
    
void Player::setX(uint8_t val) {
  _x = val;
}

void Player::setY(uint8_t val) {
  _y = val;
}

void Player::setXDelta(int8_t val) {
  _xDelta = val;
}

void Player::setYDelta(int8_t val) {
  _yDelta = val;
}

void Player::setScore(uint16_t val) {
  _score = val;
}

void Player::setMen(uint8_t val) {
  _men = val;
}

void Player::setStance(const PlayerStance val) {
  _stance = val;
}

void Player::setNextState(const GameState val) {
  _nextState = val;
}

void Player::incrementMen() {
  if (_men < 10)  _men++;
}