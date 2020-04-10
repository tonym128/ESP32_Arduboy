#include "src/utils/Arduboy2Ext.h"


void playGame_ChangeViewState(const ViewState viewState) {

	if ((viewState == ViewState::InDungeon_EnemysTurn) || (viewState == ViewState::InDungeon_EnemyDead)) {
	
    playGameVars.counter = (viewState == ViewState::InDungeon_EnemyDead) ? 0 : Constants::DialogueDelay_Enemy_Roll;
		playGameVars.matchedRuneCombination.roll();
		playGameVars.numberOfRecasts = 3;

		if (viewState == ViewState::InDungeon_EnemyDead && playGameVars.enemy.getEnemyType() == EnemyType::Astarok) {
			playGameVars.gameOver = true;
		}
	
  }
	else {
		
    playGame_ChangeViewState_Common();
    
		switch (viewState) {
        
			case ViewState::InTown:
				playGameVars.world.dungeon = 1;
				fadeInEffect.reset(2);
				sound.tones(Sounds::main);
				break;

			case ViewState::InDungeon_Entry:
				sound.noTone();
				fadeInEffect.reset(2);
				break;

			case ViewState::InShopItems:
			case ViewState::InShopRunes:
				fadeInEffect.reset(4);
				break;

      case ViewState::InDungeon_PlayerDead:
        playGameVars.counter = Constants::Player_Dead_Init;
        break;

      default: break;
    }
    
  }

	playGameVars.viewState = viewState;

}

void playGame_ChangeViewState_Common() {

	playGameVars.reroll = 0;
	playGameVars.counter = 0;
	playGameVars.runes.setReroll(true);

}

void playGame_SaveMessage_MultiPart(uint8_t arrayIndex, const uint8_t lines, const uint8_t linesOverall) {

	playGameVars.counter = Constants::DialogeDelay_MultiPart;

	playGameVars.message.arrayIndex = arrayIndex;
	playGameVars.message.lines = lines;
	playGameVars.message.topLine = 0;
	playGameVars.message.linesOverall= linesOverall;
	playGameVars.message.width= 130;
	playGameVars.message.renderRequired = true;
//	playGame_SaveMessage_MultiPart(arrayIndex, lines, linesOverall, Constants::DialogeDelay_MultiPart);

}

void playGame_SaveMessage_MultiPart(uint8_t arrayIndex, const uint8_t delay) {

	playGameVars.counter = delay;
	playGameVars.message.arrayIndex = arrayIndex;
	playGameVars.message.renderRequired = true;

}

void playGame_SaveMessage(const uint8_t message, const uint8_t lines, const uint8_t width, const uint8_t delay) {

	playGame_SaveMessage(message, lines, 1, width, delay);

}

void playGame_SaveMessage(const uint8_t message, const uint8_t lines, const uint8_t width) {

	playGame_SaveMessage(message, lines, 1, width, 0);

}

void playGame_SaveMessage(const uint8_t message, const uint8_t lines, const uint8_t linesOverall, const uint8_t width, const uint8_t delay) {

	playGameVars.counter = delay;

	playGameVars.message.message = message;
	playGameVars.message.lines = lines;
	playGameVars.message.topLine = 0;
	playGameVars.message.linesOverall= linesOverall;
	playGameVars.message.width= width;
	playGameVars.message.renderRequired = true;

}


void playGame_UpdateInventory(uint8_t justPressed) {

	switch (playGameVars.inventory.mode) {

		case InventoryMode::Inventory:
			{
				uint8_t itemCount = playGameVars.player.getDistinctItemCount();

				if (justPressed & UP_BUTTON) {
					if (playGameVars.inventory.cursorIndex > 2) { 
						playGameVars.inventory.cursorIndex--;
					}
					else {
						if (playGameVars.inventory.topIndex == 0) { 
							if (playGameVars.inventory.cursorIndex > 0) { 
								playGameVars.inventory.cursorIndex--;
							}
						} 
						else {
							playGameVars.inventory.topIndex--;
						}
					}
				}

				if (justPressed & DOWN_BUTTON) {
					if (playGameVars.inventory.cursorIndex < 2) {
						if (playGameVars.inventory.cursorIndex < itemCount - 1) { 
							playGameVars.inventory.cursorIndex++;
						}
					}
					else {
						if (playGameVars.inventory.topIndex + 4 >= itemCount) { 
							if (playGameVars.inventory.topIndex + playGameVars.inventory.cursorIndex < itemCount - 1) { 
								playGameVars.inventory.cursorIndex++;
							}
						} 
						else {
							if (playGameVars.inventory.topIndex + 4 < itemCount) {
								playGameVars.inventory.topIndex++;
							}
						}
					}
				}

				if (justPressed & A_BUTTON) {

					uint8_t itemSelected = playGameVars.player.getPlayerItemIndex_FromInvIndex(playGameVars.inventory.topIndex + playGameVars.inventory.cursorIndex);

					switch (itemSelected) {

						case 0 ... static_cast<uint8_t>(HelmetType::Count) - 1:
							playGameVars.player.setHelmetType(static_cast<HelmetType>(itemSelected));
							break;

						case static_cast<uint8_t>(HelmetType::Count) ... Constants::PlayerItems_Count - 1:
							{
								uint8_t index = itemSelected - static_cast<uint8_t>(HelmetType::Count);

								switch (static_cast<PotionType>(index)) {

									case PotionType::Fire:
										playGameVars.matchedRuneCombination.prevDmg1_Fire = playGameVars.matchedRuneCombination.prevDmg1_Fire + 10;
										// playGameVars.matchedRuneCombination.prevDmg2_Fire = playGameVars.matchedRuneCombination.prevDmg2_Fire + 10;
										break;

									case PotionType::Health:
										playGameVars.player.incHP(20);
										break;

									case PotionType::Speed:
										playGameVars.numberOfRecasts++;
										break;

									default: break;

								}

								playGameVars.player.decPotionType(static_cast<PotionType>(index));
								if (playGameVars.player.getPotionTypeCount(static_cast<PotionType>(index)) == 0 && playGameVars.inventory.cursorIndex > 0) { playGameVars.inventory.cursorIndex--; }

							}
							break;
							
					}

				}

			}

			break;

		case InventoryMode::Runes:

			if (justPressed & UP_BUTTON) {
				for (uint8_t x = playGameVars.inventory.runeIdx + Constants::RuneCombinations_Count - 1; x > playGameVars.inventory.runeIdx; x--) {
					uint8_t y = x % Constants::RuneCombinations_Count;
					if (playGameVars.player.getRuneCombination(y) > 0) {
						playGameVars.inventory.runeIdx = y;
						break;
					}
				}

			}

			if (justPressed & DOWN_BUTTON) {
				for (uint8_t x = playGameVars.inventory.runeIdx + 1; x < playGameVars.inventory.runeIdx + Constants::RuneCombinations_Count; x++) {
					uint8_t y = x % Constants::RuneCombinations_Count;
					if (playGameVars.player.getRuneCombination(y) > 0) {
						playGameVars.inventory.runeIdx = y;
						break;
					}
				}
			}
			
			break;

		default: break;

	}

	if (justPressed & LEFT_BUTTON) { 
		playGameVars.inventory.mode--;
	}

	if (justPressed & RIGHT_BUTTON) { 
		playGameVars.inventory.mode++;
	}

	if (justPressed & B_BUTTON) { 
		playGameVars.inventory.show = false; 
		playGameVars.inventory.returnVal = Constants::Inventory_NoAction;	
	}

}


// Allows a user to skip past a message as it is being shown ..

void playGame_Handle_Mulitpart_Messages(uint8_t & justPressed) {

	bool buttonsPressed = (justPressed & A_BUTTON || justPressed & B_BUTTON);

	if (buttonsPressed) {

		playGameVars.counter = 0;
		justPressed = 0;

	}

	playGameVars.message.renderRequired = !buttonsPressed;

}

void playGame_HandleMessages(uint8_t & justPressed) {

	if (justPressed > 0) {

		playGameVars.showSaveDialogue = false;
		playGameVars.counter = 0;
		playGameVars.message.renderRequired = false;
		justPressed = 0;

	}
	else {

		playGameVars.counter--;
		playGameVars.message.renderRequired = true;

	}

}


void drawMessageDialogBox(const uint8_t message1, const uint8_t message2, const uint8_t lines, const uint8_t width) {

	const int8_t left = 64 - (width / 2);
	const uint8_t yBottom = (lines * 9) + 11;

	drawMessageBox(left, -1, width, yBottom);

  font3x6.setCursor(left + 8, 5);
 	font3x6.printMessage(message1);
 	font3x6.print(static_cast<uint8_t>('\n'));
 	font3x6.printMessage(message2);

}


void drawMessageDialogBox(const uint8_t message, const uint8_t lines, const uint8_t width) {

	const int8_t left = 64 - (width / 2);
	const uint8_t yBottom = (lines * 9) + 11;

	drawMessageBox(left, -1, width, yBottom);

  font3x6.setCursor(left + 8, 5);
  font3x6.printMessage(message);

}

void drawMessageBox(const uint8_t lines, const uint8_t width) {

	const uint8_t yBottom = (lines * 9) + 11;
	const int8_t left = 64 - (width / 2);

	drawMessageBox(left, -1, width, yBottom);
	font3x6.setCursor(74 - (width / 2), 5);

}

void drawMessageBox(const int8_t x, const int8_t y, const uint8_t width, const uint8_t height) {

	arduboy.fillRect(x, y + 1, width, height, BLACK);
	arduboy.drawRect(x + 1, y + 1, width - 2, height - 1, WHITE);
	SpritesB::drawOverwrite(x, y, Images::Inventory_TL, 0);
	SpritesB::drawOverwrite(x + width - 8, y, Images::Inventory_TR, 0);
	SpritesB::drawOverwrite(x, y + height - 7, Images::Inventory_BL, 0);
	SpritesB::drawOverwrite(x + width - 8, y + height - 7, Images::Inventory_BR, 0);

}

uint8_t stealRandomItem() {

	uint8_t randomItem = arduboy.randomLFSR(0, Constants::PlayerItems_Count);
	uint8_t itemIndex = 0;

	for (uint8_t x = randomItem; x < static_cast<uint8_t>(randomItem + Constants::PlayerItems_Count); x++) {

		uint8_t y = x % Constants::PlayerItems_Count;

		if (playGameVars.player.getItemCount(y) > 0) {
			playGameVars.player.setItemCount(y, playGameVars.player.getItemCount(y) - 1);
			itemIndex = y;
			break;
		}
	
	} 

	return itemIndex;
	
}

uint8_t stealRandomRune() {

	uint8_t randomItem = arduboy.randomLFSR(0, Constants::RuneCombinations_Count);
	uint8_t itemIndex = 0;

	for (uint8_t x = randomItem; x < static_cast<uint8_t>(randomItem + Constants::RuneCombinations_Count); x++) {

		uint8_t y = x % Constants::RuneCombinations_Count;

		if (playGameVars.player.getRuneCombination(y) > 0) {
			playGameVars.player.setRuneCombination(y, playGameVars.player.getRuneCombination(y) - 1);
			itemIndex = y;
			break;
		}
	
	} 

	return itemIndex;

}