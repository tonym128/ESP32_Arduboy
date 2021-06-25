#include <Arduboy2.h>
#include "CascadePathGame.h"

Arduboy2 display;
CascadePathGame Game(&display);

void inogamesetup() {
  Serial.begin(115200);
  display.begin();
  Game.Begin();
}
void inogameloop() {

  Game.Cycle();

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
