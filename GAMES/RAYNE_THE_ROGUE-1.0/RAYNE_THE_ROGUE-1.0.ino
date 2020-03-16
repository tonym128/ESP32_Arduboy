//#include <SPI.h>
#include <ESP8266WiFi.h>
#include <Arduboy2.h>
//#include <ArduboyPlaytune.h>
#include <ArduboyTones.h>


Arduboy2 arduboy;
//ArduboyPlaytune tunes;
ArduboyTones tunes(arduboy.audio.enabled);

#include "fonts3x5.h"

//define menu states (on main menu)
#define STATE_SAVE_SLOTS         0 //play, sound on/off, info, current score
#define STATE_TITLE_SCREEN       1 //play, sound on/off, info, current score
#define STATE_INFO_SCREEN		     2 //show SHDWWZRD and CRIS and website for more
#define STATE_BOOSTS_SELECT      3 //player can purchase potions to increase boosts before starting game up to max, when player selects done initGame()
#define STATE_HELP_SCREEN        4
#define STATE_LEVEL_START        5 //show level # on screen for 3 secs then STATE_GAME_PLAYING
#define STATE_GAME_PLAYING       6 //main game playing screen
#define STATE_GAME_OVER          7 //game over text on screen, show copper collected, show new copper total a button goes back to title screen


#define FILE_NAME "RAYNEN"
#define FILE2_NAME "RAYNEH"
#define FILE_SIZE 9
struct game_data_struct{
  uint32_t copper;
  int8_t cold;
  int8_t acid;
  int8_t luck;
  int8_t invisibility;
  int8_t speed_boost;
} ;


//global defines
#define FRAME_DELAY_MS 50
#define DIR_LEFT -1
#define DIR_NONE  0
#define DIR_RIGHT 1

//player defines
#define PLAYER_DAMAGE_TIME 500;
#define PLAYER_WIDTH 8
#define PLAYER_HEIGHT 8
#define RAYNE_ANIMATION_STAT 0
#define RAYNE_ANIMATION_RUN 1
#define RAYNE_ANIMATION_RUN_FRAMES 3
#define RAYNE_ANIMATION_RUN_DELAY_MS 50
#define RAYNE_ANIMATION_DEATH 2
#define RAYNE_ANIMATION_DEATH_FRAMES 10
#define RAYNE_ANIMATION_DEATH_DELAY_MS 50


class sfs  {
public:
  char fileName[7];
  uint8_t fileSize;
  uint16_t startAddress;
 
  void GetFileAddress();
  uint16_t newfileStartIndex(uint8_t fileSize);
  sfs(); 
  sfs(const char* fname,uint8_t fsize); 
  ~sfs();
  bool Exists();
  bool Save();
  bool Load();
  bool Erase();
  void Clear();
  game_data_struct game_data;
};


// Constructor
// filename is the name of the new file

sfs::sfs() {
}

sfs::sfs(const char *fname, uint8_t fsize) {
  strncpy(fileName,fname,6);
  fileSize = fsize;
  GetFileAddress();
}


void sfs::GetFileAddress() {
 /* for (uint16_t i = 16; i < 1018; i++) {
    //find f start
    if (EEPROM.read(i) == fileName[0] &&
      EEPROM.read(i+1) == fileName[1] &&
      EEPROM.read(i+2) == fileName[2] &&
      EEPROM.read(i+3) == fileName[3] &&
      EEPROM.read(i+4) == fileName[4] &&
      EEPROM.read(i+5) == fileName[5])z*/
      startAddress = 20;
      fileSize = EEPROM.read(20+6);
//     fileSize = EEPROM.read(i+6);
 //     break;
//    }
//  }
}

bool sfs::Exists(){
  if(startAddress>0){
    return true;
  }
  return false;
}

bool sfs::Load() {
  GetFileAddress();
  if(startAddress!=0){
    for (unsigned int t=0; t<sizeof(game_data); t++){
      *((char*)(&game_data + t)) = EEPROM.read(startAddress + 7 + t);
    }
    return true;
  }
  return false;
}

uint16_t sfs::newfileStartIndex(uint8_t fSize){
  uint16_t empty_bytes=0;
  uint16_t new_start_index = 0;
  uint16_t i = 16;
  do{
    if(EEPROM.read(i)==0 && new_start_index==0){//found an empty byte
      new_start_index=i;
      empty_bytes++;
      i++;
    }else if(EEPROM.read(i)==0 && new_start_index!=0){//found an empty byte
      empty_bytes++;
      if(empty_bytes==fSize){
        return new_start_index;
      }
      i++;
    }else if(EEPROM.read(i)!=0){
      new_start_index=0;
      empty_bytes=0;
      i+=EEPROM.read(i+6)+7;//jump past f found
    }
  }while(i<=1024);
  return 0;
}

bool sfs::Save() {
  GetFileAddress();
  if(startAddress==0){//new f
    startAddress = newfileStartIndex(sizeof(game_data)+7);// find free space to save the f
    if(startAddress!=0){//found space
      //write fname
      for(uint16_t i=0;i<6;i++){// write the f
        EEPROM.write(startAddress+i, *((char*)(&fileName + i)) );
      }
      //write fsize
      EEPROM.write(startAddress+6, fileSize);
      //write fdata
      for(uint16_t i=0;i<sizeof(game_data);i++){// write the f
        EEPROM.write(startAddress+7+i, *((char*)(&game_data + i)) );
      }   
      EEPROM.commit();
      return true;
    }else{
      return false;
    }
  }else{//update existing f
    if(sizeof(game_data) == fileSize){// is new data the same size      
      //write fname
      for(uint16_t i=0;i<6;i++){// write the f
        EEPROM.write(startAddress+i, *((char*)(&fileName + i)) );
      }
      //write fsize
      EEPROM.write(startAddress+6, fileSize);
      //write fdata
      for(uint16_t i=0;i<sizeof(game_data);i++){// write the f
        EEPROM.write(startAddress+7+i, *((char*)(&game_data + i)) );
      } 
      EEPROM.commit();
    }else{
      uint16_t tstartIndex =newfileStartIndex(sizeof(game_data)+7);// find free space to save the f      
      if(tstartIndex!=0){// found enough space
        // delete old f, set all bytes to 0        
        for(uint16_t i=0;i<sizeof(game_data)+7;i++){
          EEPROM.write(startAddress+i, 0);
        } 
        //write fname
        for(uint16_t i=0;i<6;i++){// write the f
          EEPROM.write(tstartIndex+i, *((char*)(&fileName + i)) );
        }
        //write fsize
        EEPROM.write(tstartIndex+6, fileSize);
        //write fdata
        for(uint16_t i=0;i<sizeof(game_data);i++){// write the f
          EEPROM.write(tstartIndex+7+i, *((char*)(&game_data + i)) );
        } 
        startAddress=tstartIndex;
        EEPROM.commit();
        return true;
      }else{
        return false;
      }    
    }
  }
}

bool sfs::Erase(){
  if(startAddress!=0){//if no start index then f not found
    for (uint16_t i=0; i <=7+fileSize; i++) {
      EEPROM.write(startAddress+i, 0);   
    }
    startAddress=0;
    EEPROM.commit();
    return true;
  }
  return false;
}

void sfs::Clear(){
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

sfs::~sfs(){
  delete this;
}


//player variables and images
struct player_structure{
  public:    
    int8_t x;
    int8_t y;
    int8_t minX;
    int8_t minY;
    int8_t maxX;
    int8_t maxY;
    int8_t lastX;
    int8_t lastY;
    int8_t life=1;
    int8_t Direction;
    bool moving=false;
    bool damageBlink;
    int16_t damageTime;
    uint16_t frameTime;
    uint8_t frame;
    boolean animationEnded;
    uint8_t animation;
    uint8_t cold;
    uint8_t acid;
    int8_t  fire;
    uint8_t luck;
    uint8_t invisibility;
    bool invisibility_active;
    int16_t invisibilityTime;
    uint8_t speed_boost;
    bool speed_boost_active; 
    int16_t speedTime;
    bool seen;  
    bool key;
    uint16_t frame_delay_ms;
    uint8_t frame_count;
};

//object variables and images
struct object_structure{//treasure, potions, rings
  public:
    bool Exists=false;
    int8_t x;
    int8_t y;
    int8_t minX;
    int8_t minY;
    int8_t maxX;
    int8_t maxY;
    bool locked=false;
    uint8_t  type;
    /*
     * 1:
     */
};

//entities variables and images
struct entity_structure{//dragons
  public:
    bool Exists=false;
    int8_t x;
    int8_t y;
    uint8_t spd;
    uint8_t attacking=0;
    bool moving_up=true;
    bool hunting=false;
    uint8_t type;
    /*
     * 1:acid dragon
     * 2:cold dragon
     */
    
};

//projectiles variables and images
struct projectile_structure{//cloud of cold, acid spit
  public:
    bool Exists=false;
    int8_t x;
    int8_t y;
    int8_t minX;
    int8_t minY;
    int8_t maxX;
    int8_t maxY;
    uint16_t frameTime;
    uint8_t frame;
    uint8_t type;
    /*
     * 1:acid spit
     * 2:cold cloud
     */
    
};


//game takes 32 bytes of eeprom for 2 save games
//#include "sfs.h"
sfs files[3]{
  sfs(FILE_NAME,FILE_SIZE),
  sfs(FILE2_NAME,FILE_SIZE)
};
int8_t slot = 0;
#define no_save false

//initialize
const char collect_treasures[] PROGMEM = "COLLECT TREASURES";
const char move_to_exit[] PROGMEM = "MOVE TO DOOR TO EXIT";
const char dodge_dragon_attacks[] PROGMEM = "DODGE DRAGON ATTACKS";
const char collect_key[] PROGMEM = "COLLECT KEY TO UNLOCK DOOR";
const char select_a_save[] PROGMEM = "SELECT A SAVE SLOT";
const char select_string[] PROGMEM = "SELECT";
const char empty_string[] PROGMEM = "EMPTY";

int32_t lastFrameTime;
uint8_t cave; //max value of 255
int8_t gameState;
uint32_t score = 0; //max value of 4,294,967,296
uint16_t level_start_delay = 0;
bool a_button_down=false;
bool b_button_down=false;
bool up_button_down=false;
bool down_button_down=false;
bool left_button_down=false;
bool right_button_down=false;
int8_t button_on=0;
boolean soundYesNo;

#define totalObjects 10
#define totalProjectiles 10
#define totalEntities 3
object_structure objects[totalObjects];
projectile_structure projectiles[totalProjectiles];
entity_structure entities[totalEntities];
player_structure player;


/////////////////////////////////////////////////////////////////////////////
//IMAGES
/////////////////////////////////////////////////////////////////////////////

//player
PROGMEM const unsigned char rayne_l1[] = { 0x00,0xC0,0x60,0x3C,0x3F,0xEB,0x88,0x00 }; 
PROGMEM const unsigned char rayne_l2[] = { 0x00,0x00,0x60,0xFC,0xBF,0x0B,0x08,0x00 }; 
PROGMEM const unsigned char rayne_l3[] = { 0x00,0x00,0xC0,0xBC,0x7F,0x6B,0x08,0x00 }; 
PROGMEM const unsigned char rayne_r1[] = { 0x00,0x88,0xEB,0x3F,0x3C,0x60,0xC0,0x00 }; 
PROGMEM const unsigned char rayne_r2[] = { 0x00,0x08,0x0B,0xBF,0xFC,0x60,0x00,0x00 }; 
PROGMEM const unsigned char rayne_r3[] = { 0x00,0x08,0x6B,0x7F,0xBC,0xC0,0x00,0x00 }; 
const unsigned char *rayne_right_ani[] = {rayne_l1,rayne_l2,rayne_l3};
const unsigned char *rayne_left_ani[] = {rayne_r1,rayne_r2,rayne_r3};

PROGMEM const unsigned char rayne_death_1[] = { 0x00,0x88,0xEB,0x3F,0x3C,0x60,0xC0,0x00 }; 
PROGMEM const unsigned char rayne_death_2[] = { 0x00,0x44,0x68,0x3B,0xFF,0x00,0x00,0x00 }; 
PROGMEM const unsigned char rayne_death_3[] = { 0x10,0x52,0x74,0x3C,0x0F,0x03,0x00,0x00 }; 
PROGMEM const unsigned char rayne_death_4[] = { 0x08,0x28,0x3A,0x1C,0x18,0x0E,0x06,0x00 }; 
PROGMEM const unsigned char rayne_death_5[] = { 0x08,0x28,0x38,0x18,0x1E,0x18,0x0C,0x0C }; 
PROGMEM const unsigned char rayne_death_6[] = { 0x10,0x50,0x70,0x30,0x3C,0x30,0x18,0x18 }; 
PROGMEM const unsigned char rayne_death_7[] = { 0x20,0xA0,0xE0,0x60,0x70,0x60,0x30,0x30 }; 
PROGMEM const unsigned char rayne_death_8[] = { 0xC0,0xC0,0xC0,0xC0,0xE0,0xC0,0x60,0x60 };
PROGMEM const unsigned char rayne_death_9[] = { 0xC0,0xC0,0xC0,0xC0,0xC0,0x80,0xC0,0xC0 }; 
PROGMEM const unsigned char rayne_death_10[] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }; 
PROGMEM const unsigned char invisibility_1[] = {0x7E, 0x81};

const unsigned char *rayne_death_ani[] = {rayne_death_1,rayne_death_2,rayne_death_3,rayne_death_4,rayne_death_5,
rayne_death_6,rayne_death_7,rayne_death_8,rayne_death_9,rayne_death_10};



//dragons

PROGMEM const unsigned char acid_dragon_1[] = { 0xF0,0xF4,0xF4,0xF9,0xFA,0xFC,0xFD,0xF6,0x74,0x7E,0x3C,0x1C,0x1C,0x00,0x00,0x00 }; 
PROGMEM const unsigned char acid_dragon_2[] = { 0xF0,0xF4,0xF4,0xF9,0xFA,0xFC,0xFD,0xF6,0x74,0x7E,0x6C,0x4C,0x4C,0x00,0x00,0x00 }; 

PROGMEM const unsigned char fire_dragon_1[] = { 0xF8,0xF8,0x7D,0x3D,0x3F,0x7F,0x7E,0x7E,0xD6,0x9C,0x8C,0x1C,0x08,0x00,0x00,0x00 }; 
PROGMEM const unsigned char fire_dragon_2[] = { 0xF8,0xF8,0x7D,0x3D,0x3F,0x7F,0x7E,0x7E,0x76,0x3C,0x3C,0x1C,0x08,0x00,0x00,0x00 };

PROGMEM const unsigned char frost_1[] = { 0xF8,0xF8,0x7D,0x3D,0x3F,0x7F,0x7E,0x7E,0xD6,0x9C,0x8C,0x1C,0x08,0x00,0x00,0x00 }; 
PROGMEM const unsigned char frost_2[] = { 0xF8,0xF8,0x7D,0x3D,0x3F,0x7F,0x7E,0x7E,0x76,0x3C,0x3C,0x1C,0x08,0x00,0x00,0x00 };

PROGMEM const unsigned char snow_flake_1 [] = {
0x54, 0x38, 0x55, 0xBA, 0xEF, 0xBA, 0x55, 0x38, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char *cold_ani[] = {snow_flake_1,snow_flake_1};

PROGMEM const unsigned char fire_1[] = { 0x48,0x12,0x48,0x9D,0x7F,0xFE,0x7D,0x2C };
PROGMEM const unsigned char fire_2[] = { 0x48,0x12,0x48,0x9D,0x7F,0xFE,0x7D,0x2C }; 
const unsigned char *fire_ani[] = {fire_1,fire_2};

PROGMEM const unsigned char acid_spit_1[] = { 0x02,0x03,0x03,0x01,0x03,0x02,0x03,0x03,0x03,0x03,0x03,0x02,0x03,0x03,0x01 }; 
const unsigned char *acid_ani[] = {acid_spit_1,acid_spit_1};


//treasure
PROGMEM const unsigned char chest4[] = { 0x7E,0x4B,0x55,0x4B,0x4F,0x7F,0x49,0x3E }; 
PROGMEM const unsigned char chest3[] = { 0x7E,0x4B,0x57,0x4B,0x4F,0x7F,0x49,0x3E }; 
PROGMEM const unsigned char chest2[] = { 0x7E,0x4F,0x55,0x4F,0x4F,0x7F,0x49,0x3E }; 
PROGMEM const unsigned char chest1[] = { 0x7E,0x4F,0x5D,0x4F,0x4F,0x7F,0x49,0x3E }; 
PROGMEM const unsigned char pouch1[] = { 0x20,0x76,0x7D,0x75,0x7D,0x7D,0x76,0x20 }; 
PROGMEM const unsigned char pouch2[] = { 0x20,0x76,0x7D,0x55,0x7D,0x7D,0x76,0x20 }; 
PROGMEM const unsigned char pouch3[] = { 0x20,0x76,0x6D,0x5D,0x6D,0x7D,0x76,0x20 }; 
PROGMEM const unsigned char pouch4[] = { 0x20,0x76,0x6D,0x55,0x6D,0x7D,0x76,0x20 }; 
PROGMEM const unsigned char bag4[] = { 0x72,0xDB,0xAF,0xDE,0xFA,0x70,0x00,0x00 }; 
PROGMEM const unsigned char bag3[] = { 0x72,0xDB,0xBF,0xDE,0xFA,0x70,0x00,0x00 }; 
PROGMEM const unsigned char bag2[] = { 0x72,0xFB,0xAF,0xFE,0xFA,0x70,0x00,0x00 }; 
PROGMEM const unsigned char bag1[] = { 0x72,0xFB,0xEF,0xFE,0xFA,0x70,0x00,0x00 }; 
PROGMEM const unsigned char coin4[] = { 0x1C,0x36,0x6B,0x77,0x7F,0x3E,0x1C,0x00 }; 
PROGMEM const unsigned char coin3[] = { 0x1C,0x36,0x6F,0x77,0x7F,0x3E,0x1C,0x00 }; 
PROGMEM const unsigned char coin2[] = { 0x1C,0x3E,0x6B,0x7F,0x7F,0x3E,0x1C,0x00 }; 
PROGMEM const unsigned char coin1[] = { 0x1C,0x3E,0x7B,0x7F,0x7F,0x3E,0x1C,0x00 }; 
PROGMEM const unsigned char diamond[] = { 0x18,0x3C,0x7E,0xFF,0x7E,0x3C,0x18,0x00 }; 
PROGMEM const unsigned char lamp[] = { 0x01,0x07,0x2C,0x3E,0x3E,0x2F,0x09,0x0F }; 
PROGMEM const unsigned char harp[] = { 0x3F,0x44,0x7E,0x44,0x7E,0x44,0x3F,0x00 }; 
PROGMEM const unsigned char candleabra[] = { 0x07,0x44,0x66,0x7C,0x66,0x44,0x07,0x00 }; 




//objects
PROGMEM const unsigned char key[] = { 0x00,0x00,0xA6,0xA9,0xF9,0x06,0x00,0x00 }; 
PROGMEM const unsigned char closeddoor[] = { 0xFE,0xEF,0xFF,0xFF,0xFF,0xFF,0xFE,0xB4 }; 
PROGMEM const unsigned char opendoor[] = { 0xFE,0x03,0x01,0x01,0xFD,0xEF,0xFE,0xB4 }; 

//rings
PROGMEM const unsigned char aring[] = { 0x00,0x2E,0x53,0x59,0x53,0x2E,0x00,0x00 }; 
PROGMEM const unsigned char cring[] = { 0x00,0x2E,0x51,0x55,0x55,0x2E,0x00,0x00 }; 
PROGMEM const unsigned char iring[] = { 0x00,0x2E,0x55,0x51,0x55,0x2E,0x00,0x00 }; 
PROGMEM const unsigned char lring[] = { 0x00,0x2E,0x51,0x57,0x57,0x2E,0x00,0x00 }; 
PROGMEM const unsigned char pring[] = { 0x00,0x2E,0x51,0x59,0x59,0x2E,0x00,0x00 }; 

//potions
PROGMEM const unsigned char small_potion_a[] = { 0x00,0x30,0x7A,0x4E,0x66,0x4A,0x30,0x00 }; 
PROGMEM const unsigned char small_potion_c[] = { 0x00,0x30,0x7A,0x46,0x56,0x7A,0x30,0x00 }; 
PROGMEM const unsigned char small_potion_i[] = { 0x00,0x30,0x7A,0x7E,0x46,0x7A,0x30,0x00 }; 
PROGMEM const unsigned char small_potion_l[] = { 0x00,0x30,0x7A,0x46,0x5E,0x5A,0x30,0x00 }; 
PROGMEM const unsigned char small_potion_p[] = { 0x00,0x30,0x7A,0x46,0x66,0x7A,0x30,0x00 }; 


PROGMEM const unsigned char lpotion[] = { 0x10,0x38,0x45,0x5F,0x5D,0x38,0x10,0x00 }; 
PROGMEM const unsigned char ipotion[] = { 0x10,0x38,0x55,0x47,0x55,0x38,0x10,0x00 }; 
PROGMEM const unsigned char ppotion[] = { 0x10,0x38,0x45,0x67,0x65,0x38,0x10,0x00 }; 
PROGMEM const unsigned char apotion[] = { 0x10,0x38,0x4D,0x67,0x4D,0x38,0x10,0x00 }; 
PROGMEM const unsigned char cpotion[] = { 0x10,0x38,0x45,0x57,0x55,0x38,0x10,0x00 }; 

const unsigned char *img_objects[] = {
  coin1,coin2,bag1,pouch1,candleabra,
  chest1,coin3,bag2,pouch2,chest2,
  harp,coin4,bag3,pouch3,chest3,
  small_potion_c,small_potion_a,small_potion_l,small_potion_i,small_potion_p,
  lamp,bag4,pouch4,diamond,chest4,
  cpotion,apotion,lpotion,ipotion,ppotion,
  cring,aring,lring,iring,pring,
  key,closeddoor,opendoor
};

PROGMEM const unsigned char rayne_portrait [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xC0, 0xC0, 0xF0, 0xF0, 0xF0, 0xF0, 0xFC, 0xFC, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C, 0x3C, 0x3C, 0x3C, 0x30, 0x30,
0x0C, 0x0C, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0,
0xF0, 0xF0, 0xF0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0x3F,
0x0F, 0x0F, 0x0F, 0x0F, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xF0, 0xF0, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0x3F, 0xC0, 0xC0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
0xF0, 0xF0, 0xF0, 0xF0, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x0F, 0x0F, 0x3F, 0x3F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C, 0x3C, 0xF0, 0xF0,
0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x3F, 0x3F, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xC0, 0xC0, 0x3C, 0x3C, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFF, 0xFF, 0x3F, 0x3F, 0x3C, 0x3C, 0x33, 0x33, 0xC3, 0xC3,
0x0F, 0x0F, 0x3C, 0x3C, 0xF3, 0xF3, 0x0F, 0x0F, 0x0C, 0x0C, 0x30, 0x30, 0xF0, 0xF0, 0xC0, 0xC0,
0xC0, 0xC0, 0xC0, 0xC0, 0xC3, 0xC3, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xF0, 0xF0, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x3C, 0xF0, 0xF0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xC0,
0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F, 0x3C, 0x3C, 0xC0, 0xC0, 0x0F, 0x0F, 0x00, 0x00, 0x00, 0x00,
0x30, 0x30, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xFF, 0xFF,
0xFC, 0xFC, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFC, 0xC0, 0xC0, 0x03, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x0C, 0x0C, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03,
0xC0, 0xC0, 0xC0, 0xC0, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF
};


PROGMEM const int8_t rarities[] = {
1,100,0,0,0,
1,98,100,0,0,
1,96,100,0,0,
1,94,100,0,0,
1,92,100,0,0,
1,90,99,100,0,
1,88,99,100,0,
1,86,98,100,0,
1,84,98,100,0,
1,82,97,100,0,
1,80,97,100,0,
1,78,96,100,0,
1,76,96,100,0,
1,74,95,100,0,
1,72,95,100,0,
1,70,94,100,0,
1,68,94,100,0,
1,66,93,100,0,
1,64,93,100,0,
1,62,92,100,0,
1,60,92,100,0,
1,58,91,100,0,
1,56,91,100,0,
1,54,90,100,0,
2,52,90,99,100,
2,50,87,99,100,
2,48,85,98,100,
2,46,84,98,100,
2,44,82,97,100,
2,42,81,97,100,
2,40,79,96,100,
2,38,78,96,100,
2,36,76,95,100,
2,34,75,95,100,
2,32,73,94,100,
2,30,72,94,100,
2,28,70,93,100,
2,26,69,93,100,
2,24,67,92,100,
2,22,66,92,100,
2,20,64,91,100,
2,18,63,91,100,
2,16,61,90,100,
2,14,60,90,100,
3,12,58,89,100,
3,10,57,89,100,
3,8,55,88,100,
3,6,54,88,100,
3,4,52,87,100,
3,2,51,87,100,
3,0,49,86,100,
3,0,48,86,100,
3,0,46,85,100,
3,0,45,85,100,
3,0,43,84,100,
3,0,42,84,100,
3,0,40,83,100,
3,0,39,83,100,
3,0,37,82,100,
3,0,36,82,100,
3,0,34,81,100,
3,0,33,81,100,
3,0,31,80,100,
3,0,30,78,100,
3,0,29,77,100,
3,0,28,75,100,
3,0,27,74,100,
3,0,26,72,100,
3,0,25,71,100,
3,0,24,69,100,
3,0,23,68,100,
3,0,22,66,100,
3,0,21,65,100,
3,0,20,63,100,
3,0,19,62,100,
3,0,18,60,100,
3,0,17,59,100,
3,0,16,57,100,
3,0,15,56,100,
3,0,14,54,100,
3,0,13,53,100,
3,0,12,51,100,
3,0,11,50,100,
3,0,10,48,100,
3,0,9,47,100,
3,0,8,45,100,
3,0,7,44,100,
3,0,6,42,100,
3,0,5,41,100,
3,0,4,39,100,
3,0,3,38,100,
3,0,2,36,100,
3,0,1,35,100,
3,0,0,33,100,
3,0,0,32,100,
3,0,0,31,100,
3,0,0,30,100,
3,0,0,29,100,
3,0,0,28,100,
3,0,0,27,100,
3,0,0,26,100,
3,0,0,25,100,
3,0,0,24,100,
3,0,0,23,100,
3,0,0,22,100,
3,0,0,21,100,
3,0,0,20,100,
3,0,0,19,100,
3,0,0,18,100,
3,0,0,17,100,
3,0,0,16,100,
3,0,0,15,100,
3,0,0,14,100,
3,0,0,13,100,
3,0,0,12,100,
3,0,0,11,100,
3,0,0,10,100,
3,0,0,9,100,
3,0,0,8,100,
3,0,0,7,100,
3,0,0,6,100,
3,0,0,5,100,
3,0,0,4,100,
3,0,0,3,100,
3,0,0,2,100,
3,0,0,1,100,
3,0,0,0,100,
3,0,0,10,100,
3,0,0,20,100,
3,0,0,30,100,
3,0,5,41,100,
3,0,11,50,100,
3,0,18,60,100,
3,0,25,71,100,
3,0,31,80,100,
3,14,60,90,100,
3,54,90,100,0,

3,100,0,0,0,
3,98,100,0,0,
3,96,100,0,0,
3,94,100,0,0,
3,92,100,0,0,
3,90,99,100,0,
3,88,99,100,0,
3,86,98,100,0,
3,84,98,100,0,
3,82,97,100,0,
3,80,97,100,0,
3,78,96,100,0,
3,76,96,100,0,
3,74,95,100,0,
3,72,95,100,0,
3,70,94,100,0,
3,68,94,100,0,
3,66,93,100,0,
3,64,93,100,0,
3,62,92,100,0,
3,60,92,100,0,
3,58,91,100,0,
3,56,91,100,0,
3,54,90,100,0,
3,52,90,99,100,
3,50,87,99,100,
3,48,85,98,100,
3,46,84,98,100,
3,44,82,97,100,
3,42,81,97,100,
3,40,79,96,100,
3,38,78,96,100,
3,36,76,95,100,
3,34,75,95,100,
3,32,73,94,100,
3,30,72,94,100,
3,28,70,93,100,
3,26,69,93,100,
3,24,67,92,100,
3,22,66,92,100,
3,20,64,91,100,
3,18,63,91,100,
3,16,61,90,100,
3,14,60,90,100,
3,12,58,89,100,
3,10,57,89,100,
3,8,55,88,100,
3,6,54,88,100,
3,4,52,87,100,
3,2,51,87,100,
3,0,49,86,100,
3,0,48,86,100,
3,0,46,85,100,
3,0,45,85,100,
3,0,43,84,100,
3,0,42,84,100,
3,0,40,83,100,
3,0,39,83,100,
3,0,37,82,100,
3,0,36,82,100,
3,0,34,81,100,
3,0,33,81,100,
3,0,31,80,100,
3,0,30,78,100,
3,0,29,77,100,
3,0,28,75,100,
3,0,27,74,100,
3,0,26,72,100,
3,0,25,71,100,
3,0,24,69,100,
3,0,23,68,100,
3,0,22,66,100,
3,0,21,65,100,
3,0,20,63,100,
3,0,19,62,100,
3,0,18,60,100,
3,0,17,59,100,
3,0,16,57,100,
3,0,15,56,100,
3,0,14,54,100,
3,0,13,53,100,
3,0,12,51,100,
3,0,11,50,100,
3,0,10,48,100,
3,0,9,47,100,
3,0,8,45,100,
3,0,7,44,100,
3,0,6,42,100,
3,0,5,41,100,
3,0,4,39,100,
3,0,3,38,100,
3,0,2,36,100,
3,0,1,35,100,
3,0,0,33,100,
3,0,0,32,100,
3,0,0,31,100,
3,0,0,30,100,
3,0,0,29,100,
3,0,0,28,100,
3,0,0,27,100,
3,0,0,26,100,
3,0,0,25,100,
3,0,0,24,100,
3,0,0,23,100,
3,0,0,22,100,
3,0,0,21,100,
3,0,0,20,100,
3,0,0,19,100,
3,0,0,18,100,
3,0,0,17,100,
3,0,0,16,100,
3,0,0,15,100,
3,0,0,14,100,
3,0,0,13,100,
3,0,0,12,100,
3,0,0,11,100,
3,0,0,10,100,
3,0,0,9,100,
3,0,0,8,100,
3,0,0,7,100,
3,0,0,6,100,
3,0,0,5,100,
3,0,0,4,100,
3,0,0,3,100,
3,0,0,2,100,
3,0,0,1,100,
3,0,0,0,100,
3,0,0,10,100,
3,0,0,20,100,
3,0,0,30,100,
3,0,5,41,100,
3,0,11,50,100,
3,0,18,60,100,
3,0,25,71,100,
3,0,31,80,100,
3,14,60,90,100,
3,54,90,100,0,
};

void setup() {
  WiFi.mode(WIFI_OFF);
  EEPROM.begin(100);
  arduboy.boot();
  arduboy.setFrameRate(30);
  //clear_eeprom();
  if(no_save){
    files[0].game_data.copper=0;
    files[0].game_data.cold=0;
    files[0].game_data.acid=0;
    files[0].game_data.luck=0;
    files[0].game_data.invisibility=0;
    files[0].game_data.speed_boost=0;
  }else{
    if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON)){ 
      a_button_down=true;
      b_button_down=true;   
      files[0].Clear();
    }
    files[0].Load();
    if(files[0].Exists()==false){
      files[0].game_data.copper=0;
      files[0].game_data.cold=0;
      files[0].game_data.acid=0;
      files[0].game_data.luck=0;
      files[0].game_data.invisibility=0;
      files[0].game_data.speed_boost=0;
      files[0].Save();
    }
    
    files[1].Load();   
    if(files[1].Exists()==false){
      files[1].game_data.copper=0;
      files[1].game_data.cold=0;
      files[1].game_data.acid=0;
      files[1].game_data.luck=0;
      files[1].game_data.invisibility=0;
      files[1].game_data.speed_boost=0;
      files[1].Save();
    }
    if (arduboy.pressed(UP_BUTTON)){
      up_button_down=true;
      slot=1;
    }else{
      slot=0;
    }
    //up a and b then delete slot 1
    if (arduboy.pressed(UP_BUTTON) && arduboy.pressed(B_BUTTON)){
      up_button_down=true;
      b_button_down=true;
      files[1].game_data.copper=0;
      files[1].game_data.cold=0;
      files[1].game_data.acid=0;
      files[1].game_data.luck=0;
      files[1].game_data.invisibility=0;
      files[1].game_data.speed_boost=0;
      files[1].Save();
    }
    //down a b then delete slot 0
    if (arduboy.pressed(DOWN_BUTTON) && arduboy.pressed(B_BUTTON)){ 
      down_button_down=true;
      b_button_down=true;   
      files[0].game_data.copper=0;
      files[0].game_data.cold=0;
      files[0].game_data.acid=0;
      files[0].game_data.luck=0;
      files[0].game_data.invisibility=0;
      files[0].game_data.speed_boost=0;
      files[0].Save();
    }
  }
  //load sound state
  //tunes.initChannel(PIN_SPEAKER_1);
  //tunes.initChannel(PIN_SPEAKER_2);
  if (EEPROM.read(EEPROM_AUDIO_ON_OFF)) soundYesNo = true;
  if(soundYesNo){arduboy.audio.on();}else{arduboy.audio.off();}
  arduboy.initRandomSeed();  
  gameState  = STATE_TITLE_SCREEN;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!(arduboy.nextFrame())) return;
  int32_t dt = millis() - lastFrameTime;
  lastFrameTime = millis();
  switch(gameState){
    case STATE_TITLE_SCREEN:
      arduboy.clear();
      char pbuffer[2];      
      itoa(slot, pbuffer, 10); 
      drawString(0,0,pbuffer,0,NULL);  
      arduboy.drawBitmap(0,0,rayne_portrait, 68, 64, 1); 
      drawString(68,1,"RAYNE THE ROGUE",0,NULL);

      //build the menu
      drawString(84,15,"PLAY",0,NULL);
      drawString(84,25,"HELP",0,NULL);
      drawString(84,35,"SOUND ",0,NULL);
      if(soundYesNo){
        drawString(108,35,"ON",0,NULL);
      }else{
        drawString(108,35,"OFF",0,NULL);
      }
      drawString(84,45,"INFO",0,NULL);

      if (arduboy.pressed(UP_BUTTON) && !up_button_down){
        up_button_down=true;
        button_on--;
        if(button_on<0){button_on=3;}
      }//MOVE UP
      if (!arduboy.pressed(UP_BUTTON)){
        up_button_down=false;
      }
      if (arduboy.pressed(DOWN_BUTTON) && !down_button_down){
        down_button_down=true;
        button_on++;
        if(button_on>3){button_on=0;}
      }//MOVE DOWN
      if (!arduboy.pressed(DOWN_BUTTON)){
        down_button_down=false;
      }
      //draw cursor
      arduboy.drawTriangle(79,(button_on*10)+15,81,(button_on*10)+17,79,(button_on*10)+19,WHITE);
      
      if ((arduboy.pressed(A_BUTTON) && !a_button_down) ||  (arduboy.pressed(B_BUTTON) && !b_button_down)){ 
        a_button_down=true;
        b_button_down=true;
        if(button_on==0){
          cave=1;   
          score=0;
          player.luck=files[slot].game_data.luck;
          player.cold=files[slot].game_data.cold;
          player.acid=files[slot].game_data.acid;
          player.invisibility=files[slot].game_data.invisibility;
          player.speed_boost=files[slot].game_data.speed_boost;
          //if(game_data.copper>0){
            gameState = STATE_BOOSTS_SELECT;
          /*}else{
            gameState = STATE_HELP_SCREEN;
          }*/
        }else if(button_on==1){//help screen
          gameState = STATE_HELP_SCREEN;
        }else if(button_on==2){//toggle sound
          soundYesNo = !soundYesNo;
          if (soundYesNo == true){arduboy.audio.on();
            //tunes.tone(250, 250);
            tunes.tone(250, 150);
          }
          else arduboy.audio.off();
          arduboy.audio.saveOnOff();
        }else if(button_on==3){//info screen
          gameState = STATE_INFO_SCREEN;
          /*game_data.copper=0;
          game_data.cold=0;
          game_data.acid=0;
          game_data.luck=0;
          game_data.invisibility=0;
          game_data.speed_boost=0;
          f();*/
        }
      }
      if (!arduboy.pressed(A_BUTTON)){
        a_button_down=false;
      }
      if (!arduboy.pressed(B_BUTTON)){
        b_button_down=false;
      }
      drawCurrency();
      break;
    case STATE_INFO_SCREEN:
      arduboy.clear();
      arduboy.drawBitmap(0,0,rayne_portrait, 68, 64, 1); 
      drawString(68,1,"RAYNE THE ROGUE",0,NULL);
      drawString(84,15,"BY",0,NULL);

      //build the menu
      drawString(84,25,"SHDWWZRD",0,NULL);
      drawString(84,35,"CRIS",0,NULL);
      if ((arduboy.pressed(A_BUTTON) && !a_button_down) ||  (arduboy.pressed(B_BUTTON) && !b_button_down)){ 
        a_button_down=true;
        b_button_down=true;
        gameState = STATE_TITLE_SCREEN;
      }
      if (!arduboy.pressed(A_BUTTON)){
        a_button_down=false;
      }
      if (!arduboy.pressed(B_BUTTON)){
        b_button_down=false;
      }
      break;
    case STATE_HELP_SCREEN:
      arduboy.clear();
      //get treasures
      drawString(30,0,NULL,0,collect_treasures);
      arduboy.drawBitmap(14,10,img_objects[0], 8, 8, WHITE);
      arduboy.drawBitmap(24,10,img_objects[2], 8, 8, WHITE);
      arduboy.drawBitmap(34,10,img_objects[3], 8, 8, WHITE);
      arduboy.drawBitmap(44,10,img_objects[5], 8, 8, WHITE);
      arduboy.drawBitmap(54,10,img_objects[4], 8, 8, WHITE);
      arduboy.drawBitmap(64,10,img_objects[10], 8, 8, WHITE);
      arduboy.drawBitmap(74,10,img_objects[20], 8, 8, WHITE);
      arduboy.drawBitmap(84,10,img_objects[23], 8, 8, WHITE);
      arduboy.drawBitmap(94,10,img_objects[15], 8, 8, WHITE);
      arduboy.drawBitmap(104,10,img_objects[25], 8, 8, WHITE);
      arduboy.drawBitmap(114,10,img_objects[30], 8, 8, WHITE);
      //move to door to exit
      drawString(19,25,NULL,0,move_to_exit);
      arduboy.drawBitmap(101,25,img_objects[37], 8, 8, WHITE);
      
      //dodge dragon attck
      drawString(24,35,NULL,0,dodge_dragon_attacks);
      arduboy.drawBitmap(40,45,snow_flake_1, 16, 16, WHITE);
      arduboy.drawBitmap(64,55,acid_spit_1, 15, 2, WHITE);
      arduboy.drawBitmap(101,45,rayne_r1, 8, 8, WHITE);      
      
      if ((arduboy.pressed(A_BUTTON) && !a_button_down) ||  (arduboy.pressed(B_BUTTON) && !b_button_down)){ 
        a_button_down=true;
        b_button_down=true;
        gameState = STATE_TITLE_SCREEN;
      }
      if (!arduboy.pressed(A_BUTTON)){
        a_button_down=false;
      }
      if (!arduboy.pressed(B_BUTTON)){
        b_button_down=false;
      }
      break;
    case STATE_BOOSTS_SELECT:
      arduboy.clear();
      
      drawString(52,1,"BOOSTS",0,NULL);

      //build the menu
      arduboy.drawBitmap(16,8,img_objects[15], 8, 8, WHITE);
      drawString(29,9,"500",0,NULL);
      arduboy.drawBitmap(16,16,img_objects[16], 8, 8, WHITE);
      drawString(29,17,"500",0,NULL);
      arduboy.drawBitmap(16,24,img_objects[17], 8, 8, WHITE);
      drawString(27,25,"1000",0,NULL);
      arduboy.drawBitmap(16,32,img_objects[18], 8, 8, WHITE);
      drawString(29,33,"250",0,NULL);
      arduboy.drawBitmap(16,40,img_objects[19], 8, 8, WHITE);
      drawString(29,41,"250",0,NULL);

      arduboy.drawBitmap(48,8,img_objects[25], 8, 8, WHITE);
      drawString(59,9,"2000",0,NULL);
      arduboy.drawBitmap(48,16,img_objects[26], 8, 8, WHITE);
      drawString(59,17,"2000",0,NULL);
      arduboy.drawBitmap(48,24,img_objects[27], 8, 8, WHITE);
      drawString(59,25,"4500",0,NULL);
      arduboy.drawBitmap(48,32,img_objects[28], 8, 8, WHITE);
      drawString(59,33,"1000",0,NULL);
      arduboy.drawBitmap(48,40,img_objects[29], 8, 8, WHITE);
      drawString(59,41,"1000",0,NULL);

      arduboy.drawBitmap(80,8,img_objects[30], 8, 8, WHITE);
      drawString(93,9,"20K",0,NULL);
      arduboy.drawBitmap(80,16,img_objects[31], 8, 8, WHITE);
      drawString(93,17,"20K",0,NULL);
      arduboy.drawBitmap(80,24,img_objects[32], 8, 8, WHITE);
      drawString(93,25,"50K",0,NULL);
      arduboy.drawBitmap(80,32,img_objects[33], 8, 8, WHITE);
      drawString(93,33,"10K",0,NULL);
      arduboy.drawBitmap(80,40,img_objects[34], 8, 8, WHITE);
      drawString(93,41,"10K",0,NULL);

      drawString(32,49,"PRESS B TO START",0,NULL);

      if (arduboy.pressed(UP_BUTTON) && !up_button_down){
        up_button_down=true;
        button_on--;
        if(button_on<0){button_on=0;}
      }//MOVE UP
      if (!arduboy.pressed(UP_BUTTON)){
        up_button_down=false;
      }
      if (arduboy.pressed(DOWN_BUTTON) && !down_button_down){
        down_button_down=true;
        button_on++;
        if(button_on>14){button_on=14;}
      }//MOVE DOWN
      if (!arduboy.pressed(DOWN_BUTTON)){
        down_button_down=false;
      }
      if (arduboy.pressed(LEFT_BUTTON) && !left_button_down){
        left_button_down=true;
        if(button_on>4){
          button_on-=5;
        }
      }//MOVE LEFT
      if (!arduboy.pressed(LEFT_BUTTON)){
        left_button_down=false;
      }
      if (arduboy.pressed(RIGHT_BUTTON) && !right_button_down){
        right_button_down=true;
        if(button_on<10){
          button_on+=5;
        }
      }//MOVE RIGHT
      if (!arduboy.pressed(RIGHT_BUTTON)){
        right_button_down=false;
      }
      
      //draw cursor 18,55,92
      if(button_on<5){
        arduboy.drawRect(25,button_on%5*8+7,21,9,WHITE);
      }else if(button_on>9){
        arduboy.drawRect(88,button_on%5*8+7,21,9,WHITE);
      }else{
        arduboy.drawRect(56,button_on%5*8+7,21,9,WHITE);
      }
      drawHUD(true);

      if ((arduboy.pressed(A_BUTTON) && !a_button_down)){        
        a_button_down=true;
        //buy highlighted item if player has enough copper
        if(button_on==0){//small cold resist potion
          if(files[slot].game_data.copper>=500 && player.cold+1<100){
            tunes.tone(523, 250);
            files[slot].game_data.copper-=500;
            player.cold++;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==1){//small acid resist potion
          if(files[slot].game_data.copper>=500 && player.acid+1<100){  
            //tunes.tone(523, 250); 
            tunes.tone(523, 150);     
            files[slot].game_data.copper-=500;
            player.acid++;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==2){//small luck potion
          if(files[slot].game_data.copper>=1000 && player.luck+1<100){  
            //tunes.tone(523, 250); 
            tunes.tone(523, 150);     
            files[slot].game_data.copper-=1000;
            player.luck++;
          }else{
            //unes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==3){//small invisibility potion
          if(files[slot].game_data.copper>=250 && player.invisibility+2<100){   
            //tunes.tone(523, 250);    
            tunes.tone(523, 150); 
            files[slot].game_data.copper-=250;
            player.invisibility+=2;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==4){//small speed boost potion
          if(files[slot].game_data.copper>=250 && player.speed_boost+2<100){   
            //tunes.tone(523, 250);   
            tunes.tone(523, 150);  
            files[slot].game_data.copper-=250;
            player.speed_boost+=2;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==5){//large cold resist potion
          if(files[slot].game_data.copper>=2000 && player.cold+5<100){
            //tunes.tone(523, 250);
            tunes.tone(523, 150); 
            files[slot].game_data.copper-=2000;
            player.cold+=5;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==6){//large acid resist potion
          if(files[slot].game_data.copper>=2000 && player.acid+5<100){   
            //tunes.tone(523, 250); 
            tunes.tone(523, 150);    
            files[slot].game_data.copper-=2000;
            player.acid+=5;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==7){//large luck potion
          if(files[slot].game_data.copper>=4500 && player.luck+5<100){   
            //tunes.tone(523, 250);  
            tunes.tone(523, 150);   
            files[slot].game_data.copper-=4500;
            player.luck+=5;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==8){//large invisibility potion
          if(files[slot].game_data.copper>=1000 && player.invisibility+10<100){   
            //tunes.tone(523, 250);  
            tunes.tone(523, 150);   
            files[slot].game_data.copper-=1000;
            player.invisibility+=10;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==9){//large speed boost potion
          if(files[slot].game_data.copper>=1000 && player.speed_boost+10<100){  
            //tunes.tone(523, 250);     
            tunes.tone(523, 150); 
            files[slot].game_data.copper-=1000;
            player.speed_boost+=10;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==10){//ring of cold resist 
          if(files[slot].game_data.copper>=20000 && player.cold+1<100){
            //tunes.tone(523, 250);
            tunes.tone(523, 150); 
            files[slot].game_data.copper-=20000;
            files[slot].game_data.cold+=1;
            player.cold+=1;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==11){//ring of acid resist 
          if(files[slot].game_data.copper>=20000 && player.acid+1<100){   
            //tunes.tone(523, 250);
            tunes.tone(523, 150);     
            files[slot].game_data.copper-=20000;
            files[slot].game_data.acid+=1;
            player.acid+=1;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==12){//ring of luck 
          if(files[slot].game_data.copper>=50000 && player.luck+1<100){    
            //tunes.tone(523, 250); 
            tunes.tone(523, 150);   
            files[slot].game_data.copper-=50000;
            files[slot].game_data.luck+=1;
            player.luck+=1;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==13){//ring of invisibility 
          if(files[slot].game_data.copper>=10000 && player.invisibility+2<100){  
            //tunes.tone(523, 250);  
            tunes.tone(523, 150);    
            files[slot].game_data.copper-=10000;
            files[slot].game_data.invisibility+=1;
            player.invisibility+=2;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }else if(button_on==14){//ring of speed boost 
          if(files[slot].game_data.copper>=10000 && player.speed_boost+2<100){   
            //tunes.tone(523, 250);    
            tunes.tone(523, 150); 
            files[slot].game_data.copper-=10000;
            files[slot].game_data.speed_boost+=1;
            player.speed_boost+=2;
          }else{
            //tunes.tone(256, 128);
            tunes.tone(256, 28);
          }
        }
      }
      if ((arduboy.pressed(B_BUTTON) && !b_button_down)){ 
        b_button_down=true;
        bool result = true;
        if(no_save==false){
          result = files[slot].Save();
        }
        if(result==true){
          levelInit();
        }
      }
      if (!arduboy.pressed(A_BUTTON)){
        a_button_down=false;
      }
      if (!arduboy.pressed(B_BUTTON)){
        b_button_down=false;
      }
      
      break;
    case STATE_LEVEL_START://we want to show level # for 3 secs then start
      arduboy.clear();        
      if(slot==1 && cave>1){
        drawString(0,56,"QUIT",0,NULL);
        drawString(96,56,"CONTINUE",0,NULL);
        if ((arduboy.pressed(LEFT_BUTTON) && !left_button_down)){ 
          left_button_down=true;
          //ADD SCORE TO COPPER
          files[slot].game_data.copper+=score;
          if(files[slot].game_data.copper>4000000000){
            files[slot].game_data.copper=4000000000;
          }
          if(!no_save){
            files[slot].Save();      
          } 
          gameState = STATE_GAME_OVER;  
        }
        if (!arduboy.pressed(LEFT_BUTTON)){
          left_button_down=false;
        }
        if ((arduboy.pressed(B_BUTTON) && !b_button_down)){ 
          b_button_down=true;
          gameState = STATE_GAME_PLAYING;
        }
        if (!arduboy.pressed(B_BUTTON)){
          b_button_down=false;
        }
      }else{
        level_start_delay+=dt;
        if(level_start_delay>=2000){
          level_start_delay=0;
          gameState = STATE_GAME_PLAYING;
        }
      }
      drawString(52,25,"CAVE",0,NULL);
      char sbuffer[10];      
      itoa(cave, sbuffer, 10); 
      drawString(76,25,sbuffer,2,NULL);
      if(cave==5){
        drawString(14,35,NULL,0,collect_key);
      }
      break;
    case STATE_GAME_PLAYING:
      arduboy.clear();
      gameLoop(dt); 
      break;  
    case STATE_GAME_OVER:
      arduboy.clear();
      drawString(46,25,"GAME OVER",0,NULL);
      
      drawString(46,35,"CAVE",0,NULL);
      char tcbuffer[10];      
      itoa(cave, tcbuffer, 10); 
      drawString(74,35,tcbuffer,2,NULL);
      
      drawString(34,45,"SCORE",0,NULL);
      char sgobuffer[10];      
      ltoa(score, sgobuffer, 10); 
      drawString(58,45,sgobuffer,10,NULL);
      
      if ( (arduboy.pressed(A_BUTTON) && !a_button_down) || (arduboy.pressed(B_BUTTON) && !b_button_down) ){ 
        a_button_down = true;
        b_button_down = true;
        gameState=STATE_TITLE_SCREEN;
      }  
      if (!arduboy.pressed(A_BUTTON)){a_button_down = false;}
      if (!arduboy.pressed(B_BUTTON)){b_button_down = false;}
      break;
  }   
  arduboy.display();  
}

void levelInit(){
  //clear all entities aqnd objects
  for(int8_t i=0;i<totalObjects;i++){
    objects[i].Exists=false;
  }
  for (int8_t i = 0; i < totalProjectiles; i++){
    projectiles[i].Exists=false;
  }     
  for (int8_t i = 0; i < totalEntities; i++){
    entities[i].Exists=false;
  }
  //initialize player
  player.x=120;
  player.y=48;
  player.lastX=120;
  player.lastY=48;
  player.life=1;
  player.Direction=DIR_LEFT;
  player.frame=0;
  player.key=false;
  player.invisibility_active=false; 
  player.invisibilityTime=0;

  //set frame rate
  uint8_t tcave = (cave>100) ? 100 : cave;
  arduboy.setFrameRate(28+tcave*2);
  
  //choose the dragon randomly
  createEntities(random(1,3),0,random(0,48));
  if(pgm_read_byte(&rarities[(cave-1)*5])>=2){
    createEntities(random(1,3),0,random(0,48));
  }
  if(pgm_read_byte(&rarities[(cave-1)*5])>=3){
    createEntities(random(1,3),0,random(0,48));
  }

  //choose the treasures
  int8_t treasures_created = 0;
  do{
    int8_t roll = random(0,100);
    uint8_t treasure = 0;
    if(pgm_read_byte(&rarities[(cave-1+player.luck)*5+1])>0 && roll>=0 && roll<pgm_read_byte(&rarities[(cave-1+player.luck)*5+1])){//common item
      treasure = random(0,5);
    }else if(pgm_read_byte(&rarities[(cave-1+player.luck)*5+2])>0 && roll>=pgm_read_byte(&rarities[(cave-1+player.luck)*5+1]) && roll<pgm_read_byte(&rarities[(cave-1+player.luck)*5+2])){//uncommon item
      treasure = random(5,10);
    }else if(pgm_read_byte(&rarities[(cave-1+player.luck)*5+3])>0 && roll>=pgm_read_byte(&rarities[(cave-1+player.luck)*5+2]) && roll<pgm_read_byte(&rarities[(cave-1+player.luck)*5+3])){//rare item
      if(random(0,100)>=99){
        treasure = random(15,20);
      }else{
        treasure = random(10,15);
      }
    }else if(pgm_read_byte(&rarities[(cave-1+player.luck)*5+4])>0 && roll>=pgm_read_byte(&rarities[(cave-1+player.luck)*5+3]) && roll<pgm_read_byte(&rarities[(cave-1+player.luck)*5+4])){//ultra rare item
      if(random(0,100)>=99){
        treasure = random(25,30);
      }else{
        treasure = random(20,25);
      }
    }
    //determine x an y coords and make sure they havent been chosen yet
    bool object_created=false;
    uint8_t tx = 0;
    uint8_t ty = 0;
    do{
      tx = random(5,13);
      ty = random(0,7);
      if(!objectExists(tx,ty)){
        createObject(treasure,tx,ty);
        object_created=true;
      }
    }while(!object_created);
    treasures_created++;
  }while(treasures_created<8);
  
  //createObject(random(0,30),random(3,13),random(0,7));

  //if above level 5 create the key
  if(cave>=5){
    createObject(35,4,random(0,7));
  }
  
  //create exit - locked if past above cave 20
  createObject(36,15,0);//exit

  //change game state to start level
  gameState = STATE_LEVEL_START;
}

void gameLoop(int32_t dt){    
  updatePlayer(dt);
  if(player.life>0){
    updateProjectiles(dt);
    updateEntities(dt);
  }
  drawObjects();
  drawProjectiles();
  drawEntities();
  drawPlayer();
  drawHUD(false);
  //arduboy.setFrameRate(gameSpeed);
}

void updatePlayer(int32_t dt){
  if(player.life>0){
    if(player.speed_boost_active){
      player.speedTime -= dt;
      if(player.speedTime<0){
        player.speed_boost_active=false;
      }
    }

    if(player.invisibility_active){
      player.invisibilityTime -= dt;
      if(player.invisibilityTime<0){
        player.invisibility_active=false;
      }
    }
  
    if (arduboy.pressed(UP_BUTTON)){
      if(player.speed_boost_active){
        player.y-=3;
      }else{
        player.y-=2;
      }
      player.moving=true;
    }//MOVE UP
    if (arduboy.pressed(DOWN_BUTTON)){
      if(player.speed_boost_active){
        player.y+=3;
      }else{
        player.y+=2;
      }
      player.moving=true;
    }//MOVE DOWN
    if (arduboy.pressed(LEFT_BUTTON)){ 
      if(player.speed_boost_active){
        player.x-=3;
      }else{
        player.x-=2;
      }
      player.Direction=DIR_LEFT;
      player.moving=true;
    }//MOVE LEFT
    if (arduboy.pressed(RIGHT_BUTTON)){ 
      if(player.speed_boost_active){
        player.x+=3;
      }else{
        player.x+=2;
      }
      player.Direction=DIR_RIGHT;
      player.moving=true;
    }//MOVE RIGHT
    if (!arduboy.pressed(RIGHT_BUTTON) && !arduboy.pressed(LEFT_BUTTON) && !arduboy.pressed(DOWN_BUTTON) && !arduboy.pressed(UP_BUTTON)){
      player.moving=false;
    }//MOVE RIGHT
    if (arduboy.pressed(A_BUTTON) && !a_button_down && player.invisibility>0){
      a_button_down=true;
      player.invisibility--; 
      player.invisibility_active=true; 
      player.invisibilityTime=10000;
    }//INVISIBILY IF POTION OR RING IN BELT
    if (arduboy.pressed(B_BUTTON) && !b_button_down && player.speed_boost>0){
      b_button_down=true; 
      player.speed_boost--; 
      player.speed_boost_active=true; 
      player.speedTime=2000;
    }//SPEED BOOST IS POTION OR RING IN BELT
    if (!arduboy.pressed(A_BUTTON)){
      a_button_down = !a_button_down;
    }
    if (!arduboy.pressed(B_BUTTON)){
      b_button_down = !b_button_down; 
    }//SPEED BOOST IS POTION OR RING IN BELT
  }

  
  
  //check game boundries
  if(player.x>120){player.x=120;}
  if(player.x<16){player.x=16;}
  if(player.y>48){player.y=48;}
  if(player.y<0){player.y=0;}

  //unless player is invisible update last know locations
  if(!player.invisibility_active){
    player.lastX=player.x;
    player.lastY=player.y;
  }

  //set bound box
  player.minX=player.x+2;
  player.minY=player.y+1;
  player.maxX=player.x+6;
  player.maxY=player.y+7; 
  //did i collide with an object
  if(player.life>0){
    objectCollision();
  }

  if (player.damageTime > 0){
    player.damageTime -= dt;
    player.damageBlink = !player.damageBlink;
  }else{
    player.damageBlink=false;
  }

  if (player.life==0){
    if (player.animation != RAYNE_ANIMATION_DEATH){
        player.animation = RAYNE_ANIMATION_DEATH;
        player.frame_count = RAYNE_ANIMATION_DEATH_FRAMES;
        player.frame_delay_ms = RAYNE_ANIMATION_DEATH_DELAY_MS;
        player.animationEnded = false;
        player.frame = 0;
    }
  }else if(player.moving==true){
    if (player.animation != RAYNE_ANIMATION_RUN){
        player.animation = RAYNE_ANIMATION_RUN;
        player.frame_count = RAYNE_ANIMATION_RUN_FRAMES;
        player.frame_delay_ms = RAYNE_ANIMATION_RUN_DELAY_MS;
        player.animationEnded = false;
        player.frame = 0;
    }
  }else{
    player.animation = 0;
    player.animationEnded = true;
    player.frame = 0;
  }

  //determine image to view
  if (!player.animationEnded){
    player.frameTime += dt;
    if (player.frameTime >= player.frame_delay_ms){
      player.frameTime = 0;
      ++player.frame;
      
      if (player.frame >= player.frame_count){
        if(player.animation == RAYNE_ANIMATION_DEATH){   
          if(slot==1){
            files[slot].game_data.copper=0;
            files[slot].game_data.cold=0;
            files[slot].game_data.acid=0;
            files[slot].game_data.luck=0;
            files[slot].game_data.invisibility=0;
            files[slot].game_data.speed_boost=0;            
          }else{
            //ADD SCORE TO COPPER
            files[slot].game_data.copper+=score;
            if(files[slot].game_data.copper>4000000000){
              files[slot].game_data.copper=4000000000;
            }
          }
          if(no_save==false){
            files[slot].Save();       
          }
          gameState = STATE_GAME_OVER;     
        }else{
          player.frame = 0;     
          player.animation=0;    
          player.animationEnded = true; 
        }
      }
    }
  }

  if(player.x==120 && player.y==0 && player.life>0){
    if(cave>=5 && player.key || cave<5){    
      cave++;
      levelInit();           
    }
  }

  player.seen=false;
}

bool Collision(int16_t min1X, int16_t min1Y, int16_t max1X, int16_t max1Y, int16_t min2X, int16_t min2Y, int16_t max2X, int16_t max2Y){
  return (max2X>=min1X && min2X<=max1X && max2Y>=min1Y && min2Y<=max1Y);
}

bool objectExists(int8_t tx, int8_t ty){
  for(int8_t i=0;i<totalObjects;i++){
    if(objects[i].Exists){  
      if(objects[i].x==tx*8 && objects[i].y==ty*8){
        return true;
      }
    }
  }
  return false;
}

void objectCollision(){   
  for(int8_t i=0;i<totalObjects;i++){
    if(objects[i].Exists){     
      if(Collision(objects[i].minX,objects[i].minY,objects[i].maxX,objects[i].maxY,player.minX,player.minY,player.maxX,player.maxY)){
        //dragons know where you are when you pick up an item, they see it disappear
        player.lastX=player.x;
        player.lastY=player.y;
        //there is a chance the dragon will start hunting you when you pick up a treasure
        player.seen=true;
        if(objects[i].type<36){//doors
          //tunes.tone(523, 250);
          tunes.tone(223, 50);
        }
        switch(objects[i].type){
          case 0://copper coin
            score+=1;
            objects[i].Exists=false;
            break;
          case 1://silver coin
            score+=10;
            objects[i].Exists=false;
            break;
          case 2://bag of copper coins
            score+=10;
            objects[i].Exists=false;
            break;
          case 3://pouch of copper coins
            score+=20;
            objects[i].Exists=false;
            break;
          case 4://candleabra coin
            score+=25;
            objects[i].Exists=false;
            break;
          case 5://chest of copper coins
            score+=50;
            objects[i].Exists=false;
            break;
          case 6://gold coin
            score+=100;
            objects[i].Exists=false;
            break;
          case 7://bag of silver coins
            score+=100;
            objects[i].Exists=false;
            break;
          case 8://pouch of silver coins
            score+=200;
            objects[i].Exists=false;
            break;
          case 9://chest of silver coins
            score+=500;
            objects[i].Exists=false;
            break;
          case 10://Harp
            score+=750;
            objects[i].Exists=false;
            break;
          case 11://Platinum coin
            score+=1000;
            objects[i].Exists=false;
            break;
          case 12://bag of gold coins
            score+=1000;
            objects[i].Exists=false;
            break;
          case 13://pouch of gold coins
            score+=2000;
            objects[i].Exists=false;
            break;
          case 14://chest of gold coins
            score+=5000;
            objects[i].Exists=false;
            break;
          case 15://Potion of cold resist   
            player.cold++;if(player.cold>99){player.cold=99;}     
            objects[i].Exists=false;
            break;
          case 16://Potion of acid resist  
            player.acid++;if(player.acid>99){player.acid=99;}
            objects[i].Exists=false;
            break;
          case 17://Potion of luck   
            player.luck++;if(player.luck>99){player.luck=99;}
            objects[i].Exists=false;
            break;
          case 18://Potion of invisibility   
            player.invisibility+=2;if(player.invisibility>99){player.invisibility=99;}
            objects[i].Exists=false;
            break;
          case 19://Potion of speed
            player.speed_boost+=2;if(player.speed_boost>99){player.speed_boost=99;}
            objects[i].Exists=false;
            break;
          case 20://lamp
            score+=7500;
            objects[i].Exists=false;
            break;
          case 21://bag of platinum coins
            score+=10000;
            objects[i].Exists=false;
            break;
          case 22://pouch of platinum coins
            score+=20000;
            objects[i].Exists=false;
            break;
          case 23://diamond
            score+=25000;
            objects[i].Exists=false;
            break;
          case 24://chest of platinum coins
            score+=50000;
            objects[i].Exists=false;
            break;
          case 25://Large Potion of cold resist
            player.cold+=5;if(player.cold>99){player.cold=99;}
            objects[i].Exists=false;
            break;
          case 26://Large Potion of acid resist 
            player.acid+=5;if(player.acid>99){player.acid=99;}
            objects[i].Exists=false;
            break;
          case 27://Large Potion of luck 
            player.luck+=5;if(player.luck>99){player.luck=99;}
            objects[i].Exists=false;
            break;
          case 28://Large Potion of invisibility 
            player.invisibility+=10;if(player.invisibility>99){player.invisibility=99;}
            objects[i].Exists=false;
            break;
          case 29://Large Potion of speed  
            player.speed_boost+=10;if(player.speed_boost>99){player.speed_boost=99;}
            objects[i].Exists=false;
            break;
          case 30://Ring of cold resist
            player.cold+=1;if(player.cold>99){player.cold=99;}
            files[slot].game_data.cold++;if(files[slot].game_data.cold>99){files[slot].game_data.cold=99;}
            objects[i].Exists=false;
            break;
          case 31://Ring of acid resist 
            player.acid+=1;if(player.acid>99){player.acid=99;}
            files[slot].game_data.acid++;if(files[slot].game_data.acid>99){files[slot].game_data.acid=99;}
            objects[i].Exists=false;
            break;
          case 32://Ring of luck 
            player.luck+=1;if(player.luck>99){player.luck=99;}
            files[slot].game_data.luck++;if(files[slot].game_data.luck>99){files[slot].game_data.luck=99;}
            objects[i].Exists=false;
            break;
          case 33://Ringn of invisibility 
            player.invisibility+=2;if(player.invisibility>99){player.invisibility=99;}
            files[slot].game_data.invisibility++;if(files[slot].game_data.invisibility>99){files[slot].game_data.invisibility=99;}
            objects[i].Exists=false;
            break;
          case 34://Ring of speed  
            player.speed_boost+=2;if(player.speed_boost>99){player.speed_boost=99;}
            files[slot].game_data.speed_boost++;if(files[slot].game_data.speed_boost>99){files[slot].game_data.speed_boost=99;}
            objects[i].Exists=false;
            break;
          case 35://key
            player.key=true;
            objects[i].Exists=false;
            break;
          case 36://exit
            //if not locked or player has the key then go to next level
            //cave needs to go black and show level # then cave show up with all treasures and dragons and player is the door in lower right again
            if(!objects[i].locked || player.key && objects[i].locked){
              objects[i].Exists=false;
              cave++;
              levelInit();
            }
            break;
        }
      }
    }
  }
}

void createObject(int8_t type, uint8_t x, uint8_t y){
  for (int8_t i = 0; i < totalObjects; i++){
    if (!objects[i].Exists){
      objects[i].Exists=true;
      objects[i].x=x*8;
      objects[i].y=y*8;
      objects[i].minX=x*8;
      objects[i].maxX=x*8+8<0 ? 127 : x*8+8;
      objects[i].minY=y*8;
      objects[i].maxY=y*8+8;
      objects[i].type=type;
      break;
    }
  }
}

void createProjectiles(int8_t type, uint8_t x, uint8_t y){
  for (int8_t i = 0; i < totalProjectiles; i++){
    if (!projectiles[i].Exists){
      projectiles[i].Exists=true;
      projectiles[i].x=x;
      projectiles[i].y=y;
      projectiles[i].type=type;
      break;
    }
  }
}

void createEntities(int8_t type, uint8_t x, uint8_t y){
  for (int8_t i = 0; i < totalEntities; i++){
    if (!entities[i].Exists){
      entities[i].Exists=true;
      entities[i].x=x;
      entities[i].y=y;
      entities[i].type=type;
      entities[i].spd=1;
      break;
    }
  }
}

void updateProjectiles(int32_t dt){
  for (int8_t i = 0; i < totalProjectiles; i++){
    if (projectiles[i].Exists){
      switch(projectiles[i].type){
        case 1://acid spit
          projectiles[i].x++;
          if(projectiles[i].x<0){
            projectiles[i].Exists=false;
          }
          projectiles[i].minX=projectiles[i].x;
          projectiles[i].maxX=projectiles[i].x+16;
          projectiles[i].minY=projectiles[i].y;
          projectiles[i].maxY=projectiles[i].y+2;          
          if(Collision(projectiles[i].minX,projectiles[i].minY,projectiles[i].maxX,projectiles[i].maxY,player.minX,player.minY,player.maxX,player.maxY) && player.damageTime<=0){
            takeDamage(1);
            projectiles[i].Exists=false;
          }
          break;
        case 2://cloud of cold
          projectiles[i].x+=1;
          if(projectiles[i].x<0){
            projectiles[i].Exists=false;
          }
          projectiles[i].minX=projectiles[i].x;
          projectiles[i].maxX=projectiles[i].x+9;
          projectiles[i].minY=projectiles[i].y;
          projectiles[i].maxY=projectiles[i].y+9;      
          if(Collision(projectiles[i].minX,projectiles[i].minY,projectiles[i].maxX,projectiles[i].maxY,player.minX,player.minY,player.maxX,player.maxY) && player.damageTime<=0){
            takeDamage(2);
            projectiles[i].Exists=false;
          }
          break;
      }
      projectiles[i].frameTime += dt;
      if (projectiles[i].frameTime >= FRAME_DELAY_MS){
        projectiles[i].frameTime = 0;
        ++projectiles[i].frame;        
        if (projectiles[i].frame >= 2){
          projectiles[i].frame = 0;           
        }
      }
    }
  }
}

void takeDamage(int8_t type){//player takes 1 point of damage of type X
  if(type==0){//fire damage
    player.life--;
  }else if(type==1){//acid damage
    if(player.acid>0){
      player.acid--;
      player.damageTime = PLAYER_DAMAGE_TIME;
    }else{
      player.life--;
    }
  }else if(type==2){//cold damage
    if(player.cold>0){
      player.cold--;
      player.damageTime = PLAYER_DAMAGE_TIME;
    }else{
      player.life--;
    }
  }
}

void updateEntities(int32_t dt){
  for (int8_t i = 0; i < totalEntities; i++){
    if (entities[i].Exists){
      if(player.seen && !entities[i].hunting){
        if(random(4)==0){//25% chance we want to randomly hunt player down and attack for 3 secs then return to patrolling
          entities[i].hunting=true;
        }
      }
      if(!entities[i].hunting){
        if(random(50)==0 && player.lastX<120){//2% chance we want to randomly hunt player down and attack for 3 secs then return to patrolling
          entities[i].hunting=true;
        }else{
          //randomly stop a change direction 5% of the time if not hunting down player
          if(random(50)==0){
            entities[i].moving_up = entities[i].moving_up ? false : true;
          }
        }
      }
      if(entities[i].hunting){
        if(arduboy.everyXFrames(30)){
          if(random(2)==0){//50% chance in stopping the hunt
            entities[i].hunting=false;
          }
        }
        //move toward last known position of player
        if(entities[i].y<player.lastY){
          entities[i].moving_up=false;
        }else if(entities[i].y>player.lastY){
          entities[i].moving_up=true;
        }
        //if in line of sight fire cloud of cold
        if(entities[i].y-4<player.lastY+8 && entities[i].y+8>player.lastY && entities[i].attacking==0 && player.lastX<120){
          entities[i].attacking=1;
          createProjectiles(entities[i].type,entities[i].x,entities[i].y);
        }
      }
      //the dragon will move up and down 
      if(entities[i].moving_up){
        if(entities[i].hunting){
          entities[i].y-=2;
        }else{
          entities[i].y--;
        }
      }else{
        if(entities[i].hunting){
          entities[i].y+=2;
        }else{
          entities[i].y++;
        }
      }
      if(entities[i].y<0){
        entities[i].y=0;
        entities[i].moving_up=false;
      }
      if(entities[i].y>48){
        entities[i].y=48;
        entities[i].moving_up=true;
      }  

    }
  }
}

void drawPlayer(){
  if (!player.damageBlink){ 
    if(player.animation==RAYNE_ANIMATION_RUN || player.animation==RAYNE_ANIMATION_STAT){ 
      if(player.invisibility_active){
        arduboy.drawBitmap(player.x-2,player.y,invisibility_1, 8, 2, WHITE);  
      }
      if(player.Direction==DIR_LEFT){
        arduboy.drawBitmap(player.x,player.y,rayne_left_ani[player.frame], 8, 8, WHITE);  
      }else{
        arduboy.drawBitmap(player.x,player.y,rayne_right_ani[player.frame], 8, 8, WHITE);  
      }
    }else if(player.animation==RAYNE_ANIMATION_DEATH){
      player.x++;
      arduboy.drawBitmap(player.x,player.y,rayne_death_ani[player.frame], 8, 8, WHITE);  
    }
  }
}

void drawProjectiles(){
  //DRAW DRAGON ACID SPIT, CLOUD OF COLD
  for (int8_t i = 0; i < totalProjectiles; i++){
    if (projectiles[i].Exists){
      switch(projectiles[i].type){
        case 2://cloud of cold
          arduboy.drawBitmap(projectiles[i].x,projectiles[i].y,cold_ani[projectiles[i].frame], 16, 16, WHITE);  
          break;
        case 1://acid spit
          arduboy.drawBitmap(projectiles[i].x,projectiles[i].y,acid_ani[projectiles[i].frame], 15, 2, WHITE);  
          break;
        case 0://fire ball
          arduboy.drawBitmap(projectiles[i].x,projectiles[i].y,fire_ani[projectiles[i].frame], 8, 8, WHITE);  
          break;
      }
    }
  }
}

void drawObjects(){
  //DRAW TREASURE/POTIONS/RINGS
  for (int8_t i = 0; i < totalObjects; i++){
    if (objects[i].Exists){
      if(objects[i].type==36){//exit
        if(!player.key && cave>=5){
          arduboy.drawBitmap(objects[i].x,objects[i].y,img_objects[36], 8, 8, WHITE);
        }else{
          arduboy.drawBitmap(objects[i].x,objects[i].y,img_objects[37], 8, 8, WHITE);
        }
      }else{
        arduboy.drawBitmap(objects[i].x,objects[i].y,img_objects[objects[i].type], 8, 8, WHITE);
      }
    }
  }
}

void drawEntities(){
  //DRAW THE DRAGONS
  for (int8_t i = 0; i < totalEntities; i++){
    if (entities[i].Exists){
      switch(entities[i].type){
        case 0://fire
          if(entities[i].attacking>0){
            arduboy.drawBitmap(entities[i].x,entities[i].y,fire_dragon_2, 16, 8, WHITE);
            entities[i].attacking++; 
            if(entities[i].attacking>20){
              entities[i].attacking=0;
            }
          }else{
            arduboy.drawBitmap(entities[i].x,entities[i].y,fire_dragon_1, 16, 8, WHITE); 
          }
          break;
        case 1:
          if(entities[i].attacking>0){
            arduboy.drawBitmap(entities[i].x,entities[i].y,acid_dragon_2, 16, 8, WHITE);
            entities[i].attacking++; 
            if(entities[i].attacking>20){
              entities[i].attacking=0;
            }
          }else{
            arduboy.drawBitmap(entities[i].x,entities[i].y,acid_dragon_1, 16, 8, WHITE); 
          }
          break;
        case 2:
          if(entities[i].attacking>0){
            arduboy.drawBitmap(entities[i].x,entities[i].y,frost_1, 16, 8, WHITE); 
            entities[i].attacking++; 
            if(entities[i].attacking>20){
              entities[i].attacking=0;
            }
          }else{
            arduboy.drawBitmap(entities[i].x,entities[i].y,frost_2, 16, 8, WHITE); 
          }
          break;
      }
    }
  }
}

void drawCurrency(){
  char sbuffer[10];      
  ltoa(files[slot].game_data.copper, sbuffer, 10); 
  drawString(88,58,sbuffer,10,NULL);   
}

void drawHUD(bool totals){
  if(totals){
    //DRAW CURRENT copper
    char sbuffer[10];      
    ltoa(files[slot].game_data.copper, sbuffer, 10); 
    drawString(88,58,sbuffer,10,NULL); 
  }else{
    //DRAW CURRENT GAME SCORE   
    char sbuffer[10];      
    ltoa(score, sbuffer, 10); 
    drawString(88,58,sbuffer,10,NULL); 
  }

  //draw player stats
  //cold resist
  //arduboy.drawBitmap(1,58,small_fonts[35], 4, 5, 1);  
  drawString(1,58,"C",0,NULL); 
  char cbuffer[3];
  ltoa(player.cold, cbuffer, 10); 
  drawString(5,58,cbuffer,2,NULL); 
 
  //acid resist
  //arduboy.drawBitmap(17,58,small_fonts[33], 4, 5, 1);
  drawString(17,58,"A",0,NULL);  
  char abuffer[3];       
  itoa(player.acid, abuffer, 10); 
  drawString(21,58,abuffer,2,NULL); 
  
  //luck
  //arduboy.drawBitmap(33,58,small_fonts[44], 4, 5, 1); 
  drawString(33,58,"L",0,NULL); 
  char lbuffer[3];     
  itoa(player.luck, lbuffer, 10); 
  drawString(37,58,lbuffer,2,NULL); 

  //invisibilty
  //arduboy.drawBitmap(49,58,small_fonts[41], 4, 5, 1); 
  drawString(49,58,"I",0,NULL); 
  char ibuffer[3];       
  itoa(player.invisibility, ibuffer, 10); 
  drawString(53,58,ibuffer,2,NULL); 

  //speed boost
  //arduboy.drawBitmap(65,58,small_fonts[48], 4, 5, 1); 
  drawString(65,58,"P",0,NULL); 
  char pbuffer[3];      
  itoa(player.speed_boost, pbuffer, 10); 
  drawString(69,58,pbuffer,2,NULL);  
  
  if(slot==1){
    arduboy.drawCircle(81,60,2,1);
  }
}
