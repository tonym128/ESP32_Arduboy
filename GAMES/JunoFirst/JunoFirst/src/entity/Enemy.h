#pragma once

#include "../utils/Enums.h"

class Enemy {

  public: 

    Enemy() {};
  
    // Properties ..

    int16_t getX();
    int8_t getY();
    int8_t getXDelta();
    int8_t getYDelta();
    uint8_t getProtection();
    EnemyStatus getStatus();
    EnemyType getType();
    MovementSequence getMovementSequence();
    bool getPlayerOverlap();
        
    void setX(int16_t val);
    void setY(int8_t val);
    void setXDelta(int8_t val);
    void setYDelta(int8_t val);
    void setStatus(EnemyStatus val);
    void setType(EnemyType val);
    void setMovementSequence(MovementSequence val);
    void setPlayerOverlap(bool val);
    void setProtection(uint8_t val);

    // Methods ..

    bool isVisible();
    ImageSize getSize();
    int16_t getXDisplay();
    uint8_t getYDisplay();
    uint8_t getWidth();
    uint8_t getHeight();

    void moveRelativeToPlayer(Player *player);
    bool move(Player *player);
    bool inShootingRange();

  private:

    uint8_t _flags;           // bits 0 - 3 enemy type, bit 4 overlap, bits 5 - 7 movement type
    int16_t _x;
    int8_t _y;
    uint8_t _delta;           // bits 0 - 3 y, bits 4 - 7 x
    EnemyStatus _status; 
    uint8_t _protection;      // if non-zero, enemy is protected from bullets       
    uint8_t _alternate;       // used to slow down the movement of the released astronaut   

};



//--------------------------------------------------------------------------------------------------------------------------
// Properties ..

EnemyType Enemy::getType() {
  return static_cast<EnemyType>(_flags & 0x0f);
}

int16_t Enemy::getX() {
  return _x;
}

int8_t Enemy::getY() {
  return _y;
}

int8_t Enemy::getXDelta() {
  return ((_delta & 0x80) == 0x80 ? 0xF0 | ((_delta & 0xF0) >> 4) : (_delta & 0xF0) >> 4);
}

int8_t Enemy::getYDelta() {
  return ((_delta & 0x08) == 0x08 ? 0xF0 | (_delta & 0x0F) : _delta & 0x0F);
}

EnemyStatus Enemy::getStatus() {
  return _status;
}

MovementSequence Enemy::getMovementSequence() {
  return static_cast<MovementSequence>(_flags >> 5);
}

uint8_t Enemy::getProtection() {
  return _protection;
}

bool Enemy::getPlayerOverlap() {
  return (_flags & 0x10) == 0x10;
}

void Enemy::setType(EnemyType val) {
  _flags = (_flags & 0xf0) | static_cast<uint8_t>(val);
}

void Enemy::setX(int16_t val) {
  _x = val;
}

void Enemy::setY(int8_t val) {
  _y = val;
}

void Enemy::setXDelta(int8_t val) {

  _delta = (val >= 0 ? (_delta & 0x0f) | ((val & 0x0f) << 4) : (_delta & 0x0f) | 0x80 | ((val & 0x07) << 4));
}

void Enemy::setYDelta(int8_t val) {
  _delta = (val >= 0 ? (_delta & 0xF0) | (val & 0x0f) : (_delta & 0xF0) | 0x08 | (val & 0x07));
}

void Enemy::setStatus(EnemyStatus val) {
  _status = val; 
}

void Enemy::setMovementSequence(MovementSequence val) {
  _flags = (_flags & 0x1F) | (static_cast<uint8_t>(val) << 5);
}

void Enemy::setPlayerOverlap(bool val) {
  _flags = (_flags & 0xEF) | (val ? 0x10 : 0x00);
}

void Enemy::setProtection(uint8_t val) {
  _protection = val;
}


//--------------------------------------------------------------------------------------------------------------------------
// Methods ..

bool Enemy::isVisible() {
  return _status > EnemyStatus::Dead && (_y >= ENEMY_VISIBLE_HORIZON && _y - 48 < 64) && ( getXDisplay() + static_cast<int16_t>(getWidth()) >= 0 && getXDisplay() <= static_cast<int16_t>(119));
}

ImageSize Enemy::getSize() {

  // Return the size of the image to render ..

  switch (_y) {

    case ENEMY_DISTANCE_HORIZON_START_1 ... ENEMY_DISTANCE_HORIZON_END_2:
      return ImageSize::Horizon;

    case ENEMY_DISTANCE_FAR_START ... ENEMY_DISTANCE_FAR_END:
      return ImageSize::Far;

    case ENEMY_DISTANCE_MEDIUM_START ... ENEMY_DISTANCE_MEDIUM_END:
      return ImageSize::Medium;

    default:
      return ImageSize::Close;

  }

}

int16_t Enemy::getXDisplay() {

  uint16_t x = (static_cast<int16_t>(_x) * static_cast<int16_t>(_y) / static_cast<int16_t>(96)) + WIDTH_HALF;

  switch (_y) {

    case ENEMY_DISTANCE_HORIZON_START_1 ... ENEMY_DISTANCE_HORIZON_END_2:
      return x;           

    case ENEMY_DISTANCE_FAR_START ... ENEMY_DISTANCE_FAR_END:
      return x - ENEMY_DISTANCE_FAR_WIDTH_HALF;

    case ENEMY_DISTANCE_MEDIUM_START ... ENEMY_DISTANCE_MEDIUM_END:
      return x - ENEMY_DISTANCE_MEDIUM_WIDTH_HALF;

    case ENEMY_DISTANCE_CLOSE_START ... ENEMY_DISTANCE_CLOSE_END:
      return x - ENEMY_DISTANCE_CLOSE_WIDTH_HALF;

    default:
      return x;

  }  

}

uint8_t Enemy::getYDisplay() {
  
  switch (_y) {

    // case ENEMY_DISTANCE_HORIZON_START_1 ... ENEMY_DISTANCE_HORIZON_END_1:
    //   return 0;           

    case 0 ... ENEMY_DISTANCE_HORIZON_END_1:
      return 0;           

    default:
      return _y - 48;

  }

}


uint8_t Enemy::getWidth() {

  switch (_y) {

    case ENEMY_DISTANCE_HORIZON_START_1 ... ENEMY_DISTANCE_HORIZON_END_2:
      return 1;           

    case ENEMY_DISTANCE_FAR_START ... ENEMY_DISTANCE_FAR_END:
      return ENEMY_DISTANCE_FAR_WIDTH;

    case ENEMY_DISTANCE_MEDIUM_START ... ENEMY_DISTANCE_MEDIUM_END:
      return ENEMY_DISTANCE_MEDIUM_WIDTH;

    case ENEMY_DISTANCE_CLOSE_START ... ENEMY_DISTANCE_CLOSE_END:
      return ENEMY_DISTANCE_CLOSE_WIDTH;

    default: return 0;

  }  

}


uint8_t Enemy::getHeight() {

  switch (_y) {

    case ENEMY_DISTANCE_HORIZON_START_1 ... ENEMY_DISTANCE_HORIZON_END_2:
      return 1;           

    case ENEMY_DISTANCE_FAR_START ... ENEMY_DISTANCE_FAR_END:
      return ENEMY_DISTANCE_FAR_HEIGHT;

    case ENEMY_DISTANCE_MEDIUM_START ... ENEMY_DISTANCE_MEDIUM_END:
      return ENEMY_DISTANCE_MEDIUM_HEIGHT;

    case ENEMY_DISTANCE_CLOSE_START ... ENEMY_DISTANCE_CLOSE_END:
      return ENEMY_DISTANCE_CLOSE_HEIGHT;

    default: return 0;

  }  

}

void Enemy::moveRelativeToPlayer(Player *player) {

  _y = clamp(static_cast<int8_t>(_y + (player->getYDelta() / 2)), ENEMY_MINIMUM_Y, ENEMY_MAXIMUM_Y);
  
}


// returns true if the enemy was removed from play ..
bool Enemy::move(Player *player) {


  // Deplete protection if necessary ..

  if (_protection > 0) _protection--;


  // Move enemy ..

  switch (this->getMovementSequence()) {

    case MovementSequence::Sequence_1:

      if ((this->getXDelta() < 0 && _x > ENEMY_MINIMUM_X) || (this->getXDelta() > 0  && _x < ENEMY_MAXIMUM_X)) {

        _x = _x + static_cast<int16_t>(this->getXDelta());

      }
      else {

        this->setXDelta(static_cast<int8_t>(this->getXDelta()) * -1);

        if ((this->getYDelta() < 0 && _y > ENEMY_MINIMUM_Y) || (this->getYDelta() > 0  && _y < ENEMY_MAXIMUM_Y)) {

          _y = _y + (player->getYDelta() <= 0 ? this->getYDelta() : this->getYDelta() / 2);

        }
        else {

          this->setYDelta(static_cast<int8_t>(this->getYDelta()) * -1);

        }

      }
      break;

    case MovementSequence::Sequence_2:

      if ((this->getXDelta() < 0 && _x > ENEMY_MINIMUM_X) || (this->getXDelta() > 0  && _x < ENEMY_MAXIMUM_X)) {

        _x = _x + static_cast<int16_t>(this->getXDelta());

      }
      else {

        this->setXDelta(static_cast<int8_t>(this->getXDelta()) * -1);

      }

      if (this->getYDelta() < 0 && _y > ENEMY_MINIMUM_Y) {

        _y = _y + this->getYDelta();

      }
      else if (this->getYDelta() > 0  && _y < ENEMY_MAXIMUM_Y) {

//Speed        _y = _y + this->getYDelta();
        _y = _y + (player->getYDelta() <= 0 ? this->getYDelta() : this->getYDelta() / 2);

      }
      else {

        this->setYDelta(static_cast<int8_t>(this->getYDelta()) * -1);

      }

      break;

    case MovementSequence::Sequence_3:

      if ((this->getXDelta() < 0 && _x > ENEMY_MINIMUM_X) || (this->getXDelta() > 0  && _x < ENEMY_MAXIMUM_X)) {

        _x = _x + static_cast<int16_t>(this->getXDelta());

      }
      else {

        this->setXDelta(static_cast<int8_t>(this->getXDelta()) * -1);

      }

      if (this->getYDelta() > 0  && _y < ENEMY_MAXIMUM_Y) {

        _y = _y + (player->getYDelta() <= 0 ? this->getYDelta() : this->getYDelta() / 2);

      }
      else {

        _y = 0;

      }

      break;

    case MovementSequence::Sequence_4:  // Returing up the screen.  Used with Astronaut.

      _alternate++;
      if (_alternate == 3) _alternate = 0;

      if (_alternate < 1) {
          
        if ((this->getXDelta() < 0 && _x > ENEMY_MINIMUM_X) || (this->getXDelta() > 0  && _x < ENEMY_MAXIMUM_X)) {

          _x = _x + static_cast<int16_t>(this->getXDelta());

        }
        else {

          this->setXDelta(static_cast<int8_t>(this->getXDelta()) * -1);

        }

        if (this->getYDelta() < 0 && _y > ENEMY_MINIMUM_Y) {

          _y = _y + this->getYDelta();

        }
        else {

          this->setStatus(EnemyStatus::Dead);
          return true;

        }
      
      }

      break;

    default:
      _x = _x + this->getXDelta();
      _y = _y + this->getYDelta();
    
  }

  return false;

}

bool Enemy::inShootingRange() {

  return (_y - 48 > 10 &&  _y - 48 < 40);
  
}
