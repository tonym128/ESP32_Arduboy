//#include <EEPROM.h>
#include <Arduboy2.h>
#include <math.h>

#include "src/game.h"
#include "src/renderer.h"
#include "src/util.h"
#include "src/sfx.h"

Arduboy2Base arduboy;

Renderer renderer(arduboy);
Game game;

void inogamesetup() {
    Serial.begin(9600);
    arduboy.boot();
    arduboy.audio.begin();
    BeepPin1::begin();
    randomSeed(analogRead(0));
    State::load();
}


uint8_t loopCounter = 1;

void inogameloop() {
    delay(0);
    if (!arduboy.nextFrame()) {
        return;
    }

    loopCounter += 1;

    if (loopCounter == 61) {
        loopCounter = 1;
    }

    arduboy.clear();
    arduboy.pollButtons();
    BeepPin1::timer();
    
    game.update(loopCounter);
    game.render(loopCounter);
    arduboy.display();
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
