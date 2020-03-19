/*
ArduBoy Jet Pac
Version 1.1
2nd Feb 2017
by Mike McRoberts (a.k.a. TheArduinoGuy)
*/

#define SAVELOCATION (/*EEPROM_STORAGE_SPACE_START +*/ 90)

#include <ESP8266WiFi.h>
#include <Arduboy2.h>
#include "bitmaps.h"
#include <ArduboyTones.h>
//#include <avr/pgmspace.h>
//#include <EEPROM.h>

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

byte level = 1;
byte gameDifficulty = 2;
byte levelStep = 0;
byte rocketStep = 0;
byte monsterType = 0;
byte gameState = 4;
byte subState = 0;
unsigned long highScore;
int8_t lives = 4;
unsigned long score = 0;

// fuel cells
float fuelX;
float fuelY = -8;
byte fuelPercent=0;

// Jet man data
float jetManX = 60;
float jetManY= 47;
float jetManXRate = 0;
float jetManYRate = 0;
boolean jetManDirection = 1;
byte jetManState=0;

// rocket data
byte topRocketX;
float topRocketY;
byte middleRocketX;
float middleRocketY;
byte bottomRocketX;
float bottomRocketY;

// data for the bonus items
byte bonusX;
float bonusY;
boolean bonusState;
byte bonusType;

byte cloudIndex = 0;
boolean jetPacFired = 0;
boolean buttonBPressed = false;
byte animToggle;
unsigned long frameRate;
unsigned long lastPressed;

const byte platforms[] PROGMEM = {
        2,4,3, 7,5,2, 11,3,3
};

const float rocketParts[] PROGMEM= {
        24,24, 60,32, 76,55
};

struct movingObjects
{
        float x;
        float y;
        float xRate;
        float yRate;
        boolean direction;
};

movingObjects monsters[10];

struct cloudObjects
{
        byte state;
        byte x;
        byte y;
        unsigned long time;
};

cloudObjects clouds[10];

struct laserObjects
{
        byte x;
        byte y;
        byte length;
        boolean direction;
        unsigned long time;
};

laserObjects laserbeams[20];
byte laserIndex = 0;

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void setup()
{
        //Serial.begin(115200);
        WiFi.mode(WIFI_OFF); //disable wifi to save some battery power
        EEPROM.begin(200);
        arduboy.boot(); // raw hardware
        arduboy.blank(); // blank the display
        //arduboy.flashlight(); // light the RGB LED and screen if UP button is being held.
        arduboy.systemButtons();
        arduboy.bootLogo();
        arduboy.audio.on();
        arduboy.clear();
        //arduboy.begin();
        arduboy.initRandomSeed();
        arduboy.setFrameRate(60);
        frameRate = millis();
        lastPressed = millis();
        initialiseRocket();
        arduboy.drawSlowXYBitmap(0, 0, JETPACLOADINGSCREEN, 128, 64);
        arduboy.display();
        delay(5000);
        EEPROM.get(SAVELOCATION, highScore);
}

float RANDOMXL(){
        return (0-random(64));}

float RANDOMXR(){
        return (128+random(64));}

float RANDOMY(){
        return (random(48)+8);}

float RANDOMXRATE(){
        return (float(0.15+(random(90+(gameDifficulty*5))/100)));}

float RANDOMYRATE(){
        return ((random(40)/100)-0.2);}

float FUELRANDOM(){
        return (random(2) ? 16+random(48) : 88+random(16));}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void initialiseGameState(){
        gameState = 1;
        jetManX=60;
        jetManY=47;
        jetManDirection=1;
        jetManXRate = 0;
        jetManYRate = 0;

        for (int index=0; index<9; index++)
        {
                clouds[index] = {0,0,0,0};
        }
        for (int index=0; index<19; index++)
        {
                laserbeams[index] = {0,0,0,0,0};
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void createMonsters()
{
        for (int index=0; index < 9; index++)
        {
                boolean DIRECTION = (boolean)random(2);
                monsters[index] = {(DIRECTION ? RANDOMXR() : RANDOMXL()), RANDOMY(), RANDOMXRATE(), RANDOMYRATE(), DIRECTION};
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void initialiseRocket()
{
        topRocketX = pgm_read_float(&rocketParts[0]);
        topRocketY = pgm_read_float(&rocketParts[1]);
        middleRocketX = pgm_read_float(&rocketParts[2]);
        middleRocketY = pgm_read_float(&rocketParts[3]);
        bottomRocketX = pgm_read_float(&rocketParts[4]);
        bottomRocketY = pgm_read_float(&rocketParts[5]);
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void drawPlatforms()
{
        for (byte index = 0; index<9; index=index+3)
        {
                byte length = pgm_read_byte(&platforms[index+2]);
                for (byte row = 0; row < length; row++)
                {
                        byte platformX = pgm_read_byte(&platforms[index]);
                        platformX = (platformX*8)+(row*8);
                        byte platformY = pgm_read_byte(&platforms[index+1]);
                        platformY = platformY*8;
                        arduboy.drawBitmap(platformX, platformY, PLATFORM, 8, 8);
                }
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void drawJetMan()
{
        if (jetManState==0) // standing
        {

                if (jetManDirection)
                {
                        arduboy.drawBitmap(jetManX, jetManY, JETMANRIGHTHEAD, 8, 8);
                        if (jetManXRate == 0)
                                arduboy.drawBitmap(jetManX, jetManY+8, JETMANRIGHT1, 8, 8);
                        else{
                                switch(animToggle)
                                {
                                case 0:
                                        arduboy.drawBitmap(jetManX, jetManY+8, JETMANRIGHT1, 8, 8);
                                        break;
                                case 1:
                                        arduboy.drawBitmap(jetManX, jetManY+8, JETMANRIGHT2, 8, 8);
                                        break;
                                case 2:
                                        arduboy.drawBitmap(jetManX, jetManY+8, JETMANRIGHT3, 8, 8);
                                        break;
                                case 3:
                                        arduboy.drawBitmap(jetManX, jetManY+8, JETMANRIGHT2, 8, 8);
                                        break;
                                }
                        }
                }
                else
                {
                        arduboy.drawBitmap(jetManX, jetManY, JETMANLEFTHEAD, 8, 8);
                        if (jetManXRate == 0)
                                arduboy.drawBitmap(jetManX, jetManY+8, JETMANLEFT1, 8, 8);
                        else{
                                switch(animToggle)
                                {
                                case 0:
                                        arduboy.drawBitmap(jetManX, jetManY+8, JETMANLEFT1, 8, 8);
                                        break;
                                case 1:
                                        arduboy.drawBitmap(jetManX, jetManY+8, JETMANLEFT2, 8, 8);
                                        break;
                                case 2:
                                        arduboy.drawBitmap(jetManX, jetManY+8, JETMANLEFT3, 8, 8);
                                        break;
                                case 3:
                                        arduboy.drawBitmap(jetManX, jetManY+8, JETMANLEFT2, 8, 8);
                                        break;
                                }
                        }
                }
        }

        if (jetManState==1 || jetManState==2) // Rising or falling
        {
                if (jetManDirection)
                {
                        arduboy.drawBitmap(jetManX, jetManY, JETMANRIGHTHEAD, 8, 8);
                        switch(animToggle)
                        {
                        case 0:
                                arduboy.drawBitmap(jetManX, jetManY+8, JETMANRIGHTFLY1, 8, 8);
                                break;
                        case 1:
                                arduboy.drawBitmap(jetManX, jetManY+8, JETMANRIGHTFLY2, 8, 8);
                                break;
                        case 2:
                                arduboy.drawBitmap(jetManX, jetManY+8, JETMANRIGHTFLY3, 8, 8);
                                break;
                        case 3:
                                arduboy.drawBitmap(jetManX, jetManY+8, JETMANRIGHTFLY2, 8, 8);
                                break;
                        }
                }
                else
                {
                        arduboy.drawBitmap(jetManX, jetManY, JETMANLEFTHEAD, 8, 8);
                        switch(animToggle)
                        {
                        case 0:
                                arduboy.drawBitmap(jetManX, jetManY+8, JETMANLEFTFLY1, 8, 8);
                                break;
                        case 1:
                                arduboy.drawBitmap(jetManX, jetManY+8, JETMANLEFTFLY2, 8, 8);
                                break;
                        case 2:
                                arduboy.drawBitmap(jetManX, jetManY+8, JETMANLEFTFLY3, 8, 8);
                                break;
                        case 3:
                                arduboy.drawBitmap(jetManX, jetManY+8, JETMANLEFTFLY2, 8, 8);
                                break;
                        }
                }
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void drawThings()
{
        for (byte index = 0; index < gameDifficulty; index++)
        {
                float x = monsters[index].x;
                float y = monsters[index].y;
                boolean direction = monsters[index].direction;

                if (monsterType==0)
                {
                        if (direction==1) arduboy.drawBitmap(x, y, (animToggle%2)==0 ? ASTEROIDRIGHT1 : ASTEROIDRIGHT2, 8, 8);
                        else arduboy.drawBitmap(x, y, (animToggle%2)==0 ? ASTEROIDLEFT1 : ASTEROIDLEFT2, 8, 8);
                }

                if (monsterType==1)
                {
                        arduboy.drawBitmap(x, y, (animToggle%2)==0 ? FUZZY1 : FUZZY2, 8, 8);
                }

                if (monsterType==2)
                {
                        arduboy.drawBitmap(x, y, (animToggle%2)==0 ? BALL1 : BALL2, 8, 8);
                }

                if (monsterType==3)
                {
                        if (direction==1) arduboy.drawBitmap(x, y, (animToggle%2)==0 ? PLANE1 : PLANE1, 8, 8);
                        else arduboy.drawBitmap(x, y, (animToggle%2)==0 ? PLANE2 : PLANE2, 8, 8);
                }

                if (monsterType==4)
                {
                        arduboy.drawBitmap(x, y, (animToggle%2)==0 ? CROSS : CROSS, 8, 8);
                }

                if (monsterType==5)
                {
                        if (direction==1) arduboy.drawBitmap(x, y, (animToggle%2)==0 ? FALCON1 : FALCON1, 8, 8);
                        else arduboy.drawBitmap(x, y, (animToggle%2)==0 ? FALCON2 : FALCON2, 8, 8);
                }

                if (monsterType==6)
                {
                        arduboy.drawBitmap(x, y, (animToggle%2)==0 ? UFO1 : UFO2, 8, 8);
                }

                if (monsterType==7)
                {
                        arduboy.drawBitmap(x, y, (animToggle%2)==0 ? JELLY : JELLY, 8, 8);
                }

                arduboy.drawBitmap(fuelX, fuelY, FUEL, 8, 8);
                drawRocket();

                if (bonusState !=0)
                {
                        switch(bonusType)
                        {
                        case 0:
                                arduboy.drawBitmap(bonusX, bonusY, (animToggle%2)==0 ? DIAMOND : BLANK, 8, 8);
                                break;
                        case 1:
                                arduboy.drawBitmap(bonusX, bonusY, (animToggle%2)==0 ? BREAD : BLANK, 8, 8);
                                break;
                        case 2:
                                arduboy.drawBitmap(bonusX, bonusY, (animToggle%2)==0 ? GOLD : BLANK, 8, 8);
                                break;
                        case 3:
                                arduboy.drawBitmap(bonusX, bonusY, (animToggle%2)==0 ? ATOM : BLANK, 8, 8);
                                break;
                        case 4:
                                arduboy.drawBitmap(bonusX, bonusY, (animToggle%2)==0 ? NUKE : BLANK, 8, 8);
                                break;
                        }
                }
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void drawRocket()
{
        switch(rocketStep)
        {
        case 0:
                arduboy.drawBitmap(topRocketX, topRocketY, TOPROCKET, 8, 8);
                arduboy.drawBitmap(middleRocketX, middleRocketY, MIDDLEROCKET, 8, 8);
                arduboy.drawBitmap(bottomRocketX, bottomRocketY, BOTTOMROCKET, 8, 8);
                break;
        case 1:
                arduboy.drawBitmap(topRocketX, topRocketY, TOPSHUTTLE, 8, 8);
                arduboy.drawBitmap(middleRocketX, middleRocketY, MIDDLESHUTTLE, 8, 8);
                arduboy.drawBitmap(bottomRocketX, bottomRocketY, BOTTOMSHUTTLE, 8, 8);
                break;
        case 2:
                arduboy.drawBitmap(topRocketX, topRocketY, TOPFAT, 8, 8);
                arduboy.drawBitmap(middleRocketX, middleRocketY, MIDDLEFAT, 8, 8);
                arduboy.drawBitmap(bottomRocketX, bottomRocketY, BOTTOMFAT, 8, 8);
                break;
        case 3:
                arduboy.drawBitmap(topRocketX, topRocketY, TOPFUTURE, 8, 8);
                arduboy.drawBitmap(middleRocketX, middleRocketY, MIDDLEFUTURE, 8, 8);
                arduboy.drawBitmap(bottomRocketX, bottomRocketY, BOTTOMFUTURE, 8, 8);
                break;
        }

        if (subState==11)
        {
                arduboy.drawBitmap(bottomRocketX, bottomRocketY+8, (animToggle%2)==0 ? THRUST1 : THRUST2, 8, 8);
                topRocketY -= 0.2;
                middleRocketY -= 0.2;
                bottomRocketY -= 0.2;
                jetManY = -20;
                if(bottomRocketY<-8) gameState = 3;
                explosionFX();
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void rocketLand()
{
        do {
                arduboy.clear();
                switch(rocketStep)
                {
                case 0:
                        arduboy.drawBitmap(topRocketX, topRocketY, TOPROCKET, 8, 8);
                        arduboy.drawBitmap(middleRocketX, middleRocketY, MIDDLEROCKET, 8, 8);
                        arduboy.drawBitmap(bottomRocketX, bottomRocketY, BOTTOMROCKET, 8, 8);
                        break;
                case 1:
                        arduboy.drawBitmap(topRocketX, topRocketY, TOPSHUTTLE, 8, 8);
                        arduboy.drawBitmap(middleRocketX, middleRocketY, MIDDLESHUTTLE, 8, 8);
                        arduboy.drawBitmap(bottomRocketX, bottomRocketY, BOTTOMSHUTTLE, 8, 8);
                        break;
                case 2:
                        arduboy.drawBitmap(topRocketX, topRocketY, TOPFAT, 8, 8);
                        arduboy.drawBitmap(middleRocketX, middleRocketY, MIDDLEFAT, 8, 8);
                        arduboy.drawBitmap(bottomRocketX, bottomRocketY, BOTTOMFAT, 8, 8);
                        break;
                case 3:
                        arduboy.drawBitmap(topRocketX, topRocketY, TOPFUTURE, 8, 8);
                        arduboy.drawBitmap(middleRocketX, middleRocketY, MIDDLEFUTURE, 8, 8);
                        arduboy.drawBitmap(bottomRocketX, bottomRocketY, BOTTOMFUTURE, 8, 8);
                        break;
                }
                arduboy.drawBitmap(bottomRocketX, bottomRocketY+8, (animToggle%2)==0 ? THRUST1 : THRUST2, 8, 8);
                topRocketY += 0.2;
                middleRocketY += 0.2;
                bottomRocketY += 0.2;
                jetManY = -20;
                arduboy.drawFastHLine(0, 63, 128); // ground
                drawPlatforms();
                arduboy.display();
                explosionFX();
                if ((millis()-frameRate)>50)
                {
                        animToggle++;
                        if (animToggle>3) animToggle=0;
                        frameRate=millis();
                }
                //Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
                delay(1);
        } while (bottomRocketY<=55);
        clouds[cloudIndex].state = 1;
        clouds[cloudIndex].x = bottomRocketX;
        clouds[cloudIndex].y = bottomRocketY+4;
        clouds[cloudIndex].time = millis();
        cloudIndex++;
        if (cloudIndex>9) cloudIndex=0;

        clouds[cloudIndex].state = 1;
        clouds[cloudIndex].x = bottomRocketX-4;
        clouds[cloudIndex].y = bottomRocketY;
        clouds[cloudIndex].time = millis();
        cloudIndex++;
        if (cloudIndex>9) cloudIndex=0;

        clouds[cloudIndex].state = 1;
        clouds[cloudIndex].x = bottomRocketX+4;
        clouds[cloudIndex].y = bottomRocketY;
        clouds[cloudIndex].time = millis();
        cloudIndex++;
        if (cloudIndex>9) cloudIndex=0;
        jetManX=60;
        jetManY=47;
        gameState=1;
        fuelPercent = 0;
        fuelX = FUELRANDOM();
        fuelY =-8;
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void drawClouds()
{
        for (byte index=0; index<9; index++)
        {
                if (clouds[index].state >0)
                {
                        switch(clouds[index].state)
                        {
                        case 1:
                                arduboy.drawBitmap(clouds[index].x,clouds[index].y, CLOUD1, 8, 8);
                                break;
                        case 2:
                                arduboy.drawBitmap(clouds[index].x-4,clouds[index].y-4, CLOUD2, 16, 16);
                                break;
                        case 3:
                                arduboy.drawBitmap(clouds[index].x-4,clouds[index].y-4, CLOUD3, 16, 16);
                                break;
                        }
                        if ((millis()-clouds[index].time)>100)
                        {
                                clouds[index].state++;
                                if (clouds[index].state>3) clouds[index].state = 0;
                                clouds[index].time = millis();
                        }
                }
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void drawlasers()
{
        for (byte index = 0; index < 20; index++)
        {
                if(laserbeams[index].length != 0)
                {
                        byte beamLength = laserbeams[index].length;
                        if (beamLength>8) beamLength = 8;
                        for (byte multiplier=0; multiplier<beamLength; multiplier++)
                        {
                                if ((laserbeams[index].length>4) && multiplier<4) arduboy.drawBitmap(laserbeams[index].x+((8*multiplier)*(laserbeams[index].direction ? 1 : -1)),laserbeams[index].y, LASER2, 8, 8);
                                else arduboy.drawBitmap(laserbeams[index].x+((8*multiplier)*(laserbeams[index].direction ? 1 : -1)),laserbeams[index].y, LASER1, 8, 8);
                                checkForHit(laserbeams[index].x+((8*multiplier)*(laserbeams[index].direction ? 1 : -1)), laserbeams[index].y);
                        }

                        if ((millis()-laserbeams[index].time)>20)
                        {
                                laserbeams[index].length +=1;
                                if (laserbeams[index].length>8) laserbeams[index].x +=(8*(laserbeams[index].direction ? 1 : -1));
                                if (laserbeams[index].length>16) laserbeams[index].length = 0;
                                laserbeams[index].time = millis();
                        }
                }
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void moveThings()
{
        for (byte index = 0; index < gameDifficulty; index++)
        {
                boolean direction = monsters[index].direction;
                float xRate = monsters[index].xRate;
                float yRate = monsters[index].yRate;

                monsters[index].x = monsters[index].x + (direction ? +xRate : -xRate);
                monsters[index].y = monsters[index].y + yRate;

                byte topA = monsters[index].y;
                byte bottomA = monsters[index].y+7;
                byte leftA = monsters[index].x;
                byte rightA = monsters[index].x+7;
                // get the bounding boxes of the platforms and monsters
                for (byte platformIndex = 0; platformIndex<9; platformIndex=platformIndex+3)
                {
                        byte length = pgm_read_byte(&platforms[platformIndex+2]);
                        byte leftB = pgm_read_byte(&platforms[platformIndex]);
                        leftB = (leftB*8);
                        byte topB = pgm_read_byte(&platforms[platformIndex+1]);
                        topB = topB*8;
                        byte rightB = leftB+(8*length);
                        byte bottomB = topB+3;

                        // check if object A (monster) has hit object B (platform)
                        if (topA <= bottomB && bottomA >= topB && leftA <= rightB && rightA >= leftB)
                        {
                                if (monsterType==0)
                                {
                                        clouds[cloudIndex].state = 1;
                                        clouds[cloudIndex].x = leftA;
                                        clouds[cloudIndex].y = topA;
                                        clouds[cloudIndex].time = millis();
                                        cloudIndex++;
                                        if (cloudIndex>9) cloudIndex=0;

                                        boolean DIRECTION = (boolean)random(2);
                                        monsters[index] = {(DIRECTION ? RANDOMXR() : RANDOMXL()), RANDOMY(), RANDOMXRATE(), RANDOMYRATE(), DIRECTION};
                                }
                                else
                                {
                                        // horizontal collision
                                        if (leftA < leftB || rightA > rightB)
                                        {
                                                monsters[index].direction = !(monsters[index].direction);
                                        }
                                        // vertical collision
                                        if (bottomA > bottomB || topA < topB)
                                        {
                                                monsters[index].yRate = -yRate;
                                        }
                                }
                        }
                }
                if ((monsters[index].x > 127) || (monsters[index].x < -7.0))
                {
                        monsters[index].x = direction ? -7 : 127;
                        monsters[index].xRate = RANDOMXRATE();
                }
                if ((monsters[index].y > 57) || (monsters[index].y < 7))
                {
                        monsters[index].yRate = -(monsters[index].yRate);
                }
                if (random(3000) == 1500 && monsterType != 0) monsters[index].direction = !monsters[index].direction;
                if (random(1000) == 500 && monsterType != 0) monsters[index].yRate = RANDOMYRATE();

        }

        if (bonusState==1)
        {
                for (byte index = 0; index<9; index=index+3)
                {
                        byte length = pgm_read_byte(&platforms[index+2]);
                        byte platformX = pgm_read_byte(&platforms[index] );
                        platformX = (platformX*8);
                        byte platformY = pgm_read_byte(&platforms[index+1] );
                        platformY = platformY*8;
                        byte platformEnd = platformX+(8*length);

                        if ( ((byte)bonusY==(platformY-8)) && (((bonusX+6)>=platformX) && ((bonusX+2)<=platformEnd)) )
                        {
                                bonusY = platformY-8;
                                bonusState = 2; // landed
                        }
                        else bonusY += 0.1;
                        if (bonusY>55)
                        {
                                bonusY = 55;
                                bonusState = 2; // landed
                        }
                }
        }

        switch(subState)
        {
        case 1:  // picked up middle rocket section
                middleRocketX = jetManX;
                middleRocketY = jetManY+8;
                if ((jetManX==bottomRocketX))
                {
                        subState=2;
                }
                break;
        case 2: // middle section falling
                middleRocketY += 0.2;
                if (middleRocketY >= 47)
                {
                        middleRocketY =47;
                        subState = 3;
                }
                break;
        case 4: // picked up top section
                topRocketX = jetManX;
                topRocketY = jetManY+8;
                if ((jetManX==bottomRocketX))
                {
                        subState=5;
                }
                break;
        case 5: // top section falling
                topRocketY += 0.2;
                if (topRocketY >= 39)
                {
                        topRocketY =39;

                        fuelX = FUELRANDOM();
                        fuelY=-8;
                        subState = 6;
                }
                break;
        case 6: // fuel falling
                for (byte index = 0; index<9; index=index+3)
                {
                        byte length = pgm_read_byte(&platforms[index+2]);
                        byte platformX = pgm_read_byte(&platforms[index] );
                        platformX = (platformX*8);
                        byte platformY = pgm_read_byte(&platforms[index+1] );
                        platformY = platformY*8;
                        byte platformEnd = platformX+(8*length);

                        if ( ((byte)fuelY==(platformY-8)) && (((fuelX+6)>=platformX) && ((fuelX+2)<=platformEnd)) )
                        {
                                fuelY = platformY-8;
                                subState = 7;
                        }
                        else fuelY += 0.1;
                        if (fuelY>55)
                        {
                                fuelY = 55;
                                subState = 7;
                        }
                }
                break;
        case 8: // jet man carrying fuel
                fuelX = jetManX;
                fuelY = jetManY+8;
                if ((jetManX==bottomRocketX))
                {
                        subState=9;
                }
                break;
        case 9: // fuel falling onto rocket
                fuelY += 0.2;
                if (fuelY >= 55)
                {
                        fuelX=FUELRANDOM();
                        fuelY=-8;
                        fuelPercent += 25;
                        subState = 6;
                        if (fuelPercent>=100) subState = 10;
                }
                break;

        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void moveJetMan()
{
        jetManX = jetManX + (jetManDirection ? +jetManXRate : -jetManXRate);
        jetManY = jetManY + jetManYRate;

        if (jetManX > 119)
        {
                jetManXRate = 0;
                jetManX = 119;

        }
        if (jetManX < 0)
        {
                jetManXRate = 0;
                jetManX = 0;
        }

        if (jetManY <0)
        {
                jetManYRate = 0;
                jetManY = 0;
        }

        if (jetManY >=47)
        {
                jetManYRate = 0;
                jetManY = 47;
                jetManState = 0;           // Standing
        }

        for (byte index = 0; index<9; index=index+3)
        {
                byte length = pgm_read_byte(&platforms[index+2]);
                byte platformX = pgm_read_byte(&platforms[index] );
                platformX = (platformX*8);
                byte platformY = pgm_read_byte(&platforms[index+1] );
                platformY = platformY*8;
                byte platformEnd = platformX+(8*length);

                if (((jetManState == 0) && (jetManY<47)) && ((jetManX+6<platformX) || (jetManX>platformEnd)) )
                {
                        jetManState = 2;
                        jetManYRate = 0.5;
                }

                if ((((jetManX+6)>=platformX) && ((jetManX+4)<=platformEnd)) && (jetManY==platformY-16))
                {
                        jetManState = 0;
                        jetManY = platformY-16;
                        jetManXRate = 0;
                        jetManYRate = 0;
                        break;
                }
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void checkButtons()
{
        if( arduboy.pressed(A_BUTTON) == true || arduboy.pressed(UP_BUTTON) == true) {
                jetPacFire();
        }

        if( arduboy.pressed(A_BUTTON) == false && arduboy.pressed(UP_BUTTON) == false) {
                jetPacRelease();
        }

        if( arduboy.notPressed(RIGHT_BUTTON) == true ) {
                jetManXRate = 0;
        }

        if( arduboy.notPressed(LEFT_BUTTON) == true ) {
                jetManXRate = 0;
        }

        if( arduboy.pressed(RIGHT_BUTTON) == true ) {
                jetManXRate = 1;
                jetManDirection = 1;
        }

        if( arduboy.pressed(LEFT_BUTTON) == true ) {
                jetManXRate = 1;
                jetManDirection = 0;
        }

        if((subState<11) && (arduboy.pressed(B_BUTTON) == true || arduboy.pressed(DOWN_BUTTON) == true) && ((millis()-lastPressed)>100))
        {
                laserbeams[laserIndex].x = jetManX + (jetManDirection ? 10 : -10);
                laserbeams[laserIndex].length = 1;
                laserbeams[laserIndex].y = jetManY+9;
                laserbeams[laserIndex].direction = jetManDirection;
                laserbeams[laserIndex].time = millis();
                laserIndex++;
                if (laserIndex>19) laserIndex=0;

                for (int start = 50; start < 125; start = start + 1) {
                        digitalWrite(D3, HIGH); //positive square wave
                        //digitalWrite(5, LOW); //positive square wavedelayMicroseconds(start);      //192uS
                        delayMicroseconds(start);
                        digitalWrite(D3, LOW); //neutral square wave
                        //digitalWrite(5, HIGH); //positive square wave
                        delayMicroseconds(start); //192uS
                }
                lastPressed = millis();
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void jetPacFire()
{
        jetManState = 1; // rising;
        jetManYRate = -1;

        if (jetPacFired == false)
        {
                clouds[cloudIndex].state = 1;
                clouds[cloudIndex].x = jetManX;
                clouds[cloudIndex].y = jetManY+8;
                clouds[cloudIndex].time = millis();
                cloudIndex++;
                if (cloudIndex>9) cloudIndex=0;
        }
        jetPacFired = true;
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void jetPacRelease()
{
        if (jetManState == 1)
        {
                jetManState = 2; // falling;
                jetManYRate = 0.5;
        }
        jetPacFired = false;
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void checkForHit(byte laserX, byte laserY)
{
        for (byte index=0; index<gameDifficulty; index++)
        {
                boolean hit=false;
                byte x = monsters[index].x;
                byte y = monsters[index].y;
                //boolean direction = monsters[index].direction;

                if ((laserY >= y && laserY <=y+7) && (laserX>=x && laserX<=x+7))
                {
                        hit = true;
                }

                if (hit==true)
                {

                        clouds[cloudIndex].state = 1;

                        explosionFX();
                        explosionFX();

                        clouds[cloudIndex].x = x;
                        clouds[cloudIndex].y = y;
                        clouds[cloudIndex].time = millis();
                        cloudIndex++;
                        if (cloudIndex>9) cloudIndex=0;
                        score=score+25;

                        boolean DIRECTION = (boolean)random(2);
                        monsters[index] = {DIRECTION ? RANDOMXR() : RANDOMXL(), RANDOMY(),RANDOMXRATE(),RANDOMYRATE(),DIRECTION};
                }
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void checkJetManHit()
{
        for (byte index=0; index<gameDifficulty; index++)
        {
                boolean hit=false;
                byte x = monsters[index].x;
                byte y = monsters[index].y;
                byte manX = jetManX;
                byte manY = jetManY;
                //boolean direction = monsters[index].direction;

                if (((x>=manX+1 && x<=manX+6) && (y>=manY+1 && y<=manY+14)) || ((x+7>=manX+1 && x+7<=manX+6) && (y+7>=manY+1 && y+7<=manY+14)))
                {
                        hit = true;
                }

                if (hit==true)
                {
                        if (subState==8) subState = 6;

                        explosionFX();
                        explosionFX();
                        explosionFX();
                        explosionFX();

                        clouds[cloudIndex].state = 1;
                        clouds[cloudIndex].x = manX;
                        clouds[cloudIndex].y = manY+4;
                        clouds[cloudIndex].time = millis();
                        cloudIndex++;
                        if (cloudIndex>9) cloudIndex=0;

                        clouds[cloudIndex].state = 1;
                        clouds[cloudIndex].x = manX-4;
                        clouds[cloudIndex].y = manY;
                        clouds[cloudIndex].time = millis();
                        cloudIndex++;
                        if (cloudIndex>9) cloudIndex=0;

                        clouds[cloudIndex].state = 1;
                        clouds[cloudIndex].x = manX+4;
                        clouds[cloudIndex].y = manY;
                        clouds[cloudIndex].time = millis();
                        cloudIndex++;
                        if (cloudIndex>9) cloudIndex=0;

                        gameState=0;
                        jetManX=60;
                        jetManY=47;
                        jetManDirection=1;
                        jetManXRate = 0;
                        jetManYRate = 0;

                        lives=lives-1;
                        if (lives<0) gameState = 2;
                        sound.tone(800,200, 600, 200, 200, 400);
                        for (int x=0; x<120; x++)
                        {
                                arduboy.clear();
                                arduboy.drawFastHLine(0, 63, 128); // ground
                                drawPlatforms();
                                drawThings();
                                drawClouds();
                                drawlasers();
                                //Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
                                arduboy.display();
                                delay(2);
                        }

                }
        }
}

//#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void explosionFX()
{
        // Explosion sound
        for (int start = 0; start < 25; start = start + 1) {
                digitalWrite(D3, HIGH);
                //digitalWrite(5, LOW);
                delayMicroseconds(random(100));
                digitalWrite(D3, LOW);
                //digitalWrite(5, HIGH);
                delayMicroseconds(random(100));
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void gameOn()
{
        arduboy.drawFastHLine(0, 63, 128); // ground
        if (fuelPercent>0 && subState<11)
        {
                if (((animToggle%2)==0) && (fuelPercent==100)) arduboy.drawFastHLine(0,0,1.28*fuelPercent);
                if (fuelPercent<100) arduboy.drawFastHLine(0,0,1.28*fuelPercent);
        }
        drawPlatforms();
        drawThings();
        if (subState<11)
        {
                drawJetMan();
                checkJetManHit();
        }
        drawClouds();
        drawlasers();
        pickUpItem();

        checkButtons();
        moveJetMan();
        moveThings();

        if (subState>6 && bonusState==0 && random(500)==250)
        {
                bonusX = FUELRANDOM();
                bonusY=-8;
                bonusState = 1; // item created
                bonusType = random(5);
        }

        if ((millis()-frameRate)>50)
        {
                animToggle++;
                if (animToggle>3) animToggle=0;
                frameRate=millis();
        }
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void gameOver()
{
        if (score > highScore) {
           EEPROM.put(SAVELOCATION, score); EEPROM.commit();
        }
        delay(100);
        arduboy.clear();
        arduboy.setTextSize(2);
        arduboy.setCursor(8, 40);
        arduboy.print("GAME OVER ");
        arduboy.setTextSize(1);

        arduboy.setCursor(22, 2);
        if (score > highScore) arduboy.print("NEW HIGH SCORE");
        else
        {
                arduboy.setCursor(48, 2);
                arduboy.print("SCORE");
        }
        byte len = floor (log10 (abs (score))) + 1;
        arduboy.setCursor(60-(2*len),18);
        arduboy.print(score);
        arduboy.display();
        //Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
        delay(5000);

        level = 1;
        gameDifficulty = 2;
        levelStep = 0;
        rocketStep = 0;
        monsterType = 0;
        gameState = 0;
        subState = 0;
        lives = 4;
        score = 0;

        fuelY = -8;
        fuelPercent=0;

        jetManX = 60;
        jetManY= 47;
        jetManXRate = 0;
        jetManYRate = 0;
        jetManDirection = 1;
        jetManState=0;

        cloudIndex = 0;
        jetPacFired = 0;
        buttonBPressed = false;

        initialiseGameState();
        initialiseRocket();
        createMonsters();
        levelStart();
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void levelStart()
{
        EEPROM.get(SAVELOCATION, highScore);

        arduboy.clear();
        arduboy.setTextSize(1);
        arduboy.setCursor(1,1);

        arduboy.print("Score: ");
        arduboy.setCursor(70,1);
        arduboy.print(score);

        arduboy.setCursor(1,14);
        arduboy.print("High Score: ");
        arduboy.setCursor(70,14);
        if (score>highScore) arduboy.print(score);
        else arduboy.print(highScore);

        arduboy.drawBitmap(1,27, LITTLEMAN, 8, 8);
        arduboy.setCursor(12, 27);
        arduboy.print(lives);

        arduboy.setTextSize(2);
        arduboy.setCursor(12, 46);
        arduboy.print("LEVEL ");
        arduboy.print(level);

        arduboy.display();
        //Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
        do  {
          delay(5);
                //Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
        } while(arduboy.notPressed(A_BUTTON) && arduboy.notPressed(B_BUTTON));
        sound.tone(1000, 200);
        delay(500);
        createMonsters();
        gameState=1;
        bonusState=0;
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void levelComplete()
{
        initialiseGameState();
        subState = 0;
        level = level + 1;
        monsterType = monsterType + 1;
        if (monsterType==8) {
                monsterType = 0;
                gameDifficulty = gameDifficulty + 1;
                if (gameDifficulty>10) gameDifficulty=10;
        }
        levelStart();
        levelStep++;
        if (levelStep!=5)
        {
                rocketLand();
                subState=6;
        }
        else
        {
                initialiseRocket();
                subState=0;
                levelStep=0;
                rocketStep++;
                if(rocketStep==4) rocketStep=0;
        }
        fuelPercent = 0;
        arduboy.initRandomSeed();
}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void pickUpItem()
{
        byte topJ = jetManY+8;
        byte bottomJ = jetManY+15;
        byte leftJ = jetManX;
        byte rightJ = jetManX+7;

        if (bonusState!=0) // pick up the bonus item
        {
                byte topR = bonusY;
                byte bottomR = bonusY+7;
                byte leftR = bonusX;
                byte rightR = bonusX+7;
                if (topJ <= bottomR && bottomJ >= topR && leftJ <= rightR && rightJ >= leftR)
                {
                        bonusState=0;
                        score += 250;
                        sound.tone(NOTE_A5,50,NOTE_A6,50,NOTE_A7,100);
                }
        }
        if (subState==0) // pick up the middle rocket section
        {
                byte topR = middleRocketY;
                byte bottomR = middleRocketY+7;
                byte leftR = middleRocketX;
                byte rightR = middleRocketX+7;
                if (topJ <= bottomR && bottomJ >= topR && leftJ <= rightR && rightJ >= leftR)
                {
                        subState=1;
                        score += 100;
                        sound.tone(NOTE_A4,50,NOTE_A5,50,NOTE_A6,100);
                }
        }

        if (subState==3) // pick up the top rocket section
        {
                byte topR = topRocketY;
                byte bottomR = topRocketY+7;
                byte leftR = topRocketX;
                byte rightR = topRocketX+7;
                if (topJ <= bottomR && bottomJ >= topR && leftJ <= rightR && rightJ >= leftR)
                {
                        subState=4;
                        score += 100;
                        sound.tone(NOTE_A4,50,NOTE_A5,50,NOTE_A6,100);
                }
        }
        if (subState==6 || subState==7) // pick up fuel cell
        {
                byte topR = fuelY;
                byte bottomR = fuelY+7;
                byte leftR = fuelX;
                byte rightR = fuelX+7;
                if (topJ <= bottomR && bottomJ >= topR && leftJ <= rightR && rightJ >= leftR)
                {
                        subState=8;
                        score += 100;
                        sound.tone(NOTE_A4,50,NOTE_A5,50,NOTE_A6,100);
                }
        }

        if (subState==10) // Walk into the rocket
        {
                byte topR = bottomRocketY;
                byte bottomR = bottomRocketY+7;
                byte leftR = bottomRocketX;
                byte rightR = bottomRocketX+7;
                if (topJ <= bottomR && bottomJ >= topR && leftJ <= rightR && rightJ >= leftR)
                {
                        subState=11;

                        clouds[cloudIndex].state = 1;
                        clouds[cloudIndex].x = bottomRocketX;
                        clouds[cloudIndex].y = bottomRocketY+4;
                        clouds[cloudIndex].time = millis();
                        cloudIndex++;
                        if (cloudIndex>9) cloudIndex=0;

                        clouds[cloudIndex].state = 1;
                        clouds[cloudIndex].x = bottomRocketX-4;
                        clouds[cloudIndex].y = bottomRocketY;
                        clouds[cloudIndex].time = millis();
                        cloudIndex++;
                        if (cloudIndex>9) cloudIndex=0;

                        clouds[cloudIndex].state = 1;
                        clouds[cloudIndex].x = bottomRocketX+4;
                        clouds[cloudIndex].y = bottomRocketY;
                        clouds[cloudIndex].time = millis();
                        cloudIndex++;
                        if (cloudIndex>9) cloudIndex=0;
                        sound.tone(NOTE_A4,50,NOTE_A5,50,NOTE_A6,100);

                        score += 500;
                }
        }
}

void menu()
{
        boolean up = true;

        while (1)
        {
                if( arduboy.pressed(UP_BUTTON) == true ) {
                        up = true;
                        arduboy.clear();
                }

                if( arduboy.pressed(DOWN_BUTTON) == true) {
                        up = false;
                        arduboy.clear();
                }

                if (up == true)
                {
                        arduboy.fillCircle(13,23,3);
                }
                else
                {
                        arduboy.fillCircle(13,43,3);
                }

                if( arduboy.pressed(A_BUTTON) == true ) {
                    if (up == true)
                    {
                      delay(150);
                      gameState=0;
                      break;
                    }
                    else
                    {
                      delay(150);
                      eraseEEPROM();
                      gameState=0;
                      break;
                    }
                }
                arduboy.setCursor(20,20);
                arduboy.print("NEW GAME");
                arduboy.setCursor(20,40);
                arduboy.print("RESET HIGH SCORE");
                arduboy.display();
        delay(1);
        }
}

void eraseEEPROM()
{
arduboy.clear();
          boolean up = false;

          while (1)
          {
                  if( arduboy.pressed(UP_BUTTON) == true ) {
                          up = true;
                          arduboy.clear();
                  }

                  if( arduboy.pressed(DOWN_BUTTON) == true) {
                          up = false;
                          arduboy.clear();
                  }

                  if (up == true)
                  {
                          arduboy.fillCircle(13,23,3);
                  }
                  else
                  {
                          arduboy.fillCircle(13,43,3);
                  }

                  if( arduboy.pressed(A_BUTTON) == true ) {
                      if (up == true)
                      {
                        delay(150);
                        highScore = 0;   // First time run of program only then comment out and reload
                        EEPROM.put(SAVELOCATION, highScore);    // First time run of program only
                        EEPROM.commit();
                        gameState=0;
                        break;
                      }
                      else
                      {
                        delay(150);
                        gameState=0;
                        break;
                      }
                  }
                  arduboy.setCursor(20,00);
                  arduboy.print("ARE YOU SURE?");
                  arduboy.setCursor(20,20);
                  arduboy.print("YES");
                  arduboy.setCursor(20,40);
                  arduboy.print("NO");
                  arduboy.display();
          delay(1);
          }

}

// #=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=
void loop() {
        if (!arduboy.nextFrame()) {delay(1); return;}  // Keep frame rate at 60fps
        arduboy.clear();

        switch(gameState)
        {
        case 0:
                levelStart();
                break;
        case 1:
                gameOn();
                break;
        case 2:
                gameOver();
                break;
        case 3:
                levelComplete();
                break;
        case 4:
                menu();
                break;
        }
        delay(1);

        //Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
        arduboy.display();
}
