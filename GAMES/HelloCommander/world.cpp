#include "globals.h"
#include "world.h"

World world;

bool World::togglePerson( Person &p, char isEnemy, char rem ){
    
    if( p.tileY > 31 || p.tileX > 15 )
        return false;
    
    isEnemy = (isEnemy+1) << 5;
    uint8_t &tile = tiles[p.tileY][p.tileX];
    
    if( !rem ){
        if( tile&0x7C )
            return false;
        tile |= isEnemy;
    }else{
        tile &= ~isEnemy;
    }
    
    return true;
    
}

int32_t prevX, prevY;

uint16_t World::NOISE( uint8_t x, uint8_t y, uint8_t z ){
    return ((SIN( (uint16_t(((y)+seed)/z)*13) + (uint16_t(((x)+seed)/z)*78) )*23789&0xFF)*z);
}

int8_t World::random(int8_t min, int8_t max){
    return (SIN( (frame*13+(nextId++)*79)*seed )*23789&0x3FF>>2 ) % (max-min) + min;
}

void World::initSeed( uint8_t seed ){
    nextId = seed;
}

uint8_t World::point( uint8_t x, uint8_t y, uint8_t a, uint8_t b, uint8_t c ){
    int16_t h = 0x80 + NOISE(x, y, 8) + NOISE(x, y, 4) + NOISE(x, y, 2) + NOISE(x, y, 1)*4;
    h >>= 4;
    if( h > a ) return 3;
    else 
    if( h > b ) return 2;
    else 
    if( h > c ) return 1;
    return 0;
}

void World::generate( uint16_t seed ){
    this->seed = seed;
    for( uint8_t y=0; y<32; y++ )
        for( uint8_t x=0; x<16; x++ ){
            if( (tiles[y][x] = point(x, y, 200, 100, 50)) ){
                tiles[y][x] |= point(x+80, y+80, 240, 225, 190 ) << 2;
            }
        }
    
    do{
        startX = random(2, 14);
        startY = random(3, 13);
        
        endX = random(2, 14);
        endY = random(18, 29);
        
        if( seed & 1 ){
            uint8_t t = startY;
            startY = endY;
            endY = t;
        }
    }while( (tiles[startY][startX]|tiles[endY][endX])&0x7C  );
    
    prevX = (-startX*16)+(WIDTH/2)-8; 
    prevY = (-startY*16)+(HEIGHT/2)-8;
    tiles[endY][endX] |= 4 << 2;
}

void World::render( int16_t xo, int16_t yo ){
    const unsigned char *tile;
    xo = (-xo*16)+(WIDTH/2)-8; yo = (-yo*16)+(HEIGHT/2)-8;
    prevX = ((prevX<<2) + (xo<<1))/5;
    prevY = ((prevY<<2) + (yo<<1))/5;
    xo = prevX>>1;
    yo = prevY>>1;
    for( uint8_t y=0; y<32; y++ ){
        int16_t sy = y*16+yo;
    if( sy <= -15 || sy > HEIGHT ) continue; // stupid
        for( uint8_t x=0; x<16; x++ ){
            
            int16_t sx = xo+x*16;
            if( sx <= -15 || sx >= 128 ) continue; // stupid
            
            uint8_t tileId = tiles[y][x] & 0x3;
            uint8_t n=0;
            if( tileId > 0 ){
                if( x>0  && (tiles[y][x-1]&0x3) < tileId ) n |= 1;
                if( x<15 && (tiles[y][x+1]&0x3) < tileId ) n |= 2;
                if( y>0  && (tiles[y-1][x]&0x3) < tileId ) n |= 4;
                if( y<31 && (tiles[y+1][x]&0x3) < tileId ) n |= 8;
            }else{
                if( x>0  && (tiles[y][x-1]&0x3) != tileId ) n |= 1;
                if( x<15 && (tiles[y][x+1]&0x3) != tileId ) n |= 2;
                if( y>0  && (tiles[y-1][x]&0x3) != tileId ) n |= 4;
                if( y<31 && (tiles[y+1][x]&0x3) != tileId ) n |= 8;
            }
            if( tileId == 3 ) tileId--;
            
            sprite.draw( sx, sy, ::tiles, (tileId*2+(n>>3))*8+(n&0x7), NULL, 0, SPRITE_OVERWRITE );
            
            tileId = (tiles[y][x] >> 2) & 0x7;
            if( tileId )
                sprite.draw( sx, sy, ::tiles, 48+tileId, NULL, 0, SPRITE_OVERWRITE );
                
            uint8_t id = 0;
            if( tiles[y][x]&0x80 && (frame&1) ){
                id = 3;
            }else{
                id = ((tiles[y][x] >> 5)&0x3);
                if( !id ) continue;
                id--;
            }

            id = id * 2 + 56 - 3; // -3 unused tiles removed
            const unsigned char *item = ::tiles+2;
            item += id*32;
            sprite.drawBitmap( sx, sy, item, item+32, 16, 16, SPRITE_MASKED );
            
        }
    }
}
