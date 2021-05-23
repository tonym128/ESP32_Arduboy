#include "Lagunita.hpp"

/* The game instance. */
Lagunita game;


/* To save some memory. */
//ARDUBOY_NO_USB

/* Initial setup */
void setup() { Serial.begin(115200); game.init(); }

/* Game loop */
void loop() { game.loop(); }
