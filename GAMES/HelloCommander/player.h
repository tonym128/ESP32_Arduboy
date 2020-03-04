#ifndef PLAYER_H
#define PLAYER_H

#include "person.h"
#define DEFAULT_PARTY_SIZE 3

struct Player {
    
    Person  soldiers[ MAX_PARTY_SIZE ];
    uint8_t level;

    void gameReset(){
        
        level = 0;

        for( uint8_t id=0; id<MAX_PARTY_SIZE; ++id )
            soldiers[id].gameReset();

        spawn();    
    }
    
    void spawn(){
        for( uint8_t id=0; id<MAX_PARTY_SIZE; ++id )
            if( !soldiers[id].selectable() ){
                soldiers[id].spawn();
                return;
            }
    }
    
    uint8_t count(){
        uint8_t liveCount=0;
        for( uint8_t id=0; id<MAX_PARTY_SIZE; ++id )
            liveCount += soldiers[id].selectable();
        return liveCount;
    }
    
    bool roundReset(){
        uint8_t liveCount=0;
        for( uint8_t id=0; id<MAX_PARTY_SIZE; ++id )
            if( soldiers[id].tileY != 0xFF ){
                soldiers[id].roundReset();
                liveCount++;
            }
        return liveCount != 0;
    }
    
    void levelReset();
    
};

extern Player player;

#endif
