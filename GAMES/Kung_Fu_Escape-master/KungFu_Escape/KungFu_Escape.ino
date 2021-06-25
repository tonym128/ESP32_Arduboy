#include <Arduboy2.h>
#include <ArduboyTones.h>
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
#define BACKGROUND_WIDTH    9
#define BACKGROUND_HEIGHT   4
#define TILE_SIZE           16
// make an ArdBitmap instance that will use the given the screen buffer and dimensions
#define ARDBITMAP_SBUF arduboy.getBuffer()
#include <ArdBitmap.h>
ArdBitmap<WIDTH, HEIGHT> ardbitmap;


//Program variables
int ground = 32;
bool playerMoveEnable = true;
//Player position variables
int playerX = 64;
int playerY = 32;
//Health variables
float health = 100;
float enemyAttack = 0.5;
//enemy position variables
int enemySpawnFreq;
int enemySpawnRightX = 134;
int enemySpawnLeftX = -16;

//Background position variables
int bgX = 0;
int bgY = 0;
int spawnX = 0; //Used for spawning AND bg stuff
int levelProg = 0;//used to determine when to draw exit
int middleHighTileY = 16;
int middleLowTileY = 32;
int floorTileY = 48;
int ceilingTileX = 0;
int ceilingTileY = 0;

int playerSpeed = 2;
int gameState = 0;
int level = 1;


//Animation variables

int runAnimFrame = 0;
int punchAnimFrame = 0;
int kickAnimFrame = 0;
bool fallingRight = false;
bool fallingLeft = false;
bool playerRunLeft = false;
bool playerRunRight = false;
bool playerFaceRight = true;
bool playerFlyKickRight = false;
bool playerFlyKickLeft = false;
bool playerKickRight = false;
bool playerKickLeft = false;
bool playerCrouchRight = false;
bool playerCrouchLeft = false;
bool playerCrouchPunchRight = false;
bool playerCrouchPunchLeft = false;

//Collision detection variable
Rect playerRect;
Rect playerKickRect;

//Game state variables
#define GAME_TITLE        0
#define LEVEL_SCREEN      1
#define GAME_PLAY         2
#define GAME_OVER         3
#define GAME_HIGH         4
#define GAME_FINISH       5

#include "bitmaps.h"
#include "structs.h"

void inogamesetup() 
{
  arduboy.setTextSize(1);
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(20);
  arduboy.clear();

}

void inogameloop() 
{
  // put your main code here, to run repeatedly:
  delay(0);
  if (!(arduboy.nextFrame()))
  {
    return;
  }
  arduboy.clear();

  switch(gameState)
  {
    case GAME_TITLE:
            //Title screen stuff
            Titlescreen();
            break;
    case LEVEL_SCREEN:
            //Level screen
            LevelScreen();
            break;
    case GAME_PLAY: 
            //Gameplay stuff
            GamePlay();
            break;
    case GAME_OVER: 
            //Game over screen
            GameOverScreen(false);
            break;
    case GAME_FINISH:
            //Game finished screen
            GameOverScreen(true);
   }
  arduboy.display();
}


void Titlescreen()
{ 
 ardbitmap.drawCompressed(WIDTH / 2, HEIGHT / 2, StickmenTitle, WHITE, ALIGN_CENTER, MIRROR_NONE);
  if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON))
  {
    arduboy.audio.on();
    sound.tones(Kung_Fu);
    gameState = LEVEL_SCREEN;
    delay(1000);
    ResetVariables();
  }
}

void DrawInitialScreen()
{
   //Draw initial screen
    for (int x = 0; x< 9; x++)
    {
      displayFloorTiles[x] = dungeonTileMap[30];
      displayMiddleLowTiles[x] = dungeonTileMap[11];
      displayMiddleHighTiles[x] = dungeonTileMap[11];
      displayCeilingTiles[x] = dungeonTileMap[30];
    }
}

void LevelScreen()
{
  arduboy.audio.on();
  if (level == 1)
  {
    arduboy.setCursor(45, 20);
    arduboy.print("Dungeon");
    enemy[0].spawn = -20;
    enemy[1].spawn = -90;
    enemy[2].spawn = -142;
    enemy[3].spawn = -228;
    enemy[4].spawn = -314;
    enemySpawnFreq = 400;
    ResetVariables(); 
    DrawInitialScreen();
  }
    if (level == 2)
    {
      arduboy.setCursor(45, 20);
      arduboy.print("Garden");
      enemy[0].spawn = -10;
      enemy[1].spawn = -70;
      enemy[2].spawn = -102;
      enemy[3].spawn = -188;
      enemy[4].spawn = -188;
      dog[0].spawn -200;
      enemySpawnFreq = 300;
      ResetVariables();
      DrawInitialScreen();
    }

    if (level == 3)
    {
      arduboy.setCursor(30, 20);
      arduboy.print("First Floor");
      enemy[0].spawn = -10;
      enemy[1].spawn = -50;
      enemy[2].spawn = -92;
      enemy[3].spawn = -130;
      enemy[4].spawn = -100;
      dog[0].spawn = -100;
      dog[1].spawn = -200;
      enemySpawnFreq = 250;
      ResetVariables();
      DrawInitialScreen();
    }

    if (level == 4)
    {
      arduboy.setCursor(30, 20);
      arduboy.print("Second Floor");
      enemy[0].spawn = -10;
      enemy[1].spawn = -25;
      enemy[2].spawn = -50;
      enemy[3].spawn = -100;
      enemy[4].spawn = -100;
      dog[0].spawn = -50;
      dog[1].spawn = -100;
      dog[2].spawn = -200;
      enemySpawnFreq = 150;
      ResetVariables();
      DrawInitialScreen();
    }

    if (level == 5)
    {
      arduboy.setCursor(45, 20);
      arduboy.print("Rooftop");
      enemy[0].spawn = 0;
      enemy[1].spawn = -10;
      enemy[2].spawn = -20;
      enemy[3].spawn = -30;
      enemy[4].spawn = -40;
      dog[0].spawn = -50;
      dog[1].spawn = -100;
      dog[2].spawn = -150;
      dog[3].spawn = -250;
      dog[4].spawn = -350;
      enemySpawnFreq = 100;
      ResetVariables();
      DrawInitialScreen();
    }

    if (level == 6)
    {
      gameState = GAME_FINISH;
    }

    for (int i = 0; i < 9 ; i++)    //Copy levelTile array to the displayTile array for the DrawLevel() function
    {
      displayTiles[i] = levelTilePointer[level-1][i];
    }
  
  if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON))
  {
    gameState = GAME_PLAY;
    delay(1000);
  }
}

void ResetVariables()
{
  playerMoveEnable = true;
  enemy[0].moveEnable = true;  
  enemy[1].moveEnable = true;  
  enemy[2].moveEnable = true;  
  enemy[3].moveEnable = true;  
  enemy[4].moveEnable = true;  
  dog[0].moveEnable = true;
  dog[1].moveEnable = true;
  dog[2].moveEnable = true;
  dog[3].moveEnable = true;
  dog[4].moveEnable = true;
  //Reset variables
  health = 100;
  enemy[0].x = enemySpawnRightX;  
  enemy[0].y = ground;            
  enemy[1].x = enemySpawnLeftX;   
  enemy[1].y = ground;            
  enemy[2].x = enemySpawnRightX;  
  enemy[2].y = ground;            
  enemy[3].x = enemySpawnLeftX;   
  enemy[3].y = ground;            
  enemy[4].x = enemySpawnRightX;  
  enemy[4].y = ground;            
  dog[0].x = enemySpawnRightX;
  dog[0].y = ground + 8;
  dog[1].x = enemySpawnLeftX;
  dog[1].y = ground + 8;
  dog[2].x = enemySpawnRightX;
  dog[2].y = ground + 8;
  dog[3].x = enemySpawnLeftX;
  dog[3].y = ground + 8;
  dog[4].x = enemySpawnRightX;
  dog[4].y = ground + 8;
  bgX = 0;
  spawnX = 0;
  enemy[0].exist = false;     //enemy0Exist = false;
  enemy[1].exist = false;     //enemy1Exist = false;
  enemy[2].exist = false;     //enemy2Exist = false;
  enemy[3].exist = false;     //enemy3Exist = false;
  enemy[4].exist = false;     //enemy4Exist = false;
  dog[0].exist = false;
  dog[1].exist = false;
  dog[2].exist = false;
  dog[3].exist = false;
  enemy[0].rect = {0,0,0,0};  //enemy0Rect = {0,0,0,0};
  enemy[1].rect = {0,0,0,0};  //enemy1Rect = {0,0,0,0};
  enemy[2].rect = {0,0,0,0};  //enemy2Rect = {0,0,0,0};
  enemy[3].rect = {0,0,0,0};
  enemy[4].rect = {0,0,0,0};
  dog[0].rect = {0,0,0,0};
  dog[1].rect = {0,0,0,0};
  dog[2].rect = {0,0,0,0};
  dog[3].rect = {0,0,0,0};
  dog[4].rect = {0,0,0,0};
  levelProg = 0;
}

void HealthBar()
{
  arduboy.fillRect(0, 0,(health*WIDTH/100), 2); //Health bar drawn on top of screen according to health percentage
  if (health < 0)
  {
    gameState = GAME_OVER;
    sound.tones(GameOver);
  }
}

void GameOverScreen(bool finished)
{
  if (finished == false)
  {
    arduboy.setCursor(30, 20);
    arduboy.print("Game Over!");
  }
  else 
  {
     ardbitmap.drawCompressed(WIDTH / 2, HEIGHT / 2, End_Screen, WHITE, ALIGN_CENTER, MIRROR_NONE);
  }
  sound.tones(GameOver);
  delay(1000);
  arduboy.audio.off();
  if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON))
  {
    level = 1;
    gameState = GAME_TITLE;
    delay(300);
  }    
}

void GamePlay()
{
  Movement();
  DrawPlayer();
  for (int i = 0; i < 5; i++)
  {
    ScreenLimit(enemy[i], i, dog[i]);
  }
  DrawLevel();
  for (int i = 0; i < 5; i++)
  {
    EnemyControl(enemy[i], i, dog[i]);    //Call function with enemy variables and choice variable for the function.
  }
  for (int i = 0; i < 5; i++)
  {
    CollisionDetection(enemy[i], i, dog[i]);    //Call function with enemy variables and choice variable for the function.
  }
  SpawnControl();
  HealthBar();
  LevelControl();
}

void LevelControl()
{
  if (playerX == 92 && spawnX == -1024)   //When player walks enough to the right, Level changes to next level.
  {
    level++;
    gameState = LEVEL_SCREEN;
  }
}

void SpawnControl()
{
  //This function handles all the enemy spawning triggers
  if (level > 0)
  {
    for (int i = 0; i < 5; i++)
    {
      
      if (enemy[i].exist == false && spawnX < enemy[i].spawn)
      {
        enemy[i].exist = true;
      }
      
      if (dog[i].exist == false && spawnX < dog[i].spawn)
      {
        dog[i].exist = true;
      }
    }
  }
}



void EnemyControl(Enemy nme, int choice, Dog doggo)
{
  //Enemy Controls
  if (nme.exist == true)    //Check for existence
  {
    if (nme.moveEnable == true)
    {
      if (playerX < nme.x)
      {  
        nme.rect = {nme.x, nme.y-9, 4, 24};      //Create collision rect
        ardbitmap.drawCompressed(nme.x, nme.y, runAnim[nme.runAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);    //Draw enemy
        nme.x -= playerSpeed;//Runs left  
      }
      else
      {
        nme.rect = {nme.x-4, nme.y-9, 4, 24};   //Create collisioon rect.. Slightly shifted when facing other direction to keep it aligned
        ardbitmap.drawCompressed(nme.x, nme.y, runAnim[nme.runAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);    //Draw enemy
        nme.x += playerSpeed;//Runs right
      }
    }
      nme.runAnimFrame++;
      
    if (nme.runAnimFrame > 7)
    {
      nme.runAnimFrame = 0;
    }
  }
  
 

  //Enemy death animation
  if (nme.enemyDying == true)//Checks to see if enemy is dying
  {
    sound.tone(175, 25);
    if (playerX < nme.x)
    {
      ardbitmap.drawCompressed(nme.x, nme.y, dyingAnim[nme.dyingAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE); //Plays dying animation
      nme.x+=2;
    }
    if (playerX > nme.x)
    {
      ardbitmap.drawCompressed(nme.x, nme.y, dyingAnim[nme.dyingAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); //Plays dying animation
      nme.x-=2;
    }
      nme.dyingAnimFrame++;
      
      nme.y+=2;
    if (nme.dyingAnimFrame > 7)   //Resets animation counter after playing
    {
      nme.dyingAnimFrame = 0;
      nme.enemyDying = false;      //Enemy no longer dying
      nme.y = ground;         //Reset x and y variables
      if (choice % 2 == 0)
      {
        nme.x = enemySpawnRightX;
      }
      else
      {
        nme.x = enemySpawnLeftX;
      }
    }
  }
  //Dog controls
  
  if (doggo.exist == true)    //Check for existence
  {
    if (doggo.moveEnable == true)
    {
      if (playerX < doggo.x)
      {  
        doggo.rect = {doggo.x-8, doggo.y-2, 14, 8};      //Create collision rect
        ardbitmap.drawCompressed(doggo.x, doggo.y, dogRunAnim[doggo.runAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);    //Draw enemy
        doggo.x -= playerSpeed;//Runs left  
      }
      else
      {
        doggo.rect = {doggo.x-8, doggo.y-2, 14, 8};   //Create collisioon rect.. Slightly shifted when facing other direction to keep it aligned
        ardbitmap.drawCompressed(doggo.x, doggo.y, dogRunAnim[doggo.runAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);    //Draw enemy
        doggo.x += playerSpeed;//Runs right
      }
    }
      doggo.runAnimFrame++;
      
    if (doggo.runAnimFrame > 6)
    {
      doggo.runAnimFrame = 0;
    }
  }

   //Dog death animation
    if (doggo.enemyDying == true)//Checks to see if enemy is dying
  {
    sound.tone(175, 25);
    if (playerX < doggo.x)
    {
      ardbitmap.drawCompressed(doggo.x, doggo.y, dogRunAnim[doggo.runAnimFrame], WHITE, ALIGN_CENTER, MIRROR_VERTICAL); //Plays dying animation
      doggo.x+=2;
    }
    if (playerX > doggo.x)
    {
      ardbitmap.drawCompressed(doggo.x, doggo.y, dogRunAnim[doggo.runAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HOR_VER); //Plays dying animation
      doggo.x-=2;
    }
      doggo.runAnimFrame++;
      
      doggo.y+=2;
    if (doggo.runAnimFrame > 6)   //Resets animation counter after playing
    {
      doggo.runAnimFrame = 0;
      doggo.enemyDying = false;      //Enemy no longer dying
      doggo.y = ground + 8;         //Reset x and y variables
      if (choice % 2 == 0)
      {
        doggo.x = enemySpawnRightX;
      }
      else
      {
        doggo.x = enemySpawnLeftX;
      }
    }
  }
  enemy[choice] = nme;
  dog[choice] = doggo;
}


void CollisionDetection(Enemy nme, int choice, Dog doggo)
{
  //update player Collision Rect 
  if (playerFaceRight == false)       //Rect position slightly changed dependging on which way player facing. To cover sprite properly.
  {
    playerRect = {playerX-3, playerY-9, 9, 24};
    //arduboy.fillRect(playerX-3, playerY-9, 9, 24); //This was used to show the player rect
    if (playerKickLeft == true || playerFlyKickLeft == true)   //Enabling kick rect
      {
        playerKickRect = {playerX-18,playerY-4, 18, 10};
        //arduboy.drawRect(playerX-18,playerY-4, 18, 10);
      }
      if (playerCrouchPunchLeft == true)
      {
        playerKickRect = {playerX-10, playerY+5, 10, 10};
        //arduboy.drawRect(playerX-10, playerY+5, 10, 10);
      } 
  }
  
  if (playerFaceRight == true)      //Rect position slightly changed dependging on which way player facing. To cover sprite properly.
  {
    playerRect = {playerX-6, playerY-9, 9, 24};
    //arduboy.fillRect(playerX-6, playerY-9, 9, 24); //This was used to show the player rect
    if (playerKickRight == true || playerFlyKickRight == true)    //Enabling kick rect
    {
      playerKickRect = {playerX,playerY-4, 18, 10};
      //arduboy.drawRect(playerX,playerY-4, 18, 10);
    }
    if (playerCrouchPunchRight == true)
    {
      playerKickRect = {playerX, playerY+5, 10, 10};
      //arduboy.drawRect(playerX, playerY+5, 10, 10);
    }
  }
  if (playerKickRight == false && playerKickLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false)  //If player is not attacking then turn kick rect off
  {
    playerKickRect = {0,0,0,0};
    //arduboy.fillRect(0,0,0,0);
  }

  //Checking for collisions
if (arduboy.collide(playerRect, doggo.rect))              // Check for collision and if enemy hitting player then freeze both. Need to add health sub here
  {
    playerMoveEnable = false; //Freeze player
    doggo.moveEnable = false;  //Freeze enemy
    if (playerX > doggo.x)                                  //Check which side player on
    {
      ardbitmap.drawCompressed(doggo.x, doggo.y, Dog3, WHITE, ALIGN_CENTER, MIRROR_NONE);         //When dog touching player just show  the one frame that looks like they're biting the player
    }
    else
    {
      ardbitmap.drawCompressed(doggo.x, doggo.y, Dog3, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); 
    }
    health -= enemyAttack;
  }
  else                                                      //Otherwise enemy is unfrozen
  {
    doggo.moveEnable = true;
  }
  
  if (arduboy.collide(playerRect, nme.rect))              // Check for collision and if enemy hitting player then freeze both. Need to add health sub here
  {
    playerMoveEnable = false; //Freeze player
    nme.moveEnable = false;  //Freeze enemy
    if (playerX > nme.x)                                  //Check which side player on
    {
      ardbitmap.drawCompressed(nme.x, nme.y, EnemyGrab0, WHITE, ALIGN_CENTER, MIRROR_NONE); 
    }
    else
    {
      ardbitmap.drawCompressed(nme.x, nme.y, EnemyGrab0, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL); 
    }
    health -= enemyAttack;
  }
  else                                                      //Otherwise enemy is unfrozen   //NEED TO REWRITE ALL THESE USING STRUCTS
  {
    nme.moveEnable = true;
  }


  if (!arduboy.collide(playerRect, enemy[0].rect) && !arduboy.collide(playerRect, enemy[1].rect) && !arduboy.collide(playerRect, enemy[2].rect) && !arduboy.collide(playerRect, enemy[3].rect) && !arduboy.collide(playerRect, enemy[4].rect) && !arduboy.collide(playerRect, dog[0].rect) && !arduboy.collide(playerRect, dog[1].rect) && !arduboy.collide(playerRect, dog[2].rect) && !arduboy.collide(playerRect, dog[3].rect) && !arduboy.collide(playerRect, dog[4].rect))     //Remember to add dogs to this properly
  {
    playerMoveEnable = true; //Player free to move    
  }
  
  if (arduboy.collide(playerKickRect, nme.rect))  // Checking enemy
  {
    nme.exist = false;
    nme.enemyDying = true;
    nme.spawn = nme.spawn - enemySpawnFreq;   //enemySpawn is tied to bgX value
    nme.rect = {0,0,0,0};
  }
  
  if (arduboy.collide(playerKickRect, doggo.rect))  // Checking dog 0
  {
    doggo.exist = false;
    doggo.enemyDying = true;
    doggo.spawn = doggo.spawn - enemySpawnFreq;
    doggo.rect = {0,0,0,0};
  }
  enemy[choice] = nme;      //Copy temp variables back to structs
  dog[choice] = doggo;
}

void DrawLevel()
{ 
  
  // When screen has scrolled one tile over
    if (bgX < -15)
    { 
      levelProg++;
      for (int x = 0; x < 8; x++) //copy all the tiles one over in the array
      {
        displayFloorTiles[x] = displayFloorTiles[x+1];
        displayMiddleLowTiles[x] = displayMiddleLowTiles[x+1];
        displayMiddleHighTiles[x] = displayMiddleHighTiles[x+1];
        displayCeilingTiles[x] = displayCeilingTiles[x+1];
      }
  
        // Draw upcomming tiles
        displayFloorTiles[8] = dungeonTileMap[random(displayTiles[0], displayTiles[1])];
        displayCeilingTiles[8] = dungeonTileMap[random(displayTiles[6], displayTiles[7])];
  
        if (spawnX % -64 == 0)          //Spacing out the decorative tiles using modulus
        {
          displayMiddleHighTiles[8] = dungeonTileMap[displayTiles[5]];       
        }
        else
        {
          displayMiddleHighTiles[8] = dungeonTileMap[displayTiles[4]];     
        }
        
        if (spawnX % displayTiles[8] == 0)          //Spacing out the decorative tiles using modulus
        {
          displayMiddleLowTiles[8] = dungeonTileMap[random(displayTiles[2], displayTiles[3])];
        }
        else
        {
          displayMiddleLowTiles[8] = dungeonTileMap[displayTiles[4]];
        }
        
      if (levelProg > 59)   //once level nears end draw exit
      {
        if ( level < 5)
        {
          DrawExit(1);
        }
        else 
        {
          DrawExit(2);
        }
      }
     
      bgX = 0; //Reset the scroll variable
   }

    
    //Draw screen
    for (int x = 0; x< 9; x++)
    {
      ardbitmap.drawCompressed(x * TILE_SIZE + bgX, floorTileY, displayFloorTiles[x], WHITE, ALIGN_H_LEFT, MIRROR_NONE);       //Draw floor tiles
      ardbitmap.drawCompressed(x * TILE_SIZE + bgX, middleHighTileY, displayMiddleHighTiles[x], WHITE, ALIGN_H_LEFT, MIRROR_NONE);     //Draw middle high tiles
      ardbitmap.drawCompressed(x * TILE_SIZE + bgX, middleLowTileY, displayMiddleLowTiles[x], WHITE, ALIGN_H_LEFT, MIRROR_NONE);     //Draw middle low tiles
      ardbitmap.drawCompressed(x * TILE_SIZE + bgX, ceilingTileY, displayCeilingTiles[x], WHITE, ALIGN_H_LEFT, MIRROR_NONE);   //Draw ceiling tiles
    }
}

void DrawExit(int x)
{
  if (x == 1)
  {
    if (levelProg == 60)
    {
       displayFloorTiles[8] = dungeonTileMap[36];
       displayMiddleLowTiles[8] = dungeonTileMap[30];    
       displayMiddleHighTiles[8] = dungeonTileMap[30];    
       displayCeilingTiles[8] = dungeonTileMap[30];
    }
    if (levelProg == 61) //if near end of level then start drawing exit
    {
       displayFloorTiles[8] = dungeonTileMap[36];
       displayMiddleLowTiles[8] = dungeonTileMap[15];    
       displayMiddleHighTiles[8] = dungeonTileMap[13];    
       displayCeilingTiles[8] = dungeonTileMap[30];
    }
    if (levelProg == 62) //if near end of level then start drawing exit
    {
       displayFloorTiles[8] = dungeonTileMap[36];
       displayMiddleLowTiles[8] = dungeonTileMap[16];    
       displayMiddleHighTiles[8] = dungeonTileMap[14];    
       displayCeilingTiles[8] = dungeonTileMap[30];
    }
    if (levelProg > 62)
    {
       displayFloorTiles[8] = dungeonTileMap[36];
       displayMiddleLowTiles[8] = dungeonTileMap[30];    
       displayMiddleHighTiles[8] = dungeonTileMap[30];    
       displayCeilingTiles[8] = dungeonTileMap[30];
    }
  }
  else if (x == 2)
  {
    if (levelProg == 60)
    {
       displayFloorTiles[8] = dungeonTileMap[37];
       displayMiddleLowTiles[8] = dungeonTileMap[11];    
       displayMiddleHighTiles[8] = dungeonTileMap[11];    
       displayCeilingTiles[8] = dungeonTileMap[random(31, 36)];
    }
    if (levelProg == 61) //if near end of level then start drawing exit
    {
       displayFloorTiles[8] = dungeonTileMap[37];
       displayMiddleLowTiles[8] = dungeonTileMap[11];    
       displayMiddleHighTiles[8] = dungeonTileMap[11];    
       displayCeilingTiles[8] = dungeonTileMap[random(31, 36)];
    }
    if (levelProg == 62) //if near end of level then start drawing exit
    {
       displayFloorTiles[8] = dungeonTileMap[37];
       displayMiddleLowTiles[8] = dungeonTileMap[38];    
       displayMiddleHighTiles[8] = dungeonTileMap[38];    
       displayCeilingTiles[8] = dungeonTileMap[38];
    }
    if (levelProg > 62)
    {
       displayFloorTiles[8] = dungeonTileMap[37];
       displayMiddleLowTiles[8] = dungeonTileMap[11];    
       displayMiddleHighTiles[8] = dungeonTileMap[11];    
       displayCeilingTiles[8] = dungeonTileMap[random(31, 36)];
    }
  }
}

void DrawPlayer()
{
  //Idle animation
  if (playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchLeft == false && playerCrouchRight == false && playerCrouchPunchLeft == false && playerCrouchPunchRight == false)  
  { 
    if (playerFaceRight == true)
     {
        ardbitmap.drawCompressed(playerX, playerY, Idle0, WHITE, ALIGN_CENTER, MIRROR_NONE);
     }
    else
     {
        ardbitmap.drawCompressed(playerX, playerY, Idle0, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);
     }
   }
  
  if (playerRunRight == true)
  {
    ardbitmap.drawCompressed(playerX, playerY, runAnim[runAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);
    runAnimFrame++;
    if (runAnimFrame > 7)
    {
      runAnimFrame = 0;
    }
  }
        
  if (playerRunLeft == true)
  {
    ardbitmap.drawCompressed(playerX, playerY, runAnim[runAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);
    runAnimFrame++;
    if (runAnimFrame > 7)
    {
      runAnimFrame = 0;
    }
  }

 if (playerFlyKickRight == true)
 {
    ardbitmap.drawCompressed(playerX, playerY, flyKick[punchAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);
    punchAnimFrame++;
    playerX += playerSpeed;
    if (punchAnimFrame > 9)
    {
      punchAnimFrame = 0;
      playerFlyKickRight = false;
    }
 }

 if (playerFlyKickLeft == true)
 {
    ardbitmap.drawCompressed(playerX, playerY, flyKick[punchAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);
    punchAnimFrame++;
    playerX -= playerSpeed;
    if (punchAnimFrame > 9)
    {
      punchAnimFrame = 0;
      playerFlyKickLeft = false;
    }
 }

 if (playerKickRight == true)
 {
    ardbitmap.drawCompressed(playerX, playerY, kickAnim[kickAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);
    kickAnimFrame++;
    if (kickAnimFrame > 7)
    {
      kickAnimFrame = 0;
      playerKickRight = false;
    }
 }

  if (playerKickLeft == true)
 {
    ardbitmap.drawCompressed(playerX, playerY, kickAnim[kickAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);
    kickAnimFrame++;
    if (kickAnimFrame > 7)
    {
      kickAnimFrame = 0;
      playerKickLeft = false;
    }
 }

  if (playerCrouchRight == true)
  {
     ardbitmap.drawCompressed(playerX, playerY, Crouch0, WHITE, ALIGN_CENTER, MIRROR_NONE);
  }

  if (playerCrouchLeft == true)
  {
     ardbitmap.drawCompressed(playerX, playerY, Crouch0, WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);
  }

    if (playerCrouchPunchRight == true)
  {
     ardbitmap.drawCompressed(playerX, playerY, crouchPunchAnim[kickAnimFrame], WHITE, ALIGN_CENTER, MIRROR_NONE);
     kickAnimFrame++;
     if (kickAnimFrame > 6)
     {
        kickAnimFrame = 0;
        playerCrouchPunchRight = false;
     }
  }

     if (playerCrouchPunchLeft == true)
  {
     ardbitmap.drawCompressed(playerX, playerY, crouchPunchAnim[kickAnimFrame], WHITE, ALIGN_CENTER, MIRROR_HORIZONTAL);
     kickAnimFrame++;
     if (kickAnimFrame > 6)
     {
        kickAnimFrame = 0;
        playerCrouchPunchLeft = false;
     }
  }
 
}

void Movement()
{
  //Setting up movement buttons
  //run left
    if (arduboy.pressed(LEFT_BUTTON) && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false)
    {
      if (playerMoveEnable == true)
      {
        playerX = playerX - playerSpeed;
      }
      playerRunLeft = true;
      playerRunRight = false;
      playerFaceRight = false;
      playerCrouchPunchRight = false;
      playerCrouchPunchLeft = false;
      
    }
    // run right
    if (arduboy.pressed(RIGHT_BUTTON) && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false)
    {
      if (playerMoveEnable == true)
      {
        playerX = playerX + playerSpeed;
      }
      playerRunRight = true;
      playerRunLeft = false;
      playerFaceRight = true;
      playerCrouchPunchRight = false;
      playerCrouchPunchLeft = false;
      
    }
  
  // idle
  if (!arduboy.pressed(LEFT_BUTTON) && !arduboy.pressed(RIGHT_BUTTON) && !arduboy.pressed(DOWN_BUTTON))
  {
    playerRunRight = false;
    playerRunLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }
  //Crouch right
  if (arduboy.pressed(DOWN_BUTTON) && playerFaceRight == true && playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchRight == false && playerCrouchLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false)
  {
    playerCrouchRight = true;
    playerCrouchLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
  }

  //Crouch left
  if (arduboy.pressed(DOWN_BUTTON) && playerFaceRight == false && playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchRight == false && playerCrouchLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false)
  {
    playerCrouchRight = false;
    playerCrouchLeft = true;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
  }
  
  //Setting up attack buttons
  //Flykick right
  if (arduboy.pressed(A_BUTTON) && playerFaceRight == true && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && !arduboy.collide(playerRect, dog[0].rect) && !arduboy.collide(playerRect, dog[1].rect) && !arduboy.collide(playerRect, dog[2].rect) && !arduboy.collide(playerRect, dog[3].rect) && !arduboy.collide(playerRect, dog[4].rect) && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false && playerMoveEnable == true)  
  {
    playerFlyKickRight = true;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
    
  }
  //Flykick left
    if (arduboy.pressed(A_BUTTON) && playerFaceRight == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && !arduboy.collide(playerRect, dog[0].rect) && !arduboy.collide(playerRect, dog[1].rect) && !arduboy.collide(playerRect, dog[2].rect) && !arduboy.collide(playerRect, dog[3].rect) && !arduboy.collide(playerRect, dog[4].rect) && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false && playerMoveEnable == true)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = true;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }

    //kick right
  if (arduboy.pressed(B_BUTTON) && playerFaceRight == true && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = true;
    playerKickLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }
    //kick left
    if (arduboy.pressed(B_BUTTON) && playerFaceRight == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false && playerCrouchPunchRight == false && playerCrouchPunchLeft == false && playerCrouchRight == false && playerCrouchLeft == false)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = true;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }

  //Crouch punch right
  if (arduboy.pressed(B_BUTTON) && arduboy.pressed(DOWN_BUTTON) && playerFaceRight == true && playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = false;
    playerCrouchPunchRight = true;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }

  //Crouch punch left
  if (arduboy.pressed(B_BUTTON) &&  arduboy.pressed(DOWN_BUTTON) && playerFaceRight == false && playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = true;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }

  //Crouch punch right
  if (arduboy.pressed(A_BUTTON) && arduboy.pressed(DOWN_BUTTON) && playerFaceRight == true && playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = false;
    playerCrouchPunchRight = true;
    playerCrouchPunchLeft = false;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }

  //Crouch punch left
  if (arduboy.pressed(A_BUTTON) &&  arduboy.pressed(DOWN_BUTTON) && playerFaceRight == false && playerRunRight == false && playerRunLeft == false && playerFlyKickRight == false && playerFlyKickLeft == false && playerKickRight == false && playerKickLeft == false)
  {
    playerFlyKickRight = false;
    playerFlyKickLeft = false;
    playerRunRight = false;
    playerRunLeft = false;
    playerKickRight = false;
    playerKickLeft = false;
    playerCrouchPunchRight = false;
    playerCrouchPunchLeft = true;
    playerCrouchRight = false;
    playerCrouchLeft = false;
  }
}

void ScreenLimit(Enemy nme, int choice, Dog doggo)
{ 
  //setting boundires for screen
 
  if (spawnX > -1024)         //Determines how far along the level the player is
  { 
    if (playerX > 70)       //Code for scrolling to the right and scrolling enemies with it
    {
      if (choice == 4)
      {
        playerX = 70;
        bgX = bgX - playerSpeed;
        spawnX -= playerSpeed;
      }
      if (nme.exist == true)
      {
        nme.x = nme.x - playerSpeed + 1;
      }
      if (doggo.exist == true)
      {
        doggo.x = doggo.x - playerSpeed + 1;
      }
      
      
    }
  }
  if (playerX < 4)   //This stops the player walking off the screen after the background stops scrolling
  {
    playerX = 4;
  }
  if (playerX > 96)
  {
    playerX = 96;
  }
  enemy[choice] = nme; //copy nme variables back to enemy struct
  dog[choice] = doggo;
}
void gameLogicLoop(void *)
{
  for (;;) {
    inogameloop(); 
    // ArduinoOTA.handle();
  }
}

void setup() {
  inogamesetup();
  xTaskCreatePinnedToCore(gameLogicLoop, "g", 4096, nullptr, 0, nullptr, 0);
}

void loop() {
	delay(60000);
}
