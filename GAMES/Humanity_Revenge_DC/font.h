PROGMEM const unsigned char fonts[] =
{
  0xF6, 0xDE, // 0 
  0x59, 0x24, // 1
  0xE7, 0xCE, // 2
  0xE5, 0x9E, // 3
  0x92, 0xF2, // 4
  0xF3, 0x9E, // 5
  0x93, 0xDE, // 6
  0xE5, 0x92, // 7
  0xF7, 0xDE, // 8
  0xF7, 0x9E, // 9
  0xF7, 0xDA, // A
  0xD7, 0xDE, // B
  0xF2, 0x4E, // C
  0xD6, 0xDC, // D
  0xF3, 0x4E, // E
  0xF3, 0x48, // F
  0xF2, 0xDE, // G
  0xB7, 0xDA, // H
  0x49, 0x24, // I
  0x49, 0x2C, // J
  0xB7, 0x5A, // K
  0x92, 0x4E, // L
  0xF6, 0xDA, // M
  0xBE, 0xDA, // N
  0xF6, 0xDE, // O
  0xF7, 0xC8, // P
  0xF6, 0xF2, // Q
  0xF7, 0xEA, // R
  0xF3, 0x9E, // S
  0xE9, 0x24, // T
  0xB6, 0xDE, // U
  0xB6, 0xD4, // V
  0xB7, 0xFE, // W
  0xB5, 0x5A, // X
  0xB7, 0xA4, // Y
  0xE5, 0x4E, // Z 
};

bool drawchar(unsigned char ch, byte x, byte y)
{
  unsigned char offset = 0;
  byte startchar = 0;

  if (ch >= '0' && ch <= '9')
  {
    startchar = 0;
    offset = '0';
  }
  else if (ch >= 'A' && ch <= 'Z')
  {
    startchar = 20;
    offset = 'A';
  }
  else if (ch >= 'a' && ch <= 'z')
  { 
    startchar = 20;
    offset = 'a';
  }
  else if (ch == '.')
  {
    arduboy.drawPixel(x + 1, y + 4, 1);
    return true;
  }
  else if (ch == ' ')
  {
    return true;
  }
  
  if (offset != 0)
  {
    byte charf = pgm_read_byte(fonts + startchar + (ch - offset) * 2);
    byte charl = pgm_read_byte(fonts + startchar + (ch - offset) * 2 + 1);

    byte i = 0;
    while (i <= 7)
    {
      arduboy.drawPixel(x + (i % 3), y + (i / 3), bitRead(charf, 7 - i));
      if (i <= 6)
        arduboy.drawPixel(x + ((i + 2) % 3), y + 2 + ((i + 2)/ 3), bitRead(charl, 7 - i));
      i++;
    }

    return true;
  } else
    return false;
}

void drawnumber(word number, byte x, byte y)
{
  char buf[8];
  itoa(number, buf, 10);

  for (byte i = 0; i < strlen(buf); i++)
  {
    if (!drawchar(buf[i], x, y))
      break;
    x += 4;
  }
}

void drawstring(const char* data, byte x, byte y)
{
  PGM_P p = reinterpret_cast<PGM_P>(data);
  while (1)
  {
    unsigned char c = pgm_read_byte(p++);
    if (c == 0)
      break;
    if (!drawchar(c, x , y))
      break;
    x += 4;
  }
}

void drawstring(const __FlashStringHelper *data, byte x, byte y)
{
  drawstring((const char*)data, x, y);
}

void testfonts()
{
  unsigned char ch = 'A';
  int i = 0;
  while (ch <= 'Z')
  {
    drawchar(ch, i, 0);
    ch++;
    i+= 4;
  }

  ch = 'a';
  i = 0;
  while (ch <= 'z')
  {
    drawchar(ch, i, 6);
    ch++;
    i+= 4;
  }

  ch = '0';
  i = 0;
  while (ch <= '9')
  {
    drawchar(ch, i, 12);
    ch++;
    i+= 4;
  }
}

