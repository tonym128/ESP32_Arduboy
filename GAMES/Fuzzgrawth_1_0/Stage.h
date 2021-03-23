// Stage Class and Functions

class Stage {
  public:
    Stage ();
    void Construct ();
    void Draw ();
    void MoveUp ();
    void MoveDown ();
    bool TouchTop (uint8_t x_pos);
    bool TouchBottom (uint8_t x_pos);
    bool TouchLeft (uint8_t x_pos);
    bool TouchRight (uint8_t x_pos);
    bool SawLeft (uint8_t x_pos);
    bool SawRight (uint8_t x_pos);
    bool Zap (uint8_t x_pos);
    bool TouchLeft_WS (uint8_t x_pos);
    bool TouchRight_WS (uint8_t x_pos);
    void Hit_check (uint8_t l_x_lim, uint8_t h_x_lim);
  private:
    uint8_t _stagelayout [12];
    uint8_t _block_pos;
    uint8_t _y_rel;
    uint8_t _current_layout [16][13];
    uint8_t _frame;
    bool _frameswitch;
};

Stage::Stage () {
}

void Stage::Hit_check (uint8_t l_x_lim, uint8_t h_x_lim) {
  //Check Block hit condition
  uint8_t l_y_lim = 5;
  uint8_t h_y_lim = 5;
  if (_y_rel < 4) h_y_lim = 6;
  else if (_y_rel > 4) l_y_lim = 4;

  for (int16_t i = l_x_lim; i <= h_x_lim; i++) {
    for (int16_t j = l_y_lim; j <= h_y_lim; j++) {

      //Destructable Block or fireball launcher
      if ((_current_layout [i][j] == 4) || (_current_layout [i][j] == 7)) {
        if (_current_layout [i][j] == 7) overlay.Kill_Hit();
        _current_layout [i][j] = 0;
        particles.Add ((i * 8), ((j * 8) - 8 + _y_rel));
      }

      //Destructable Window Top & bottom
      else if ((_current_layout [i][j] == 5) || (_current_layout [i][j] == 6)) {
        _current_layout [i][j] = _current_layout [i][j] + 45;
        particles.Add ((i * 8), ((j * 8) - 8 + _y_rel));
        overlay.Win_Hit ();
      }
    }
  }
}

//check of player is touching their head
bool Stage::TouchTop (uint8_t x_pos) {
  if (_y_rel != 0) return (false);
  for (int16_t i = 0; i < 12; i++) {
    uint8_t _tile_check = _current_layout [int16_t ((x_pos + i) / 8)][4];
    if (_tile_check != 0) {
      if ((_tile_check == 8) && (!_frameswitch)) return (false);
      else if ((_tile_check == 9) && (_frameswitch)) return (false);
      else if (_tile_check == 11) ;
      else if (_tile_check == 13) {
        overlay.Level_End ();
        return (false);
      }
      else return (true);
    }
  }
  return (false);
}

//Check to see if player touching the ground
bool Stage::TouchBottom (uint8_t x_pos) {
  if (_y_rel != 0) return (false);
  for (int16_t i = 0; i < 12; i++) {
    uint8_t _tile_check = _current_layout [int16_t ((x_pos + i) / 8)][7];
    if (_tile_check != 0) {
      if ((_tile_check == 8) && (!_frameswitch)) return (false);
      else if ((_tile_check == 9) && (_frameswitch)) return (false);
      else if (_tile_check == 11) ;
      else if (_tile_check == 13) {
        overlay.Level_End ();
        return (false);
      }
      else return (true);
    }
  }
  return (false);
}

//Check for collision on LHS
bool Stage::TouchLeft (uint8_t x_pos) {
  if ((x_pos % 8) != 0) return (false);
  for (int16_t i = 0; i < 16; i++) {
    uint8_t _tile_check = _current_layout [(x_pos / 8) - 1][(40 - _y_rel + i) / 8];
    if (_tile_check != 0) {
      if ((_tile_check == 8) && (!_frameswitch)) return (false);
      else if ((_tile_check == 9) && (_frameswitch)) return (false);
      else if (_tile_check == 11) ;
      else if (_tile_check == 13) {
        overlay.Level_End ();
        return (false);
      }
      else return (true);
    }
  }
  return (false);
}

// Check for collision on RHS
bool Stage::TouchRight (uint8_t x_pos) {
  if (((x_pos + 12) % 8) != 0) return (false);
  for (int16_t i = 0; i < 16; i++) {
    uint8_t _tile_check = _current_layout [(x_pos + 12) / 8][(40 - _y_rel + i) / 8];
    if (_tile_check != 0) {
      if ((_tile_check == 8) && (!_frameswitch)) return (false);
      else if ((_tile_check == 9) && (_frameswitch)) return (false);
      else if (_tile_check == 11) ;
      else if (_tile_check == 13) {
        overlay.Level_End ();
        return (false);
      }
      else return (true);
    }
  }
  return (false);
}

//Check for Zapper
bool Stage::Zap (uint8_t x_pos) {
  if ((_frameswitch) && (_frame < 25)) {
    int16_t y_min = 4;
    int16_t x_min = int16_t (x_pos / 8);;
    int16_t x_max = int16_t ((x_pos + 12) / 8);;
    if (_y_rel == 0) y_min = 5;
    for (int16_t i = x_min; i <= x_max; i++) {
      for (int16_t j = y_min; j < 6; j++) {
        if (_current_layout [i][j] == 11){ 
          return (true);}
      }
    }
  }
  return (false);
}

//Check for Saw on LHS
bool Stage::SawLeft (uint8_t x_pos) {
  if ((x_pos % 8) != 0) return (false);
  uint8_t _i_temp = (x_pos / 8) - 1;
  for (int16_t i = 0; i < 16; i++) {
    if (_current_layout [_i_temp][(40 - _y_rel + i) / 8] == 2) return (true);
  }
  return (false);
}

// Check for Saw on RHS
bool Stage::SawRight (uint8_t x_pos) {
  if (((x_pos + 12) % 8) != 0) return (false);
  uint8_t _i_temp = (x_pos + 12) / 8;
  for (int16_t i = 0; i < 16; i++) {
    if (_current_layout [_i_temp][(40 - _y_rel + i) / 8] == 3) return (true);
  }
  return (false);
}

//Check for WS condition on LHS
bool Stage::TouchLeft_WS (uint8_t x_pos) {
  if ((x_pos % 8) != 0) return (false);
  uint8_t _i_temp = (x_pos / 8) - 1;
  for (int16_t i = 0; i < 8; i++) {
   uint8_t _tile_check =  _current_layout [_i_temp][(48 - _y_rel + i) / 8];
    if (_tile_check != 0) {
      if ((_tile_check == 8) && (!_frameswitch)) return (false);
      else if ((_tile_check == 9) && (_frameswitch)) return (false);
      else if (_tile_check == 11) return (false);
      else if (_tile_check == 13) return (false);
      else return (true);
    }
  }
  return (false);
}

// Check for WS condition on RHS
bool Stage::TouchRight_WS (uint8_t x_pos) {
  if (((x_pos + 12) % 8) != 0) return (false);
  uint8_t _i_temp = (x_pos + 12) / 8;
  for (int16_t i = 0; i < 8; i++) {
    uint8_t _tile_check = _current_layout [_i_temp][(48 - _y_rel + i) / 8];
    if (_tile_check != 0) {
      if ((_tile_check == 8) && (!_frameswitch)) return (false);
      else if ((_tile_check == 9) && (_frameswitch)) return (false);
      else if (_tile_check == 11) return (false);
      else if (_tile_check == 13) return (false);
      else return (true);
    }
  }
  return (false);
}

//Procedural stage generation
void Stage::Construct () {
  _frame = 0;
  _frameswitch = false;
  uint8_t _tile_check = 0;
  uint8_t test_block = 0;
  bool case_check = false;
  bool loop_check = false;

  //Deconstruct the stage first!
  for (int16_t i = 0; i < 12; i++) {
    _stagelayout [i] = 0;
  }
  //Randomise order of the stage blocks
  //each one needs to be checked for duplication
  //Its not an efficient way to do it, but should work
  for (int16_t i = 1; i < 11; i++) {
    case_check = false;
    while (!case_check) {
      test_block = int(random (1, 11)) + (5 * (overlay.Level() - 1));
      loop_check = false;
      for (int16_t j = 1; j < 12; j++) {
        if (test_block == _stagelayout[j])loop_check = true;
      }
      if (loop_check == false) {
        case_check = true;
        _stagelayout [i] = test_block;
      }
    }
  }

  //Final Block position
  _stagelayout [11] = 20 + overlay.Level();

  //Resets player position vertically and loads initial data
  _block_pos = 8;
  _y_rel = 0;
  for (int16_t i = 0; i < 9; i++) {
    for (int16_t j = 0; j < 16; j++) {
      _current_layout [j][i] = pgm_read_byte_near (block_data + (112 + (i * 16) + (j)));
    }
  }

}

void Stage::Draw () {
  //Draw the stage
  _frame++;
  if (_frame > 200) {
    _frame = 0;
    _frameswitch = !_frameswitch;
  }

  //Easy full bricks first
  uint8_t grab = 0;
  for (int16_t i = 0; i < 8; i++) {
    uint8_t _i_temp = (i * 8) + _y_rel;
    for (int16_t j = 0; j < 16; j++) {
      uint8_t _j_temp = j * 8;
      if (_current_layout[j][i + 1] == 1) arduboy.drawBitmap(_j_temp, _i_temp, Brick, 8, 8, 1);
      else if (_current_layout[j][i + 1] == 2) arduboy.drawBitmap(_j_temp, _i_temp, Saw_L, 8, 8, 1);
      else if (_current_layout[j][i + 1] == 3)arduboy.drawBitmap(_j_temp, _i_temp, Saw_R, 8, 8, 1);
      else if (_current_layout[j][i + 1] == 4)arduboy.drawBitmap(_j_temp, _i_temp, D_Brick, 8, 8, 1);
      else if (_current_layout[j][i + 1] == 5)arduboy.drawBitmap(_j_temp, _i_temp, Wind_t, 8, 8, 1);
      else if (_current_layout[j][i + 1] == 6)arduboy.drawBitmap(_j_temp, _i_temp, Wind_b, 8, 8, 1);
      else if (_current_layout[j][i + 1] == 50)arduboy.drawBitmap(_j_temp, _i_temp, BWind_t, 8, 8, 1);
      else if (_current_layout[j][i + 1] == 51)arduboy.drawBitmap(_j_temp, _i_temp, BWind_b, 8, 8, 1);
      else if (_current_layout[j][i + 1] == 12)arduboy.drawBitmap(_j_temp, _i_temp, Zapper, 8, 8, 1);

      //End
      else if (_current_layout[j][i + 1] == 13) {
        if (_frame < 50) arduboy.drawBitmap(_j_temp, _i_temp, Coin1, 8, 8, 1);
        else if (_frame < 100) arduboy.drawBitmap(_j_temp, _i_temp, Coin2, 8, 8, 1);
        else if (_frame < 150) arduboy.drawBitmap(_j_temp, _i_temp, Coin3, 8, 8, 1);
        else arduboy.drawBitmap(_j_temp, _i_temp, Coin2, 8, 8, 1);
      }

      // Lightning ZAP
      else if (_current_layout[j][i + 1] == 11) {
        //Zapping first
        if ((_frameswitch) && (_frame < 25)) {
          if (_frame % 2 == 0) arduboy.fillRect(_j_temp, _i_temp, 8, 8, 1);
          else if ((_frame + 1) % 4 == 0) arduboy.drawBitmap (_j_temp, _i_temp, ZapA, 8, 8, 1);
          else arduboy.drawBitmap (_j_temp, _i_temp, ZapB, 8, 8, 1);
        }
        //Fizzing for preceeding second
        else if ((!_frameswitch) && (_frame > 100 )) {
          if (_frame % 2 == 0 ) arduboy.drawBitmap (_j_temp, _i_temp, FizzA, 8, 8, 1);
          else arduboy.drawBitmap (_j_temp, _i_temp, FizzB, 8, 8, 1);
        }
      }

      else if (_current_layout[j][i + 1] == 8) {
        if (_frameswitch)arduboy.drawBitmap(_j_temp, _i_temp, M_Brick, 8, 8, 1);
        else if ((_frame < 20) || (_frame > 180))arduboy.drawBitmap(_j_temp, _i_temp, FM_Brick, 8, 8, 1);
      }
      else if (_current_layout[j][i + 1] == 9) {
        if (!_frameswitch)arduboy.drawBitmap(_j_temp, _i_temp, M_Brick, 8, 8, 1);
        else if ((_frame < 20) || (_frame > 180))arduboy.drawBitmap(_j_temp, _i_temp, FM_Brick, 8, 8, 1);
      }
      else if (_current_layout [j][i + 1] == 7) {
        arduboy.drawBitmap(_j_temp, _i_temp, Launch, 8, 8, 1);
        if ((_frame == 99) || (_frame == 199)) {
          uint8_t dist = 0;
          for (int16_t l = 1; l < 16; l++) {
            if (_current_layout [j + l][i + 1] == 0) {
              dist = dist + 8;
            }
            else {
              l = 17;
            }
          }
          fire.Add (_j_temp, (_i_temp + 8), dist);
        }
      }
    }
  }

  //Partial tile draw
  if (_y_rel != 0) {
    //only need partial draw tiles for middle set (edges obscured by overlay)
    for (int16_t i = 3; i < 13; i++) {
      if (_current_layout [i][0] != 0) {
        for (int16_t j = 0; j < 8; j++) {
          if (_current_layout [i][0] == 1) grab = pgm_read_byte_near(Brick + j);
          else if (_current_layout [i][0] == 2) grab = pgm_read_byte_near(Saw_L + j);
          else if (_current_layout [i][0] == 3) grab = pgm_read_byte_near(Saw_R + j);
          else if (_current_layout [i][0] == 4) grab = pgm_read_byte_near(D_Brick + j);
          else if (_current_layout [i][0] == 5) grab = pgm_read_byte_near(Wind_t + j);
          else if (_current_layout [i][0] == 6) grab = pgm_read_byte_near(Wind_b + j);
          else if (_current_layout [i][0] == 7) grab = pgm_read_byte_near(Launch + j);
          else if (_current_layout [i][0] == 50) grab = pgm_read_byte_near(BWind_t + j);
          else if (_current_layout [i][0] == 51) grab = pgm_read_byte_near(BWind_b + j);
          else if (_current_layout [i][0] == 12) grab = pgm_read_byte_near(Zapper + j);

          else if (_current_layout[j][i + 1] == 13) {
            if (_frame < 50) grab = pgm_read_byte_near(Coin1 + j);
            else if (_frame < 100) grab = pgm_read_byte_near(Coin2 + j);
            else if (_frame < 150) grab = pgm_read_byte_near(Coin3 + j);
            else grab = pgm_read_byte_near(Coin2 + j);
          }

          // Lightning ZAP
          else if (_current_layout[i][0] == 11) {
            //Zapping first
            if ((_frameswitch) && (_frame < 25)) {
              if (_frame % 2 == 0) grab = 255;
              else if ((_frame + 1) % 4 == 0) grab = pgm_read_byte_near(ZapA + j);
              else grab = pgm_read_byte_near(ZapB + j);
            }
            //Fizzing for preceeding second
            else if ((!_frameswitch) && (_frame > 100 )) {
              if (_frame % 2 == 0 ) grab = pgm_read_byte_near(FizzA + j);
              else grab = pgm_read_byte_near(FizzB + j);
            }
            else grab = 0;
          }

          else if (_current_layout [i][0] == 8) {
            if (_frameswitch) grab = pgm_read_byte_near(M_Brick + j);
            else if ((_frame < 20) || (_frame > 180))grab = pgm_read_byte_near(FM_Brick + j);
            else grab = 0;
          }
          else if (_current_layout [i][0] == 9) {
            if (!_frameswitch) grab = pgm_read_byte_near(M_Brick + j);
            else if ((_frame < 20) || (_frame > 180))grab = pgm_read_byte_near(FM_Brick + j);
            else grab = 0;
          }

          else grab = 0;

          uint8_t _i_temp = (i * 8) + j;
          for (int16_t k = 1; k <= _y_rel; k++) {
            if (bitRead (grab, 8 - k) == 1) arduboy.drawPixel( _i_temp, _y_rel - k, 1);
          }
        }
      }
    }
  }
}

void Stage :: MoveUp () {
  particles.Move_up ();
  fire.MoveUp ();
  ghost.MoveUp ();
  _y_rel ++;
  if (_y_rel == 8) {
    _y_rel = 0;
    _block_pos ++;

    //reshuffle tiles
    for (int16_t i = 0; i < 12; i++) {
      uint8_t _i_temp = 12 - i;
      uint8_t _j_temp = 11 - i; //I know this vaiable name makes no sense... it's just convenient...
      for (int16_t j = 0; j < 16; j++) {
        _current_layout [j][_i_temp] = _current_layout [j][_j_temp];
      }
    }

    //Bring in next row
    int16_t temp_uint8_tskip = (_stagelayout[int16_t (_block_pos / 16)] * 256) + (240 - ((_block_pos % 16) * 16));
    for (int16_t i = 0; i < 16; i++) {
      _current_layout [i][0] = pgm_read_byte_near (block_data + temp_uint8_tskip + i);
      if (_current_layout [i][0] == 10) {
        _current_layout [i][0] = 0;
        ghost.Add ((i * 8), 0);
      }
    }
  }
}

void Stage :: MoveDown () {
  particles.Move_down();
  fire.MoveDown();
  ghost.MoveDown ();
  if (_y_rel != 0) _y_rel --;
  else {
    _y_rel = 7;
    _block_pos --;

    //reshuffle tiles
    for (int16_t i = 0; i < 12; i++) {
      uint8_t _i_temp = i + 1;
      for (int16_t j = 0; j < 16; j++) {
        _current_layout [j][i] = _current_layout [j][_i_temp];
      }
    }

    //Bring in next row
    int16_t temp_uint8_tskip = (_stagelayout[int16_t ((_block_pos - 12) / 16)] * 256) + (240 - (((_block_pos - 12) % 16) * 16));
    for (int16_t i = 0; i < 16; i++) {
      _current_layout [i][12] = pgm_read_byte_near (block_data + temp_uint8_tskip + i);
      if (_current_layout [i][12] == 10) {
        _current_layout [i][12] = 0;
        ghost.Add ((i * 8), 103);
      }
    }
  }
}
