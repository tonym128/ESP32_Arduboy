#include "Lagunita.hpp"

/* The game instance. */
Lagunita game;


/* To save some memory. */
//ARDUBOY_NO_USB

/* Initial setup */
void inogamesetup() { Serial.begin(115200); game.init(); }

/* Game loop */
void inogameloop() { game.loop(); }
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
