// Overlay Class & Functions
// Governs the overall game aspects along with displaying overlay graphics

class Overlay {
  public:
    Overlay ();
    void Setlevel (uint8_t level);
    bool Ingame();
    uint8_t Level ();
    void Draw ();
    void Hit ();
    void Win_Hit ();
    void Kill_Hit ();
    void Level_End ();
    bool Level_End_Check ();
  private:
    uint8_t _level;
    uint8_t _lives;
    int16_t _time;
    bool _ingame;
    uint8_t _seccount;
    int16_t _wincount;
    int16_t _killcount;
    uint8_t _vand;
    uint8_t _bru;
    bool _levelstart;
    bool _levelend;
};

Overlay::Overlay () {
  _ingame = false;
}

void Overlay:: Draw () {
  //Draw: Time, Lives, Game messages

  if (_levelstart) {
    arduboy.clear();
    uint8_t _rshift = 8 * (3 - _level);
    arduboy.drawSlowXYBitmap(0 + _rshift, 12, Lett_L, 16, 8, 1);
    arduboy.drawSlowXYBitmap(16 + _rshift, 12, Lett_E, 16, 8, 1);
    arduboy.drawSlowXYBitmap(32 + _rshift, 12, Lett_V, 16, 8, 1);
    arduboy.drawSlowXYBitmap(48 + _rshift, 12, Lett_E, 16, 8, 1);
    arduboy.drawSlowXYBitmap(64 + _rshift, 12, Lett_L, 16, 8, 1);
    for (int16_t i = 0; i < _level; i++) {
      arduboy.drawSlowXYBitmap((80 + (i * 16) + _rshift), 12, Lett_I, 16, 8, 1);
    }
    arduboy.setCursor (38, 32);
    arduboy.print ("Scale the");
    arduboy.setCursor (50, 45);
    arduboy.print ("Tower");
    arduboy.display();
    delay (2000);
    _levelstart = false;
  }

  else if (_levelend) {
    //End level
    arduboy.display ();
    delay (500);
    arduboy.fillRect (0, 9, 128, 14, 0);
    arduboy.drawSlowXYBitmap(16, 12, Lett_G, 16, 8, 1);
    arduboy.drawSlowXYBitmap(32, 12, Lett_R, 16, 8, 1);
    arduboy.drawSlowXYBitmap(48, 12, Lett_O, 16, 8, 1);
    arduboy.drawSlowXYBitmap(64, 12, Lett_O, 16, 8, 1);
    arduboy.drawSlowXYBitmap(80, 12, Lett_V, 16, 8, 1);
    arduboy.drawSlowXYBitmap(96, 12, Lett_E, 16, 8, 1);
    arduboy.display ();
    delay (500);
    arduboy.fillRect (16, 24, 96, 24, 0);
    arduboy.drawRect (16, 24, 96, 24, 1);
    arduboy.setCursor (64, 32);
    arduboy.print ("= +");
    arduboy.print (_lives * 30);
    arduboy.print ("s");
    for (int16_t i = 0; i < _lives; i++) {
      arduboy.drawBitmap( (45 - (i * 8) - i), 32, Heart, 8, 8, 1);
    }
    arduboy.display ();
    delay (3000);
    _levelstart = true;
    _level++;
    if (_level > 3) {
      //End game
      //SPINNING THINGS AT Y 14
      _ingame = false;
      uint8_t end_frame = 0;
      uint8_t end_delay = 0;
      while (end_frame < 201) {
        end_frame ++;
        if (end_frame == 100) {
          end_frame = 0;
          end_delay ++;
          if (end_delay > 3) end_delay = 3;
        }
        if (arduboy.nextFrame ()) {
          arduboy.clear ();
          arduboy.drawRect (0, 0, 128, 64, 1);
          arduboy.setCursor (44, 3);
          arduboy.print ("COSMIC!");
          arduboy.setCursor (14, 23);
          arduboy.print ("YOU GOT 3 THINGS!");
          arduboy.setCursor (8, 31);
          arduboy.print ("TIME LEFT: ");
          arduboy.print (_time);
          arduboy.setCursor (8, 39);
          arduboy.print ("WINDOWS  : ");
          arduboy.print (_wincount);
          arduboy.setCursor (8, 47);
          arduboy.print ("ENEMIES  : ");
          arduboy.print (_killcount);
          arduboy.setCursor (8, 55);
          arduboy.print ("OVERALL  : ");
          arduboy.print (_killcount + _wincount + _time);

          if (end_frame < 25) {
            arduboy.drawBitmap(60, 13, Coin1, 8, 8, 1);
            arduboy.drawBitmap(44, 13, Coin1, 8, 8, 1);
            arduboy.drawBitmap(76, 13, Coin1, 8, 8, 1);
          }
          else if (end_frame < 50) {
            arduboy.drawBitmap(60, 13, Coin2, 8, 8, 1);
            arduboy.drawBitmap(44, 13, Coin2, 8, 8, 1);
            arduboy.drawBitmap(76, 13, Coin2, 8, 8, 1);
          }
          else if (end_frame < 75) {
            arduboy.drawBitmap(60, 13, Coin3, 8, 8, 1);
            arduboy.drawBitmap(44, 13, Coin3, 8, 8, 1);
            arduboy.drawBitmap(76, 13, Coin3, 8, 8, 1);
          }
          else {
            arduboy.drawBitmap(60, 13, Coin2, 8, 8, 1);
            arduboy.drawBitmap(44, 13, Coin2, 8, 8, 1);
            arduboy.drawBitmap(76, 13, Coin2, 8, 8, 1);
          }

          arduboy.display ();

          if (((arduboy.pressed (A_BUTTON)) || (arduboy.pressed (B_BUTTON))) && (end_delay == 3)) end_frame = 201;
        }
      }
      while (((arduboy.pressed (A_BUTTON)) || (arduboy.pressed (B_BUTTON)))) {}
  }
  else {
    _time = _time + (_lives * 30);
      //30s for each life
      _lives = 3;
    }
  }

  else {
    //Draw Lives & Game Over
    arduboy.fillRect (0, 0, 26, 8, 0);
    if (_lives == 0) {
      //Game Over
      _ingame = false;
      arduboy.fillRect (0, 9, 128, 14, 0);
      arduboy.drawSlowXYBitmap(0, 12, Lett_G, 16, 8, 1);
      arduboy.drawSlowXYBitmap(16, 12, Lett_A, 16, 8, 1);
    }
    else {
      for (int16_t i = 0; i < _lives ; i++) {
        arduboy.drawBitmap(((i * 8) + i), 0, Heart, 8, 8, 1);
      }
    }

    //Update & Draw Timer
    _seccount ++;
    if (_seccount > 99) {
      _seccount = 0;
      _time --;
    }
    arduboy.fillRect (104, 0, 24, 8, 0);
    arduboy.setCursor(108, 0);
    arduboy.print (_time);
    if (_time == 0) {
      //Time Over
      _ingame = false;
      arduboy.fillRect (0, 9, 128, 14, 0);
      arduboy.drawSlowXYBitmap(0, 12, Lett_T, 16, 8, 1);
      arduboy.drawSlowXYBitmap(16, 12, Lett_I, 16, 8, 1);
    }

    // Vandal
    if (_vand > 0) {
      arduboy.setCursor(46, 12);
      arduboy.print ("VANDAL");
      if (_vand < 50) {
        arduboy.setCursor(110, 12 - int(_vand / 10));
        arduboy.print ("+10");
      }
      _vand ++;
      if (_vand > 100) _vand = 0;
    }

    // Brutal
    if (_bru > 0) {
      arduboy.setCursor(46, 12);
      arduboy.print ("BRUTAL");
      if (_bru < 50) {
        arduboy.setCursor(110, 12 - int(_bru / 10));
        arduboy.print ("+10");
      }
      _bru ++;
      if (_bru > 100) _bru = 0;
    }

    if (!_ingame) {
      arduboy.drawSlowXYBitmap(32, 12, Lett_M, 16, 8, 1);
      arduboy.drawSlowXYBitmap(48, 12, Lett_E, 16, 8, 1);
      arduboy.drawSlowXYBitmap(64, 12, Lett_O, 16, 8, 1);
      arduboy.drawSlowXYBitmap(80, 12, Lett_V, 16, 8, 1);
      arduboy.drawSlowXYBitmap(96, 12, Lett_E, 16, 8, 1);
      arduboy.drawSlowXYBitmap(112, 12, Lett_R, 16, 8, 1);
      arduboy.display();
      delay (3000);
    }
  }

}

bool Overlay::Level_End_Check () {
  if (_levelend) {
    _levelend = false;
    return (true);
  }
  else return (false);
}

void Overlay::Level_End () {
  // Level End
  _levelend = true;
}

void Overlay::Win_Hit () {
  _wincount ++;
  if (_wincount % 10 == 0) {
    _time = _time + 10;
    _vand = 1;
  }
}

void Overlay::Kill_Hit () {
  _killcount ++;
  if (_killcount % 10 == 0) {
    _time = _time + 10;
    _bru = 1;
  }
}

void Overlay::Hit () {
  _lives --;
}

void Overlay::Setlevel (uint8_t level) {
  //Combined with resetting all overlay variables & ingame status
  _level = level;
  _vand = 0;
  _bru = 0;
  _levelstart = true;
  _levelend = false;
  _lives = 3;
  _time = 300;
  _seccount = 0;
  _wincount = 0;
  _killcount = 0;
  _ingame = true;
}

bool Overlay::Ingame () {
  if (_ingame) return (true);
  else return (false);
}

uint8_t Overlay::Level () {
  return (_level);
}
