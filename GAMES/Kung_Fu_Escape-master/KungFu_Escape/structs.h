//Struct definitions
typedef struct
{
  bool  exist;
  Rect  rect;
  int   x;
  int   y;
  bool  moveEnable;
  int   runAnimFrame;
  bool  enemyDying;
  int   dyingAnimFrame;
  int   spawn;
}Enemy;

typedef struct
{
  bool  exist;
  Rect  rect;
  int   x;
  int   y;
  bool  moveEnable;
  int   runAnimFrame;
  bool  enemyDying;
  int   dyingAnimFrame;
  int   spawn;
}Dog;



//Structs for enemy variables
Enemy enemy[5] = {
  { // [0] enemy0
     .exist = false,
     .rect = {0, 0, 0, 0},
     .x = enemySpawnRightX,
     .y = ground,
     .moveEnable = true,
     .runAnimFrame = 0,
     .enemyDying = false,
     .dyingAnimFrame = 0,
     .spawn = 0
  },
  
  { // [1] enemy1
     .exist = false,
     .rect = {0, 0, 0, 0},
     .x = enemySpawnLeftX,
     .y = ground,
     .moveEnable = true,
     .runAnimFrame = 0,
     .enemyDying = false,
     .dyingAnimFrame = 0,
     .spawn = 0
  },
  
  { // [2] enemy2
     .exist = false,
     .rect = {0, 0, 0, 0},
     .x = enemySpawnRightX,
     .y = ground,
     .moveEnable = true,
     .runAnimFrame = 0,
     .enemyDying = false,
     .dyingAnimFrame = 0,
     .spawn = 0
  },
  
  { // [4] enemy3
     .exist = false,
     .rect = {0, 0, 0, 0},
     .x = enemySpawnLeftX,
     .y = ground,
     .moveEnable = true,
     .runAnimFrame = 0,
     .enemyDying = false,
     .dyingAnimFrame = 0,
     .spawn = 0
  },

  { // [5] enemy4
     .exist = false,
     .rect = {0, 0, 0, 0},
     .x = enemySpawnLeftX,
     .y = ground,
     .moveEnable = true,
     .runAnimFrame = 0,
     .enemyDying = false,
     .dyingAnimFrame = 0,
     .spawn = 0
  },
};

Dog dog[5] = {
  { // [0] dog0
     .exist = false,
     .rect = {0, 0, 0, 0},
     .x = enemySpawnRightX,
     .y = ground,
     .moveEnable = true,
     .runAnimFrame = 0,
     .enemyDying = false,
     .dyingAnimFrame = 0,
     .spawn = -2000
  },
  
  { // [1] dog1
     .exist = false,
     .rect = {0, 0, 0, 0},
     .x = enemySpawnLeftX,
     .y = ground,
     .moveEnable = true,
     .runAnimFrame = 0,
     .enemyDying = false,
     .dyingAnimFrame = 0,
     .spawn = -2000
  },
  
  { // [2] dog2
     .exist = false,
     .rect = {0, 0, 0, 0},
     .x = enemySpawnRightX,
     .y = ground,
     .moveEnable = true,
     .runAnimFrame = 0,
     .enemyDying = false,
     .dyingAnimFrame = 0,
     .spawn = -2000
  },
  
  { // [4] dog3
     .exist = false,
     .rect = {0, 0, 0, 0},
     .x = enemySpawnLeftX,
     .y = ground,
     .moveEnable = true,
     .runAnimFrame = 0,
     .enemyDying = false,
     .dyingAnimFrame = 0,
     .spawn = -2000
  },

  { // [5] dog4
     .exist = false,
     .rect = {0, 0, 0, 0},
     .x = enemySpawnLeftX,
     .y = ground,
     .moveEnable = true,
     .runAnimFrame = 0,
     .enemyDying = false,
     .dyingAnimFrame = 0,
     .spawn = -2000
  },
};
