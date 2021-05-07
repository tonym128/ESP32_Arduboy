class Star
{
  public:
    byte x = 0;
    byte y = 0;
    // 1 visible, 2speed
    byte starstatus = 0b00000000;

    void reset()
    {
      x = 0;
      y = 0;
      starstatus = 0b00000000;
    }
};

class Background
{
  private:
    Star stars[BACKGROUND_STAR_NUM];
  public:
    void update()
    {
      for (byte i = 0; i < BACKGROUND_STAR_NUM; i++)
        if ((stars[i].starstatus & 0b10000000) == 0b10000000)
        {
          if (stars[i].x <= 3)
            stars[i].reset();
          else
            stars[i].x -= ((byte)(stars[i].starstatus << 1) >> 6);
        }

      if (arduboy.everyXFrames(15))
      {
        for (byte i = 0; i < BACKGROUND_STAR_NUM; i++)
          if (!((stars[i].starstatus & 0b10000000) == 0b10000000))
          {
            stars[i].x = 128;
            stars[i].y = random(MIN_SCREENHUD_Y / 10 + 1, MAX_SCREEN_Y / 10 - 1);
            stars[i].starstatus = 0b10000000 | (byte)((random(1, 3) << 5));
            break;
          }
      }
    }

    void draw()
    {
      for (byte i = 0; i < BACKGROUND_STAR_NUM; i++)
        if ((stars[i].starstatus & 0b10000000) == 0b10000000)
          if (arduboy.everyXFrames(2))
            arduboy.drawPixel(stars[i].x, stars[i].y, 1);
    }

    void reset()
    {
      for (byte i = 0; i < BACKGROUND_STAR_NUM; i++)
        stars[i].reset();
    }
};

