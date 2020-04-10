#include "globals.h"
#include "world.h"
#include "person.h"
#include "enemy.h"
#include "bmp.h"

int8_t fx, fy, blink;
const char nameParts[] PROGMEM = "SaKaMoNimomikutokoranijiRoBoJuLimonokutokoroniji";

void nextSoldier( Person *soldiers, int8_t &current, uint8_t max, int8_t add ){
    int8_t first = current;
    if( first < 0 ) first = max-1;
    else if( first >= max ) first = 0;
    current = first;
    
    do{
        current += add;
        if( current < 0 ) current = max-1;
        else if( current >= max ) current = 0;
    }while( current != first && soldiers[current].points == 0 );
}

Person::Person(){}

void Person::printHP(){
    arduboy.print( F(" HP:") );
    arduboy.print( (health*100L) / defHealth );
    arduboy.print( F("%") );
}

void swap( int8_t &a, int8_t &b ){
    int8_t t = a;
    a = b;
    b = t;
}

bool isBlocked( int8_t x0, int8_t y0, int8_t x1, int8_t y1 )
{
    uint8_t alt = world.tiles[y0][x0] & 0x3;
    if( x0 < x1 ) x0++;
    else if( x0 > x1 ) x0--;
    if( y0 < y1 ) y0++;
    else if( y0 > y1 ) y0--;
  // bresenham's algorithm - thx wikpedia
  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }

  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }

  int8_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int8_t err = dx / 2;
  int8_t ystep;

  if (y0 < y1)
  {
    ystep = 1;
  }
  else
  {
    ystep = -1;
  }

  for (; x0 <= x1; x0++){
      uint8_t x, y;
    if (steep)
    {
        x = y0;
        y = x0;
    }
    else
    {
        x = x0;
        y = y0;
    }
    
    uint8_t t = world.tiles[y][x];
    if( (t & 0x3) > alt ){
        // world.tiles[y][x] |= 0x80;
        return true;
    }
    
    t = t & 0x1C;
    if( t && t <= (3<<2) ){
        world.tiles[y][x] |= 3 << 2;
        // world.tiles[y][x] |= 0x80;
        return true;
    }

    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
  return false;
}

void Person::shoot( uint8_t tx, uint8_t ty, Person *soldiers, uint8_t max ){
    for( uint8_t i=0; i<max; ++i ){
        Person &soldier = soldiers[i];
        if( soldier.tileX == tx && soldier.tileY == ty ){
            
            uint8_t dmg = 10;
            if( id ) dmg += (5 - id) << 1;
            dmg *= world.random(4, 6);

            if( soldier.perks & PERKT_HIDEF ) dmg >>= 1;
            if( isBlocked(tileX, tileY, tx, ty) )
                dmg >>= 2;
            
            points = 0;
            ammo--;
            uint8_t ex = soldier.experience / 5 + 10;

            if( soldiers != enemy.soldiers ){
                world.tiles[ty][tx] |= 0x80;
                HIGHBYTE(camera) = tileX;
                LOWBYTE(camera) = tileY;
                waitXFrames(10);
                world.tiles[ty][tx] &= 0x7F;
            }

            for( uint8_t j = id ? 5-id : 1; j; --j ){
                printstrX = world.random(0, 128-8*5);
                printstrY = world.random(8, 55-8);
                
                HIGHBYTE(camera) = tx+world.random(-1, 2);
                LOWBYTE(camera) = ty+world.random(-1, 2);
                
                printstr = F("BANG!");
                waitXFrames(5);
            }

            if( dmg > soldier.health ){
                soldier.health = 0;
                soldier.points = 0;
                world.togglePerson( soldier, soldiers == enemy.soldiers, 1 );
                soldier.tileY = 0xFF;
                ex *= 10;
            }else{
                soldier.health -= dmg;
                ex *= 2;
            }
            waitXFrames(1);

            arduboy.setCursor(0, 56);
            arduboy.print(dmg);
            arduboy.print(F("DMG  +"));
            arduboy.print(ex);
            arduboy.print(F("EXP"));
            waitXFrames(20, false);
            printstr = NULL;

            addExp( ex );
            
            return;
            
        }
    }
}

void Person::randomize(){
    
    flags = world.random(0x80, 0x7F) & 0xFF;
    
    uint16_t b = 1<<7;
    for( uint8_t i=0; i<=8; ++i, b<<=1 )
        if( !world.random(0, 7) )
            flags |= b;
            
    if( !(flags & MALE) )
        flags &= ~BEARD;
    else 
        flags &= ~EARS_H;
        
    unsigned char nameLength = (world.random(0, 10) * world.random(1, 10)) / 30;
    unsigned char pos = 0;
    const char *parts = nameParts;
    if( flags & MALE )
        parts += 24; // 4x3x2
    
    uint8_t i=0;
    pos = world.random(0, 4)*2;
    name[i++] = pgm_read_byte(parts+pos++);
    name[i++] = pgm_read_byte(parts+pos);
    for( uint8_t p=0; p<nameLength; ++p ){
        pos = world.random(0, 4)*2 + 4*2;
        name[i++] = pgm_read_byte(parts+pos++);
        name[i++] = pgm_read_byte(parts+pos);
    }
    
    pos = world.random(0, 4)*2 + 4*2*2;
    name[i++] = pgm_read_byte(parts+pos++);
    name[i++] = pgm_read_byte(parts+pos);
    
    name[i++] = 0;

}

uint8_t Person::scan( uint8_t target, uint8_t &tx, uint8_t &ty, uint8_t maxCount ){
    uint8_t range = 2+id, count = 0;
    int8_t sy=tileY-range,
        ey=tileY+range,
        sx=tileX-range,
        ex=tileX+range;
        
    if( sy < 0 ) sy = 0;
    else if( ey>31 ) ey = 31;
    if( sx < 0 ) sx = 0;
    else if( ex>15 ) ex = 15;
    
    
    for( uint8_t y=sy; y<=ey; ++y ){
        for( uint8_t x=sx; x<=ex; ++x ){
            if( (world.tiles[y][x] & 0x60) == target ){
                if( count < maxCount ) count++;
                else{
                    tx = x;
                    ty = y;
                    return count;
                }
            }
        }
    }
    return count;
}

void Person::updateAppearance(){
            
    addLayer( base1, NULL );

    uint8_t eyes = EYES(flags)%2;
    if( flags & MALE )
        eyes = 3 - eyes;
    
    const uint8_t *eyeSprite, *eyeMask;

    switch( eyes ){
    case 0: eyeSprite=eyes1; eyeMask=eyes1_mask; break;
    case 1: eyeSprite=eyes2; eyeMask=eyes2_mask; break;
    default:
    case 2: eyeSprite=eyes3; eyeMask=eyes3_mask; break;
    case 3: eyeSprite=eyes5; eyeMask=eyes5_mask; break;
    }
    
    addLayer( eyeSprite, eyeMask, 1 );
    /*
    const uint8_t *earSprite, *earMask;
    earSprite = ears5;
    earMask = ears5_mask;
    
    switch( EARS(flags) ){
    case 0: earSprite=ears1; earMask=ears1_mask; break;
    case 1: earSprite=ears2; earMask=ears2_mask; break;
    case 2: earSprite=ears3; earMask=ears3_mask; break;
    case 3: earSprite=ears4; earMask=ears4_mask; break;
    }
    addLayer( earSprite, earMask, 2 );
    */
    
    uint8_t hair = HAIR(flags);
    if( flags & MALE )
        hair = 5 - hair;

    switch( hair ){ 
    case 0:  addLayer( hair1, hair1_mask ); break;
    case 1:  addLayer( hair4, hair4_mask ); break;
    case 2:  addLayer( hair6, hair6_mask ); break;
    case 3:  addLayer( hair5, hair5_mask ); break;
    case 4:  addLayer( hair3, hair3_mask ); break;
    case 5:  addLayer( hair2, hair2_mask ); break;
    }
    
    if( flags & BEARD )
        addLayer( acc6, acc6_mask );
    else if( flags & CHBBY )
        addLayer( acc1, acc1_mask );
    if( flags & GLASS )
        addLayer( acc2, acc2_mask );
    if( flags & CLOWN )
        addLayer( acc5, acc5_mask );
    if( flags & SCAR )
        addLayer( acc3, acc3_mask );
    if( flags & MOLE )
        addLayer( acc4, acc4_mask );
    
}

void Person::render( int8_t x, int8_t y, char _blink ){
    
    fx = x; fy = y; blink = _blink;
    updateAppearance();

}

void Person::addLayer( const uint8_t *s, const uint8_t *m, uint8_t type ){

    int8_t ox=fx, oy=fy;

    if( type == 1 ){ 
        ox += 13; 
        oy += 12;
        if( blink ){
            s = eyes4;
            m = eyes4_mask;
        }
    }/*
    else if( type == 2 ){
        ox += 1; 
        oy += 15;
    }
    */
    
    sprite.draw( ox, oy, s, 0, m, 0, SPRITE_AUTO_MODE );

}
