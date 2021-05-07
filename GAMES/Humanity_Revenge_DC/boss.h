class BossPart
{
  public:
    byte x = 0;
    byte y = 0;
    byte life = 0;
    // 1 visible, 1 canshoot, 1 main, 5 boss part
    byte partstatus = INITIAL_BOSS_PART_STATUS;

    bool canshoot()
    {
      return (partstatus & 0b01000000) == 0b01000000;
    }

    bool main()
    {
      return (partstatus & 0b00100000) == 0b00100000;
    }

    void reset()
    {
      x = 0;
      y = 0;
      life = 0;
      partstatus = INITIAL_BOSS_PART_STATUS;
    }

    bool visible()
    {
      return (partstatus & 0b10000000) == 0b10000000;
    }

    byte bosspart()
    {
      return (partstatus & 0b00011111);
    }

    byte getscore()
    {
      switch (bosspart())
      {
        case BOSS_WINGS_UP:
        case BOSS_WINGS_DOWN:
          return BOSS_WINGS_PART_SCORE;
        case BOSS_SWORDS_UP:
        case BOSS_SWORDS_DOWN:
        case BOSS_SWORDS_MAIN:
          return BOSS_SWORDS_PART_SCORE;
        case BOSS_TYSON_MAIN:
        case BOSS_TYSON_UP_FRONT:
        case BOSS_TYSON_UP_BACK:
        case BOSS_TYSON_DOWN_FRONT:
        case BOSS_TYSON_DOWN_BACK:
          return BOSS_TYSON_PART_SCORE;
      }

      return 0;
    }

    bool decreaselife(byte shootdamage)
    {
      if (life > shootdamage)
        life -= shootdamage;
      else
      {
        reset();
        return true;
      }

      return false;
    }

    void init(byte _x, byte _y, byte _bosspart, bool _canshoot, bool _main)
    {
      x = _x;
      y = _y;
      partstatus = partstatus | 0b10000000 | _bosspart;
      if (_canshoot)
        partstatus |= 0b01000000;
      if (_main)
        partstatus |= 0b00100000;

      switch (bosspart())
      {
        case BOSS_WINGS_UP:
        case BOSS_WINGS_DOWN:
          life = BOSS_WINGS_PART_LIFE;
          break;
        case BOSS_SWORDS_UP:
        case BOSS_SWORDS_DOWN:
        case BOSS_SWORDS_MAIN:
          life = BOSS_SWORDS_PART_LIFE;
          break;
        case BOSS_TYSON_MAIN:
        case BOSS_TYSON_UP_FRONT:
        case BOSS_TYSON_UP_BACK:
        case BOSS_TYSON_DOWN_FRONT:
        case BOSS_TYSON_DOWN_BACK:
          life = BOSS_TYSON_PART_LIFE;
          break;
      }
    }

    Rect gethitbox()
    {
      Rect rect;
      rect.x = x;
      rect.y = y;
      rect.width = 8;
      rect.height = 8;
      return rect;
    }

    void draw()
    {
      if (visible())
      {
#ifdef DRAW_ONLY_HITBOX
        Rect hitbox = gethitbox();
        arduboy.drawRect(hitbox.x, hitbox.y, hitbox.width, hitbox.height, 1);
#else
        switch (bosspart())
        {
          case BOSS_WINGS_UP:
            arduboy.drawBitmap(x, y, boss_wings_up, 8, 8, 1);
            break;
          case BOSS_WINGS_DOWN:
            arduboy.drawBitmap(x, y, boss_wings_down, 8, 8, 1);
            break;
          case BOSS_SWORDS_MAIN:
            arduboy.drawBitmap(x, y, boss_swords_main, 8, 8, 1);
            break;
          case BOSS_SWORDS_UP:
            arduboy.drawBitmap(x, y, boss_swords_up, 8, 8, 1);
            break;
          case BOSS_SWORDS_DOWN:
            arduboy.drawBitmap(x, y, boss_swords_down, 8, 8, 1);
            break;
          case BOSS_TYSON_MAIN:
            arduboy.drawBitmap(x, y, boss_tyson_main, 8, 8, 1);
            break;
          case BOSS_TYSON_UP_BACK:
            arduboy.drawBitmap(x, y, boss_tyson_up_back, 8, 8, 1);
            break;
          case BOSS_TYSON_DOWN_BACK:
            arduboy.drawBitmap(x, y, boss_tyson_down_back, 8, 8, 1);
            break;
          case BOSS_TYSON_UP_FRONT:
            arduboy.drawBitmap(x, y, boss_tyson_up_front, 8, 8, 1);
            break;
          case BOSS_TYSON_DOWN_FRONT:
            arduboy.drawBitmap(x, y, boss_tyson_down_front, 8, 8, 1);
            break;

        }
#endif
      }
    }
};

class Boss
{
  private:
    byte x = 0;
    byte y = 0;
    // 1 visible, 1 alternate, 1 alternate_2, 2 movementtype, 3 bosstype
    byte bossstatus = 0b00000000;
  public:
    BossPart bossparts[MAX_BOSS_PARTS];

    bool visible()
    {
      return (bossstatus & 0b10000000) == 0b10000000;
    }

    void alternate()
    {
      bossstatus ^= 0b01000000;
    }

    bool alternatestatus()
    {
      return (bossstatus & 0b01000000) == 0b01000000;
    }

    void alternate_2()
    {
      bossstatus ^= 0b00100000;
    }

    bool alternatestatus_2()
    {
      return (bossstatus & 0b00100000) == 0b00100000;
    }

    byte movementtype()
    {
      return (bossstatus & 0b00011000) >> 3;
    }

    void setmovementtype(byte movementtype)
    {
      bossstatus = (bossstatus & 0b11100111) | (byte)(movementtype << 3);
    }

    byte bosstype()
    {
      return (bossstatus & 0b00000111);
    }

    void reset()
    {
      bossstatus = INITIAL_BOSS_STATUS;
      for (byte i = 0; i < MAX_BOSS_PARTS; i++)
        bossparts[i].reset();
    }

    bool partsvisible()
    {
      if (visible())
        for (byte i = 0; i < MAX_BOSS_PARTS; i++)
          if (bossparts[i].visible())
            return true;

      return false;
    }

    void init(byte _bosstype)
    {
      bossstatus = 0b10000000 | _bosstype;

      switch (bosstype())
      {
        case BOSS_WINGS:
          x = BOSS_WINGS_START_X;
          y = BOSS_WINGS_START_Y;
          bossparts[0].init(x, y, BOSS_WINGS_UP, true, false);
          bossparts[1].init(x, y + 8, BOSS_WINGS_DOWN, true, false);
          break;
        case BOSS_SWORDS:
          x = BOSS_SWORDS_START_X;
          y = BOSS_SWORDS_START_Y;
          bossparts[0].init(x + 8, y + 5, BOSS_SWORDS_MAIN, true, true);
          bossparts[1].init(x, y, BOSS_SWORDS_UP, true, false);
          bossparts[2].init(x, y + 10, BOSS_SWORDS_DOWN, true, false);
          break;
        case BOSS_TYSON:
          x = BOSS_TYSON_START_X;
          y = BOSS_TYSON_START_Y;
          bossparts[0].init(x + 8, y + 8, BOSS_TYSON_MAIN, true, true);
          bossparts[1].init(x + 8, y, BOSS_TYSON_UP_BACK, true, false);
          bossparts[2].init(x + 8, y + 16, BOSS_TYSON_DOWN_BACK, true, false);
          bossparts[3].init(x, y + 3, BOSS_TYSON_UP_FRONT, true, false);
          bossparts[4].init(x, y + 13, BOSS_TYSON_DOWN_FRONT, true, false);
          break;
      }

      setmovementtype(BOSS_MOVEMENT_UPDOWN);
    }

    void draw()
    {
      if (visible())
        for (byte i = 0; i < MAX_BOSS_PARTS; i++)
          bossparts[i].draw();
    }

    void update()
    {
      if (visible())
      {
        switch (bosstype())
        {
          case BOSS_WINGS:
            {
              switch (movementtype())
              {
                case BOSS_MOVEMENT_UPDOWN:
                  if (!bossparts[0].visible())
                    y = bossparts[1].y;

                  if (!alternatestatus())
                  {
                    y--;
                    if (bossparts[0].visible()) bossparts[0].y--;
                    if (bossparts[1].visible()) bossparts[1].y--;
                    if ((y <= MIN_SCREENHUD_Y / 10) || random(0, 51) == 0)
                      alternate();
                  } else
                  {
                    y++;
                    if (bossparts[0].visible()) bossparts[0].y++;
                    if (bossparts[1].visible()) bossparts[1].y++;
                    if ((y + (bossparts[0].visible() ? 8 : 0) + (bossparts[1].visible() ? 8 : 0) >= MAX_SCREEN_Y / 10 + 1) || random(0, 51) == 0)
                      alternate();
                  }
                  break;
              }
              break;
            }
          case BOSS_SWORDS:
            // 0 main , 1 up, 2 down
            if (!bossparts[1].visible())
              y = bossparts[0].y;

            if (!bossparts[1].visible() && !bossparts[2].visible())
              x = bossparts[0].x;

            switch (movementtype())
            {
              case BOSS_MOVEMENT_UPDOWN:
                if (!alternatestatus())
                {
                  y--;
                  if (bossparts[0].visible()) bossparts[0].y--;
                  if (bossparts[1].visible()) bossparts[1].y--;
                  if (bossparts[2].visible()) bossparts[2].y--;
                  if ((y <= MIN_SCREENHUD_Y / 10) || random(0, 51) == 0)
                    alternate();
                } else
                {
                  y++;
                  if (bossparts[0].visible()) bossparts[0].y++;
                  if (bossparts[1].visible()) bossparts[1].y++;
                  if (bossparts[2].visible()) bossparts[2].y++;

                  if (bossparts[1].visible() && bossparts[2].visible())
                  {
                    if ((y + 18 >= MAX_SCREEN_Y / 10 + 1) || random(0, 51) == 0)
                      alternate();
                  } else if  ((bossparts[1].visible() && !bossparts[2].visible()) || (!bossparts[1].visible() && bossparts[2].visible()))
                  {
                    if ((y + 13 >= MAX_SCREEN_Y / 10 + 1) || random(0, 51) == 0)
                      alternate();
                  } else if  (!bossparts[1].visible() && !bossparts[2].visible())
                  {
                    if ((y + 8 >= MAX_SCREEN_Y / 10 + 1) || random(0, 51) == 0)
                      alternate();
                  }
                }

                if (random(0, 300) == 0)
                  setmovementtype(BOSS_MOVEMENT_LEFTRIGHT);
                break;
              case BOSS_MOVEMENT_LEFTRIGHT:
                if (!alternatestatus())
                {
                  if (x >= 3)
                  {
                    x -= 3;
                    if (bossparts[0].visible()) bossparts[0].x -= 3;
                    if (bossparts[1].visible()) bossparts[1].x -= 3;
                    if (bossparts[2].visible()) bossparts[2].x -= 3;
                  }
                  else
                    alternate();
                } else
                {
                  x++;
                  if (bossparts[0].visible()) bossparts[0].x++;
                  if (bossparts[1].visible()) bossparts[1].x++;
                  if (bossparts[2].visible()) bossparts[2].x++;
                  if (x >= BOSS_SWORDS_START_X + (!bossparts[1].visible() && !bossparts[2].visible()) ? 8 : 0)
                  {
                    alternate();
                    if (random(0, 2) == 0)
                      setmovementtype(BOSS_MOVEMENT_UPDOWN);
                  }
                }
                break;
            }
            break;
          case BOSS_TYSON:
            // 0 main , 1 up back, 2 down back, 3 up front, 4 down front
            if (!bossparts[1].visible() && !bossparts[3].visible())
              y = bossparts[0].y;

            if (!bossparts[3].visible() && !bossparts[4].visible())
              x = bossparts[0].x;

            switch (movementtype())
            {
              case BOSS_MOVEMENT_UPDOWN:
                if (!alternatestatus())
                {
                  y--;
                  if (bossparts[0].visible()) bossparts[0].y--;
                  if (bossparts[1].visible()) bossparts[1].y--;
                  if (bossparts[2].visible()) bossparts[2].y--;
                  if (bossparts[3].visible()) bossparts[3].y--;
                  if (bossparts[4].visible()) bossparts[4].y--;
                  if ((y <= MIN_SCREENHUD_Y / 10) || random(0, 51) == 0)
                    alternate();
                } else
                {
                  y++;
                  if (bossparts[0].visible()) bossparts[0].y++;
                  if (bossparts[1].visible()) bossparts[1].y++;
                  if (bossparts[2].visible()) bossparts[2].y++;
                  if (bossparts[3].visible()) bossparts[3].y++;
                  if (bossparts[4].visible()) bossparts[4].y++;
                  if ((y + 8 + (bossparts[1].visible() || bossparts[3].visible() ? 8 : 0)  + (bossparts[2].visible() || bossparts[4].visible() ? 8 : 0) >= MAX_SCREEN_Y / 10 + 1) || random(0, 51) == 0)
                    alternate();
                }

                if (random(0, 300) == 0)
                  setmovementtype(BOSS_MOVEMENT_LEFTRIGHT);
                else if (random(0, 300) == 0)
                {
                  setmovementtype(BOSS_MOVEMENT_LEFTRIGHT_V);
                  if (random(0, 2) == 0)
                    alternate_2();
                }
                break;
              case BOSS_MOVEMENT_LEFTRIGHT:
                if (!alternatestatus())
                {
                  if (x >= 3)
                  {
                    x -= 3;
                    if (bossparts[0].visible()) bossparts[0].x -= 3;
                    if (bossparts[1].visible()) bossparts[1].x -= 3;
                    if (bossparts[2].visible()) bossparts[2].x -= 3;
                    if (bossparts[3].visible()) bossparts[3].x -= 3;
                    if (bossparts[4].visible()) bossparts[4].x -= 3;
                  }
                  else
                    alternate();
                } else
                {
                  x++;
                  if (bossparts[0].visible()) bossparts[0].x++;
                  if (bossparts[1].visible()) bossparts[1].x++;
                  if (bossparts[2].visible()) bossparts[2].x++;
                  if (bossparts[3].visible()) bossparts[3].x++;
                  if (bossparts[4].visible()) bossparts[4].x++;
                  if (x >= BOSS_SWORDS_START_X + (!bossparts[3].visible() && !bossparts[4].visible()) ? 8 : 0)
                  {
                    alternate();
                    setmovementtype(BOSS_MOVEMENT_UPDOWN);
                  }
                }
                break;
              case BOSS_MOVEMENT_LEFTRIGHT_V:
                if (!alternatestatus())
                {
                  if (x >= 3)
                  {
                    x -= 3;
                    if (bossparts[0].visible()) bossparts[0].x -= 3;
                    if (bossparts[1].visible()) bossparts[1].x -= 3;
                    if (bossparts[2].visible()) bossparts[2].x -= 3;
                    if (bossparts[3].visible()) bossparts[3].x -= 3;
                    if (bossparts[4].visible()) bossparts[4].x -= 3;

                    if (alternatestatus_2())
                    {
                      if (y > MIN_SCREENHUD_Y / 10)
                      {
                        y -= 1;
                        if (bossparts[0].visible()) bossparts[0].y -= 1;
                        if (bossparts[1].visible()) bossparts[1].y -= 1;
                        if (bossparts[2].visible()) bossparts[2].y -= 1;
                        if (bossparts[3].visible()) bossparts[3].y -= 1;
                        if (bossparts[4].visible()) bossparts[4].y -= 1;
                      }
                    } else
                    {
                      if (y + 8 + (bossparts[1].visible() || bossparts[3].visible() ? 8 : 0)  + (bossparts[2].visible() || bossparts[4].visible() ? 8 : 0) >= MAX_SCREEN_Y / 10 + 1)
                      {
                        y += 1;
                        if (bossparts[0].visible()) bossparts[0].y += 1;
                        if (bossparts[1].visible()) bossparts[1].y += 1;
                        if (bossparts[2].visible()) bossparts[2].y += 1;
                        if (bossparts[3].visible()) bossparts[3].y += 1;
                        if (bossparts[4].visible()) bossparts[4].y += 1;
                      }
                    }
                  }
                  else
                    alternate();
                } else
                {
                  x++;
                  if (bossparts[0].visible()) bossparts[0].x++;
                  if (bossparts[1].visible()) bossparts[1].x++;
                  if (bossparts[2].visible()) bossparts[2].x++;
                  if (bossparts[3].visible()) bossparts[3].x++;
                  if (bossparts[4].visible()) bossparts[4].x++;
                  if (x >= BOSS_SWORDS_START_X + (!bossparts[3].visible() && !bossparts[4].visible()) ? 8 : 0)
                  {
                    alternate();
                    setmovementtype(BOSS_MOVEMENT_UPDOWN);
                  }
                }
                break;
            }
            break;
        }
      }
    }
};

