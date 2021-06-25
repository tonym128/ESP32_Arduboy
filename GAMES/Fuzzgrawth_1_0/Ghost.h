//Ghost definition, movement, & behaviour

class Ghost {
  public:
    Ghost ();
    void Add (uint8_t x, uint8_t y);
    void Move ();
    void Draw ();
    void MoveUp ();
    void MoveDown ();
    void Reset ();
    bool H_Check (uint8_t x);
    bool Hit_check (uint8_t l_x_lim, uint8_t h_x_lim);
  private:
    uint8_t _current [6][4];
    uint8_t _ani_time;
};

Ghost::Ghost () {
}

void Ghost::Reset () {
  _ani_time = 0;
  for (int16_t i = 0; i < 6; i++) {
    _current [i][0] = 0;
  }
}

void Ghost::Add (uint8_t x, uint8_t y) {
  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] == 0) {
      _current [i][0] = 1;
      _current [i][1] = x;
      _current [i][2] = y;
      _current [i][3] = 0;
      i = 7;
    }
  }
}

void Ghost::Move () {
  //Moves 4 tiles right then left, pausing every 8 frames
  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] != 0) {
      if ((_current [i][3] < 16) && (_current [i][3] % 2 == 0)) {
        if (_current [i][0] < 5) _current [i][1] ++;
        else _current [i][1] --;
      }
      _current [i][3]++;
      if (_current [i][3] > 33) {
        _current [i][3] = 0;
        _current [i][0] ++;
      }
      if (_current [i][0] > 8) _current [i][0] = 1;
    }
  }
}

void Ghost::MoveUp () {
  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] != 0) {
      if (_current [i][2] < 102)_current [i][2] ++;
      else _current [i][0] = 0;
    }
  }
}

void Ghost::MoveDown () {
  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] != 0) {
      if (_current [i][2] > 0)_current [i][2] --;
      else _current [i][0] = 0;
    }
  }
}

bool Ghost::Hit_check (uint8_t l_x_lim, uint8_t h_x_lim) {
  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] != 0) {
      if ((_current [i][2] > 36) && (_current [i][2] < 60 )) {
        if ((_current [i][1] > (l_x_lim - 8)) && (_current [i][1] <= h_x_lim)) {
          _current [i][0] = 0;
          particles.AddP (_current [i][1], _current[i][2] - 8);
          particles.AddP (_current [i][1], _current[i][2] - 16);
          overlay.Kill_Hit();
          return (true);
        }
      }
    }
  }
  return (false);
}

bool Ghost::H_Check (uint8_t x) {
  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] != 0) {
      if ((_current [i][2] > 32) && (_current [i][2] < 64 )) {
        if ((_current [i][1] > (x - 8)) && (_current [i][1] < (x + 12))) {
          return (true);
        }
      }
    }
  }
  return (false);
}

void Ghost::Draw () {
  uint8_t grab [8];

  _ani_time ++;
  if (_ani_time > 100) _ani_time = 0;

  for (int16_t i = 0; i < 6; i++) {
    if (_current [i][0] != 0) {

      //Draw base first (ghost postion defined through base tile
      //Easy bit (full tile)
      if (_current [i][2] > 7) {
        if (_ani_time < 50) arduboy.drawBitmap(_current [i][1], (_current [i][2]) - 8, G_BaseA, 8, 8, 1);
        else arduboy.drawBitmap(_current [i][1], (_current [i][2]) - 8, G_BaseB, 8, 8, 1);
      }

      //Awkward Bit, partial tile
      else {

        // if the y position is 0 then don't draw anything... because it's not on the screen yet!
        if (_current [i][2] != 0) {
          for (int16_t j = 0; j < 8; j++) {
            if (_ani_time < 50) grab [j] = pgm_read_byte_near( G_BaseA + j);
            else grab [j] = pgm_read_byte_near( G_BaseB + j);
          }
          for (int16_t j = 0; j < 8; j++) {
            for (int16_t k = 1; k <= _current [i][2]; k++) {
              if (bitRead (grab [j], 8 - k) == 1) arduboy.drawPixel( _current [i][1] + j, _current [i][2] - k, 1);
            }
          }
        }
      }

      //Draw Head
      //Easy bit (full tile)
      if (_current [i][2] > 15) {
        arduboy.drawBitmap(_current [i][1], _current [i][2] - 16, G_Head, 8, 8, 1);
      }

      //Awkward Bit, partial tile
      else {
        // if the y position is < 9 then don't draw anything... because it's not on the screen yet!
        if (_current [i][2] > 8 ) {
          for (int16_t j = 0; j < 8; j++) {
            grab [j] = pgm_read_byte_near( G_Head + j);
          }
          for (int16_t j = 0; j < 8; j++) {
            for (int16_t k = 1; k <= _current [i][2] - 8; k++) {
              if (bitRead (grab [j], 8 - k) == 1) arduboy.drawPixel( _current [i][1] + j, _current [i][2] - k - 8, 1);
            }
          }
        }
      }
    }
  }
}
