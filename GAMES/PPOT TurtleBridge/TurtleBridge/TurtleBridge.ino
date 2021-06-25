#include <ArduboyTones.h>
#include "src/Game.h"
#include "src/utils/Arduboy2Ext.h"

#ifdef DEBUG_NO_USB
//ARDUBOY_NO_USB 
#endif

boolean outEn() {return true;}
ArduboyTones sound(outEn);
Game game;


void setup() {
	game.setup();
}

void loop() {
	game.loop();
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
