#define MENU_START     1
#define MENU_INTRO     2
#define MENU_HELP      3
#define MENU_SOUND     4
#define MENU_HIGHSCORE 5
#define NO_SELECTION   255

//#define min(a, b) (a < b ? a : b)
//#define max(a, b) (a > b ? a : b)

byte menuselection = MENU_START;

byte menuManagement()
{
  arduboy.drawBitmap(8, 0, logo, 110, 18, 1);

  drawstring(F("start"), 50, 25);
  drawstring(F("intro"), 49, 33);
  drawstring(F("help"), 50, 41);
  drawstring(F("sound"), 50, 49);
  drawstring(F("highscore"), 50, 57);
  
  drawstring(F(VERSION), 0, 59);

  if (arduboy.justPressed(DOWN_BUTTON))
  {
    menuselection = min(menuselection++, MENU_HIGHSCORE);
    #ifdef ARDUBOY_TONES
    sound.tone(200, 20);
    #else
    beep2.tone(beep2.freq(200), 2);
    #endif
  }
  else if (arduboy.justPressed(UP_BUTTON))
  {
    menuselection = max(menuselection--, MENU_START);
    #ifdef ARDUBOY_TONES
    sound.tone(200, 20);
    #else
    beep2.tone(beep2.freq(200), 2);
    #endif
  }

  if (menuselection == MENU_SOUND)
    if (arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON))
    {
      if (arduboy.audio.enabled())
        arduboy.audio.off();
      else
        arduboy.audio.on();
    }

  if (arduboy.audio.enabled())
    drawstring(F("on"), 74, 49);
  else
    drawstring(F("off"), 74, 49);

  switch (menuselection)
  {
    case MENU_START:
      arduboy.drawRect(48, 23, 23, 9, 1);
      break;
    case MENU_INTRO:
      arduboy.drawRect(48, 31, 22, 9, 1);
      break;
    case MENU_HELP:
      arduboy.drawRect(48, 39, 19, 9, 1);
      break;
    case MENU_SOUND:
      arduboy.drawRect(48, 47, 23, 9, 1);
      break;
    case MENU_HIGHSCORE:
      arduboy.drawRect(48, 55, 39, 9, 1);
      break;
  }

  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON))
  {
     #ifdef ARDUBOY_TONES
     sound.tone(1000, 20);
     #else
     beep2.tone(beep2.freq(1000), 2);
     #endif
    return menuselection;
  }
  else
    return NO_SELECTION;
}
