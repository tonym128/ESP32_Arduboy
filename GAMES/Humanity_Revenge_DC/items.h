class Item {
    byte y = 0;

    // 1 visible, 2 type, 5 duration
    byte itemstatus = 0b00000000;

    byte getduration()
    {
      return (itemstatus & 0b00011111);
    }
  public:
    byte x = 0;

    void init(byte _x, byte _y, byte _type, byte _duration)
    {
      if (_duration > 15)
        _duration = 15;

      x = min(_x, MAX_SHIP_X / 10);
      y = _y;
      itemstatus = 0b10000000 | (byte)(_type << 5) | _duration;
    }

    bool visible()
    {
      return (itemstatus & 0b10000000) == 0b10000000;
    }

    byte itemtype()
    {
      return ((byte)(itemstatus << 1)) >> 6;
    }

    bool decduration()
    {
      byte duration = getduration();
      if (duration > 0)
      {
        duration--;
        itemstatus = (itemstatus & 0b11100000) | duration;
        return true;
      } else
        return false;
    }

    void reset()
    {
      x = 0;
      y = 0;
      itemstatus = INITIAL_ITEM_STATUS;
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
      if (arduboy.everyXFrames(2))
        if (visible())
        {
#ifdef DRAW_ONLY_HITBOX
          Rect hitbox = gethitbox();
          arduboy.drawRect(hitbox.x, hitbox.y, hitbox.width, hitbox.height, 1);
#else
          switch (itemtype())
          {
            case ITEM_1UP:
              arduboy.drawBitmap(x, y, item_1up, 8, 8, 1);
              break;
            case ITEM_BOMB:
              arduboy.drawBitmap(x, y, item_bomb, 8, 8, 1);
              break;
            case ITEM_SHIELD:
              arduboy.drawBitmap(x, y, item_shield, 8, 8, 1);
              break;
          }
#endif
        }
    }
};

class Items
{
  public:
    Item iteml[MAX_ITEMS];

    void additem(byte x, byte y, byte type, byte duration)
    {
      for (byte i = 0; i < MAX_ITEMS; i++)
        if (!iteml[i].visible())
        {
          iteml[i].init(x, y, type, duration);
          break;
        }
    }

    void reset()
    {
      for (byte i = 0; i < MAX_ITEMS; i++)
        iteml[i].reset();
    }

    void update()
    {
      for (byte i = 0; i < MAX_ITEMS; i++)
      {
        if (iteml[i].visible())
          if (arduboy.everyXFrames(3))
            if (iteml[i].x > 0)
              iteml[i].x--;
            else
              iteml[i].reset();

        if (iteml[i].visible())
          if (arduboy.everyXFrames(30))
            if (!iteml[i].decduration())
              iteml[i].reset();
      }
    }

    void draw()
    {
      for (byte i = 0; i < MAX_ITEMS; i++)
        iteml[i].draw();
    }
};
