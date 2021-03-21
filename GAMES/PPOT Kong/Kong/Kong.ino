#include "src/Game.h"
#include "src/utils/Arduboy2Ext.h"

#ifdef NO_USB
//ARDUBOY_NO_USB 
#endif

Game game;

boolean outEn() {return true;}
ArduboyTones sound(outEn);

void setup() {
	game.setup();
}

void loop() {
	game.loop();
}
