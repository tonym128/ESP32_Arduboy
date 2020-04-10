#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
public:
    Person soldiers[ MAX_ENEMY_COUNT ];
    void levelReset( uint8_t difficulty );
    void roundReset();
};

extern Enemy enemy;

#endif
