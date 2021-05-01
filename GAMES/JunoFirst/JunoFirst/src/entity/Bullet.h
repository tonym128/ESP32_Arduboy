#pragma once

class Bullet {

  public: 

    Bullet() {};
  
    // Properties ..

    uint8_t getX();
    uint8_t getY();
    int8_t getXDelta();
    int8_t getYDelta();
    
    void setX(uint8_t val);
    void setY(uint8_t val);
    void setXDelta(int8_t val);
    void setYDelta(int8_t val);


    // Methods ..

    void move(Player *player);
    void moveRelativeToPlayer(Player *player);

  private:

    uint8_t _x;
    uint8_t _y;
    int8_t _delta;

};


//--------------------------------------------------------------------------------------------------------------------------
// Properties ..

uint8_t Bullet::getX() {
  return _x;
}

uint8_t Bullet::getY() {
  return _y;
}

int8_t Bullet::getXDelta() {
  return ((_delta & 0x80) == 0x80 ? 0xF0 | ((_delta & 0xF0) >> 4) : (_delta & 0xF0) >> 4);
}

int8_t Bullet::getYDelta() {
  return ((_delta & 0x08) == 0x08 ? 0xF0 | (_delta & 0x0F) : _delta & 0x0F);
}

void Bullet::setX(uint8_t val) {
  _x = val;
}

void Bullet::setY(uint8_t val) {
  _y = val;
}

void Bullet::setXDelta(int8_t val) {
  _delta = (val >= 0 ? (_delta & 0x0f) | ((val & 0x0f) << 4) : (_delta & 0x0f) | 0x80 | ((val & 0x07) << 4));
}

void Bullet::setYDelta(int8_t val) {
  _delta = (val >= 0 ? (_delta & 0xF0) | (val & 0x0f) : (_delta & 0x0f) | 0x08 | (val & 0x07));
}


//--------------------------------------------------------------------------------------------------------------------------
// Methods ..

void Bullet::move(Player *player) {

  _x = _x + getXDelta();

  if (player->getYDelta() <=0) {
    _y = _y + getYDelta();
  }
  else {
    _y = _y + (getYDelta() / 2);
  }

  if (_y > HEIGHT) { _y = 0; }
  
}

void Bullet::moveRelativeToPlayer(Player *player) {

//Speed  _y = static_cast<int8_t>(_y + (player->getYDelta() / 4));
  _y = static_cast<int8_t>(_y + (player->getYDelta() / 2));
  if (_y > HEIGHT) { _y = 0; }
  
}