#include "globals.h"
#include "person.h"
#include "world.h"
#include "enemy.h"

void Enemy::roundReset(){
    for( uint8_t i=0; i<MAX_ENEMY_COUNT; ++i ){
        if( soldiers[i].tileY != 0xFF ) 
            soldiers[i].roundReset();
    }
}

void Enemy::levelReset( uint8_t difficulty ){
    
    for( uint8_t i=0; i<MAX_ENEMY_COUNT; ++i )
        soldiers[i].gameReset();

    if( !difficulty )
        return;
    
    // uint16_t seed = world.seed * world.random(1,4);
    // randomSeed( seed );

    uint8_t count = difficulty + 1;
    if( count >= MAX_ENEMY_COUNT )
        count = MAX_ENEMY_COUNT-1;
        
    for( uint8_t i=0; i<count; ++i ){
        Person &p = soldiers[i];
        p.spawn();
        p.addExp( world.random(difficulty*5, difficulty*15) );
        p.ammo = 3;
        do{
            
            p.tileX = world.random(2, 13);
            if( world.startY > 16 )
                p.tileY = world.random(2, 13);
            else
                p.tileY = world.random(18, 28);
                
        }while( !world.togglePerson(p, 1, 0) );
        
    }
    
}
