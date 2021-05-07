class EnemyManager
{
  private:
    byte easyenemyonstage = 0;
    byte mediumenemyonstage = 0;
    byte shootsonstage = 0;

    byte canspawn = 0;
  public:
    Enemy enemyl[MAX_ENEMIES];
    Boss boss;
    EnemyShoot enemyshootl[MAX_ENEMIES_SHOOTS];

    void spawnboss(byte bosstype)
    {
      boss.init(bosstype);
    }

    void enemyspawn(byte currentlevel)
    {
      if (canspawn == 0)
      {
        byte allowedeasyenemies = readleveldata(currentlevel - 1, MAX_EASY_ON_STAGE) - easyenemyonstage;
        byte allowedmediumenemies = readleveldata(currentlevel - 1, MAX_MEDIUM_ON_STAGE) - mediumenemyonstage;
        byte formationelement = readleveldata(currentlevel - 1, MAX_FORMATION_ELEMENT);

        if ((allowedeasyenemies > 0 || allowedmediumenemies > 0) && random(0, readleveldata(currentlevel - 1, SPAWN_RATE) + 1) == 0)
        {
          byte easytospawn = random(min(allowedeasyenemies, 1), allowedeasyenemies + 1);
          byte mediumtospawn = random(min(allowedmediumenemies, 1), allowedmediumenemies + 1);

          byte totaltospawn = easytospawn + mediumtospawn;

          byte formations = totaltospawn / formationelement + (totaltospawn % formationelement > 0 ? 1 : 0);
          byte yrange = ((MAX_SCREEN_Y - MIN_SCREENHUD_Y) / 10) / formations;

          for (byte i = 0; i < formations; i++)
          {
            byte inc = 0;
            byte y = (MIN_SCREENHUD_Y / 10) + random(yrange * i, yrange * i + yrange - 8);

            byte thisspawn = 0;
            if (totaltospawn % formationelement == 0)
              thisspawn = formationelement;
            else
              thisspawn = random(totaltospawn % formationelement, min(formationelement, totaltospawn) + 1);

            totaltospawn -= thisspawn;

            for (byte i = 0; i < MAX_ENEMIES; i++)
              if (!enemyl[i].visible())
              {
                byte enemyclass = random(easytospawn > 0 ? 0 : 1, mediumtospawn > 0 ? 2 : 1);
                byte enemytype = 0;
                byte movementtype = MOVEMENT_TYPE_SIMPLE;
                byte enemyx = 128 + inc;
                byte enemyy = y;
                switch (enemyclass)
                {
                  case EASY_ENEMY:
                    enemytype = random(0, readleveldata(currentlevel - 1, MAX_EASY_ENEMY_TYPE) + 1);
                    if (enemytype == EASY_TYPE_MISSILE)
                      enemytype = random(0, readleveldata(currentlevel - 1, MAX_EASY_ENEMY_TYPE) + 1);
                    switch (enemytype)
                    {
                      case EASY_TYPE_SINUSOIDE:
                        movementtype = random(MOVEMENT_TYPE_SIMPLE, MOVEMENT_TYPE_SINUSOIDE + 1);
                        break;
                      case EASY_TYPE_KAMIKAZE:
                        movementtype = MOVEMENT_TYPE_KAMIKAZE;
                        break;
                      case EASY_TYPE_MISSILE:
                        movementtype = MOVEMENT_TYPE_MISSILE;
                        enemyx = random(0, MAX_SHIP_X / 10 + 1);
                        enemyy = 70;
                        break;
                    }
                    break;
                  case MEDIUM_ENEMY:
                    enemytype = random(0, readleveldata(currentlevel - 1, MAX_MEDIUM_ENEMY_TYPE) + 1);
                    switch (enemytype)
                    {
                      case MEDIUM_TYPE_SINUSOIDE:
                        movementtype = MOVEMENT_TYPE_SINUSOIDE;
                        break;
                    }
                    break;
                }

                enemyl[i].init(enemyx, enemyy, enemyclass, enemytype, movementtype);
                thisspawn--;

                switch (enemyclass)
                {
                  case EASY_ENEMY:
                    if (enemytype != EASY_TYPE_MISSILE)
                      inc += 8;
                    easytospawn--;
                    break;
                  case MEDIUM_ENEMY:
                    if (enemytype != EASY_TYPE_MISSILE)
                      inc += 10;
                    mediumtospawn--;
                    break;
                }

                if (thisspawn == 0)
                  break;
              }

            if (inc > canspawn)
              canspawn = inc;
          }
        }
      }
    }

    void enemyshootspawn(int shipX, int shipY, byte currentlevel)
    {
      //shoot
      byte shoot_spawn_rate = readleveldata(currentlevel - 1, SHOOT_SPAWN_RATE);
      if (random(0, shoot_spawn_rate + 1) == 0)
      {
        if (shootsonstage < readleveldata(currentlevel - 1, SHOOT_NUMBER))
          for (byte i = 0; i < MAX_ENEMIES; i++)
          {
            bool shoot = false;
            if (enemyl[i].visible() && (enemyl[i].x <= 127) && (enemyl[i].x > shipX + 28) && (enemyl[i].enemytype() != EASY_TYPE_KAMIKAZE && enemyl[i].enemytype() != EASY_TYPE_MISSILE))
              if (random(0, (easyenemyonstage + mediumenemyonstage) + 1) == 0)
              {
                for (byte j = 0; j < MAX_ENEMIES_SHOOTS; j++)
                  if (!enemyshootl[j].visible())
                  {
                    byte randomy = random(shipY >= 2 ? shipY - 2 : 0 , shipY + 10 + 1);
                    byte randomx = random(shipX >= 2 ? shipX - 2 : 0 , shipX + 10 + 1);
                    byte randomlaunch = enemyl[i].y + random(3, 5);

                    if (randomlaunch > randomy)
                      enemyshootl[j].init(enemyl[i].x - 1, randomlaunch, (byte)(atan2(randomlaunch - randomy, enemyl[i].x - randomx) * 180 / pi), 1);
                    else
                      enemyshootl[j].init(enemyl[i].x - 1, randomlaunch, (byte)abs(atan2(randomy - randomlaunch, enemyl[i].x - randomx) * 180 / pi), 0);

                    shootsonstage++;
                    shoot = true;
                    break;
                  }

                if (shoot)
                  break;
              }
          }
      }
    }

    void bossshootspawn(int shipX, int shipY, byte currentlevel)
    {
      if (shootsonstage < readleveldata(currentlevel - 1, BOSS_SHOOTS))
        if (random(0, readleveldata(currentlevel - 1, BOSS_SHOOTS_SPAWN_RATE) + 1) == 0)
          for (byte i = 0; i < MAX_BOSS_PARTS; i++)
            if (boss.bossparts[i].visible() && boss.bossparts[i].canshoot() && boss.bossparts[i].x > shipX + 60)
              for (byte j = 0; j < MAX_ENEMIES_SHOOTS; j++)
                if (!enemyshootl[j].visible())
                {
                  byte randomy = random(shipY >= 12 ? shipY - 12 : 0 , shipY + 24 + 1);
                  byte randomx = random(shipX >= 12 ? shipX - 12 : 0 , shipX + 24 + 1);
                  byte randomlaunch = boss.bossparts[i].y + random(0, 8);

                  if (randomlaunch > randomy)
                    enemyshootl[j].init(boss.bossparts[i].x - 1, randomlaunch, (byte)(atan2(randomlaunch - randomy, boss.bossparts[i].x - randomx) * 180 / pi), 1);
                  else
                    enemyshootl[j].init(boss.bossparts[i].x - 1, randomlaunch, (byte)abs(atan2(randomy - randomlaunch, boss.bossparts[i].x - randomx) * 180 / pi), 0);

                  shootsonstage++;
                  break;
                }
    }

    void update(byte shipX, byte shipY)
    {
      if (canspawn > 0)
        canspawn--;

      easyenemyonstage = 0;
      mediumenemyonstage = 0;

      for (byte i = 0; i < MAX_ENEMIES ; i++)
      {
        enemyl[i].update(shipX, shipY);

        if (enemyl[i].visible())
        {
          if (enemyl[i].enemyclass() == EASY_ENEMY)
            easyenemyonstage++;
          else if (enemyl[i].enemyclass() == MEDIUM_ENEMY)
            mediumenemyonstage++;
        }
      }

      boss.update();

      shootsonstage = 0;
      for (byte i = 0; i < MAX_ENEMIES_SHOOTS; i++)
      {
        enemyshootl[i].update();

        if (enemyshootl[i].visible())
          shootsonstage++;
      }
    }

    void draw()
    {
      for (byte i = 0; i < MAX_ENEMIES ; i++)
        enemyl[i].draw();

      boss.draw();

      for (byte i = 0; i < MAX_ENEMIES_SHOOTS; i++)
        enemyshootl[i].draw();
    }

    void clrshoot()
    {
      shootsonstage = 0;

      for (byte i = 0; i < MAX_ENEMIES_SHOOTS; i++)
        enemyshootl[i].reset();
    }

    void reset()
    {
      easyenemyonstage = 0;
      mediumenemyonstage = 0;

      for (byte i = 0; i < MAX_ENEMIES; i++)
        enemyl[i].reset();

      boss.reset();

      clrshoot();
    }

    bool noenemyinsight()
    {
      return (easyenemyonstage == 0) && (mediumenemyonstage == 0)  && (shootsonstage == 0) && (!boss.visible());
    }
};
