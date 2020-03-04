#ifndef WORLD_H
#define WORLD_H

#include "person.h"

struct World {
    uint8_t tiles[32][16];
    uint8_t startX, startY, endX, endY, nextId;
    uint16_t seed;

    uint8_t point( uint8_t x, uint8_t y, uint8_t a, uint8_t b, uint8_t c );
    void generate( uint16_t );
    void render(int16_t xo, int16_t yo );
    
    bool togglePerson( Person &p, char isEnemy, char rem );

    uint16_t NOISE( uint8_t x, uint8_t y, uint8_t z );
    void initSeed( uint8_t seed );
    int8_t random(int8_t min, int8_t max);
    
    uint8_t cost( uint8_t tx, uint8_t ty, int8_t ox, int8_t oy ){
        if( !ox && !oy ) return 0;
        ox += tx;
        oy += ty;
        
        if( ox>15 || oy>31 || ox < 0 || oy < 0 )
            return 0xFF;
        
        uint8_t sa = tiles[ty][tx] & 3;
        uint8_t ta = tiles[oy][ox];
        
        if( ta&0x7C ) // person
            return 0xFF;
        
        ta &= 3;
        
        if( ta > sa ) // elevation gain
            return (ta - sa) * 2;
        
        if( ta == 0 ) // water
            return 2;
            
        return 1;
    }
};

extern World world;

#endif