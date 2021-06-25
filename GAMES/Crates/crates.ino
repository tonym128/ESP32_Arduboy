#include "globals.h"

void inogamesetup()
{
	arduboy.boot(); // raw hardware
	arduboy.safeMode(); // light the RGB LED and screen if UP button is being held.
	arduboy.audio.begin();
}

void inogameloop()
{
	// arduboy setup for each frame
	if (!arduboy.nextFrame()) return;
	// if (!arduboy.nextFrameDEV()) return; // for debug (will light yellow LED when a frame takes too long to render)
	arduboy.pollButtons();

	if (!compycore.introduce()) return; // draw the intro logo if we haven't finished that already

	if (!menu.show()) return; // show the menu if we haven't already picked an option

	game.play();
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
