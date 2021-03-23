// Fireball Class and Movement

class Fire {
  public:
    Fire ();
    void Add (uint8_t x, uint8_t y, uint8_t dist);
    void Move ();
    void Draw ();
    void MoveUp ();
    void MoveDown ();
    bool H_Check (uint8_t x);
    void Reset ();
  private:
    uint8_t _current [6][3];
    uint8_t _ani_time;
};

Fire::Fire () {
}

void Fire::Reset () {
  _ani_time = 0;
  for (int16_t i = 0; i < 6; i++) {
    _current [i][0] = 0;
  }
}

void Fire::Add (uint8_t x, uint8_t y, uint8_t dist) {
  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] == 0) {
      _current [i][0] = dist;
      _current [i][1] = x;
      _current [i][2] = y;
      i = 7;
    }
  }
}

void Fire::Draw () {
  _ani_time ++;
  if (_ani_time > 20) _ani_time = 0;
  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] > 0) {
      if (_current [i][2] > 7) {
        if (_ani_time < 10) arduboy.drawBitmap(_current [i][1], (_current [i][2]) - 8, FB_A, 8, 8, 1);
        else arduboy.drawBitmap(_current [i][1], (_current [i][2]) - 8, FB_B, 8, 8, 1);
      }
    }
  }
}

bool Fire::H_Check (uint8_t x) {
  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] != 0) {
      if ((x < (_current [i][1] + 8)) && (x > (_current [i][1] - 12))) {
        if ((_current[i][2] < 56) && (_current [i][2] > 32)) {
          _current [i][0] = 0;
          particles.Add (_current [i][1], (_current [i][2] - 8));
          return (true);
        }
      }
    }
  }
  return (false);
}


void Fire :: Move () {
  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] != 0) {
      _current [i][0] --;
      _current [i][1] ++;
      if (_current [i][0] == 0) particles.Add (_current [i][1], (_current [i][2] - 8));
    }
  }
}

void Fire :: MoveUp () {
  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] != 0) {
      _current [i][2] ++;
    }
  }
}

void Fire :: MoveDown () {
  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] != 0) {
      _current [i][2] --;
    }
  }
}
