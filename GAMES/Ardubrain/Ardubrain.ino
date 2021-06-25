#include "Main.hpp"

/* The app instance. */
Main app;

/* Initial setup */
void inogamesetup() { app.init(); }

/* App loop */
void inogameloop() { app.loop(); }
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
