class Ship
{
  private:
    int x = SHIP_DEFAULT_X;
    int y = SHIP_DEFAULT_Y;

    void alternateengine()
    {
      if (arduboy.everyXFrames(10))
        shipstatus ^= 0b00010000;
    }

    bool enginealternatestatus()
    {
      return (shipstatus & 0b00010000) == 0b00010000;
    }

    byte getimmunitycountdown()
    {
      return (shipstatus_2 & 0b01111111);
    }

    void setimmunitycountdown(byte immunitycountdown)
    {
      if (immunitycountdown > 127)
        immunitycountdown = 127;

      shipstatus_2 = (shipstatus_2 & 0b10000000) | immunitycountdown;
    }

    // 2 type, 1 immune, 1 engine, 2 bombs, 2 lives
    byte shipstatus = INITIAL_SHIPSTATUS;
    // 1 shield active, 7 immunitycountdown
    byte shipstatus_2 = INITIAL_SHIPSTATUS_2;
  public:
    byte getdrawX()
    {
      return x / 10;
    }

    byte getdrawY()
    {
      return y / 10;
    }

    void setposition(byte _x, byte _y)
    {
      x = _x * 10;
      y = _y * 10;
    }

    void reset()
    {
      x = SHIP_DEFAULT_X;
      y = SHIP_DEFAULT_Y;
      shipstatus = INITIAL_SHIPSTATUS;
      shipstatus_2 = INITIAL_SHIPSTATUS_2;
    }

    byte getlives()
    {
      return (shipstatus & 0b00000011);
    }

    void inclives()
    {
      byte lives = getlives();
      if (lives < SHIP_MAX_LIVES)
      {
        lives++;
        shipstatus = (shipstatus & 0b11111100) | lives;
      }
    }

    bool declives()
    {
      if (shieldactive())
      {
        deactivateshield();
        return true;
      }
      else
      {
        byte lives = getlives();
        if (lives > 0)
        {
          lives--;
          shipstatus = (shipstatus & 0b11111100) | lives;
          return true;
        }
      }

      return false;
    }

    byte getbombs()
    {
      return (shipstatus & 0b00001100) >> 2;
    }

    void incbombs()
    {
      byte bombs = getbombs();
      if (bombs < SHIP_MAX_BOMBS)
      {
        bombs++;
        shipstatus = (shipstatus & 0b11110011) | (byte)(bombs << 2);
      }
    }

    bool decbombs()
    {
      byte bombs = getbombs();
      if (bombs > 0)
      {
        bombs--;
        shipstatus = (shipstatus & 0b11110011) | (byte)(bombs << 2);
        return true;
      } else
        return false;
    }

    byte shieldactive()
    {
      return (shipstatus_2 & 0b10000000) == 0b10000000;
    }

    void activateshield()
    {
      shipstatus_2 |= 0b10000000;
    }

    void deactivateshield()
    {
      shipstatus_2 &= 0b01111111;
    }

    byte getshiptype()
    {
      return shipstatus >> 6;
    }

    void setshiptype(byte type)
    {
      shipstatus &= 0b00111111;
      shipstatus |= (byte)(type << 6);
    }

    bool immune()
    {
      return (shipstatus & 0b00100000) == 0b00100000;
    }

    void setimmune(bool immune, byte _immunitycountdown)
    {
      if (immune)
        shipstatus |= 0b00100000;
      else
        shipstatus = shipstatus & 0b11011111;

      setimmunitycountdown(_immunitycountdown);
    }

    void moveship(byte mdirection)
    {
      byte step = 10;
      switch (getshiptype())
      {
        case SHIP_THOR:
          step = 6;
          break;
        case SHIP_FREYR:
          step = 14;
          break;
      }

      if (mdirection == MOVE_UP)
      {
        if (y > MIN_SHIP_Y)
        {
          if ( y - step >= MIN_SHIP_Y)
            y -= step;
          else
            y = MIN_SHIP_Y;
        }
      }
      else if (mdirection == MOVE_DOWN)
        y = min(y + step, MAX_SHIP_Y);
      else if (mdirection == MOVE_LEFT)
      {
        if (x > MIN_SHIP_X)
        {
          if (x >= step)
            x -= step;
          else
            x = 0;
        }
      }
      else if (mdirection == MOVE_RIGHT)
        x = min(x + step, MAX_SHIP_X);
    }

    void update()
    {
      byte immunitycountdown = getimmunitycountdown();
      if (immunitycountdown > 0)
        setimmunitycountdown(immunitycountdown - 1);
      else
      {
        if (immune())
          setimmune(false, 0);
      }
      alternateengine();
    }

    Rect gethitbox()
    {
      Rect rect;
      rect.x = getdrawX() + 3;
      rect.y = getdrawY() + 2;
      rect.width = 4;
      rect.height = 4;
      return rect;
    }

    virtual void draw()
    {
      if (!immune() || arduboy.everyXFrames(4))
      {
#ifdef DRAW_ONLY_HITBOX
        Rect hitbox = gethitbox();
        arduboy.drawRect(hitbox.x, hitbox.y, hitbox.width, hitbox.height, 1);
#else
        switch (getshiptype())
        {
          case SHIP_ODINO:
            if (enginealternatestatus())
              arduboy.drawBitmap(getdrawX(), getdrawY(), ship_odino_1, 8, 8);
            else
              arduboy.drawBitmap(getdrawX(), getdrawY(), ship_odino_2, 8, 8);
            break;
          case SHIP_THOR:
            if (enginealternatestatus())
              arduboy.drawBitmap(getdrawX(), getdrawY(), ship_thor_1, 8, 8);
            else
              arduboy.drawBitmap(getdrawX(), getdrawY(), ship_thor_2, 8, 8);
            break;
          case SHIP_FREYR:
            if (enginealternatestatus())
              arduboy.drawBitmap(getdrawX(), getdrawY(), ship_freyr_1, 8, 8);
            else
              arduboy.drawBitmap(getdrawX(), getdrawY(), ship_freyr_2, 8, 8);
            break;
        }
#endif
        if (shieldactive())
          arduboy.drawCircle(getdrawX() + 4, getdrawY() + 4, 8, 1);
      }
    }

    byte getmainweapon()
    {
      switch (getshiptype())
      {
        case SHIP_ODINO:
          return GUN;
        case SHIP_THOR:
          return SMALL_BOMB;
        case SHIP_FREYR:
          return ALTERNATE_GUN;
      }

      return GUN;
    }

    void getweaponavailable(byte& mainweapon, byte& secondaryweapon)
    {
      switch (getshiptype())
      {
        case SHIP_ODINO:
          mainweapon = getmainweapon();
          secondaryweapon = ALTERNATE_UPDOWN_GUN;
          break;
        case SHIP_THOR:
          mainweapon = getmainweapon();
          secondaryweapon = LASER;
          break;
        case SHIP_FREYR:
          mainweapon = getmainweapon();
          secondaryweapon = SPLIT_GUN;
          break;
      }
    }
};
