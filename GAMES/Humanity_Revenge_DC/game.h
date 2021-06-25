#define PLAYING_LEVEL              0
#define NEED_TO_CHANGE_LEVEL       1
#define CHANGING_LEVEL             2
#define CHANGE_LEVEL               3
#define SPAWN_BOSS                 4
#define FIGHT_BOSS                 5

class Game
{
  private:
    byte changelevel = NEED_TO_CHANGE_LEVEL;
    unsigned long levelbegin = 0;
    unsigned int score = 0;
    byte currentlevel = 0;

    Ship ship;
    Background background;
    Weapon weapon;
    Bomb bomb;
    EnemyManager enemymanager;
    Explosions explosions;
    Items items;

    void drawbombs(byte x)
    {
      arduboy.drawFastHLine(x + 1, 0, 2, 1);
      arduboy.drawFastHLine(x, 1, 4, 1);
      arduboy.drawFastHLine(x, 2, 4, 1);
      arduboy.drawFastHLine(x + 1, 3, 2, 1);
    }

    void drawlives(byte x)
    {
      arduboy.drawFastHLine(x + 2, 0, 3, 1);
      arduboy.drawFastHLine(x, 1, 3, 1);
      arduboy.drawFastHLine(x, 2, 3, 1);
      arduboy.drawFastHLine(x + 2, 3, 3, 1);
    }

    void drawhud()
    {
      for (byte i = 0; i < ship.getlives(); i++)
        drawlives((i * 6) + 2);

      for (byte i = 0; i < ship.getbombs(); i++)
        drawbombs((i * 6) + 24);

      drawstring(F("stage"), 46, 0);
      drawnumber(currentlevel, 68, 0);
      drawstring(F("score"), 80, 0);
      drawnumber(score, 104, 0);
    }
  public:
    byte getcurrentlevel()
    {
      return currentlevel;
    }

    byte getshiptype()
    {
      return ship.getshiptype();
    }

    unsigned int getscore()
    {
      return score;
    }

    byte shipselectionmenu()
    {
      drawstring(F("ship selection"), 40, 2);
      drawshipname(ship.getshiptype(), 56, 12);
      switch (ship.getshiptype())
      {
        case SHIP_ODINO:
          //          drawstring(F("ODINO"), 56, 12);
          drawstring(F("POWER  B"), 56, 22);
          drawstring(F("SPEED  B"), 56, 28);
          break;
        case SHIP_THOR:
          //          drawstring(F("THOR"), 56, 12);
          drawstring(F("POWER  A"), 56, 22);
          drawstring(F("SPEED  C"), 56, 28);
          break;
        case SHIP_FREYR:
          //          drawstring(F("FREYR"), 56, 12);
          drawstring(F("POWER  C"), 56, 22);
          drawstring(F("SPEED  A"), 56, 28);
          break;
      }

      if (arduboy.justPressed(LEFT_BUTTON))
      {
         #ifdef ARDUBOY_TONES
        sound.tone(200, 20);
        #else
        beep2.tone(beep2.freq(200), 2);
        #endif
        weapon.reset();
        switch (ship.getshiptype())
        {
          case SHIP_ODINO:
            initship(SHIP_FREYR);
            break;
          case SHIP_THOR:
            initship(SHIP_ODINO);
            break;
          case SHIP_FREYR:
            initship(SHIP_THOR);
            break;
        }
      }

      if (arduboy.justPressed(RIGHT_BUTTON))
      {
        #ifdef ARDUBOY_TONES
        sound.tone(200, 20);
        #else
        beep2.tone(beep2.freq(200), 2);
        #endif
        weapon.reset();
        switch (ship.getshiptype())
        {
          case SHIP_ODINO:
            initship(SHIP_THOR);
            break;
          case SHIP_THOR:
            initship(SHIP_FREYR);
            break;
          case SHIP_FREYR:
            initship(SHIP_ODINO);
            break;
        }
      }

      if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON))
      {
        #ifdef ARDUBOY_TONES
        sound.tone(1000, 20);
        #else
        beep2.tone(beep2.freq(1000), 2);
        #endif
        
        return ship.getshiptype();
      }

      ship.setposition(64, 44);
      if (arduboy.everyXFrames(180))
      {
        byte mainweapon = 0, secondaryweapon = 0;
        ship.getweaponavailable(mainweapon, secondaryweapon);
        weapon.shiftweapon(mainweapon, secondaryweapon);
      }
      weapon.shoot(ship.getdrawX(), ship.getdrawY());

      ship.update();
      weapon.update();

      ship.draw();
      weapon.draw();

      return 255;
    }

    void reset()
    {
      changelevel = NEED_TO_CHANGE_LEVEL;
      currentlevel = 0;
      levelbegin = 0;
      score = 0;

      background.reset();
      ship.reset();
      weapon.reset();
      bomb.reset();
      enemymanager.reset();
      explosions.reset();
      items.reset();
    }

    void initship(byte shiptype)
    {
      ship.setshiptype(shiptype);
      weapon.setweapontype(ship.getmainweapon());
    }

    void init(byte shiptype)
    {
      initship(shiptype);
    }

    void destroyboss()
    {
      #ifdef ARDUBOY_TONES
      sound.tone(50, 20, 70 , 30);
      #else
      beep2.tone(beep2.freq(50), 2);
      #endif
      for (byte i = 0; i < MAX_BOSS_PARTS; i++)
        if (enemymanager.boss.bossparts[i].visible())
        {
          explosions.addexplosion(enemymanager.boss.bossparts[i].x, enemymanager.boss.bossparts[i].y, EXPLOSION_MEDIUM);
          enemymanager.boss.bossparts[i].reset();
        }
      enemymanager.boss.reset();
    }

    byte checkcollision()
    {
      for (byte i = 0; i < MAX_ITEMS; i++)
        if (items.iteml[i].visible())
          if (arduboy.collide(ship.gethitbox(), items.iteml[i].gethitbox()))
          {
            switch (items.iteml[i].itemtype())
            {
              case ITEM_1UP:
                ship.inclives();
                break;
              case ITEM_BOMB:
                ship.incbombs();
                break;
              case ITEM_SHIELD:
                ship.activateshield();
                break;
            }

            #ifdef ARDUBOY_TONES
            sound.tone(1000, 30);
            #else
            beep2.tone(beep2.freq(1000), 2);
            #endif
            items.iteml[i].reset();
          }

      // boss visible
      if (enemymanager.boss.visible())
      {
        for (byte i = 0; i < MAX_BOSS_PARTS; i++)
        {
          if (!enemymanager.boss.visible())
            break;

          byte bosspartlife = enemymanager.boss.bossparts[i].life;
          byte bosspartscore = enemymanager.boss.bossparts[i].getscore();
          bool mainpart = enemymanager.boss.bossparts[i].main();

          if (bomb.visible() && bomb.todamage())
            if (enemymanager.boss.bossparts[i].visible())
            {
              if (bosspartlife <= BOMB_DAMAGE)
                explosions.addexplosion(enemymanager.boss.bossparts[i].x, enemymanager.boss.bossparts[i].y, EXPLOSION_MEDIUM);
              if (enemymanager.boss.bossparts[i].decreaselife(BOMB_DAMAGE))
              {
                #ifdef ARDUBOY_TONES
                sound.tone(100, 20, 80, 20, 60 , 20);
                #else
                beep2.tone(beep2.freq(100), 2);
                #endif
                score += bosspartscore;
                if (mainpart || (!enemymanager.boss.partsvisible()))
                  destroyboss();
              } else
                bosspartlife -= BOMB_DAMAGE;
            }

          // ship weapon with boss
          if (enemymanager.boss.bossparts[i].visible())
            for (byte j = 0; j < MAX_SHOOT_NUMBER; j++)
            {
              if (!enemymanager.boss.bossparts[i].visible())
                continue;

              if (weapon.shootsl[j].visible())
                if (arduboy.collide(weapon.shootsl[j].gethitbox(), enemymanager.boss.bossparts[i].gethitbox()))
                {
                  byte shootdamage = weapon.shootsl[j].getdamage();
                  if (bosspartlife <= shootdamage)
                    explosions.addexplosion(enemymanager.boss.bossparts[i].x, enemymanager.boss.bossparts[i].y, EXPLOSION_MEDIUM);
                  if (enemymanager.boss.bossparts[i].decreaselife(shootdamage))
                  {
                    #ifdef ARDUBOY_TONES
                    sound.tone(100, 20, 80, 20, 60 , 20);
                    #else
                    beep2.tone(beep2.freq(100), 2);
                    #endif
                    score += bosspartscore;
                    if (mainpart || (!enemymanager.boss.partsvisible()))
                      destroyboss();
                  } else
                    bosspartlife -= shootdamage;
                  weapon.shootsl[j].decreasedamage(bosspartlife);
                }
            }

          // boss part with ship
          if (!ship.immune() && enemymanager.boss.bossparts[i].visible())
            if (arduboy.collide(ship.gethitbox(), enemymanager.boss.bossparts[i].gethitbox()))
            {
              if (ship.declives())
              {
                #ifdef ARDUBOY_TONES
                sound.tone(30, 30, 40, 40);
                #else
                beep2.tone(beep2.freq(30), 2);
                #endif
                
                ship.setimmune(true, 120);

                if (enemymanager.boss.bossparts[i].decreaselife(SHIP_DAMAGE))
                {
                  #ifdef ARDUBOY_TONES
                  sound.tone(100, 20, 80, 20, 60 , 20);
                  #else
                  beep2.tone(beep2.freq(100), 2);
                  #endif
                  score += bosspartscore;;
                  if (mainpart || (!enemymanager.boss.partsvisible()))
                    destroyboss();
                }
              }
              else
                return GAME_OVER;
            }
        }
      }
      else
      {
        for (byte i = 0; i < MAX_ENEMIES; i++)
        {
          byte enemylife = enemymanager.enemyl[i].getlife();
          byte enemyscore = enemymanager.enemyl[i].getscore();

          // bomb with enemies
          if (bomb.visible() && bomb.todamage())
            if (enemymanager.enemyl[i].visible())
            {
              explosions.addexplosion(enemymanager.enemyl[i].x, enemymanager.enemyl[i].y, enemymanager.enemyl[i].getexplosionstep());
              if (enemymanager.enemyl[i].decreaselife(enemymanager.enemyl[i].getlife()))
              {
                #ifdef ARDUBOY_TONES
                sound.tone(100, 20, 80, 20, 60 , 20);
                #else
                beep2.tone(beep2.freq(100), 2);
                #endif
                score += enemyscore;
              }
            }

          // enemy with ship weapon
          if (enemymanager.enemyl[i].visible())
            for (byte j = 0; j < MAX_SHOOT_NUMBER; j++)
            {
              if (!enemymanager.enemyl[i].visible())
                continue;

              byte shootdamage = weapon.shootsl[j].getdamage();

              if (weapon.shootsl[j].visible())
                if (arduboy.collide(weapon.shootsl[j].gethitbox(), enemymanager.enemyl[i].gethitbox()))
                {
                  if (enemylife <= shootdamage)
                  {
                    if (random(0, 401) == 0 && ship.getlives() < SHIP_MAX_LIVES)
                      items.additem(enemymanager.enemyl[i].x, enemymanager.enemyl[i].y, ITEM_1UP, ITEM_DURATION);
                    else if (random(0, 351) == 0 && ship.getbombs() < SHIP_MAX_BOMBS)
                      items.additem(enemymanager.enemyl[i].x, enemymanager.enemyl[i].y, ITEM_BOMB, ITEM_DURATION);
                    else if (random(0, 301) == 0 && !ship.shieldactive())
                      items.additem(enemymanager.enemyl[i].x, enemymanager.enemyl[i].y, ITEM_SHIELD, ITEM_DURATION);
                    else
                      explosions.addexplosion(enemymanager.enemyl[i].x, enemymanager.enemyl[i].y, enemymanager.enemyl[i].getexplosionstep());
                  } else
                    enemylife -= shootdamage;

                  if (enemymanager.enemyl[i].decreaselife(shootdamage))
                  {
                    #ifdef ARDUBOY_TONES
                    sound.tone(100, 20, 80, 20, 60 , 20);
                    #else
                    beep2.tone(beep2.freq(100), 2);
                    #endif
                    score += enemyscore;
                  }

                  weapon.shootsl[j].decreasedamage(enemylife);
                }
            }

          // ship with enemies
          if (!ship.immune() && enemymanager.enemyl[i].visible())
            if (arduboy.collide(ship.gethitbox(), enemymanager.enemyl[i].gethitbox()))
            {
              if (ship.declives())
              {
                #ifdef ARDUBOY_TONES
                sound.tone(30, 30, 40, 40);
                #else
                beep2.tone(beep2.freq(30), 2);
                #endif
                ship.setimmune(true, 120);
                if (enemymanager.enemyl[i].decreaselife(SHIP_DAMAGE))
                {
                  #ifdef ARDUBOY_TONES
                  sound.tone(100, 20, 80, 20, 60 , 20);
                  #else
                  beep2.tone(beep2.freq(100), 2);
                  #endif
                  score += enemyscore;
                }
                else
                  enemylife -= SHIP_DAMAGE;
              }
              else
                return GAME_OVER;
            }
        }
      }

      // enemies shoots with bomb and ship
      for (byte i = 0; i < MAX_ENEMIES_SHOOTS; i++)
      {
        if (bomb.visible())
          if (enemymanager.enemyshootl[i].visible())
            enemymanager.enemyshootl[i].reset();

        if (!ship.immune() && enemymanager.enemyshootl[i].visible())
          if (arduboy.collide(enemymanager.enemyshootl[i].gethitbox(), ship.gethitbox()))
          {
            enemymanager.enemyshootl[i].reset();

            if (ship.declives())
            {
              #ifdef ARDUBOY_TONES
              sound.tone(30, 30, 40, 40);
              #else
              beep2.tone(beep2.freq(30), 2);
              #endif
              ship.setimmune(true, 120);
            }
            else
              return GAME_OVER;
          }
      }

      if (bomb.visible())
        bomb.setdamaged();

      return 0;
    }

    void manageinput()
    {
      if (arduboy.pressed(A_BUTTON))
        if (weapon.shoot(ship.getdrawX(), ship.getdrawY()))
            #ifdef ARDUBOY_TONES
            sound.tone(880, 20);
            #else
            beep1.tone(beep1.freq(880), 2);
            #endif

      if (arduboy.pressed(A_BUTTON) && arduboy.justPressed(B_BUTTON) && (changelevel == PLAYING_LEVEL || changelevel == FIGHT_BOSS))
      {
        if (ship.getbombs() > 0)
          if (bomb.shoot(ship.getdrawX(), ship.getdrawY()))
          {
            #ifdef ARDUBOY_TONES
            sound.tone(20, 50, 30, 50);
            #else
            beep1.tone(beep1.freq(20), 3);
            #endif
            ship.decbombs();
          }
      }
      else if (!arduboy.pressed(A_BUTTON) && arduboy.justPressed(B_BUTTON))
      {
        byte mainweapon = 0, secondaryweapon = 0;
        ship.getweaponavailable(mainweapon, secondaryweapon);
        weapon.shiftweapon(mainweapon, secondaryweapon);
      }

      if (arduboy.pressed(UP_BUTTON))
        ship.moveship(MOVE_UP);
      if (arduboy.pressed(DOWN_BUTTON))
        ship.moveship(MOVE_DOWN);
      if (arduboy.pressed(LEFT_BUTTON))
        ship.moveship(MOVE_LEFT);
      if (arduboy.pressed(RIGHT_BUTTON))
        ship.moveship(MOVE_RIGHT);
    }

    byte gameloop()
    {
      drawhud();

#ifdef DEBUG_SERIAL
      Serial.println("changelevel: ");
      Serial.println(changelevel);
      Serial.println("levelbegin: ");
      Serial.println(levelbegin);
      Serial.println("currentlevel: ");
      Serial.println(currentlevel);
#endif

      if (changelevel == PLAYING_LEVEL)
      {
        if ((millis() / 1000 - levelbegin) > (readleveldata(currentlevel - 1, LEVEL_TIMER)))
        {
          byte bosstype = readleveldata(currentlevel - 1, BOSS_TYPE);
          if (bosstype != 0)
            changelevel = SPAWN_BOSS;
          else
            changelevel = NEED_TO_CHANGE_LEVEL;
          levelbegin = millis() / 1000;
        }
      } else if (changelevel == SPAWN_BOSS)
      {
        if (enemymanager.noenemyinsight() && (millis() / 1000 - levelbegin > 3))
        {
          enemymanager.spawnboss(readleveldata(currentlevel - 1, BOSS_TYPE));
          changelevel = FIGHT_BOSS;
        }
      } else if (changelevel == FIGHT_BOSS)
      {
        if (!enemymanager.boss.visible())
        {
          changelevel = NEED_TO_CHANGE_LEVEL;
          levelbegin = millis() / 1000;
        }
      } else if (changelevel == NEED_TO_CHANGE_LEVEL)
      {
        if (enemymanager.noenemyinsight() && (millis() / 1000 - levelbegin > 2))
        {
          // no enemy, change
          changelevel = CHANGING_LEVEL;
          levelbegin = millis() / 1000;
          currentlevel++;
#ifdef DEBUG_SERIAL
          Serial.println(sizeof(Levels) / sizeof(Levels[0]));
#endif
          if (currentlevel > sizeof(Levels) / sizeof(Levels[0]))
            return VICTORY;
        }
      } else if (changelevel == CHANGING_LEVEL)
      {
        levelintro(currentlevel - 1);
        if (millis() / 1000 - levelbegin > 4)
          changelevel = CHANGE_LEVEL;
      } else if (changelevel == CHANGE_LEVEL)
      {
        changelevel = PLAYING_LEVEL;
        levelbegin = millis() / 1000;
      }

      manageinput();

      background.update();
      ship.update();
      weapon.update();
      bomb.update();
      explosions.update();
      items.update();
      enemymanager.update(ship.getdrawX(), ship.getdrawY());

      if (changelevel == PLAYING_LEVEL && !bomb.visible())
      {
        enemymanager.enemyspawn(currentlevel);
        enemymanager.enemyshootspawn(ship.getdrawX(), ship.getdrawY(), currentlevel);
      } else if (changelevel == FIGHT_BOSS && !bomb.visible())
        enemymanager.bossshootspawn(ship.getdrawX(), ship.getdrawY(), currentlevel);

#ifndef DRAW_ONLY_HITBOX
      background.draw();
#endif
      ship.draw();
      weapon.draw();
      bomb.draw();
#ifndef DRAW_ONLY_HITBOX
      explosions.draw();
#endif
      items.draw();

      switch (checkcollision())
      {
        case GAME_OVER:
          arduboy.delayShort(1200);
          return GAME_OVER;
      }

      enemymanager.draw();

      return 255;
    }
};

