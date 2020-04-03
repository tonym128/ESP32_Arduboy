//
//
//

#ifndef _ENEMY_H
#define _ENEMY_H

#include "Arduboy2.h"
#include "common.h"

//=============================================================================
typedef enum {
    BOSS_ALIEN = 0,
    RED_ALIEN,
    PURPLE_ALIEN,
    GREEN_ALIEN
} eEnemyType;

//=============================================================================
typedef enum {
    ENEMY_ST_NONE,
    ENEMY_ST_LAUNCHED,
    ENEMY_ST_LANDING,
    ENEMY_ST_FALLING,
    ENEMY_ST_TO_CONVOY,
    ENEMY_ST_RETURNED,
} eEnemyState;

//=============================================================================
class Enemy {
 public:
    eEnemyType type;

    Position pos;
    int pat;
    int dir;

    int freq;

    eEnemyState state;

    int col;
    int row;

    int score;

    int fire;
    int fire_cnt;

    float vx;
    float ax;
    int atk_pat;
    int atk_cnt;

    int boss_idx;

    Enemy() {
        pos.set(0, 0);
        fire = 0;
        state = ENEMY_ST_NONE;
    };
    ~Enemy() {};

    void init(int _x, int _y, eEnemyType _type, int _dir);
    void do_state(int frame, int ship_x, int ship_y, Enemy *boss, bool attack);
    bool fire_missile();
    void draw(int frame);
};

//=============================================================================
class Explosion : public Sprite {
 public:
    void draw();
};

//=============================================================================
class BossScore : public Sprite {
 public:
    int counter;
    void draw();
};

//=============================================================================
class Missile {
 public:
    Position pos;
    float vx;

    Missile() { pos.set(0, 0); vx = 0.0f; };
    void init(int _x, int _y, float _vx);
    void do_state(int frame);
    void draw();
};

//=============================================================================
#define CONVOY_COLS (10)
#define CONVOY_ROWS (5)
#define MAX_ENEMIES (10)
#define MAX_MISSILES (20)

//=============================================================================
class Convoy {
 public:
    int x;
    int y;
    int dir;

    int dont_attack;

    int convoy[CONVOY_ROWS][CONVOY_COLS]; // 10 x 5
    int left;
    int right;

    int num_enemies;

    int max_zako_in_scr;
    int zako_in_scr;
    int boss_in_scr;

    int boss_attack;

    Explosion *exp;
    BossScore *bscore;

    Enemy *enemy[MAX_ENEMIES];

    Missile missile[MAX_MISSILES];

    Convoy() {
        exp = NULL;
        bscore = NULL;
        for (int idx = 0; idx < MAX_ENEMIES; idx ++) {
            enemy[idx] = NULL;
        }
    };

    void init(int stage_no);

    void delete_alien(int row, int col);
    int defeat_alien(Position *missile);
    bool missile_hit(int ship_x, int ship_y);
    int alien_hit(int ship_x, int ship_y);
    int do_launch_alien(int row, int col, int dir);
    void remove_enemy(int idx);
    bool do_launch_boss(int col);
    bool launch_boss();
    void launch_zako();

    void fire_missile(int _x, int _y, float _vx);

    void do_state(int frame, bool attack, int ship_x, int ship_y);
    void draw(int frame);
};


#endif // !_ENEMY_H

//================================ END-OF-FILE ================================
