class EnemyShoot
{
  private:
    int x = 0, y = 0;
    byte angle = 0;
    // 6, 1 positive, 1 visible
    byte shootstatus = 0b00000000;
  public:
    void init(byte _x, byte _y, byte _angle, byte _positive)
    {
      x = _x * 10;
      y = _y * 10;
      angle = _angle;
      shootstatus = (byte)(_positive << 1) | 0b00000001;
    }

    void reset()
    {
      x = 0;
      y = 0;
      angle = 0;
      shootstatus = INITIAL_SHOOT_STATUS;
    }

    byte positive()
    {
      return (shootstatus & 0b00000010) == 0b00000010;
    }

    byte visible()
    {
      return (shootstatus & 0b00000001) == 0b00000001;
    }

    byte getdrawX()
    {
      return x / 10;
    }

    byte getdrawY()
    {
      return y / 10;
    }

    void update()
    {
      if (visible())
      {
        x -= 15 * cos(angle * pi / 180);
        if (getdrawX() <= 0)
          reset();
        else
        {
          if (positive())
          {
            y -= 15 * sin(angle * pi / 180);
            if (getdrawY() <= 0)
              reset();
          }
          else
          {
            y += 15 * sin(angle * pi / 180);
            if (getdrawY() > 64)
              reset();
          }
        }
      }
    }

    void draw()
    {
      if (visible())
      {
#ifdef DRAW_ONLY_HITBOX
        Point hitbox = gethitbox();
        arduboy.drawRect(hitbox.x, hitbox.y, 1, 1, 1);
#else
        arduboy.drawLine(getdrawX() - 1, getdrawY(), getdrawX() + 1, getdrawY() , 1);
        arduboy.drawLine(getdrawX(), getdrawY() + 1, getdrawX(), getdrawY() - 1, 1);
#endif
      }
    }

    Point gethitbox()
    {
      Point point;
      point.x = getdrawX();
      point.y = getdrawY();
      return point;
    }
};

class Enemy
{
  private:
    // 2 life ( 0 + 1 / 3 + 1), 1 visible, 1 enemy class, 2 enemy type, 2 movtype
    byte enemystatus = INITIAL_ENEMY_STATUS;
    byte firsty = 0;
    byte randomseed = 0;
  public:
    byte x = 0;
    byte y = 0;
    
    void reset()
    {
      enemystatus = INITIAL_ENEMY_STATUS;
      x = 0;
      y = 0;
      firsty = 0;
      randomseed = 0;
    }

    bool visible()
    {
      return ((enemystatus & 0b00100000) == 0b00100000);
    }

    byte enemyclass()
    {
      return (byte)(enemystatus << 3) >> 7;
    }

    byte enemytype()
    {
      return (byte)(enemystatus << 4) >> 6;
    }

    byte movementtype()
    {
      return (byte)(enemystatus << 6) >> 6;
    }

    byte getlife()
    {
      return (enemystatus >> 6) + 1;
    }

    void setlife(byte life)
    {
      enemystatus &= 0b00111111;
      enemystatus |= (byte)(life << 6);
    }

    bool decreaselife(byte damage)
    {
      if (getlife() > damage)
        setlife(getlife() - 1 - damage);
      else
      {
        reset();
        return true;
      }

      return false;
    }

    void init(byte _x, byte _y, byte enemyclass, byte enemytype, byte movementtype)
    {
      x = _x;
      y = _y;
      firsty = _y;
      randomseed = random(0, 100);
      byte life = EASY_ENEMY_LIFE;
      if (enemyclass == MEDIUM_ENEMY)
        life = MEDIUM_ENEMY_LIFE;
      enemystatus = 0b00100000 | (byte)(life << 6) | (byte)(enemyclass << 4) | (byte)(enemytype << 2) | (byte)(movementtype);
    }

    Rect gethitbox()
    {
      switch (enemyclass())
      {
        case EASY_ENEMY:
          {
            Rect rect;
            rect.x = x + 2;
            rect.y = y + 1;
            rect.width = 4;
            rect.height = 6;
            return rect;
          }
        case MEDIUM_ENEMY:
        default:
          {
            Rect rect;
            rect.x = x;
            rect.y = y;
            rect.width = 8;
            rect.height = 8;
            return rect;
          }
      }
    }

    void update(byte shipx, byte shipy)
    {
      if (visible())
        switch (movementtype())
        {
          case MOVEMENT_TYPE_SIMPLE:
            x -= 1;
            if (x == MIN_SCREEN_X)
              reset();
            break;
          case MOVEMENT_TYPE_SINUSOIDE:
            x -= 1;
            if (x == MIN_SCREEN_X)
              reset();
            else
            {
              y = (6 * sin((arduboy.frameCount + randomseed) * 0.5 * pi / 33)) + firsty;
              if (y < MIN_SCREENHUD_Y / 10)
                y = MIN_SCREENHUD_Y / 10;
              if (y > MAX_SCREEN_Y / 10 - 7)
                y =  MAX_SCREEN_Y / 10 - 7;
            }
            break;
          case MOVEMENT_TYPE_KAMIKAZE:
            if (x >= 2)
            {
              x -= 2;
              if (shipx < x)
              {
                if (shipy > y + 1)
                  y++;
                else
                  y--;
              }
            } else
              reset();
            break;
          case MOVEMENT_TYPE_MISSILE:
            if (y > 0)
              y -= 1;
            else
              reset();
            break;
        }
    }

    byte getscore()
    {
      byte enemyscore = EASY_ENEMY_SCORE;
      if (enemyclass() == MEDIUM_ENEMY)
        enemyscore = MEDIUM_ENEMY_SCORE;

      return enemyscore;
    }

    byte getexplosionstep()
    {
      switch (enemyclass())
      {
        case EASY_ENEMY:
            return EXPLOSION_SMALL;
        case MEDIUM_ENEMY:
            return EXPLOSION_MEDIUM;
      }

      return EXPLOSION_SMALL;
    }

    void draw()
    {
      if (visible())
      {
#ifdef DRAW_ONLY_HITBOX
        Rect hitbox = gethitbox();
        arduboy.drawRect(hitbox.x, hitbox.y, hitbox.width, hitbox.height, 1);
#else
        if (enemyclass() == EASY_ENEMY)
        {
          if (enemytype() == EASY_TYPE_SIMPLE)
            arduboy.drawBitmap(x, y, easy_enemy_simple, 8, 8, 1);
          else if (enemytype() == EASY_TYPE_SINUSOIDE)
            arduboy.drawBitmap(x, y, easy_enemy_sinusoide, 8, 8, 1);
          else if (enemytype() == EASY_TYPE_KAMIKAZE)
            arduboy.drawBitmap(x, y, easy_enemy_kamikaze, 8, 8, 1);
          else if (enemytype() == EASY_TYPE_MISSILE)
          {
            if (arduboy.everyXFrames(2))
              arduboy.drawBitmap(x, y, easy_enemy_missile_1, 8, 8, 1);
            else
              arduboy.drawBitmap(x, y, easy_enemy_missile_2, 8, 8, 1);
          }
        } else if (enemyclass() == MEDIUM_ENEMY)
          arduboy.drawBitmap(x, y, medium_enemy_simple, 8, 8, 1);
#endif
      }
    }
};
