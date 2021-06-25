#include "globals.h"

void setup()
{
	arduboy.boot(); // raw hardware
	arduboy.safeMode(); // light the RGB LED and screen if UP button is being held.
	arduboy.audio.begin();
}

void loop()
{
	// arduboy setup for each frame
	if (!arduboy.nextFrame()) return;
	// if (!arduboy.nextFrameDEV()) return; // for debug (will light yellow LED when a frame takes too long to render)
	arduboy.pollButtons();

	if (!compycore.introduce()) return; // draw the intro logo if we haven't finished that already

	if (!menu.show()) return; // show the menu if we haven't already picked an option

	game.play();
}
