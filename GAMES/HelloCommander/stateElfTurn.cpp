#include "globals.h"
#include "world.h"
#include "person.h"
#include "player.h"
#include "enemy.h"

bool turn(Person *soldiers, uint8_t count);

void stateElfTurn(){
    uint8_t &soldierId = stateVars[0];
    uint8_t oldId = soldierId;

    if( turn(enemy.soldiers, MAX_ENEMY_COUNT) ){
        gameState = stateRoundStart;
        return;
    }

    printstrX = 40;
    printstrY = 0;
    printstr = F("Elf Turn");
    
    if( oldId != soldierId )
        stateVars[1] = 20;

    Person &soldier = enemy.soldiers[soldierId];
    
    stateVars[1]--;
    if( !stateVars[1] )
        soldier.points = 0;
    
    int8_t ox, oy, tries=5;
    uint8_t cost;
    
    if( soldier.points > 5 ){
        if( soldier.ammo == 0 ){
            soldier.reload();
        }else{
            uint8_t tx=0xFF, ty;
            soldier.scan( 0x20, tx, ty, 0 );
            if( tx != 0xFF ){
                HIGHBYTE(camera) = tx;
                LOWBYTE(camera) = ty;
                soldier.shoot( tx, ty, player.soldiers, MAX_PARTY_SIZE );
            }
        }
    }
    

    do{
        ox = world.random(-1, 2);
        oy = world.random(-1, 2);
        cost = world.cost( soldier.tileX, soldier.tileY, ox, oy );
    }while( tries-- && ((ox==0 && oy==0) || cost == 0xFF) );
    
    if( !cost ){
    }else if( cost*3 < soldier.points ){
        world.togglePerson( soldier, 1, 1 );
        soldier.tileX += ox;
        soldier.tileY += oy;
        if( world.togglePerson( soldier, 1, 0 ) ){
            soldier.points -= cost*3;
        }else{
            soldier.tileX -= ox;
            soldier.tileY -= oy;
            world.togglePerson( soldier, 1, 0 );
        }
    }else // if( cost != 0xFF )
        soldier.points = 0;    

}
