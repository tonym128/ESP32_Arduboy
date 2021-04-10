#include <Arduboy2.h>
#include "Images.h"
#include "Init.h"
Arduboy2 arduboy;
BeepPin1 beep;


void setup() {
  arduboy.begin();
  arduboy.initRandomSeed();
  beep.begin();
  arduboy.setFrameRate(60);
  rectFirst.x = 0;
  rectFirst.y = 54;
  rectFirst.width = 30;
  rectFirst.height = 10;
  
  rectSecond.x = 65;
  rectSecond.y = 54;
  rectSecond.width = 20;
  rectSecond.height = 10;
}

void loop() {
if(!arduboy.nextFrame()) {
    return;
  }    
  beep.timer();
  arduboy.pollButtons();
  arduboy.clear();
  makePlayer();

  switch (gamestate){
   
  case 0: 
  Sprites::drawOverwrite (0, 0, title, 0);
  coinFonTime++;
  if(coinFonTime > 15){
     Sprites::drawOverwrite (96, 33, coinFon, 0);
     Sprites::drawOverwrite (16, 33, coinFon, 0);
  }
  if(coinFonTime == 30)
     coinFonTime = 0;  
  updateMenu();
  if(arduboy.justPressed(A_BUTTON) && (markerPos == 0)){
     beep.tone(beep.freq(1000), 1);
     perfectCount = 0;
     gamestate = 1;
     soundOn = 0;
  }
  if(arduboy.justPressed(A_BUTTON) && (markerPos == 1)){
     beep.tone(beep.freq(1000), 1);
     gamestate = 2;
  }
  break;
  
  case 1:
  if((player.y > 64) || (player.x > 128)){
    beep.tone(beep.freq(50), 3);
    gamestate = 4;
  }
  if((platformFirstMove == 0) && (platformSecondMove == 2))
      platformStrate = true;
  if((platformFirstMove == 2) && (platformSecondMove == 0))
      platformStrate = false;   
      
  gameBar();
  movePlayer();
  movePlatform();
  drawLines();
  getBonus();
  drawPlayer();
  doSound();
   
  if(arduboy.justPressed(A_BUTTON))
     gamestate = 3;
  break;

  case 2:
  Players();
  doSound();
  if(arduboy.justPressed(A_BUTTON))
     gamestate = 0;
  break;

  case 3:
  arduboy.setCursor(26, 23);
  arduboy.print(F("Quit the game?"));
  arduboy.setCursor(0, 55);
  arduboy.print(F("LEFT-yes"));
  arduboy.setCursor(80, 55);
  arduboy.print(F("RIGHT-no"));
  arduboy.drawRoundRect(23, 20, 88, 13, 3);
  
  if(arduboy.justPressed(RIGHT_BUTTON))
     gamestate = 1; 
  if(arduboy.justPressed(LEFT_BUTTON)){
    gamestate = 0;
    reset();
  }
  break;

  case 4:
  arduboy.setCursor(41, 30);
  arduboy.print(F("You lose"));
  arduboy.setCursor(35, 50);
  arduboy.print(F("A-Continue"));
  arduboy.setCursor(0, 50);
  arduboy.drawRoundRect(37, 27, 53, 13, 3);

  if(arduboy.justPressed(A_BUTTON)) {
  reset();
  gamestate = 1; 
  }

  break;
  
  case 5:
  if(arduboy.frameCount < 100)
     Sprites::drawOverwrite (46, 23, logo, 0);
  if(arduboy.frameCount == 100)
     gamestate = 0;
  break;
  }
  arduboy.display();
}


void makePlayer(){
  if((playerMarker >= 0) && (playerMarker <= 12))
  {
    selectedPlayer = players[playerMarker];
    selectedPlayer2 = players2[playerMarker];
  }
}

void reset(){
bridgeIsDown = false;
bridgeOnPlatform = false;
buttOn = false;
collFirst = false;
collSecond = false;
playerOnPlatform = 0;
allowDrawLine = true;
spriteAnim = false;
frames = 0;
randFirstWidth = 30;
randFirstPos = 0;
randSecondWidth = 20;
randSecondPos = 0;
lineOnX = 29;
lineOnY = 54;
platformFirstMove = 0;
platformSecondMove = 2;
bridgeLength = 0;
player.x = 15;
player.y = 41;
rectFirst.x = 0;
rectFirst.y = 54;
rectFirst.width = 30;
rectFirst.height = 10;
rectSecond.x = 65;
rectSecond.y = 54;
rectSecond.width = 20;
rectSecond.height = 10; 
countPlatform = 0;
flipPlayer = false;
coinRandF = - 10;
coinRandS = -10;
coinsCount = 0;
perfect = 0;
perfectCount = 0;
spriteAnim = false;
endLine.x = -100;
}


void drawPlayer(){
  for(uint8_t i = 0; i < 12; i++){
      if(chosenPlayer == i){
         selectedPlayer = players[i];
         selectedPlayer2 = players2[i];
      }
  }
  if(arduboy.justPressed(B_BUTTON) && flipPlayer && Timer > 5){
    beep.tone(beep.freq(200), 3);
    flipPlayer = false;
    Timer = 0;
  }
  
  if(!spriteAnim)
    frames = 0; 
  else{
  if(arduboy.everyXFrames(8)) frames++;
  if(frames > 1) frames = 0;  
  }
  
  if(!flipPlayer){
    Sprites::drawPlusMask (player.x, player.y + 1, selectedPlayer, frames);
    Timer++;
  }
  if(flipPlayer){
    Sprites::drawPlusMask (player.x, player.y + 12, selectedPlayer2, frames);
    Timer++;
  }
  if(platformStrate && arduboy.justPressed(B_BUTTON) && !flipPlayer){
    if((player.x > (rectFirst.x + randFirstWidth)) && (player.x < rectSecond.x) && (Timer > 5)){
       beep.tone(beep.freq(200), 3);
       flipPlayer = true;
       Timer = 0;
    } 
  }
  if(!platformStrate && arduboy.justPressed(B_BUTTON) && !flipPlayer){
    if((player.x > (rectSecond.x + randSecondWidth)) && (player.x < rectFirst.x) && (Timer > 5)){
       flipPlayer = true;
       Timer = 0;
    }
  }
  
}


void movePlayer(){  
  
  if(arduboy.collide(player, rectFirst) && (player.x > rectFirst.x))
     collFirst = true;
  else 
     collFirst = false;
  
  if(arduboy.collide(player, rectSecond) && (player.x > rectSecond.x))
     collSecond = true;
  else 
     collSecond = false;
  
  if(((platformFirstMove == 0) && (platformSecondMove == 2))){
      if(flipPlayer && (player.x + 8) > rectSecond.x)
         player.y = 65;
          if(bridgeIsDown && (player.x < (bridgeLength + randFirstWidth + 1)) && !bridgeOnPlatform){
          player.x++;
          spriteAnim = true;
          }
          if((player.x < (rectSecond.x + rectSecond.width - 13)) && bridgeOnPlatform){
          player.x++;
          spriteAnim = true; 
          }
          if((player.x > (bridgeLength + randFirstWidth)) && !collSecond && !bridgeOnPlatform)
          player.y++;
  }


  
  if(((platformFirstMove == 2) && (platformSecondMove == 0))){
     if(flipPlayer && ((player.x + 8) > rectFirst.x))
        player.y = 65; 
        if(bridgeIsDown && player.x < bridgeLength + randSecondWidth + 1 && !bridgeOnPlatform){
           player.x++;
           spriteAnim = true;
        }  
        if(player.x < rectFirst.x + rectFirst.width - 13 && bridgeOnPlatform){
           player.x++;
           spriteAnim = true; 
        }
        if((player.x > (bridgeLength + randSecondWidth)) && !collFirst && !bridgeOnPlatform)
           player.y++;
  } 
  
  if((player.x == (rectFirst.x + rectFirst.width - 13)) && !platformStrate && bridgeOnPlatform || (player.x == (rectSecond.x + rectSecond.width - 13)) && platformStrate && bridgeOnPlatform){
    playerOnPlatform = true;
   
  }
  else 
    playerOnPlatform = false;
    
  if((platformFirstMove == 3) && !bridgeOnPlatform)
     player.x--;
  if((platformSecondMove == 3) && !bridgeOnPlatform)
     player.x--;
}


void drawLines(){
  if(bridgeLength > 0){
    if(platformStrate)
        drawBridge(rectFirst.x + randFirstWidth - 1, 53, bridgeLength, arduboy.frameCount);
    if(!platformStrate)
        drawBridge(rectSecond.x + randSecondWidth - 1, 53, bridgeLength, arduboy.frameCount);
  }
  if(arduboy.pressed(B_BUTTON) && allowDrawLine){
    if(((platformFirstMove == 0) && (platformSecondMove == 2))|| ((platformFirstMove == 2) && (platformSecondMove == 0))){
         buttOn = true;
         bridgeLength++;
    }
  }
  else
         buttOn = false;

  if(arduboy.collide(endLine, rectFirst) || arduboy.collide(endLine, rectSecond))
     bridgeOnPlatform = true;
  if(!buttOn && (bridgeLength > 1))
     allowDrawLine = false;

  if  (playerOnPlatform)
      endLine.x = -100;
 }


void drawBridge(int x, int y, int length, int frame){  
  if(buttOn){ 
    arduboy.drawLine(x, y, x, y - bridgeLength);
    arduboy.frameCount = 20;
    bridgeIsDown = false;
    soundOn = 2;
  }
  else if(frame < ANIMATION_FRAMES){
    float phase = ((frame - ANIMATION_FRAMES / 2) * PI_HALF) / ((float) ANIMATION_FRAMES / 2); 
    arduboy.drawLine(x, y, x + sin(phase) * length, y - cos(phase) * length);
  }
    else{ // bridge on the ground: just draw a horizontal line
       arduboy.drawLine(x, y, x + length, y);
         if(bridgeLength > 1){
         endLine.x = x + length;
         endLine.y = y + 1;
         }
       if(bridgeLength > 0)
          bridgeIsDown = true;
    }   
}

void movePlatform(){

// First

if((rectFirst.x < 126) && playerOnPlatform && !buttOn && platformStrate){
  platformFirstMove = 4;
  spriteAnim = false;
}
if(rectFirst.x == - 1){
  countPlatform++; 
  bridgeLength = 0;
  bridgeIsDown = false;
}
if(platformFirstMove == 4)
   rectFirst.x--;
if(rectFirst.x <= (0 - randFirstWidth))
   rectFirst.x = 128;
if(rectFirst.x == 128){
    if(!level){
       randFirstWidth = random(15, 40);
       randFirstPos = random(randSecondWidth + 16, 90);
       randCoin = random(0, 4);
    }

    if(level){
       randFirstWidth = random(5, 20);
       randFirstPos = random(randSecondWidth + 16, 90);
       randCoin = random(0, 3);
    }
platformFirstMove = 1; 
allowDrawLine = true;  
  }

  if(platformFirstMove == 1){
     rectFirst.width = randFirstWidth;
      if(rectFirst.x > randFirstPos)
      rectFirst.x --;
  }
  if(rectFirst.x == randFirstPos)
     platformFirstMove = 2;

  if(!platformStrate && !buttOn && playerOnPlatform)
      platformFirstMove = 3;
  if((platformFirstMove == 3) && (rectFirst.x > (0 - randFirstWidth)))
      rectFirst.x--;
  if(rectFirst.x == 0){
    platformFirstMove = 0;
    bridgeOnPlatform = false;
  }

  
//Second

if((rectSecond.x < 126) && playerOnPlatform && !buttOn && !platformStrate){
    platformSecondMove = 4;
    spriteAnim = false;
}
if(rectSecond.x == - 1){ 
   bridgeLength = 0;
   countPlatform++; 
   bridgeIsDown = false;
}
if(platformSecondMove == 4)
   rectSecond.x--;
if(rectSecond.x <= 0 - randSecondWidth)
   rectSecond.x = 128;
if(rectSecond.x == 128){
    if(!level){
       randSecondWidth = random(15, 40);
       randSecondPos = random(randFirstWidth + 10, 90);
       randCoin = random(0, 4);
    }
    if(level){
       randSecondWidth = random(5, 20);
       randSecondPos = random(randFirstWidth + 10, 90);
       randCoin = random(0, 3);
    }
   platformSecondMove = 1; 
   allowDrawLine = true;   
  }

  if(platformSecondMove == 1){
     rectSecond.width = randSecondWidth;
     if(rectSecond.x > randSecondPos) rectSecond.x --;
  }
  if(rectSecond.x == randSecondPos){
     platformSecondMove = 2;
  }
  if((platformFirstMove == 4) && !buttOn && (platformSecondMove == 2)  && playerOnPlatform)
      platformSecondMove = 3;
  if((platformSecondMove == 3) && (rectSecond.x > (0 - randSecondWidth)))
      rectSecond.x--;
  if(rectSecond.x == 0)
     platformSecondMove = 0;

  if((platformStrate) || (!platformStrate))
       bridgeOnPlatform = false;
        
  arduboy.fillRect(rectFirst.x, rectFirst.y, rectFirst.width, rectFirst.height);
  arduboy.fillRect(rectSecond.x, rectSecond.y, rectSecond.width, rectSecond.height);
}

void getBonus(){
  rectFirstMiddle.x = rectFirst.x + (rectFirst.width/2 - 2);
  rectSecondMiddle.x = rectSecond.x + (rectSecond.width/2 - 2);
  arduboy.fillRect(rectFirstMiddle.x, rectFirstMiddle.y, rectFirstMiddle.width, rectFirstMiddle.height, BLACK);
  arduboy.fillRect(rectSecondMiddle.x, rectSecondMiddle.y, rectSecondMiddle.width, rectSecondMiddle.height, BLACK);
  if((arduboy.collide(endLine, rectFirstMiddle) && !middleOn) || (arduboy.collide(endLine, rectSecondMiddle) && !middleOn)){
      perfect++;
      coinsCount++;
      allMoney++;
      middleOn = true;
      soundOn = 1;
      perfectCount++;
        if(level){
           coinsCount++;
           allMoney++;
        }
  }

  if(!middleOn && bridgeIsDown)
     perfectCount = 0;
  
  if(middleOn && !bridgeIsDown)
     middleOn = false;

  //coin
  if(randCoin == 2){
    if(platformSecondMove == 1){
      if(rectSecond.x == randSecondPos + 1)
         coinRandF = random(rectFirst.x + randFirstWidth, rectSecond.x - 6);
    }
    if(platformFirstMove == 1){
      if(rectFirst.x == randFirstPos + 1)
         coinRandS = random(rectSecond.x + randSecondWidth, rectFirst.x - 6);
    }
  }
  
  if((platformFirstMove == 0) && (platformSecondMove == 2)) {
      rectCoin.x = coinRandF;
      Sprites::drawPlusMask (rectCoin.x, rectCoin.y + 12, coin, frameCoin);
  }
  if((platformFirstMove == 2) && (platformSecondMove == 0)){
      rectCoin.x = coinRandS;
      Sprites::drawPlusMask (rectCoin.x, rectCoin.y + 12, coin, frameCoin);
  }
  Timer2++;
  if(Timer2 < 10)
     frameCoin = 0;
  if(Timer2 > 9)
     frameCoin = 1;
  if(Timer2 == 20)
     Timer2 = 0;
  
    if(arduboy.collide(player, rectCoin) && flipPlayer){ 
    soundOn = 1;
    coinsCount++;
    allMoney++;
    coinRandF = - 10;
    coinRandS = - 10;
      if(level) {
         coinsCount++;
         allMoney++;  
      }
    }

  if(perfectCount == 5){
     soundOn = 3;
     allMoney = allMoney + 5; 
     coinsCount = coinsCount + 5;
     perfectCount = 0;
      if(level){
         allMoney = allMoney + 5; 
         coinsCount = coinsCount + 5;
      }
  }

  if(soundOn == 3){
    if(level){
       arduboy.setCursor(105, 12);
       arduboy.print(F("+10"));  
    }
     else{
       arduboy.setCursor(110, 12);
       arduboy.print(F("+5"));
     }
  }  
}

void updateMenu(){   
  if(arduboy.justPressed(DOWN_BUTTON) && (markerPos < 3)){
     beep.tone(beep.freq(300), 1);
     markerPos++;
  }
  if(arduboy.justPressed(UP_BUTTON) && (markerPos > 0)){
     beep.tone(beep.freq(300), 1);
     markerPos--;
  }
  
  if(markerPos == 0){
     lineMarkerX = 47;
     lineMarkerY = 33;
     lineMarkerEndX = 81;
  }

  if(markerPos == 1){
     lineMarkerX = 53;
     lineMarkerY = 40;
     lineMarkerEndX = 74; 
  }

  if(markerPos == 2){
     lineMarkerX = 56;
     lineMarkerY = 47;
     lineMarkerEndX = 71; 
  }

  if(markerPos == 3){
     lineMarkerX = 53;
     lineMarkerY = 54;
     lineMarkerEndX = 73; 
  }
  if(arduboy.justPressed(LEFT_BUTTON) && (markerPos == 2)){
     beep.tone(beep.freq(300), 1);
     level = false;
  }
  if(arduboy.justPressed(RIGHT_BUTTON) && (markerPos == 2)){
     beep.tone(beep.freq(300), 1);
     level = true;
  }
  if(level)
     Sprites::drawPlusMask (57, 42, hard, 0); 
  arduboy.drawLine(lineMarkerX, lineMarkerY, lineMarkerEndX, lineMarkerY);
   
  if(arduboy.justPressed(B_BUTTON) && (markerPos == 3))
     arduboy.audio.off();
  if(arduboy.justPressed(A_BUTTON) && (markerPos == 3)){
     arduboy.audio.on();
  }  
  if(arduboy.audio.enabled())
     Sprites::drawOverwrite (76, 49, on1, 0);
}

void Players(){
  Sprites::drawPlusMask (5, 57, coin, 0);
  arduboy.setCursor(15, 57);
  arduboy.print(allMoney);
  arduboy.drawLine(6, 14, 120, 14);
  arduboy.setCursor(10, 5);
  arduboy.print(F("Choose your runner"));

  if(arduboy.everyXFrames(10))
     frames++;
  if(frames > 1)
     frames = 0;  
  if(arduboy.justPressed(RIGHT_BUTTON) &&  (playerMarker < 11)){
     beep.tone(beep.freq(100), 2);
     playerMarker++;   
  }
  if(arduboy.justPressed(LEFT_BUTTON) &&  (playerMarker > 0)){
     beep.tone(beep.freq(100), 2);
     playerMarker--;  
  }
  
  Sprites::drawPlusMask (60, 35, selectedPlayer, frames);
  if(playerMarker < 11)
     Sprites::drawOverwrite (75, 38, MPR, 0);
  if(playerMarker > 0)
     Sprites::drawOverwrite (50, 38, MPL, 0);
  if(playerMarker > 1)
     Sprites::drawPlusMask (35, 85, coin, 0);
     
  arduboy.setCursor(xCursor[playerMarker], 53);   
  arduboy.print(names[playerMarker]);

    for (uint8_t i = 0; i < 12; i++){
       if((costPlayer[i] <= allMoney) && arduboy.justPressed(B_BUTTON) && (buyPlayer[i] == false) && (playerMarker == i)){
          buyPlayer[i] = true;     
          allMoney = allMoney - costPlayer[i];
          soundOn = 3;
       }
      if((buyPlayer[i] == false) && (playerMarker == i)){
          Sprites::drawOverwrite (62, 25, LOCK, 0);
          Sprites::drawPlusMask (25, 41, coin, 0);
          arduboy.drawRoundRect(0, 25, 42, 27, 5);
          arduboy.setCursor(5, 30);
          arduboy.print(F("Price:"));
            if(costPlayer[i] < 100)
               arduboy.setCursor(13, 41);
            else
               arduboy.setCursor(7, 41);
          arduboy.println(costPlayer[i]);
      }
  
    selectedPlayer = players[playerMarker];
    
    if((buyPlayer[i] == true) && (playerMarker == i) && arduboy.justPressed(B_BUTTON)){
        chosenPlayer = i;
        beep.tone(beep.freq(800), 3);
    } 
    
    if(chosenPlayer == i && playerMarker == i){
      arduboy.setCursor(41, 23);
      arduboy.print(F("SELECTED"));
      arduboy.drawRoundRect(39, 21, 51, 11, 3);
    }
    }   
}

void gameBar(){
  if(arduboy.everyXFrames(80))
     cloudXOne--;
  if(arduboy.everyXFrames(150))
     cloudXTwo--;  
  if(cloudXOne < -20)
     cloudXOne = 128;
  if(cloudXTwo < -20)
     cloudXTwo = 128;
  Sprites::drawOverwrite (cloudXOne, 20, cloud, 0);
  Sprites::drawOverwrite (cloudXTwo, 25, cloud, 0);
  arduboy.print(countPlatform);
  arduboy.fillCircle (62, 20, 9, WHITE);
  Sprites::drawPlusMask (102, 0, coin, 0);
  arduboy.setCursor(112, 0);
  arduboy.print(coinsCount);
}

void doSound(){
  //coin
    if((soundOn == 1) && (soundOn != 3))
        soundTimer++;
    if((soundTimer < 3) && (soundTimer > 0))
        beep.tone(beep.freq(900), 3);
    if(soundTimer > 3)
       beep.tone(beep.freq(1500), 3);
    if(soundTimer == 6){
       soundOn = 0;
       soundTimer = 0;
    }
    
    //platform move up
    if(soundOn == 2)
       soundTimer2++;
    if((soundTimer2 < 8) && (soundTimer2 > 0))
        beep.tone(beep.freq(50), 3);
    if(soundTimer2 > 8)
       beep.tone(beep.freq(100), 3);
    if(soundTimer2 == 13){
       soundOn = 0;
       soundTimer2 = 0;
    }
    
  //perfectCount
    if((soundOn == 3) && (soundOn != 1))
        soundTimer3++;
    if((soundTimer3 < 5) && (soundTimer3 > 0))
        beep.tone(beep.freq(300), 3);
    if((soundTimer3 >= 5) && (soundTimer3 < 10))
        beep.tone(beep.freq(600), 3);
    if(soundTimer3 > 15)
       beep.tone(beep.freq(900), 3);
    if(soundTimer3 == 20){
       soundOn = 0;
       soundTimer3 = 0;
    }
}
