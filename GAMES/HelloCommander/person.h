#ifndef PERSON_H
#define PERSON_H

#define MALE  (1<<0)
#define COOP  (1<<1)
#define LAZY  (1<<2)
#define EARS_H (1<<3)

#define EARS(f)  (((f)>>2)&0x3)
#define HAIR(f)  (((f)>>4)&0x3)
#define EYES(f)  (((f)>>6)&0x3)

#define CRAZY (1<<8)
#define HSTL  (1<<9)
#define CLOWN (1<<10)
#define CHBBY (1<<11)
#define GLASS (1<<12)
#define SCAR  (1<<13)
#define BEARD (1<<14)
#define MOLE  (1<<15)


#define PERK_REGEN (1<<0)
#define PERK_WALK  (1<<1)
#define PERK_WALK2 (1<<2)
#define PERK_OVRACH (1<<3)

#define PERKT_HIDEF (1<<4)

#define ROOKIE  0
#define HEAVY   1
#define ASSAULT 2
#define SNIPER  3

struct Person {
    
    char name[10];
    uint16_t flags;

    uint8_t    id=0, perks=0, points=0,
        defHealth=0, health=0, ammo=0;
        
    union{
        uint16_t pos=0;
        struct{
            uint8_t tileY;
            uint8_t tileX;
        };
    };
    
    uint32_t experience = 0;
    
    Person();
    void randomize();
    void render( int8_t x, int8_t y, char blink );
    
    uint8_t level(){
        return (isqrt(experience)>>3)+1;
    }
    
    void shoot( uint8_t tx, uint8_t ty, Person *soldiers, uint8_t max );
    
    void reload(){
        ammo = 3;
        points -= 5;
        
        printstrX = 40;
        printstrY = 56;
        printstr = F("RELOADING!");
        waitXFrames(5);
        
    }

    PGMSTR role(){
        if( level() < 2 ){
            id = ROOKIE;
            return F("ROOKIE ");
        }
        if( flags&CRAZY ){
            id = HEAVY;
            return F("HEAVY ");
        }
        if( flags&LAZY  ){
            id = SNIPER;
            return F("SNIPER ");
        }
        id = ASSAULT;
        return F("ASSAULT ");
    }
    
    void addExp( uint32_t exp ){
        
        auto oldLevel = level();
        
        experience += exp;
        
        auto newLevel = level();
        
        levelUp(oldLevel, newLevel);
            
    }

    bool selectable(){
        return health;
    }
    
    void roundReset(){
        points = 10;
        if( perks & PERK_OVRACH )
            points = 15;
        if( (perks & PERK_REGEN) && health < defHealth )
            health++;
        perks &= 0xF;
    }
    
    void gameReset(){
        id=0;
        perks=0;
        points=0;
        defHealth=0; health=0;
        tileX=0xFF;  tileY=0xFF;
        experience = 0; ammo=0;
    }

    void spawn(){
        randomize();
        addExp(0);
        health = defHealth;
    }
    
    void printHP();
    
    uint8_t scan( uint8_t target, uint8_t &tx, uint8_t &ty, uint8_t maxCount );
    
private:
    void addLayer( const unsigned char *layer, const unsigned char *mask, uint8_t type=0 );
    void updateAppearance();
    void levelUp( uint8_t oldLevel, uint8_t newLevel ){
        for( ; oldLevel <= newLevel; oldLevel++ ){
            if( oldLevel < 30 )
                defHealth = 100 + oldLevel*3;
            switch( oldLevel ){
            case 3: 
                perks |= PERK_WALK;
                break;
            case 7: 
                perks |= PERK_REGEN;
                break;
            case 13:
                perks |= PERK_OVRACH;
                break;
            case 21:
                perks |= PERK_WALK2;
                break;
            }
        }
        role();
    }
    
};

void nextSoldier( Person *soldiers, int8_t &current, uint8_t max, int8_t add );

#endif
