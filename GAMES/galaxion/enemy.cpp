//
//
//

#include "game.h"
#include "ArduboyTones.h"

#include "bmp_chara.h"
#include "sound.h"

extern Arduboy2 mArduboy;
extern Game mGame;
extern ArduboyTones sound;

//=============================================================================
static int get_dir(int dx)
{
  if (dx < -20) {
    return (6);
  } else if (dx < -10) {
    return (7);
  } else if (dx < 10) {
    return (8);
  } else if (dx < 20) {
    return (9);
  } else {
    return (10);
  }
}

//=============================================================================
void Enemy::init(int _x, int _y, eEnemyType _type, int _dir)
{
  type = _type;
  pos.set(_x, _y);
  pat = 0;
  dir = _dir;
  state = ENEMY_ST_LAUNCHED;
  freq = 800;

  atk_pat = 0;
  atk_cnt = 0;

  fire_cnt = 0;
}

//=============================================================================
// Check Enemy State
//=============================================================================
void Enemy::do_state(int frame, int ship_x, int ship_y, Enemy *boss, bool attack)
{
  const float move_x[] = {
    0.0f, -0.19f, -0.35f, -0.46f, -0.5f, -0.46f, -0.35f, -0.19f,
    0.0f,  0.19f,  0.35f,  0.46f,  0.5f,  0.46f,  0.35f,  0.19f
  };
  const float move_y[] = {
    -0.5f, -0.46f, -0.35f, -0.19f, 0.0f,  0.19f,  0.35f,  0.46f,
     0.5f,  0.46f,  0.35f,  0.19f, 0.0f, -0.19f, -0.35f, -0.46f
  };

  int x, y, dx, dy;

  switch (state) {
  case ENEMY_ST_LAUNCHED:
    // ---------------------------------------------------- Launched (Turn) ---
    if ((frame & 3) == 0) {
      if (dir > 0) {
	if (pat == 0) {
	  pat = 15;
	} else {
	  pat --;
	}
      } else {
	pat ++;
      }
    }

    pos.move_x(move_x[pat]);
    pos.move_y(move_y[pat]);

    if (pat == 8) {
      // ------------------------------------------------ Change to Landing ---
      state = ENEMY_ST_LANDING;

      if (type == GREEN_ALIEN) {
	dx = (ship_x - pos.get_x());
	dy = (ship_y - pos.get_y());
	vx = Position::get_v(dx, Position::get_frames(dy, 0.25f));
	if (random(2) == 0) {
	  vx *= 1.3f;
	}
	if (vx >= 0.0f && vx < 0.1f) {
	  vx = 0.1f;
	} else if (vx <= 0.0f && vx > -0.1f) {
	  vx = -0.1f;
	} else if (vx > 0.5f) {
	  vx = 0.5f;
	} else if (vx < -0.5f) {
	  vx = -0.5f;
	}
      } else {
	x = pos.get_x();
	y = pos.get_y();
	if (x < (SCREEN_W - 5) / 2) {
	  dx = ((SCREEN_W - 10) - x);
	  dy = (36 - y);
	} else {
	  dx = (10 - x);
	  dy = (36 - y);
	}
	vx = Position::get_v(dx, Position::get_frames(dy, 0.25f));
	if (random(2) == 0) {
	  vx *= 1.2f;
	  if (type == PURPLE_ALIEN) {
	    ax = -vx * 0.08f;
	  } else {
	    ax = -vx * 0.05f;
	  }
	} else {
	  vx *= 1.5f;
	  if (type == PURPLE_ALIEN) {
	    ax = -vx * 0.1f;
	  } else {
	    ax = -vx * 0.07f;
	  }
	}
      }

      atk_cnt = 32;
    }
    break;

  case ENEMY_ST_LANDING:
    // ------------------------------------------------------------ Landing ---
    x = pos.get_x();
    y = pos.get_y();

    if ((frame & 3) == 0) {
      dx = (ship_x - x);
      pat = get_dir(dx);
    }

    if (y < (64 - 18)) {
      pos.move_y(0.25f);
    } else {
      pos.move_y(0.5f);
    }
    if (boss != NULL) {
      vx = boss->vx;
    }
    if (type == GREEN_ALIEN) {
      if (atk_cnt <= 0) {
	pos.move_x(vx * 1.5f);
      } else {
	atk_cnt --;
	pos.move_x(vx / 1.5f);
      }
    } else {
      if ((frame & 7) == 0) {
	vx += ax;
      }
      pos.move_x(vx);
    }

    x = pos.get_x();
    y = pos.get_y();
    if (y >= 64 || x < (-5) || x >= SCREEN_W) {
      pos.set_x(mGame.mConvoy.x + col * 6);
      pos.set_y(-5);
      pat = 8;
      if (attack && mGame.mConvoy.num_enemies <= 4) {
	fire = 0;
	fire_cnt = 0;
	atk_cnt = 64;
	if ((ship_x - pos.get_x()) < 0) {
	  vx = ((float)random(9) / 8.0f) - 0.7f;
	} else {
	  vx = ((float)random(9) / 8.0f) - 0.3f;
	}
	if (type == BOSS_ALIEN) {
	  score = 15;
	}
	state = ENEMY_ST_FALLING;
      } else {
	state = ENEMY_ST_TO_CONVOY;
      }
    }
    break;

  case ENEMY_ST_FALLING:
    // ------------------------------------------------------------ Falling ---
    x = pos.get_x();
    y = pos.get_y();

    if ((frame & 3) == 0) {
      dx = (ship_x - x);
      pat = get_dir(dx);
    }

    pos.move_x(vx);

    atk_cnt --;
    if (atk_cnt < 0) {
      if ((ship_x - pos.get_x()) < 0) {
	vx = ((float)random(9) / 8.0f) - 0.7f;
      } else {
	vx = ((float)random(9) / 8.0f) - 0.3f;
      }
      atk_cnt = 32;
    }

    if (y < (64 - 18)) {
      pos.move_y(0.25f);
    } else {
      pos.move_y(0.5f);
    }

    x = pos.get_x();
    y = pos.get_y();
    if (y >= 64 || x < (-5) || x >= SCREEN_W) {
      pos.set_x(mGame.mConvoy.x + col * 6);
      pos.set_y(-5);
      pat = 8;
      if (attack) {
	fire = 0;
	fire_cnt = 0;
	atk_cnt = 64;
	if ((ship_x - pos.get_x()) < 0) {
	  vx = ((float)random(9) / 8.0f) - 0.7f;
	} else {
	  vx = ((float)random(9) / 8.0f) - 0.3f;
	}
	state = ENEMY_ST_FALLING;
      } else {
	state = ENEMY_ST_TO_CONVOY;
      }
    }
    break;

  case ENEMY_ST_TO_CONVOY:
    // --------------------------------------------------- Return to Convoy ---
    int ret_y;
    ret_y = mGame.mConvoy.y + row * 5;
    pos.set_x(mGame.mConvoy.x + col * 6);
    pos.move_y(0.5f);
    if (ret_y <= pos.get_y()) {
      freq = 0;
      state = ENEMY_ST_RETURNED;
    }
    break;

  case ENEMY_ST_RETURNED:
    // ---------------------------------------------------- Returned (Turn) ---
    if ((frame & 3) == 0) {
      pos.set_y(mGame.mConvoy.y + row * 5);
      pos.set_x(mGame.mConvoy.x + col * 6);
      if (dir > 0) {
	pat --;
      } else {
	pat ++;
	if (pat >= 16) pat = 0;
      }
      if (pat == 0) {
	state = ENEMY_ST_NONE;
      }
    }
    break;
  }

  if (freq >= 400) {
    freq -= 5;
  } else if (freq >= 300) {
    freq -= 1;
  }
}

//=============================================================================
bool Enemy::fire_missile()
{
  if (state == ENEMY_ST_LANDING || state == ENEMY_ST_FALLING) {
    fire_cnt ++;
    if (fire == 0 && fire_cnt >= 24) {
      fire = 1;
      if (boss_idx >= 0) return (false);
      return (true);
    } else if (fire == 1 && fire_cnt >= 48) {
      fire = 2;
      return (true);
    } else if (fire == 2 && fire_cnt >= 72) {
      fire = 3;
      return (true);
    } else if (fire == 3 && fire_cnt >= 96 && pos.get_y() < 40) {
      fire = 4;
      if (boss_idx >= 0) return (false);
      return (true);
    }
  }

  return (false);
}

//=============================================================================
void Enemy::draw(int frame)
{
  if (type == BOSS_ALIEN) {
    mArduboy.drawBitmap(pos.get_x() + SCREEN_X, pos.get_y(),
			enemyboss_bmp[pat], 5, 5, WHITE);
  } else {
    mArduboy.drawBitmap(pos.get_x() + SCREEN_X, pos.get_y(),
			enemyzako_bmp[pat + 8], 5, 5, WHITE);
  }
}

//=============================================================================
void Explosion::draw()
{
  mArduboy.drawBitmap(x + SCREEN_X, y, enemyexp_bmp[pat], 7, 7, WHITE);
}

//=============================================================================
void BossScore::draw()
{
  mArduboy.drawBitmap(x + SCREEN_X, y, score_bmp[pat], 9, 5, WHITE);
}

//=============================================================================
void Missile::init(int _x, int _y, float _vx)
{
  pos.set(_x, _y);
  vx = _vx;
}

//=============================================================================
void Missile::do_state(int frame)
{
  if (pos.get_y() <= 0) return;

  pos.move_x(vx);
  pos.move_y(0.75f);

  //x += vx;
  //y += (POS_SCALE * 3) / 4;

  if (pos.get_y() >= 64) {
    pos.set(0, 0);
  }
}

//=============================================================================
void Missile::draw()
{
  int y = pos.get_y();
  if (y <= 0) return;

  mArduboy.drawFastVLine(pos.get_x() + SCREEN_X, y, 2, WHITE);
}

//=============================================================================
void Convoy::init(int stage_no)
{
  x = (SCREEN_W - 6 * 10) / 2;
  y = 2;
  dir = 1;

  if (stage_no == 1) {
    dont_attack = FRAMES_IN_SECOND * (5 + 3);
  } else {
    dont_attack = FRAMES_IN_SECOND * 3;
  }

  for (int row = 0; row < CONVOY_ROWS; row ++) {
    for (int col = 0; col < CONVOY_COLS; col ++) {
      convoy[row][col] = -1;
    }
  }

  num_enemies = 36;

  convoy[0][3] = BOSS_ALIEN;
  convoy[0][6] = BOSS_ALIEN;
  for (int col = 2; col < 8; col ++) {
    convoy[1][col] = RED_ALIEN;
  }
  for (int col = 1; col < 9; col ++) {
    convoy[2][col] = PURPLE_ALIEN;
  }
  for (int col = 0; col < 10; col ++) {
    convoy[3][col] = GREEN_ALIEN;
    convoy[4][col] = GREEN_ALIEN;
  }

  left = 0;
  right = 9;

  if (stage_no < 5) {
    max_zako_in_scr = stage_no + 2;
  } else {
    max_zako_in_scr = 7;
  }
  zako_in_scr = 0;
  boss_in_scr = 0;

  boss_attack = 3;

  for (int idx = 0; idx < MAX_ENEMIES; idx ++) {
    if (enemy[idx] != NULL) {
      delete enemy[idx];
      enemy[idx] = NULL;
    }
  }
}

//=============================================================================
void Convoy::delete_alien(int row, int col)
{
  convoy[row][col] = -1;
  num_enemies --;

  if (num_enemies > 0) {
    if (col == left) {
      int row2;
      do {
	for (row2 = 0; row2 < CONVOY_ROWS; row2 ++) {
	  if (convoy[row2][left] >= 0) break;
	}
	if (row2 == CONVOY_ROWS) {
	  left ++;
	}
      } while (row2 == CONVOY_ROWS);
    } else if (col == right) {
      int row2;
      do {
	for (row2 = 0; row2 < CONVOY_ROWS; row2 ++) {
	  if (convoy[row2][right] >= 0) break;
	}
	if (row2 == CONVOY_ROWS) {
	  right --;
	}
      } while (row2 == CONVOY_ROWS);
    }
  }
}

//=============================================================================
int Convoy::defeat_alien(Position *missile)
{
  int row, col;
  int chk_x, chk_y;
  int score = 0;

  int mis_x = missile->get_x();
  int mis_y = missile->get_y();

  int dx, dy;
  bool hit;

  row = (mis_y - y) / 5;
  col = (mis_x - x) / 6;

  chk_y = y + row * 5 + 2;
  chk_x = x + col * 6 + 2;

  // -------------------------------------------------- Check Convoy Aliens ---
  if (row >= 0 && col >= 0 && row < CONVOY_ROWS && col < CONVOY_COLS) {
    hit = false;
    switch (mGame.mMode) {
    case GAME_MODE_PRACTICE:
      dx = chk_x - mis_x;
      dy = chk_y - mis_y;
      if (dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1) {
	hit = true;
      }
      break;
    case GAME_MODE_NORMAL:
      if (mis_x == chk_x && (mis_y == chk_y || (mis_y + 1) == chk_y)) {
	hit = true;
      }
      break;
    }

    if (hit
	&& convoy[row][col] >= BOSS_ALIEN && convoy[row][col] <= GREEN_ALIEN) {
      const int convoy_score[4] = { 6, 5, 4, 3 };
      score = convoy_score[convoy[row][col]];

      if (exp != NULL) delete exp;
      exp = new Explosion();
      exp->init(chk_x - 3, chk_y - 3, 0);
      if (mArduboy.audio.enabled() && !sound.playing()) {
	    //mArduboy.tunes.playScore(snd_zakoexp);
      //sound.tones(snd_zakoexp);
      sound.tone(300, 50, 200, 50, 100, 50);
      }

      delete_alien(row, col);

      return (score);
    }
  }

  // ------------------------------------------------ Check Launched Aliens ---
  for (int idx = 0; idx < MAX_ENEMIES; idx ++) {
    if (enemy[idx] != NULL) {
      chk_x = enemy[idx]->pos.get_x() + 1;
      chk_y = enemy[idx]->pos.get_y() + 1;

      hit = false;
      switch (mGame.mMode) {
      case GAME_MODE_PRACTICE:
	if ((mis_x - chk_x) >= -1 && (mis_x - chk_x) <=3
	    && (mis_y - chk_y) >= -1 && (mis_y - chk_y) <= 3) {
	  hit = true;
	}
	break;
      case GAME_MODE_NORMAL:
	if ((mis_x - chk_x) >= 0 && (mis_x - chk_x) <=2
	    && (mis_y - chk_y) >= 0 && (mis_y - chk_y) <= 2) {
	  hit = true;
	}
	break;
      }

      if (hit) {
	score = enemy[idx]->score;

	if (exp != NULL) delete exp;
	exp = new Explosion();
	exp->init(chk_x - 2, chk_y - 2, 0);

	if (enemy[idx]->type == BOSS_ALIEN) {
	  if (mArduboy.audio.enabled() && !sound.playing()) {
	    //mArduboy.tunes.playScore(snd_bossexp);
      //sound.tones(snd_zakoexp);
	  }
	  if (bscore != NULL) delete bscore;
	  bscore = new BossScore();
	  bscore->init(chk_x - 3, chk_y - 1, 0);
	  bscore->counter = FRAMES_IN_SECOND * 2;
	  if (enemy[idx]->score == 20) {
	    bscore->pat = 1;
	  } else if (enemy[idx]->score == 30) {
	    bscore->pat = 2;
	  } else if (enemy[idx]->score == 80) {
	    bscore->pat = 3;
	  }
	  dont_attack = FRAMES_IN_SECOND * 4;
	} else {
	  if (enemy[idx]->type == RED_ALIEN && enemy[idx]->boss_idx >= 0) {
	    int boss_idx = enemy[idx]->boss_idx;
	    if (enemy[boss_idx] != NULL) {
	      enemy[boss_idx]->boss_idx --;
	      if (enemy[boss_idx]->boss_idx == 0) {
		enemy[boss_idx]->score = 80;
	      }
	    }
	  }

	  if (mArduboy.audio.enabled() && !sound.playing()) {
	    //mArduboy.tunes.playScore(snd_zakoexp);
      //sound.tones(snd_zakoexp);
	  }
	}

	delete_alien(enemy[idx]->row, enemy[idx]->col);

	remove_enemy(idx);

	return (score);
      }
    }
  }

  return (score);
}

//=============================================================================
bool Convoy::missile_hit(int ship_x, int ship_y)
{
  int chk_x, chk_y, dx, dy;

  for (int idx = 0; idx < MAX_MISSILES; idx ++) {
    chk_y = missile[idx].pos.get_y();
    if (chk_y > 0) {
      chk_x = missile[idx].pos.get_x();
      dx = chk_x - ship_x;
      dy = chk_y - ship_y;
      if (dx >= 1 && dx <= 3 && dy >= 2 && dy <= 6) {
	missile[idx].pos.set(0, 0);
	return (true);
      }
    }
  }

  return (false);
}

//=============================================================================
int Convoy::alien_hit(int ship_x, int ship_y)
{
  int chk_x, chk_y, dx, dy;

  for (int idx = 0; idx < MAX_ENEMIES; idx ++) {
    if (enemy[idx] != NULL) {
      chk_x = enemy[idx]->pos.get_x() + 1;
      chk_y = enemy[idx]->pos.get_y() + 1;
      dx = chk_x - (ship_x + 1);
      dy = chk_y - (ship_y + 3);
      if (dx >= -2 && dx <= 2 && dy >= -2 && dy <= 2) {
	int score = enemy[idx]->score;

	delete_alien(enemy[idx]->row, enemy[idx]->col);

	remove_enemy(idx);

	return (score);
      }
    }
  }
  return (0);
}

//=============================================================================
int Convoy::do_launch_alien(int row, int col, int dir)
{
  if (row < 0 || row >= CONVOY_ROWS || col < 0 || col >= CONVOY_COLS) {
    return (-1);
  }
  if (convoy[row][col] < BOSS_ALIEN || convoy[row][col] > GREEN_ALIEN) {
    return (-1);
  }

  for (int idx = 0; idx < MAX_ENEMIES; idx ++) {
    if (enemy[idx] == NULL) {
      enemy[idx] = new Enemy();
      if (enemy[idx] == NULL) {
	return (-1);
      }

      enemy[idx]->init(x + col * 6, y + row * 5,
		       (eEnemyType)convoy[row][col], dir);
      //
      enemy[idx]->row = row;
      enemy[idx]->col = col;
      enemy[idx]->boss_idx = -1;

      const int score[4] = { 15, 10, 8, 6 };
      enemy[idx]->score = score[convoy[row][col]];

      convoy[row][col] = 0x40 + idx;

      if (enemy[idx]->type == BOSS_ALIEN) {
	boss_in_scr ++;
      } else {
	zako_in_scr ++;
      }

      return(idx);
    }
  }

  return (-1);
}

//=============================================================================
void Convoy::remove_enemy(int idx)
{
  if (enemy[idx] == NULL) return;

  if (enemy[idx]->type == BOSS_ALIEN) {
    boss_in_scr --;
  } else {
    zako_in_scr --;
  }

  delete enemy[idx];
  enemy[idx] = NULL;
}

//=============================================================================
bool Convoy::do_launch_boss(int col)
{
  int dir = ((col - left) < (right - col)) ? -1 : 1;
  int boss_idx = do_launch_alien(0, col, dir);
  int fidx[2] = { -1, -1 };
  int follow = 0;

  if (boss_idx < 0) return (false);

  for (int fcol = col - 1; fcol <= (col + 1); fcol ++) {
    if (convoy[1][fcol] == RED_ALIEN) {
      fidx[follow] = do_launch_alien(1, fcol, dir);
      if (fidx[follow] < 0) break;
      enemy[fidx[follow]]->boss_idx = boss_idx;
      follow ++;
      if (follow == 2) break;
    }
  }
  enemy[boss_idx]->boss_idx = 2;
  if (follow == 2) {
    enemy[boss_idx]->score = 30;
  } else if (follow == 1) {
    enemy[boss_idx]->score = 20;
  }

  return (true);
}

//=============================================================================
bool Convoy::launch_boss()
{
  if (boss_in_scr == 0 && boss_attack > 0) {
    boss_attack --;
    if (boss_attack == 0) {
      boss_attack = 4;
      if (random(2) == 0) {
	for (int col = 0; col < CONVOY_COLS; col ++) {
	  if (convoy[0][col] == BOSS_ALIEN) {
	    return (do_launch_boss(col));
	  }
	}
      } else {
	for (int col = (CONVOY_COLS - 1); col >= 0; col --) {
	  if (convoy[0][col] == BOSS_ALIEN) {
	    return (do_launch_boss(col));
	  }
	}
      }
    }
  }
  return (false);
}

//=============================================================================
void Convoy::launch_zako()
{
  int col, dir;

  int rnd = random(8);
  if ((rnd & 6) == 0) {
    int row = 2; // PURPLE
    if ((rnd & 1) == 0) {
      for (col = 0; col < CONVOY_COLS; col ++) {
	if (convoy[row][col] >= 0) {
	  do_launch_alien(row, col, -1);
	  return;
	}
      }
    } else {
      for (col = (CONVOY_COLS - 1); col >= 0; col --) {
	if (convoy[row][col] >= 0) {
	  do_launch_alien(row, col, -1);
	  return;
	}
      }
    }
  } else {
    if ((rnd & 1) == 0) {
      col = left;
      dir = -1;
    } else {
      col = right;
      dir = 1;
    }
    for (int row = CONVOY_ROWS - 1; row >= 0; row --) {
      if (convoy[row][col] >= BOSS_ALIEN && convoy[row][col] <= GREEN_ALIEN) {
	do_launch_alien(row, col, dir);
	return;
      }
    }
  }
}

//=============================================================================
void Convoy::fire_missile(int _x, int _y, float _vx)
{
  for (int idx = 0; idx < MAX_MISSILES; idx ++) {
    if (missile[idx].pos.get_y() == 0) {
      missile[idx].init(_x + 2, _y, _vx);
      return;
    }
  }
}

//=============================================================================
void Convoy::do_state(int frame, bool attack, int ship_x, int ship_y)
{
  int chk_x;

  // ---------------------------------------------------------- Move Convoy ---
  if ((frame & 15) == 0 && random(8) > 0) {
    if (dir < 0) {
      chk_x = x + left * 6 - 1;
      if (chk_x < 6) {
	dir = 1;
      }
    } else {
      chk_x = x + right * 6 + 6 + 1;
      if (chk_x > (SCREEN_W - 5)) {
	dir = -1;
      }
    }
    x += dir;
  }

  // -------------------------------------------------------- Move Missiles ---
  for (int idx = 0; idx < MAX_MISSILES; idx ++) {
    missile[idx].do_state(frame);
  }

  // ------------------------------------------------- Move Launched Aliens ---
  bool atk_flag = ((dont_attack == 0) && attack);
  int freq = 0;
  for (int idx = 0; idx < MAX_ENEMIES; idx ++) {
    if (enemy[idx] != NULL) {
      Enemy *boss = NULL;
      if (enemy[idx]->type == RED_ALIEN && enemy[idx]->boss_idx >= 0) {
	boss = enemy[enemy[idx]->boss_idx];
      }
      enemy[idx]->do_state(frame, ship_x, ship_y, boss, atk_flag);

      if (enemy[idx]->freq > freq) {
	freq = enemy[idx]->freq;
      }

      if (atk_flag && enemy[idx]->fire_missile()) {
	fire_missile(enemy[idx]->pos.get_x(), enemy[idx]->pos.get_y(),
		     enemy[idx]->vx / 4);
      }

      if (enemy[idx]->state == ENEMY_ST_NONE) {
	convoy[enemy[idx]->row][enemy[idx]->col] = enemy[idx]->type;

	remove_enemy(idx);
      }
    }
  }
  if (freq != 0) {
    freq += (frame & 3) * 30;
//    if (mArduboy.audio.enabled() && !mArduboy.tunes.playing()) {
    if (mArduboy.audio.enabled() && !sound.playing()) {
      if (freq>700) sound.tone(freq-300, 5);
    }
  }

  // ------------------------------------------------- Check Alien Launcing ---
  if (atk_flag && zako_in_scr < max_zako_in_scr && (frame & 15) == 0 && random(8) == 0) {
    if (!launch_boss()) {
      launch_zako();
    }
  }

  // ----------------------------------------------------- Explosion Sprite ---
  if (exp != NULL && (frame & 3) == 0) {
    exp->pat ++;
    if (exp->pat >= 4) {
      delete exp;
      exp = NULL;
    }
  }

  // ---------------------------------------------------- Boss Score Sprite ---
  if (bscore != NULL) {
    bscore->counter --;
    if (bscore->counter < 0) {
      delete bscore;
      bscore = NULL;
    }
  }

  if (dont_attack > 0) dont_attack --;
}

//=============================================================================
void Convoy::draw(int frame)
{
  int dx, dy, anim;

  for (int row = 0; row < CONVOY_ROWS; row ++) {
    dy = y + row * 5;
    for (int col = left; col <= right; col ++) {
      dx = x + col * 6 + SCREEN_X;
      switch (convoy[row][col]) {
      case BOSS_ALIEN:
	mArduboy.drawBitmap(dx, dy, enemyboss_bmp[0], 5, 5, WHITE);
	break;
      case RED_ALIEN:
      case PURPLE_ALIEN:
      case GREEN_ALIEN:
	anim = ((frame >> 4) + col) & 3;
	if (anim == 2) {
	  anim = 0;
	} else if (anim == 3) {
	  anim = 2;
	}
	mArduboy.drawBitmap(dx, dy, enemyzako_bmp[anim], 5, 5, WHITE);
	break;
      }
    }
  }

  // draw launched alien
  for (int idx = 0; idx < MAX_ENEMIES; idx ++) {
    if (enemy[idx] != NULL) {
      enemy[idx]->draw(frame);
    }
  }

  for (int idx = 0; idx < MAX_MISSILES; idx ++) {
    missile[idx].draw();
  }

  if (exp != NULL) {
    exp->draw();
  }

  if (bscore != NULL) {
    bscore->draw();
  }
}

//================================ END-OF-FILE ================================
