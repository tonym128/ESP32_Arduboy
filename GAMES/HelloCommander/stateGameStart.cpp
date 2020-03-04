#include "globals.h"
#include "world.h"
#include "player.h"
#include "enemy.h"

uint16_t nextMap, prevMap;
void stateGameStart(){
    
    int16_t &timer = * (int16_t *) &stateVars[0];
    uint8_t &x = stateVars[2];
    
    if( !stateInitialized ){
        uint16_t seed = nextMap + prevMap;
        prevMap = nextMap;
        nextMap = seed;
        world.generate( seed );
        msgDelivered = 0;
        player.levelReset();
        enemy.levelReset( level );
        timer = 60;
    }
    
    printstrX = 5;
    printstrY = 10;
    printstr = F("v0.7] Bunker #");
    arduboy.print( level );
    
    if( !timer-- ){
        gameState = stateRoundStart;
        arduboy.setFrameRate(15);
    }
    
}
