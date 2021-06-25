// Class for particle explosions

class Particles {
  public:
    Particles ();
    void Add (uint8_t x, uint8_t y);
    void AddP (uint8_t x, uint8_t y);
    void Draw ();
    void Move_up();
    void Move_down ();
    void Reset ();
  private:
    uint8_t _current [4][3];
    uint8_t _currentP [4][3];
};

Particles::Particles () {
}

void Particles::Reset () {
  for (int16_t i = 0; i < 4; i++) {
      _current [i][0] = 0;
      _currentP [i][0] = 0;
  }
}

void Particles :: Move_up () {
  for (int16_t i = 0; i < 4; i++) {
    if (_current [i][0] != 0) _current [i][2] ++;
    if (_currentP [i][0] != 0) _currentP [i][2] ++;
  }
}

void Particles :: Move_down () {
  for (int16_t i = 0; i < 4; i++) {
    if (_current [i][0] != 0) _current [i][2] --;
    if (_currentP [i][0] != 0) _currentP [i][2] --;
  }
}

void Particles :: Draw () {
  for (int16_t i = 0; i < 4; i++) {
    if (_current [i][0] != 0) {
      _current [i][0]++;
      if (_current [i][0] < 11) {
        arduboy.drawPixel (_current [i][1] + 1, _current [i][2] + 1, 1);
        arduboy.drawPixel (_current [i][1] + 6, _current [i][2] + 1, 1);
        arduboy.drawPixel (_current [i][1] + 1, _current [i][2] + 6, 1);
        arduboy.drawPixel (_current [i][1] + 6, _current [i][2] + 6, 1);
      }
      else if (_current [i][0] < 21) {
        arduboy.drawPixel (_current [i][1] - 1, _current [i][2] - 2, 1);
        arduboy.drawPixel (_current [i][1] + 8, _current [i][2] - 2, 1);
        arduboy.drawPixel (_current [i][1] - 1, _current [i][2] - 3, 1);
        arduboy.drawPixel (_current [i][1] + 8, _current [i][2] - 3, 1);
      }
      else if (_current [i][0] < 31) {
        arduboy.drawPixel (_current [i][1] - 3, _current [i][2] - 4, 1);
        arduboy.drawPixel (_current [i][1] + 10, _current [i][2] - 4, 1);
        arduboy.drawPixel (_current [i][1] - 3, _current [i][2] - 5, 1);
        arduboy.drawPixel (_current [i][1] + 10, _current [i][2] - 5, 1);
      }
      else if (_current [i][0] < 41) {
        arduboy.drawPixel (_current [i][1] - 5, _current [i][2] - 2, 1);
        arduboy.drawPixel (_current [i][1] + 12, _current [i][2] - 2, 1);
        arduboy.drawPixel (_current [i][1] - 5, _current [i][2] - 3, 1);
        arduboy.drawPixel (_current [i][1] + 12, _current [i][2] - 3, 1);
      }
      else if (_current [i][0] < 51) {
        arduboy.drawPixel (_current [i][1] - 7, _current [i][2] + 1, 1);
        arduboy.drawPixel (_current [i][1] + 14, _current [i][2] + 1, 1);
        arduboy.drawPixel (_current [i][1] - 7, _current [i][2] + 6, 1);
        arduboy.drawPixel (_current [i][1] + 14, _current [i][2] + 6, 1);
      }
      else _current [i][0] = 0;
    }
    if (_currentP [i][0] != 0) {
      _currentP [i][0]++;
      if (_currentP [i][0] < 11)  arduboy.drawBitmap(_currentP [i][1], _currentP [i][2], PopA, 8, 8, 1);
      else if (_currentP [i][0] < 21)  arduboy.drawBitmap(_currentP [i][1], _currentP [i][2], PopB, 8, 8, 1);
      else _currentP [i][0]= 0;
    }
  }
}

void Particles::AddP (uint8_t x, uint8_t y) {
  //Add new pop explosion if slot available
  for (int16_t i = 0; i < 4; i++) {
    if (_currentP [i][0] == 0) {
      _currentP [i][0] = 1;
      _currentP [i][1] = x;
      _currentP [i][2] = y;
      i = 4;
    }
  }
}

void Particles::Add (uint8_t x, uint8_t y) {
  //Add new particle explosion if slot available
  //x, y refer to upper left co-ordinate of 'tile' to be centered around
  for (int16_t i = 0; i < 4; i++) {
    if (_current [i][0] == 0) {
      _current [i][0] = 1;
      _current [i][1] = x;
      _current [i][2] = y;
      i = 4;
    }
  }
}
