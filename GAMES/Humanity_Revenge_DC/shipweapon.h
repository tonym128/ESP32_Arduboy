class Shoot
{
  private:
    int x = 0;
    int y = 0;

    byte shoottype()
    {
      return (shootstatus & 0b01110000) >> 4;
    }
  public:
    // 1 visible, 3 shoottype, 4 damage
    byte shootstatus = INITIAL_SHOOTSTATUS;

    byte getdrawX()
    {
      return x / 10;
    }

    byte getdrawY()
    {
      return y / 10;
    }

    bool visible()
    {
      return (shootstatus & 0b10000000) == 0b10000000;
    }

    byte getdamage()
    {
      return (shootstatus & 0b00001111);
    }

    void decreasedamage(byte damage)
    {
      switch (shoottype())
      {
        case SHOOT_LASER:
          if (getdamage() <= damage)
            reset();
          else
            shootstatus -= damage;
          break;
        default:
          reset();
          break;
      }
    }

    void reset()
    {
      x = 0;
      y = 0;
      shootstatus = INITIAL_SHOOTSTATUS;
    }

    void init(byte _x, byte _y, byte shoottype, byte damage)
    {
      x = _x * 10;
      y = _y * 10;
      shootstatus = 0b10000000 | (byte)(shoottype << 4) | damage;
    }

    Rect gethitbox()
    {
      switch (shoottype())
      {
        case SHOOT_GUN:
        case SHOOT_GUN_UP:
        case SHOOT_GUN_DOWN:
          Rect gunrect;
          gunrect.x = getdrawX();
          gunrect.y = getdrawY();
          gunrect.width = GUN_SIZE;
          gunrect.height = 1;
          return gunrect;
        case SHOOT_LASER:
          Rect gunlaser;
          gunlaser.x = getdrawX();
          gunlaser.y = getdrawY();
          gunlaser.width = LASER_SIZE;
          gunlaser.height = 2;
          return gunlaser;
        case SHOOT_SMALL_BOMB:
          Rect gunsmallbomb;
          gunsmallbomb.x = getdrawX();
          gunsmallbomb.y = getdrawY();
          gunsmallbomb.width = SMALL_BOMB_SIZE;
          gunsmallbomb.height = 2;
          return gunsmallbomb;
      }

      Rect gundefault;
      gundefault.x = getdrawX();
      gundefault.y = getdrawY();
      gundefault.width = GUN_SIZE;
      gundefault.height = 1;
      return gundefault;
    }

    void draw()
    {
      if (visible())
      {
#ifdef DRAW_ONLY_HITBOX
        Rect hitbox = gethitbox();
        arduboy.drawRect(hitbox.x, hitbox.y, hitbox.width, hitbox.height, 1);
#else
        switch (shoottype())
        {
          case SHOOT_GUN:
          case SHOOT_GUN_UP:
          case SHOOT_GUN_DOWN:
            arduboy.drawFastHLine(getdrawX(), getdrawY(), GUN_SIZE, 1);
            break;
          case SHOOT_LASER:
            arduboy.drawFastHLine(getdrawX(), getdrawY(), LASER_SIZE, 1);
            arduboy.drawFastHLine(getdrawX(), getdrawY() + 1, LASER_SIZE, 1);
            break;
          case SHOOT_SMALL_BOMB:
            arduboy.drawPixel(getdrawX() + 1, getdrawY() - 1, 1);
            arduboy.drawFastHLine(getdrawX(), getdrawY(), SMALL_BOMB_SIZE, 1);
            arduboy.drawFastHLine(getdrawX(), getdrawY() + 1, SMALL_BOMB_SIZE, 1);
            arduboy.drawPixel(getdrawX() + 1, getdrawY() + 2, 1);
            break;
        }
#endif
      }
    }

    void update()
    {
      if (visible())
      {
        switch (shoottype())
        {
          case SHOOT_GUN:
          case SHOOT_LASER:
          case SHOOT_SMALL_BOMB:
            x += 20;
            if (x >= MAX_SHOOT_X)
              reset();
            break;
          case SHOOT_GUN_UP:
            x += 20;
            if (x >= MAX_SHOOT_X)
              reset();
            else
            {
              if (y >= MIN_SHOOT_Y + 2)
                y -= 2;
              else
                reset();
            }
            break;
          case SHOOT_GUN_DOWN:
            x += 20;
            if (x >= MAX_SHOOT_X)
              reset();
            else
            {
              y += 2;
              if (y > MAX_SHOOT_Y)
                reset();
            }
            break;
        }
      }
    }
};

class Weapon
{
  private:
    // 1 alternate, 3 weapon tipe, 4 canshoot
    byte weaponstatus = 0b00000000;

    byte getalternate()
    {
      return (weaponstatus & 0b10000000) == 0b10000000;
    }

    void setalternate()
    {
      weaponstatus ^= 0b10000000;
    }

    byte canshoot()
    {
      return (weaponstatus & 0b00001111);
    }

    void setcanshoot(byte canshoot)
    {
      weaponstatus = (weaponstatus & 0b11110000) | canshoot;
    }
  public:
    Shoot shootsl[MAX_SHOOT_NUMBER];

    byte getweapontype()
    {
      return ((byte)(weaponstatus << 1) >> 5);
    }

    void setweapontype(byte weapontype)
    {
      weaponstatus = (weaponstatus & 0b10001111) | (byte)(weapontype << 4);
    }

    bool shoot(byte x, byte y)
    {
      if (canshoot() == 0)
      {
        switch (getweapontype())
        {
          case GUN:
            {
              byte shootn = 0;
              for (byte i = 0; i < MAX_SHOOT_NUMBER; i++)
              {
                if (!shootsl[i].visible())
                {
                  shootsl[i].init(x + 8, y + (shootn == 1 ? 7 : 0), SHOOT_GUN, GUN_DAMAGE);
                  shootn++;
                }

                if (shootn == 2)
                  break;
              }

              setcanshoot(GUN_INTERVAL);
            }
            break;
          case ALTERNATE_UPDOWN_GUN:
            {
              byte shootn = 0;
              for (byte i = 0; i < MAX_SHOOT_NUMBER; i++)
                if (!shootsl[i].visible())
                {
                  if (shootn == 0)
                  {
                    if (!getalternate())
                      shootsl[i].init(x + 8, y, SHOOT_GUN, GUN_DAMAGE);
                    else
                      shootsl[i].init(x + 8, y, SHOOT_GUN_UP, GUN_DAMAGE);
                  } else if (shootn == 1)
                  {
                    if (!getalternate())
                      shootsl[i].init(x + 8, y + 7, SHOOT_GUN_DOWN, GUN_DAMAGE);
                    else
                      shootsl[i].init(x + 8, y + 7, SHOOT_GUN, GUN_DAMAGE);
                  }
                  shootn++;

                  if (shootn == 2)
                  {
                    setalternate();
                    break;
                  }
                }

              setcanshoot(GUN_INTERVAL);
            }
            break;
          case ALTERNATE_GUN:
            {
              for (byte i = 0; i < MAX_SHOOT_NUMBER; i++)
                if (!shootsl[i].visible())
                {
                  shootsl[i].init(x + 8, y + ((getalternate()) ? 6 : 1), SHOOT_GUN, GUN_DAMAGE);
                  setalternate();
                  break;
                }

              setcanshoot(ALTERNATE_GUN_INTERVAL);
            }
            break;
          case LASER:
            {
              for (byte i = 0; i < MAX_SHOOT_NUMBER; i++)
                if (!shootsl[i].visible())
                {
                  shootsl[i].init(x + 8, y + 3, SHOOT_LASER, LASER_DAMAGE);
                  break;
                }

              setcanshoot(LASER_INTERVAL);
            }
            break;
          case SMALL_BOMB:
            {
              for (byte i = 0; i < MAX_SHOOT_NUMBER; i++)
                if (!shootsl[i].visible())
                {
                  shootsl[i].init(x + 8, y + 3, SHOOT_SMALL_BOMB, SMALL_BOMB_DAMAGE);
                  break;
                }

              setcanshoot(SMALL_BOMB_INTERVAL);
            }
            break;
          case SPLIT_GUN:
            {
              byte shootn = 0;
              for (byte i = 0; i < MAX_SHOOT_NUMBER; i++)
                if (!shootsl[i].visible())
                {
                  if (shootn == 0)
                    shootsl[i].init(x + 8, y + 1, SHOOT_GUN_UP, GUN_DAMAGE);
                  else if (shootn == 1)
                    shootsl[i].init(x + 8, y + 6, SHOOT_GUN_DOWN, GUN_DAMAGE);
                  shootn++;

                  if (shootn == 2)
                    break;
                }

              setcanshoot(SPLIT_GUN_INTERVAL);
            }
            break;
        }

        return true;
      }

      return false;
    }

    void update()
    {
      if (canshoot() > 0)
        setcanshoot(canshoot() - 1);

      for (byte i = 0; i < MAX_SHOOT_NUMBER; i++)
        shootsl[i].update();
    }

    void draw()
    {
      for (byte i = 0; i < MAX_SHOOT_NUMBER; i++)
        shootsl[i].draw();
    }

    void reset()
    {
      setcanshoot(0);
      weaponstatus = INITIAL_WEAPONSTATUS;

      for (byte i = 0; i < MAX_SHOOT_NUMBER; i++)
        shootsl[i].reset();
    }

    void shiftweapon(byte mainweapon, byte secondaryweapon)
    {
      byte currentweapon = getweapontype();
      if (currentweapon == mainweapon)
        setweapontype(secondaryweapon);
      else
        setweapontype(mainweapon);
    }
};

