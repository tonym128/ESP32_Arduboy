class Bomb
{
  private:
    byte x = 0;
    byte y = 0;
  public:
    // 1 visible, 1 to damage 0 damaged, 6 duration
    byte bombstatus = 0b00111100;

    byte todamage()
    {
      return (bombstatus & 0b01000000) == 0b01000000;
    }

    void setdamaged()
    {
      bombstatus &= 0b10111111;
    }

    byte visible()
    {
      return (bombstatus & 0b10000000) == 0b10000000;
    }

    void reset()
    {
      x = 0;
      y = 0;
      bombstatus = INITIAL_BOMB_STATUS;
    }

    void update()
    {
      if (visible())
      {
        bombstatus--;

        if ((bombstatus & 0b00111111) == 0b00000000)
          reset();
      }
    }

    void draw()
    {
      if (visible())
      {
        if ((bombstatus & 0b00111111) % 2 == 1)
        {
          arduboy.drawCircle(x, y, 40, 1);
          arduboy.drawCircle(x, y, 72, 1);
          arduboy.drawCircle(x, y, 104, 1);
        } else
        {
          arduboy.drawCircle(x, y, 32, 1);
          arduboy.drawCircle(x, y, 64, 1);
          arduboy.drawCircle(x, y, 96, 1);
          arduboy.drawCircle(x, y, 128, 1);
        }
      }
    }

    bool shoot(byte _x, byte _y)
    {
      if (!visible())
      {
        bombstatus = 0b11000000 | INITIAL_BOMB_STATUS;
        x = _x;
        y = _y;

        return true;
      }

      return false;
    }
};

