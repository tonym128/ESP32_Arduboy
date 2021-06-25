//Player Movement & Variables

class Player {
  public:
    Player ();
    void Reset ();
    void Sit_update ();
    void Input_read ();
    void Move ();
    void Draw ();

  private:
    uint8_t _x_pos;
    uint8_t _x_speed;
    uint8_t _y_speed;
    uint8_t _situation;          //0 = in air, 1 = wallslide left, 2 = wallside right, 3 = jump held, 4 = attack held, 5 = inair jump flag held, 6= facing_left
    uint8_t _x_skip;
    uint8_t _y_skip;
    uint8_t _ani_count;
    uint8_t _step_count;
    uint8_t _attacking;
    uint8_t _player_hit;
    bool _hitfreeze;
};

Player::Player () {
}

void Player::Reset () {
  _x_pos = 58;
  _x_speed = 100;
  _y_speed = 100;
  _situation = 0;
  _x_skip = 0;
  _y_skip = 0;
  _player_hit = 0;
  _attacking = false;
  _ani_count = 0;
  _step_count = 0;
  _attacking = false;
  _player_hit = 0;
  _hitfreeze = false;
}

// Updates the _situation variable with some information about the current player situation
void Player::Sit_update () {
  if (!stage.TouchBottom (_x_pos)) {
    bitWrite (_situation, 0, 1);
    if (stage.TouchLeft_WS (_x_pos)) {
      bitWrite (_situation, 1, 1);
      bitWrite (_situation, 6, 0);
    }
    else bitWrite (_situation, 1, 0);
    if (stage.TouchRight_WS (_x_pos)) {
      bitWrite (_situation, 2, 1);
      bitWrite (_situation, 6, 1);
    }
    else bitWrite (_situation, 2, 0);
  }
  else {
    bitWrite (_situation, 0, 0);
    bitWrite (_situation, 1, 0);
    bitWrite (_situation, 2, 0);
  }
}

void Player::Input_read () {
  //Read inputs and change speed; no movement at this stage

  //_hitfreeze is true when player control temporarily disabled following a hit
  if ((_player_hit > 0) && (_player_hit  < 45)) _hitfreeze = true;
  else _hitfreeze = false;

  //Accelerate faster on ground than in the air, ie. 50% reduced x axis air control
  if (!_hitfreeze) {
    if (arduboy.pressed(RIGHT_BUTTON))_x_speed = _x_speed + 2;
    if (arduboy.pressed(LEFT_BUTTON))_x_speed = _x_speed - 2;
  }

  //Slow x speed on ground
  if ((_x_speed != 100) && (!arduboy.pressed (RIGHT_BUTTON)) && (!arduboy.pressed(LEFT_BUTTON)) && (bitRead(_situation, 0) == 0)) {
    if (_x_speed > 100) _x_speed = _x_speed - 2;
    else _x_speed = _x_speed + 2;
  }

  //Limit X Speed to Maximum
  if (_x_speed > 160) _x_speed = 160;
  else if (_x_speed < 40) _x_speed = 40;

  // Reset Button held flag when released
  if (!(arduboy.pressed(B_BUTTON)) && (bitRead (_situation, 3) == 1) && ((bitRead (_situation, 0) == 0) || (bitRead (_situation, 0) == 1) || (bitRead (_situation, 0) == 2))) {
    bitWrite (_situation, 3, 0);
    bitWrite (_situation, 5, 0);
  }

  //Jump!
  if ((arduboy.pressed(B_BUTTON)) && (bitRead(_situation, 0) == 0) && (bitRead(_situation, 3) == 0) && (!_hitfreeze)) {
    _y_speed = 170;
    bitWrite (_situation, 3, 1);
    bitWrite (_situation, 5, 1);
  }

  //When in air decelerate, but decelerate more when button not held
  if (bitRead(_situation, 0) == 1) {
    if ((arduboy.pressed(B_BUTTON)) && (bitRead(_situation, 5) == 1) && (!_hitfreeze) && (_y_speed > 117)) _y_speed --;
    else {
      _y_speed = _y_speed - 2;
      bitWrite (_situation, 5, 0);
    }
  }

  //Limit Y speed to maximum
  if (_y_speed > 170) _y_speed = 170;
  else if (_y_speed < 30) _y_speed = 30;

  //Wall Jump
  if ((arduboy.pressed(B_BUTTON)) && (bitRead(_situation, 3) == 0) && (!_hitfreeze) && ((bitRead (_situation, 1) == 1) || (bitRead (_situation, 2) == 1))) {
    _y_speed = 170;
    bitWrite (_situation, 3, 1);
    bitWrite (_situation, 5, 1);
    if (bitRead (_situation, 1) == 1) _x_speed = 130;
    else  _x_speed = 70;
  }

  //Attack
  if ((bitRead (_situation, 4) == 1) && (!arduboy.pressed(A_BUTTON)))bitWrite (_situation, 4, 0);
  if ((_attacking == 0) && (bitRead (_situation, 4) == 0) && (arduboy.pressed(A_BUTTON))) {
    bitWrite (_situation, 4, 1);
    _attacking = 1;
  }
}

void Player::Move () {

  //Player Hit Checking

  if (_player_hit == 0) {
    //Check for Saw... well .. spikes...
    if (stage.SawRight (_x_pos)) {
      _x_speed = 70;
      _y_speed = 170;
      _player_hit = 1;
      overlay.Hit ();
      //Serial.println("stage.SawRight");
    }
    if ((stage.SawLeft (_x_pos)) || (fire.H_Check(_x_pos))) {
      _x_speed = 130;
      _y_speed = 170;
      _player_hit = 1;
      overlay.Hit ();
      //Serial.println("stage.SawLeft");
    }
    if ((ghost.H_Check(_x_pos)) || (stage.Zap(_x_pos))) {
      if (_x_speed < 100) _x_speed = 130;
      else _x_speed = 70;
      _y_speed = 170;
      _player_hit = 1;
      overlay.Hit ();
      //Serial.println("ghost.H_Check");
    }
  }

  //Stop on contact
  if ((_x_speed > 100) && (stage.TouchRight (_x_pos))) _x_speed = 100;
  else if ((_x_speed < 100) && (stage.TouchLeft (_x_pos))) _x_speed = 100;
  if ((_y_speed > 100) && (stage.TouchTop (_x_pos))) _y_speed = 100;
  else if ((_y_speed < 100) && (stage.TouchBottom (_x_pos))) _y_speed = 100;

  //Correct facing direction
  if (_x_speed > 100) bitWrite (_situation, 6, 0);
  else if (_x_speed < 100) bitWrite (_situation, 6, 1);

  //move according to frame skip,4 speed 'zones' - X Movement
  if (_x_speed == 100) _x_skip = 0;
  else if (_x_speed > 100) {
    if ((_x_speed > 145) || ((_x_speed > 130) && (_x_skip > 0)) || ((_x_speed > 115) && (_x_skip > 1)) || (_x_skip > 2)) {
      _x_pos ++;
      _step_count ++;
      _x_skip = 0;
    }
    else _x_skip ++;
  }
  else if (_x_speed < 100) {
    if ((_x_speed < 55) || ((_x_speed < 70) && (_x_skip > 0)) || ((_x_speed < 85) && (_x_skip > 1)) || (_x_skip > 2)) {
      _x_pos --;
      _step_count ++;
      _x_skip = 0;
    }
    else _x_skip ++;
  }

  //stop on contact
  if ((_x_speed > 100) && (stage.TouchRight (_x_pos))) _x_speed = 100;
  else if ((_x_speed < 100) && (stage.TouchLeft (_x_pos))) _x_speed = 100;
  if ((_y_speed > 100) && (stage.TouchTop (_x_pos))) _y_speed = 100;
  else if ((_y_speed < 100) && (stage.TouchBottom (_x_pos))) _y_speed = 100;

  //move according to frame skip, 4 speed 'zones' - Y Movement
  if (_y_speed == 100) _y_skip = 0;
  else if (_y_speed > 100) {
    if ((_y_speed > 145) || ((_y_speed > 130) && (_y_skip > 0)) || ((_y_speed > 115) && (_y_skip > 1)) || (_y_skip > 2)) {
      stage.MoveUp ();
      _y_skip = 0;
    }
    else {
      _y_skip ++;
    }
  }
  else if (_y_speed < 100) {
    if ((_y_speed < 55) || ((_y_speed < 70) && (_y_skip > 0)) || ((_y_speed < 85) && (_y_skip > 1)) || (_y_skip > 2)) {
      stage.MoveDown ();
      _y_skip = 0;
    }
    else _y_skip ++;
  }

}

void Player::Draw () {
  uint8_t l_x_lim = 0;
  uint8_t h_x_lim = 0;
  //before anything, see if player hit
  if (_player_hit > 0) {
    _player_hit ++;
    if (_player_hit > 99) _player_hit = 0;
    if (_ani_count % 2 == 0) {
      if (bitRead (_situation, 6) == 0)arduboy.drawSlowXYBitmap (_x_pos, 32, P_H_L, 12, 16, 1);
      else arduboy.drawSlowXYBitmap (_x_pos, 32, P_H_R, 12, 16, 1);
    }
  }

  //Stationary
  else  if ((bitRead (_situation, 0) == 0) && (_x_speed == 100)) {
    _step_count = 0;
    if (bitRead (_situation, 6) == 0) {
      if (_ani_count < 50)arduboy.drawSlowXYBitmap (_x_pos, 32, P_stand_1R, 12, 16, 1);
      else arduboy.drawSlowXYBitmap (_x_pos, 32, P_stand_2R, 12, 16, 1);
    }
    else {
      if (_ani_count < 50)arduboy.drawSlowXYBitmap (_x_pos, 32, P_stand_1L, 12, 16, 1);
      else arduboy.drawSlowXYBitmap (_x_pos, 32, P_stand_2L, 12, 16, 1);
    }
  }

  //Walking
  else if ((bitRead (_situation, 0) == 0) && (_x_speed != 100)) {
    if (_step_count > 60) _step_count = 1;
    if ((_step_count < 11) || ((_step_count < 41) && (_step_count > 30))) {
      if (_x_speed > 100) arduboy.drawSlowXYBitmap (_x_pos, 32, P_walk_1R, 12, 16, 1);
      else arduboy.drawSlowXYBitmap (_x_pos, 32, P_walk_1L, 12, 16, 1);
    }
    else if ((_step_count < 21) || ((_step_count < 51) && (_step_count > 40))) {
      if (_x_speed > 100) arduboy.drawSlowXYBitmap (_x_pos, 32, P_walk_2R, 12, 16, 1);
      else arduboy.drawSlowXYBitmap (_x_pos, 32, P_walk_2L, 12, 16, 1);
    }
    else if (_step_count < 31) {
      if (_x_speed > 100) arduboy.drawSlowXYBitmap (_x_pos, 32, P_walk_3R, 12, 16, 1);
      else  arduboy.drawSlowXYBitmap (_x_pos, 32, P_walk_3L, 12, 16, 1);
    }
    else if (_step_count > 50) {
      if (_x_speed > 100) arduboy.drawSlowXYBitmap (_x_pos, 32, P_stand_1R, 12, 16, 1);
      else arduboy.drawSlowXYBitmap (_x_pos, 32, P_stand_1L, 12, 16, 1);
    }
  }

  //Jump
  else if ((bitRead (_situation, 0) == 1) && (bitRead (_situation, 1) == 0) && (bitRead (_situation, 2) == 0)) {
    if (bitRead (_situation, 6) == 0) arduboy.drawSlowXYBitmap (_x_pos, 32, P_jump_R, 12, 16, 1);
    else arduboy.drawSlowXYBitmap (_x_pos, 32, P_jump_L, 12, 16, 1);
  }

  //wall slide
  else if (bitRead (_situation, 1) == 1) arduboy.drawSlowXYBitmap (_x_pos, 32, P_ws_L, 12, 16, 1);
  else if (bitRead (_situation, 2) == 1) arduboy.drawSlowXYBitmap (_x_pos, 32, P_ws_R, 12, 16, 1);

  //Sword Animation
  if ((_attacking != 0) && (_player_hit == 0)) {
    if (bitRead (_situation, 6) == 0) {
      if (_attacking < 7) arduboy.drawSlowXYBitmap (_x_pos + 4, 36, Sword_1R, 16, 8, 1);
      else if (_attacking < 13) {
        arduboy.drawSlowXYBitmap (_x_pos + 4 , 36, Sword_2R, 16, 8, 1);
        //Hit check for stage blocks (facing right)
        l_x_lim = int16_t ((_x_pos + 8) / 8);
        h_x_lim = int16_t ((_x_pos + 20) / 8);
        if (h_x_lim > 15) h_x_lim = 15;
        stage.Hit_check (l_x_lim, h_x_lim);
        ghost.Hit_check (_x_pos + 8, _x_pos + 20);
      }
      else arduboy.drawSlowXYBitmap (_x_pos + 4 , 36, Sword_3R, 16, 8, 1);
    }
    else {
      if (_attacking < 7) arduboy.drawSlowXYBitmap (_x_pos - 8, 36, Sword_1L, 16, 8, 1);
      else if (_attacking < 13) {
        arduboy.drawSlowXYBitmap (_x_pos - 8 , 36, Sword_2L, 16, 8, 1);
        //Perform Hitcheck in themiddle of the sword animation, pretty sweet huh? (facing left)
        if (_x_pos > 12) l_x_lim = int16_t ((_x_pos - 12) / 8);
        else l_x_lim = 0;
        h_x_lim = int16_t ((_x_pos) / 8);
        stage.Hit_check (l_x_lim, h_x_lim);
        //Right.. sorry.... this is messy...
        if (_x_pos > 12) l_x_lim = _x_pos - 12;
        else l_x_lim = 0;
        ghost.Hit_check (l_x_lim, _x_pos);
      }
      else arduboy.drawSlowXYBitmap (_x_pos - 8 , 36, Sword_3L, 16, 8, 1);
    }
    _attacking ++;
    if (_attacking > 19) _attacking = 0;
  }
  _ani_count ++;
  if (_ani_count > 99) _ani_count = 0;
}
