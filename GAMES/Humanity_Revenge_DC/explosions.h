class Explosion {
  public:
    byte x = 0;
    byte y = 0;
    // 1 visible, 3 maxstep, 4 currentstep
    byte explosionstatus = INITIAL_EXPLOSION_STATUS;

    void init(byte _x, byte _y, byte _maxstep)
    {
      x = _x;
      y = _y;
      explosionstatus = 0b10000001 | (byte)(_maxstep << 4);
    }

    bool visible()
    {
      return (explosionstatus & 0b10000000) == 0b10000000;
    }

    byte maxstep()
    {
      return (byte)(explosionstatus << 1) >> 5;
    }

    byte currentstep()
    {
      return (byte)(explosionstatus << 4) >> 4;
    }

    void reset()
    {
      x = 0;
      y = 0;
      explosionstatus = INITIAL_EXPLOSION_STATUS;
    }
};

class Explosions
{
  private:
    Explosion explosionl[MAX_EXPLOSIONS];
  public:
    void addexplosion(byte x, byte y, byte maxstep)
    {
      for (byte i = 0; i < MAX_EXPLOSIONS; i++)
        if (!explosionl[i].visible())
        {
          explosionl[i].init(x, y, maxstep);
          break;
        }
    }

    void reset()
    {
      for (byte i = 0; i < MAX_EXPLOSIONS; i++)
        explosionl[i].reset();
    }

    void update()
    {
      for (byte i = 0; i < MAX_EXPLOSIONS; i++)
        if (explosionl[i].visible())
        {
          if (arduboy.everyXFrames(15))
            explosionl[i].explosionstatus += 1;

          if (explosionl[i].currentstep() > explosionl[i].maxstep())
            explosionl[i].reset();
        }
    }

    void draw()
    {
      for (byte i = 0; i < MAX_EXPLOSIONS; i++)
        if (explosionl[i].visible())
        {
          if (explosionl[i].currentstep() == 1)
            arduboy.drawBitmap(explosionl[i].x, explosionl[i].y, explosion_1, 8, 8, 1);
          else if (explosionl[i].currentstep() == 2)
            arduboy.drawBitmap(explosionl[i].x, explosionl[i].y, explosion_2, 8, 8, 1);
          else if (explosionl[i].currentstep() == 3)
            arduboy.drawBitmap(explosionl[i].x, explosionl[i].y, explosion_1, 8, 8, 1);
          else if (explosionl[i].currentstep() == 4)
            arduboy.drawBitmap(explosionl[i].x, explosionl[i].y, explosion_3, 8, 8, 1);
          else if (explosionl[i].currentstep() == 5)
            arduboy.drawBitmap(explosionl[i].x, explosionl[i].y, explosion_4, 8, 8, 1);
        }
    }
};
