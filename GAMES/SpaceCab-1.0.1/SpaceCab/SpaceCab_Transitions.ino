#include "src/Utils/Arduboy2Ext.h"
#include "src/Utils/Vector2.h"

Vector2 stars[16];

constexpr Vector2 centreScreen = Vector2(WIDTH / 2, HEIGHT / 2);

const Vector2 offsets[16] PROGMEM = {
	Vector2(1.0000, 0.0000),
	Vector2(0.9239, 0.3827),
	Vector2(0.7071, 0.7071),
	Vector2(0.3827, 0.9239),
	Vector2(0.0000, 1.0000),
	Vector2(-0.3827, 0.9239),
	Vector2(-0.7071, 0.7071),
	Vector2(-0.9239, 0.3827),
	Vector2(-1.0000, 0.0000),
	Vector2(-0.9239, -0.3827),
	Vector2(-0.7071, -0.7071),
	Vector2(-0.3827, -0.9239),
	Vector2(0.0000, -1.0000),
	Vector2(0.3827, -0.9239),
	Vector2(0.7071, -0.7071),
	Vector2(0.9239, -0.3827),
};

void initialiseStars(void) {

	for(uint8_t i = 0; i < 16; ++i) stars[i] = centreScreen;
	starsInitialised = true;

}

void resetStars(void) {
	starsInitialised = false;
}

void updateStarfield(void) {

	for(uint8_t i = 0; i < 16; ++i) {

		stars[i].x += SQ7x8::fromInternal(pgm_read_word(&offsets[i].x));
		stars[i].y += SQ7x8::fromInternal(pgm_read_word(&offsets[i].y));
		
		if (stars[i].x < 0 || stars[i].y < 0) stars[i] = centreScreen;

	}

}

void drawStarfield() {

	for(uint8_t i = 0; i < 16; ++i) {
		arduboy.drawPixel(static_cast<int8_t>(stars[i].x), static_cast<int8_t>(stars[i].y));
  }
  
}

void drawLevelStart(Font4x6 &font4x6, Level &level) {

  if (!starsInitialised) initialiseStars();

  updateStarfield();
  drawStarfield();

  arduboy.fillRect(18, 23, 94, 17, BLACK);
  arduboy.drawFastHLine(19, 25, 92, WHITE);
  arduboy.drawFastHLine(19, 38, 92, WHITE);

  font4x6.setCursor(22 + level.getLevelNameOffset(), 28);
  font4x6.print(getLevelName(level));

  if (arduboy.justPressed(A_BUTTON)) { 
    
    state = GameState::PlayGame_InitLevel; 
    
  }

}

void drawMaxLevelCompleted(Font4x6 &font4x6) {

  if (!starsInitialised) initialiseStars();

  updateStarfield();
  drawStarfield();

  arduboy.fillRect(15, 18, 98, 30, BLACK);
  arduboy.drawFastHLine(16, 19, 96, WHITE);
  arduboy.drawFastHLine(16, 46, 96, WHITE);

  font4x6.setCursor(43, 24);
  font4x6.print(F("You completed"));
  font4x6.setCursor(49, 34);
  font4x6.print(F("all levels!"));
	Sprites::drawOverwrite(18, 21, EndOfGameBadge, 0);

  if (arduboy.justPressed(A_BUTTON)) { 
    
    state = GameState::GameOver_Init; 
    
  }

}