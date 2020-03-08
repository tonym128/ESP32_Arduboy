#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../ardBitmap/ArdBitmap.h"
#include "../fonts/Font3x6.h"

class Enemy { 
    
  public:

    Enemy();

    EnemyType getEnemyType();
    uint8_t getHP();
    uint8_t getDEF();

    void setEnemyType(Arduboy2Ext &arduboy, const EnemyType value);
    void setHP(const uint8_t hp);
    void setDEF(const uint8_t def);

    void inflictDMG(const uint8_t dmg);
    void render(Arduboy2Ext &arduboy, ArdBitmap<128, 64> &ardBitMap, Font3x6 &font3x6);
    void renderName(Font3x6 &font3x6);

    void incEnemyType(Arduboy2Ext &arduboy);
    void decEnemyType(Arduboy2Ext &arduboy);

  private: 

    void updateFrame(Arduboy2Ext &arduboy, uint8_t frameMax);

  private:

    EnemyType enemyType;
    uint8_t hp;
    uint8_t def;
    uint8_t hpMax;
    uint8_t frame;
    uint8_t names[3];

};
