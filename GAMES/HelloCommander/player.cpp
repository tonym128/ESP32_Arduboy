#include "globals.h"
#include "world.h"
#include "player.h"

void Player::levelReset(){
    
    for( uint8_t i=0; i<MAX_PARTY_SIZE; ++i ){
        Person &p = soldiers[i];
        if( p.tileY == 0xFF ) continue;
        p.ammo = 3;
        do{
            p.tileX = world.startX + world.random(-2, 2);
            p.tileY = world.startY + world.random(-2, 2);
        }while( !world.togglePerson(p, 0, 0) );
        
    }
    
}
