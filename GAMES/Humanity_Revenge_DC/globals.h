#define HUD_SIZE                  60          // * 10

#define MIN_SCREEN_Y              0           
#define MIN_SCREENHUD_Y           (MIN_SCREEN_Y + HUD_SIZE)
#define MAX_SCREEN_Y              630
#define MIN_SCREEN_X              0
#define MAX_SCREEN_X              1270

// SHIP
#define MIN_SHIP_X                0           // * 10
#define MAX_SHIP_X                1200
#define MIN_SHIP_Y                MIN_SCREENHUD_Y              
#define MAX_SHIP_Y                560

#define SHIP_DEFAULT_X            40
#define SHIP_DEFAULT_Y            320

#define INITIAL_SHIPSTATUS        0b00001111
#define INITIAL_SHIPSTATUS_2      0b00000000

#define SHIP_MAX_LIVES            3
#define SHIP_MAX_BOMBS            3

#define SHIP_DAMAGE               4

// ship movement type
#define MOVE_UP                   1
#define MOVE_DOWN                 2
#define MOVE_LEFT                 3
#define MOVE_RIGHT                4

// ship type
#define SHIP_ODINO                0
#define SHIP_THOR                 1
#define SHIP_FREYR                2
// #define SHIP_LOKI                 3
// END SHIP

// SHIP WEAPON
#define GUN                        0
#define ALTERNATE_UPDOWN_GUN       1
#define SMALL_BOMB                 2
#define LASER                      3
#define ALTERNATE_GUN              4
#define SPLIT_GUN                  5

#define MIN_SHOOT_Y                0              // * 10
#define MAX_SHOOT_Y                630
#define MIN_SHOOT_X                90
#define MAX_SHOOT_X                1270

#define SHOOT_GUN                  0
#define SHOOT_LASER                1
#define SHOOT_SMALL_BOMB           2
#define SHOOT_GUN_UP               3
#define SHOOT_GUN_DOWN             4

#define GUN_DAMAGE                 1
#define GUN_SIZE                   4
#define GUN_INTERVAL               10
#define ALTERNATE_GUN_INTERVAL     8
#define SPLIT_GUN_INTERVAL         12

#define LASER_DAMAGE               4
#define LASER_SIZE                 5
#define LASER_INTERVAL             15

#define SMALL_BOMB_DAMAGE          2
#define SMALL_BOMB_SIZE            3
#define SMALL_BOMB_INTERVAL        8

#define MAX_SHOOT_NUMBER           18

#define INITIAL_WEAPONSTATUS       0b00000000;
#define INITIAL_SHOOTSTATUS        0b00000000;
// END SHIP WEAPON

// ENEMY AND ENEMY SHOOTS
#define EASY_ENEMY                 0            // enemy class
#define MEDIUM_ENEMY               1

#define EASY_ENEMY_SCORE           1
#define MEDIUM_ENEMY_SCORE         4

#define EASY_ENEMY_LIFE            0            // +1
#define MEDIUM_ENEMY_LIFE          3

#define EASY_TYPE_SIMPLE           0            // enemy type
#define EASY_TYPE_SINUSOIDE        1
#define EASY_TYPE_KAMIKAZE         2
#define EASY_TYPE_MISSILE          3

#define MEDIUM_TYPE_SIMPLE         0
#define MEDIUM_TYPE_SINUSOIDE      1

#define MOVEMENT_TYPE_SIMPLE       0
#define MOVEMENT_TYPE_SINUSOIDE    1
#define MOVEMENT_TYPE_KAMIKAZE     2
#define MOVEMENT_TYPE_MISSILE      3

#define INITIAL_ENEMY_STATUS       0b00000000

#define INITIAL_SHOOT_STATUS       0b00000000
// END ENEMY AND ENEMY SHOOTS

// BOMB
#define INITIAL_BOMB_STATUS        0b00011111
#define BOMB_DAMAGE                5
// END BOMB

// ENEMY MANAGER
#define MAX_ENEMIES                16
#define MAX_ENEMIES_SHOOTS         10
// END ENEMY MANAGER

// EXPLOSIONS 
#define MAX_EXPLOSIONS             16
#define INITIAL_EXPLOSION_STATUS   0b00000000

#define EXPLOSION_SMALL            3
#define EXPLOSION_MEDIUM           5
// END EXPLOSIONS

// BACKGROUND
#define BACKGROUND_STAR_NUM         6
// END BACKGROUND

// ITEMS
#define ITEM_1UP                    1
#define ITEM_BOMB                   2
#define ITEM_SHIELD                 3

#define INITIAL_ITEM_STATUS         0b00000000

#define MAX_ITEMS                   1

#define ITEM_DURATION               10
// END ITEMS

// BOSS
#define INITIAL_BOSS_STATUS            0b00000000
#define INITIAL_BOSS_PART_STATUS       0b00000000

#define MAX_BOSS_PARTS                 5

#define BOSS_WINGS                     1
#define BOSS_SWORDS                    2
#define BOSS_TYSON                     3

// parts
#define BOSS_WINGS_UP                  1
#define BOSS_WINGS_DOWN                2

#define BOSS_SWORDS_UP                 3
#define BOSS_SWORDS_MAIN               4
#define BOSS_SWORDS_DOWN               5

#define BOSS_TYSON_MAIN                6
#define BOSS_TYSON_UP_FRONT            7
#define BOSS_TYSON_UP_BACK             8
#define BOSS_TYSON_DOWN_FRONT          9
#define BOSS_TYSON_DOWN_BACK           10
// end parts

#define BOSS_WINGS_START_X             116
#define BOSS_WINGS_START_Y             28
#define BOSS_WINGS_PART_LIFE           30
#define BOSS_WINGS_PART_SCORE          30

#define BOSS_SWORDS_START_X            108
#define BOSS_SWORDS_START_Y            34
#define BOSS_SWORDS_PART_LIFE          35
#define BOSS_SWORDS_PART_SCORE         35

#define BOSS_TYSON_START_X             108
#define BOSS_TYSON_START_Y             28
#define BOSS_TYSON_PART_LIFE           40
#define BOSS_TYSON_PART_SCORE          50

#define BOSS_MOVEMENT_UPDOWN           0
#define BOSS_MOVEMENT_LEFTRIGHT        1
#define BOSS_MOVEMENT_LEFTRIGHT_V      2
// END BOSS
