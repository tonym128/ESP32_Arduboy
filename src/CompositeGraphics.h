#pragma once

class CompositeGraphics
{ 
  public:
  int xres;
  int yres;
  bool **frame;
  bool **backbuffer;
  int cursorX, cursorY, cursorBaseX;
  int frontColor, backColor;

  CompositeGraphics(int w, int h)
    :xres(w), 
    yres(h)
  {
    cursorX = cursorY = cursorBaseX = 0;
    frontColor = 50;
    backColor = -1;
  }

  void setTextColor(int front, int back = -1)
  {
    //-1 = transparent back;
    frontColor = front;
    backColor = back;
  }
  
  void init()
  {
    frame = (bool**)malloc(yres * sizeof(bool*));
    backbuffer = (bool**)malloc(yres * sizeof(bool*));
    for(int y = 0; y < yres; y++)
    {
      frame[y] = (bool*)malloc(xres);
      backbuffer[y] = (bool*)malloc(xres);
    }
  }

  void setCursor(int x, int y)
  {
    cursorX = cursorBaseX = x;  
    cursorY = y;  
  }

  inline void begin(int clear = -1, bool clearZ = true)
  {
    if(clear > -1)
      for(int y = 0; y < yres; y++)
        for(int x = 0; x < xres; x++)
          backbuffer[y][x] = clear;
  }

  inline void dotFast(int x, int y, bool color)
  {
    backbuffer[y][x] = color;
  }
  
  inline void dot(int x, int y, bool color)
  {
    if((unsigned int)x < xres && (unsigned int)y < yres)
      backbuffer[y][x] = color;
  }
  
  inline void dotAdd(int x, int y, bool color)
  {
    if((unsigned int)x < xres && (unsigned int)y < yres)
      backbuffer[y][x] = min(54, color + backbuffer[y][x]);
  }
  
  inline char get(int x, int y)
  {
    if((unsigned int)x < xres && (unsigned int)y < yres)
      return backbuffer[y][x];
    return 0;
  }
    
  inline void xLine(int x0, int x1, int y, bool color)
  {
    if(x0 > x1)
    {
      int xb = x0;
      x0 = x1;
      x1 = xb;
    }
    if(x0 < 0) x0 = 0;
    if(x1 > xres) x1 = xres;
    for(int x = x0; x < x1; x++)
      dotFast(x, y, color);
  }
  
  void line(int x1, int y1, int x2, int y2, bool color)
  {
    int x, y, xe, ye;
    int dx = x2 - x1;
    int dy = y2 - y1;
    int dx1 = labs(dx);
    int dy1 = labs(dy);
    int px = 2 * dy1 - dx1;
    int py = 2 * dx1 - dy1;
    if(dy1 <= dx1)
    {
      if(dx >= 0)
      {
        x = x1;
        y = y1;
        xe = x2;
      }
      else
      {
        x = x2;
        y = y2;
        xe = x1;
      }
      dot(x, y, color);
      for(int i = 0; x < xe; i++)
      {
        x = x + 1;
        if(px < 0)
        {
          px = px + 2 * dy1;
        }
        else
        {
          if((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
          {
            y = y + 1;
          }
          else
          {
            y = y - 1;
          }
          px = px + 2 *(dy1 - dx1);
        }
        dot(x, y, color);
      }
    }
    else
    {
      if(dy >= 0)
      {
        x = x1;
        y = y1;
        ye = y2;
      }
      else
      {
        x = x2;
        y = y2;
        ye = y1;
      }
      dot(x, y, color);
      for(int i = 0; y < ye; i++)
      {
        y = y + 1;
        if(py <= 0)
        {
          py = py + 2 * dx1;
        }
        else
        {
          if((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
          {
            x = x + 1;
          }
          else
          {
            x = x - 1;
          }
          py = py + 2 * (dx1 - dy1);
        }
        dot(x, y, color);
      }
    }
  }
  
  inline void flush()
  {
  }

  inline void end()
  {
    bool **b = backbuffer;
    backbuffer = frame;
    frame = b;    
  }

  void fillRect(int x, int y, int w, int h, bool color)
  {
    if(x < 0)
    {
      w += x;
      x = 0;
    }
    if(y < 0)
    {
      h += y;
      y = 0;
    }
    if(x + w > xres)
      w = xres - x;
    if(y + h > yres)
      h = yres - y;
    for(int j = y; j < y + h; j++)
      for(int i = x; i < x + w; i++)
        dotFast(i, j, color);
  }

  void rect(int x, int y, int w, int h, bool color)
  {
    fillRect(x, y, w, 1, color);
    fillRect(x, y, 1, h, color);
    fillRect(x, y + h - 1, w, 1, color);
    fillRect(x + w - 1, y, 1, h, color);
  }
};

