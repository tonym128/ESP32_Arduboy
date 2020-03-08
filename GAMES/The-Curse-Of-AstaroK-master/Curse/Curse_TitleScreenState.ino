#include "src/utils/Arduboy2Ext.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void titleScreen_Activate() {

	memset(&titleScreenVars, 0, 2);
	sound.tones(Sounds::title);

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void titleScreen_Update() {

  auto justPressed = arduboy.justPressedButtons();
  auto pressed = arduboy.pressedButtons();


	// Handle sound counter ..
	
	#ifdef SOUND_BUTTONS
	if (titleScreenVars.soundCounter > 0) titleScreenVars.soundCounter--;
	#endif


	// Restart ?

	if (pressed & DOWN_BUTTON) {

		if (titleScreenVars.restart < UPLOAD_DELAY) {
			titleScreenVars.restart++;
		}
		else {
			arduboy.exitToBootloader();
		}

	}
	else {
		titleScreenVars.restart = 0;
	}


	// Update counter ..

	if (titleScreenVars.counter < 32) titleScreenVars.counter++;


	// Handle other input ..

	if (justPressed & A_BUTTON) {
		gameState = GameStateType::PlayGame_Activate; 
	}


	// Toggle sound ..

	#ifdef SOUND_BUTTONS
	if (justPressed & B_BUTTON) {
		titleScreenVars.soundIndex = titleScreen_ToggleSoundSettings();
		titleScreenVars.soundCounter = 40;
	}
	#endif

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void titleScreen_Render() {

	ardBitmap.drawCompressed(0, 6, Images::TitleScreen, WHITE, MIRROR_NONE);
	if (titleScreenVars.counter == 32) ardBitmap.drawCompressed(48, 53, Images::TitleScreen_PressA, WHITE, MIRROR_NONE);

	#ifdef SOUND_BUTTONS
	if (titleScreenVars.soundCounter > 0) { SpritesB::drawOverwrite(120, 56, Images::Sounds, titleScreenVars.soundIndex); }
	#endif
	arduboy.display(true);
	
} 


/* ----------------------------------------------------------------------------
 *  Toggle the sound setting and commit to the EEPROM.
 */
uint8_t titleScreen_ToggleSoundSettings() {

	uint8_t index = 0;

  if (arduboy.audio.enabled()) {
  
    arduboy.audio.off(); 
		index = 0;
  
  }
  else {
  
    arduboy.audio.on(); 
		index = 1;
  
  }

  arduboy.audio.saveOnOff();

	return index;
    
}