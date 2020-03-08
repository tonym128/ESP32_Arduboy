#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Constants.h"

// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void playGame_Update() {

	auto justPressed = arduboy.justPressedButtons();
	auto pressed = arduboy.pressedButtons();

// Serial.print((uint8_t)playGameVars.viewState);
// Serial.print(" ");
// Serial.println(playGameVars.counter);


	switch (playGameVars.viewState) {

		case ViewState::InTown:
	
			if (!playGameVars.inventory.show && !playGameVars.showSaveDialogue) {

				if (playGameVars.counter > 0) {
					if (playGameVars.counter != Constants::DialogeDelay_MultiPart) {
						playGame_HandleMessages(justPressed);
					}
					else {
						playGame_Handle_Mulitpart_Messages(justPressed);
					}
				}

				if (justPressed & B_BUTTON) { 
					playGameVars.inventory.show = true; }


				// Left ..

				if (playGameVars.counter != Constants::DialogeDelay_MultiPart) {

					if (justPressed & LEFT_BUTTON) {
						playGameVars.player.setPlayerDirection(PlayerDirection::IdleLeft);
					}
					else if (pressed & LEFT_BUTTON) {

						playGameVars.player.setPlayerDirection(PlayerDirection::WalkingLeft);
						if (playGameVars.player.getX() > Constants::Player_CentrePos) {
							playGameVars.player.decX();
						}
						else if (playGameVars.world.buildingXPos < 255) {

							if (arduboy.getFrameCount(2) == 0) playGameVars.world.cloudsXPos++;
							playGameVars.world.foregroundXPos = playGameVars.world.foregroundXPos + 2;
							playGameVars.world.buildingXPos++;
						}
						else if (playGameVars.world.buildingXPos == 255) {

							if (playGameVars.player.getX() > 2) {
								playGameVars.player.decX();
							}
						}

					}
					else if (!(pressed & LEFT_BUTTON) && playGameVars.player.getPlayerDirection() == PlayerDirection::WalkingLeft) {
						playGameVars.player.setPlayerDirection(PlayerDirection::IdleLeft);
					}

					


					// Right ..

					if (justPressed & RIGHT_BUTTON) {
						playGameVars.player.setPlayerDirection(PlayerDirection::IdleRight);
					}
					else if (pressed & RIGHT_BUTTON) {

						playGameVars.player.setPlayerDirection(PlayerDirection::WalkingRight);
						if (playGameVars.player.getX() < Constants::Player_CentrePos) {
							playGameVars.player.incX();
						}
						else {
							if (playGameVars.world.buildingXPos > Constants::Town_RightPos) {
								if (arduboy.getFrameCount(2) == 0) playGameVars.world.cloudsXPos--;
								playGameVars.world.foregroundXPos = playGameVars.world.foregroundXPos - 2;
								playGameVars.world.buildingXPos--;
							}
							else {
								if (!playGameVars.gameOver) {
									if (playGameVars.player.getX() < 128) {
										playGameVars.player.incX();
									}
									else {

										uint8_t dungeon = playGameVars.world.dungeon - 1;
										playGameVars.player.setX(-11);
										playGame_ChangeViewState(ViewState::InDungeon_Entry);
										playGame_InitEnenmy(dungeon);
										//								playGameVars.enemy.setEnemyType(arduboy, EnemyType::Astarok);

									}
								}
							}
						}
					}
					else if (!(pressed & RIGHT_BUTTON) && playGameVars.player.getPlayerDirection() == PlayerDirection::WalkingRight) {
						playGameVars.player.setPlayerDirection(PlayerDirection::IdleRight);
					}

				}


				// Doorway

				if (justPressed & UP_BUTTON) {

					if (playGameVars.counter == Constants::DialogeDelay_MultiPart) {

						if (playGameVars.message.topLine > 0) playGameVars.message.topLine--;

					}
					else {

						bool processed = playGame_ProcessAorUpButton();

						if (!processed) {
							playGameVars.player.setJumping(true);
						}
						// auto x = (playGameVars.world.buildingXPos - playGameVars.player.getX());

						// for (uint8_t y = 0; y < 3; y++) {

						// 	if (!playGameVars.gameOver) {
								
						// 		if (playGame_CloseTo(playGameVars.world.buildings[0] + x)) {

						// 			playGame_ChangeViewState(ViewState::InShopItems);

						// 		}

						// 		if (playGame_CloseTo(playGameVars.world.buildings[1] + x)) {

						// 			playGame_ChangeViewState(ViewState::InShopRunes);

						// 		}

						// 	}

						// 	if (playGameVars.world.townItems[Constants::HP_Index].getEnabled() && playGame_CloseTo(playGameVars.world.townItems[Constants::HP_Index].getX() + x - 6)) {

						// 		playGame_SaveMessage(Pickup_HP_Idx, 1, 94, Constants::DialogueDelay);
						// 		playGameVars.world.townItems[Constants::HP_Index].setEnabled(false);
						// 		playGameVars.player.incHP(arduboy.randomLFSR(5, 15));
						// 		sound.tones(Sounds::hpOrGPGained);

						// 	}

						// 	if (playGame_CloseTo(Constants::Tombstone_Locations[y] + x - 6)) {

						// 		playGame_SaveMessage(Tombstone_Inscription_00_Idx + y, 2, 94, Constants::DialogueDelay);

						// 	}

						// 	if (playGame_CloseTo(Constants::NPC_Locations[y] + x)) {

						// 		if (playGameVars.gameOver) {
						// 			playGame_SaveMessage(Thank_You_Idx, 2, 104, Constants::DialogueDelay);
						// 		}
						// 		else {
						// 			playGame_SaveMessage_MultiPart(pgm_read_byte(&multiPart_ArrayIndex[y]), 2, pgm_read_byte(&multiPart_LinesOverall[y]));
						// 		}

						// 	}

						// 	if (playGame_CloseTo(Constants::SavingPost_Location + x)) {
								
						// 		playGameVars.showSaveDialogue = true;
						// 		playGameVars.counter = 0;

						// 	}

						// }

					}
					
				}

				if (justPressed & DOWN_BUTTON) {

					if (playGameVars.counter == Constants::DialogeDelay_MultiPart) {

						if (playGameVars.message.topLine + 2 < playGameVars.message.linesOverall) playGameVars.message.topLine++;
						
					}

				}


				// Tombstones or NPC ?
				
				if (justPressed & A_BUTTON) {
					
					playGame_ProcessAorUpButton();

				}
							
			}
			else {

				if (playGameVars.inventory.show) {

					playGame_UpdateInventory(justPressed);

				}
				else {

					if (playGameVars.counter == 0) {

						if (playGameVars.counter > 0) {
							playGame_HandleMessages(justPressed);
						}
						
						if (justPressed & UP_BUTTON) {
							if (playGameVars.inventory.saveGameIndex > 0) playGameVars.inventory.saveGameIndex--;
						}

						if (justPressed & DOWN_BUTTON) {

							uint8_t c;
							EEPROM.get(101, c);
							if (playGameVars.inventory.saveGameIndex == 0 || (c == 'c' && playGameVars.inventory.saveGameIndex < 2)) playGameVars.inventory.saveGameIndex++;

						}

						if (justPressed & B_BUTTON) {
							playGameVars.showSaveDialogue = false;
						}						

						if (justPressed & A_BUTTON) {

							switch (playGameVars.inventory.saveGameIndex) {

								case 0:
									{
										uint8_t c;
										EEPROM.get(101, c);

										if (c == 'c') {

											playGame_SaveMessage(Overwrite_Confirm_Idx, 2, 104, 255);

										}
										else {
											playGame_SaveGame();
										}
									}
									break;

								case 1:
									playGame_SaveMessage(New_Game_Confirm_Idx, 2, 84, 254);
									break;

								case 2:
									EEPROM.get(101, playGameVars);
									playGame_SaveMessage(Game_Restored_Idx, 1, 100, Constants::DialogueDelay);

							}

						}

					}
					else {

						playGameVars.message.renderRequired = true;

						if (justPressed & B_BUTTON) {
							playGameVars.showSaveDialogue = false;
							playGameVars.counter = 0;
						}						

						if (justPressed & A_BUTTON) {
							
							switch (playGameVars.inventory.saveGameIndex) {
							
								case 0:
									playGame_SaveGame();
									break;

								case 1:
									playGameVars.showSaveDialogue = false;
									playGameVars.counter = 0;
									playGameVars.gameOver = false;
									playGame_Activate();
									break;
							
							}

						}

					}

					break;
				}

			}
			break;	

		case ViewState::InShopItems:
	
			if (!playGameVars.inventory.show) {
					
				if (playGameVars.counter > 0) {
					playGame_HandleMessages(justPressed);
				}

				bool upPressed = (justPressed & UP_BUTTON);
				if (upPressed) { 
					playGameVars.itemIdx = (playGameVars.itemIdx == 0 ? Constants::PlayerItems_Count : playGameVars.itemIdx - 1);
				}
				playGameVars.highlight_UpArrow = upPressed;

				bool downPressed = (justPressed & DOWN_BUTTON);
				if (downPressed) { 
					playGameVars.itemIdx = (playGameVars.itemIdx >= Constants::PlayerItems_Count ? 0 : playGameVars.itemIdx + 1);
				}
				playGameVars.highlight_DownArrow = downPressed;

				if (justPressed & B_BUTTON) { playGameVars.inventory.show = true; }

				if (justPressed & A_BUTTON) {
					
					PlayerItem playerItem;
					memcpy_P(&playerItem, &Player_Items[(playGameVars.itemIdx * 4)], 4);
					bool purchaseCancelled = false;

					switch (playerItem.itemType) {

						case PlayerItemType::Helmet:
							if (static_cast<uint8_t>(playGameVars.player.getHelmetTypeCount(static_cast<HelmetType>(playerItem.itemIndex))) == 1) {
								playGame_SaveMessage(Same_Helmet_Idx, 2, 94, Constants::DialogueDelay);
								purchaseCancelled = true;
							}
							break;

						case PlayerItemType::Potion:
							if (playGameVars.player.getItemCount(static_cast<uint8_t>(HelmetType::Count) + playerItem.itemIndex) == Constants::PlayerPotions_InvMax) {
								playGame_SaveMessage(Max_Potions_Idx, 3, 88, Constants::DialogueDelay);
								purchaseCancelled = true;
							}
							break;

						case PlayerItemType::Exit:
							purchaseCancelled = true;
							playGame_ChangeViewState(ViewState::InTown);
							break;

					}

					if (!purchaseCancelled) {

						if (playerItem.gold <= playGameVars.player.getGP()) {
							playGame_SaveMessage(Buy_Item_Idx, 2, 76);
							playGameVars.viewState = ViewState::InShopItems_ConfirmPurchase;
						}
						else {
							playGame_SaveMessage(No_GP_Idx, 2, 70, Constants::DialogueDelay);
						}

					}

				}

			}
			else {

				playGame_UpdateInventory(justPressed);

			}
			break;

		case ViewState::InShopItems_ConfirmPurchase:

			playGameVars.message.renderRequired = true;

			if (justPressed & A_BUTTON) {

				PlayerItem playerItem;
				memcpy_P(&playerItem, &Player_Items[(playGameVars.itemIdx * 4)], 4);
				sound.tones(Sounds::purchase);

				switch (playerItem.itemType) {

					case PlayerItemType::Helmet:
						playGameVars.player.incHelmetType(static_cast<HelmetType>(playerItem.itemIndex));
						break;

					case PlayerItemType::Potion:
						playGameVars.player.incPotionType(static_cast<PotionType>(playerItem.itemIndex));
						break;

					default: break;

				}

				playGameVars.player.setGP(playGameVars.player.getGP() - playerItem.gold);
				playGameVars.viewState = ViewState::InShopItems;

			}
			

			// Cancel purchase ..

			if (justPressed & B_BUTTON) {
				playGameVars.viewState = ViewState::InShopItems;
			}
			break;

		case ViewState::InShopRunes:

			if (!playGameVars.inventory.show) {

				if (playGameVars.counter > 0) {
					playGame_HandleMessages(justPressed);
				}

				if (justPressed & UP_BUTTON) { 
					playGameVars.runeIdx = (playGameVars.runeIdx == 0 ? Constants::RuneCombinations_Count : playGameVars.runeIdx - 1);
					playGameVars.highlight_UpArrow = 5;
				}
				else {
					if (playGameVars.highlight_UpArrow) playGameVars.highlight_UpArrow = !playGameVars.highlight_UpArrow;
				}
				if (justPressed & DOWN_BUTTON) { 
					playGameVars.runeIdx = (playGameVars.runeIdx >= Constants::RuneCombinations_Count ? 0 : playGameVars.runeIdx + 1);
					playGameVars.highlight_DownArrow = 5;
				}
				else {
					if (playGameVars.highlight_DownArrow) playGameVars.highlight_DownArrow = !playGameVars.highlight_DownArrow;
				}

				if (justPressed & B_BUTTON) { playGameVars.inventory.show = true; }

				if (justPressed & A_BUTTON) {
					
					uint8_t sp = pgm_read_byte(&RuneCombinations[(playGameVars.runeIdx * Constants::RuneCombinations_RecordLength) + Constants::RuneCombinations_Price]);
					bool purchaseCancelled = false;

					switch (playGameVars.runeIdx) {

						case static_cast<uint8_t>(RuneCombinationType_Purchased::Count):
							purchaseCancelled = true;
							playGame_ChangeViewState(ViewState::InTown);
							break;

						default:						
							if (playGameVars.player.getRuneCombination(playGameVars.runeIdx) > 0) {
								playGame_SaveMessage(Same_Rune_Idx, 2, 86, Constants::DialogueDelay);
								purchaseCancelled = true;
							}
							break;

					}

					if (!purchaseCancelled) {

						if (sp <= playGameVars.player.getSP()) {
							playGame_SaveMessage(Buy_Rune_Idx, 2, 74);
							playGameVars.viewState = ViewState::InShopRunes_ConfirmPurchase;
						}
						else {
							playGame_SaveMessage(No_SP_Idx, 2, 70, Constants::DialogueDelay);
						}

					}

				}
				
			}
			else {

				playGame_UpdateInventory(justPressed);

			}
			break;

		case ViewState::InShopRunes_ConfirmPurchase:

			playGameVars.message.renderRequired = true;

			if (justPressed & A_BUTTON) {
				sound.tones(Sounds::purchase);
				uint8_t sp = pgm_read_byte(&RuneCombinations[(playGameVars.runeIdx * Constants::RuneCombinations_RecordLength) + Constants::RuneCombinations_Price]);

				playGameVars.player.incRuneCombination(static_cast<RuneCombinationType_Purchased>(playGameVars.runeIdx));
				playGameVars.player.setSP(playGameVars.player.getSP() - sp);
				playGameVars.viewState = ViewState::InShopRunes;

				if (playGameVars.inventory.runeIdx == Constants::RuneNoValue) { playGameVars.inventory.runeIdx = playGameVars.runeIdx; }

			}
			

			// Cancel purchase ..

			if (justPressed & B_BUTTON) {
				playGameVars.viewState = ViewState::InShopRunes;
			}

			break;

		case ViewState::InDungeon_Entry:

			#ifdef CHANGE_ENEMY	
			if (justPressed & UP_BUTTON) {
				playGameVars.enemy.decEnemyType();
			}

			if (justPressed & DOWN_BUTTON) {
				playGameVars.enemy.incEnemyType();
			}
			#endif
		
			if (playGameVars.player.getX() == Constants::Castle_EntryLimit) {
				if (arduboy.randomLFSR(0,2) == 0) {
					playGame_ChangeViewState(ViewState::InDungeon_PlayersTurn);
				}
				else {
					playGame_ChangeViewState(ViewState::InDungeon_EnemysAttacksFirst);
					playGame_SaveMessage(Monster_Attack_Idx, 1, 130, Constants::DialogueDelay);
				}
			}

			playGame_handleLeftRightMovement(justPressed, pressed, Constants::Castle_LeftLimit, Constants::Castle_RightLimit_Monster);

			break;

		case ViewState::InDungeon_PlayersTurn:

			if (!playGameVars.inventory.show) {

				switch (playGameVars.counter) {

					case Constants::DialogueDelay_End_Of_Roll_Start ... Constants::DialogueDelay_End_Of_Roll_End:
						playGameVars.counter--;
						if (justPressed & A_BUTTON || playGameVars.counter == Constants::DialogueDelay_End_Of_Roll_Start) {
							playGameVars.counter = Constants::DialogueDelay_Fight_Roll;
						}
						break;

					case Constants::DialogueDelay_Fight_Roll:
						playGameVars.message.renderRequired = true;
						if (justPressed & A_BUTTON) {
							if (playGameVars.matchedRuneCombination.id == Constants::RuneNoValue) {
								playGame_ChangeViewState(ViewState::InDungeon_EnemysTurn);
							}
							else {
								if (playGameVars.matchedRuneCombination.dmg1 > 0) {
									playGameVars.counter = Constants::DialogueDelay_Fight_Damage_ThisRoll;
								}
								else if (playGameVars.matchedRuneCombination.hp > 0) {
									playGameVars.counter = Constants::DialogueDelay_Fight_Health;
								}
							}
						}
						break;

					case Constants::DialogueDelay_Fight_Damage_ThisRoll:
						playGameVars.message.renderRequired = true;
						if (justPressed & A_BUTTON) {
							if (playGameVars.matchedRuneCombination.id == Constants::RuneNoValue) {
								playGame_ChangeViewState(ViewState::InDungeon_EnemysTurn);
							}
							else {
								playGameVars.counter = Constants::DialogueDelay_Fight_Damage_Apply;
								playGameVars.invert = 8;
								playGameVars.enemy.inflictDMG(playGameVars.matchedRuneCombination.dmg1);
							}

						}
						break;

					case Constants::DialogueDelay_Fight_Damage_PrevRoll:
						playGameVars.message.renderRequired = true;
						if (justPressed & A_BUTTON) {
							playGameVars.counter = Constants::DialogueDelay_Fight_Damage_Apply;
							playGameVars.invert = 8;
							playGameVars.enemy.inflictDMG(playGameVars.matchedRuneCombination.getPrevDmg1(playGameVars.enemy.getEnemyType())); //SJH
							playGameVars.matchedRuneCombination.prevDmg1_Fire = 0;
							playGameVars.matchedRuneCombination.prevDmg1_Venom = 0;
						}
						break;

					case Constants::DialogueDelay_Fight_Damage_Apply:
						if (playGameVars.invert > 1)	{
							sound.tones(Sounds::playerHit);
							arduboy.invert(playGameVars.invert % 2);
							if (arduboy.everyXFrames(4)) playGameVars.invert--;
						}
						else {

							if (playGameVars.enemy.getHP() == 0) {
								sound.tones(Sounds::enemyDead);
								playGame_ChangeViewState(ViewState::InDungeon_EnemyDead);
								LevelData levelData;
								memcpy_P(&levelData, &Levels[(playGameVars.world.dungeon - 1) * Constants::LevelData_RecordSize], Constants::LevelData_RecordSize);

								if (playGameVars.enemy.getEnemyType() != EnemyType::Astarok) {
									
									playGameVars.victoryGP = (arduboy.randomLFSR(0, 3) <= 1 ? arduboy.randomLFSR(levelData.minGP, levelData.maxGP) : 0);

									{
										const int16_t xPos[] = { -53, -229, -22, 90, 237 };
										uint8_t x = arduboy.randomLFSR(0, 12);

										if (x <= 4) {
											playGameVars.world.townItems[Constants::HP_Index].setX(xPos[x]);
											playGameVars.world.townItems[Constants::HP_Index].setEnabled(true);
										}

									} 

								}
		
								playGameVars.message.victorySP = arduboy.randomLFSR(levelData.minSP, levelData.maxSP);
								playGameVars.player.incSP(playGameVars.message.victorySP);
								playGameVars.message.renderRequired = true;
								playGameVars.counter = Constants::DialogueDelay;

							}
							else {

								if (playGameVars.matchedRuneCombination.getPrevDmg1(playGameVars.enemy.getEnemyType()) > 0) {
									playGameVars.counter = Constants::DialogueDelay_Fight_Damage_PrevRoll;
								}
								else {

									if (playGameVars.enemy.getEnemyType() == EnemyType::Astarok) {
										playGame_SaveMessage_MultiPart(playGameVars.matchedRuneCombination.dmg1 / 4, Constants::DialogueDelay_Enemy_HP_Steal);
										playGameVars.player.decHP(playGameVars.matchedRuneCombination.dmg1);
									}
									else {
										playGame_ChangeViewState(ViewState::InDungeon_EnemysTurn);
									}

								}

							}
						}
						break;

					case Constants::DialogueDelay_Fight_Health:
						playGameVars.message.renderRequired = true;
						if (justPressed & A_BUTTON) {
							if (playGameVars.matchedRuneCombination.getPrevDmg1(playGameVars.enemy.getEnemyType())) {
								playGameVars.counter = Constants::DialogueDelay_Fight_Damage_PrevRoll;
							}
							else {
								playGameVars.player.incHP(playGameVars.matchedRuneCombination.hp);
								playGameVars.counter = Constants::DialogueDelay_Enemy_Roll;
								playGame_ChangeViewState(ViewState::InDungeon_EnemysTurn);
							}
						}
						break;

					case Constants::DialogueDelay_Enemy_HP_Steal:
						playGameVars.message.renderRequired = true;
						if (justPressed & A_BUTTON) {
							if (playGameVars.player.getHP() == 0) {
								sound.tones(Sounds::playerDead);
								playGame_ChangeViewState(ViewState::InDungeon_PlayerDead);
							}
							else {
								playGame_ChangeViewState(ViewState::InDungeon_EnemysTurn);
							}
						}
						break;

					case 1 ... Constants::DialogueDelay:
						playGame_HandleMessages(justPressed);
						break;

					default:
						{

							if (justPressed & B_BUTTON) { playGameVars.inventory.show = true; }

							playGame_handleLeftRightMovement(justPressed, pressed, 0, Constants::Castle_RightLimit_Monster);

							if (justPressed & A_BUTTON && playGameVars.reroll < playGameVars.numberOfRecasts && !playGameVars.runes.anyRotating()) {

								if (playGameVars.reroll == 0) {

									sound.tones(Sounds::cast);
									playGameVars.player.setRecast();
									playGameVars.runes.startSpinning();
									playGameVars.reroll++;
							
								}
								else {

								// Firee Blaze
								// playGameVars.runes.getRune(0).setValue(1);
								// playGameVars.runes.getRune(1).setValue(1);
								// playGameVars.runes.getRune(2).setValue(3);
								// playGameVars.runes.getRune(3).setValue(4);

								// Venom Mist
								// playGameVars.runes.getRune(0).setValue(0);
								// playGameVars.runes.getRune(1).setValue(1);
								// playGameVars.runes.getRune(2).setValue(2);
								// playGameVars.runes.getRune(3).setValue(3);
																															
									MatchedRuneCombination matchedRuneCombination;
									playGameVars.runes.checkRuneCombination(playGameVars.player, matchedRuneCombination);
	
									if (!playGameVars.runes.anyRotatingOrRerolling() && matchedRuneCombination.id == Constants::RuneNoValue) {
										playGame_SaveMessage(Throw_Again_Idx, 1, 94, Constants::DialogueDelay);
									}
									else {

										if (playGameVars.runes.anyRotatingOrRerolling()) {
											sound.tones(Sounds::cast);
											for (uint8_t x = 0; x < 5; x++) {
												if (playGameVars.runes.getRune(x).getReroll()) playGameVars.runes.getRune(x).startSpinning();
											}
						
											playGameVars.player.setRecast();
											playGameVars.reroll++;
										
										}
										else {

											playGameVars.reroll = playGameVars.numberOfRecasts;

										}

									}

								}

							}

							if (playGameVars.reroll > 0 && playGameVars.reroll < playGameVars.numberOfRecasts && !playGameVars.runes.anyRotating()) {

								if (justPressed & UP_BUTTON) { 
									
									if (playGameVars.player.getX() >= 0 && playGameVars.player.getX() <= 58) {
										sound.tones(Sounds::touch);
										uint8_t i = (playGameVars.player.getX() + 7) / 13;
										playGameVars.runes.getRune(i).alternateReroll();
										playGameVars.player.setJumping(true);

									}

								}

							}


							// End of player's turn ..  inflict any damage!

							if (!playGameVars.runes.anyRotatingOrRerolling() && playGameVars.reroll == playGameVars.numberOfRecasts) {

							// Firee Blaze
							// playGameVars.runes.getRune(0).setValue(1);
							// playGameVars.runes.getRune(1).setValue(1);
							// playGameVars.runes.getRune(2).setValue(3);
							// playGameVars.runes.getRune(3).setValue(4);

							// Venom Mist
							// playGameVars.runes.getRune(0).setValue(0);
							// playGameVars.runes.getRune(1).setValue(1);
							// playGameVars.runes.getRune(2).setValue(2);
							// playGameVars.runes.getRune(3).setValue(3);


								playGameVars.runes.checkRuneCombination(playGameVars.player, playGameVars.matchedRuneCombination);
								playGameVars.message.message = 0;

								if (justPressed & A_BUTTON) {
									playGameVars.counter = Constants::DialogueDelay_Fight_Roll;
								}
								else {
									playGameVars.counter = Constants::DialogueDelay_End_Of_Roll_End;
								}

							}
								
							playGameVars.runes.update(arduboy);

						}

					break;


				}
				
			}
			else {

				playGame_UpdateInventory(justPressed);

			}
			break;

		case ViewState::InDungeon_EnemysAttacksFirst:

				if (justPressed & A_BUTTON || justPressed & B_BUTTON) {
					playGameVars.message.message = 0;
					playGame_ChangeViewState(ViewState::InDungeon_EnemysTurn);
				}

				playGame_handleLeftRightMovement(justPressed, pressed, Constants::Castle_LeftLimit, Constants::Castle_RightLimit_Monster);
				playGameVars.message.renderRequired = true;

			break;

		case ViewState::InDungeon_EnemysTurn:
			{

				switch (playGameVars.counter) {

					case Constants::DialogueDelay_Enemy_Roll:
						{
							LevelData levelData;
							memcpy_P(&levelData, &Levels[(playGameVars.world.dungeon - 1) * Constants::LevelData_RecordSize], Constants::LevelData_RecordSize);
							playGameVars.dmgInflictedOnPlayer = arduboy.randomLFSR(levelData.minAttack, levelData.maxAttack);
							playGameVars.message.renderRequired = true;
							playGameVars.counter = Constants::DialogueDelay_Enemy_Damage;
						}
						[[fallthrough]];

					case Constants::DialogueDelay_Enemy_Damage:
						playGameVars.message.renderRequired = true;
						if (justPressed & A_BUTTON) {
							if (playGameVars.dmgInflictedOnPlayer > 0) {
								sound.tones(Sounds::enemyHit);
								playGameVars.counter = Constants::DialogueDelay_Enemy_Damage_Apply;
								playGameVars.invert = 8;
								playGameVars.player.inflictDMG(playGameVars.dmgInflictedOnPlayer);
							}
							else {
								playGame_ChangeViewState(ViewState::InDungeon_PlayersTurn);
							}
						}
						break;

					case Constants::DialogueDelay_Enemy_Damage_Apply:
						if (playGameVars.invert > 1)	{
							arduboy.invert(playGameVars.invert % 2);
							if (arduboy.everyXFrames(4)) playGameVars.invert--;
						}
						else {

							if (playGameVars.player.getHP() == 0) {
								sound.tones(Sounds::playerDead);
								playGame_ChangeViewState(ViewState::InDungeon_PlayerDead);
							}
							else {

								if (playGameVars.enemy.getEnemyType() == EnemyType::GelatinousCube) {

									uint8_t itemCount = playGameVars.player.getDistinctItemCount();
									uint8_t potionCount = playGameVars.player.getPotionTypeCount();
									uint8_t runeCount = playGameVars.player.getDistinctRuneCombinationCount();
									
									if ((itemCount > 0 || potionCount > 0 || runeCount > 0) && arduboy.randomLFSR(0, 6) == 0) {

										if (itemCount > 0 || potionCount > 0) {

											// uint8_t randomItem = arduboy.randomLFSR(0, Constants::PlayerItems_Count);
											// uint8_t itemIndex = 0;

											// for (uint8_t x = randomItem; x < randomItem + Constants::PlayerItems_Count; x++) {

											// 	uint8_t y = x % Constants::PlayerItems_Count;

											// 	if (playGameVars.player.getItemCount(y) > 0) {
											// 		playGameVars.player.setItemCount(y, playGameVars.player.getItemCount(y) - 1);
											// 		itemIndex = y;
											// 		break;
											// 	}
											
											// } 

											uint8_t itemIndex = stealRandomItem();
											playGame_SaveMessage_MultiPart(itemIndex, Constants::DialogueDelay_Enemy_Item_Steal);

										}
										else {

											// uint8_t randomItem = arduboy.randomLFSR(0, Constants::RuneCombinations_Count);
											// uint8_t itemIndex = 0;

											// for (uint8_t x = randomItem; x < randomItem + Constants::RuneCombinations_Count; x++) {

											// 	uint8_t y = x % Constants::RuneCombinations_Count;

											// 	if (playGameVars.player.getRuneCombination(y) > 0) {
											// 		playGameVars.player.setRuneCombination(y, playGameVars.player.getRuneCombination(y) - 1);
											// 		itemIndex = y;
											// 		break;
											// 	}
											
											// } 

											uint8_t itemIndex = stealRandomRune();
											playGame_SaveMessage_MultiPart(Constants::PlayerItems_Count + itemIndex, Constants::DialogueDelay_Enemy_Item_Steal);

										}

									}
									else {

										playGame_ChangeViewState(ViewState::InDungeon_PlayersTurn);

									}

								}
								else {

									playGame_ChangeViewState(ViewState::InDungeon_PlayersTurn);

								}

							}

						}
						break;

					case Constants::DialogueDelay_Enemy_Item_Steal:
						playGameVars.message.renderRequired = true;
						if (justPressed & A_BUTTON) {
							playGame_ChangeViewState(ViewState::InDungeon_PlayersTurn);
						}
						break;

				}

			}
			break;

		case ViewState::InDungeon_PlayerDead:

			playGameVars.message.renderRequired = true;

			switch (playGameVars.counter) {

				case Constants::Player_Dead_Init:
					if (justPressed & A_BUTTON) {
						playGameVars.counter = Constants::Player_Dead_End;
					}
					break;

				case Constants::Player_Dead_End:
					{
						uint8_t itemCount = playGameVars.player.getDistinctItemCount();
						uint8_t potionCount = playGameVars.player.getPotionTypeCount();

						if (itemCount > 0 || potionCount > 0) {

							stealRandomItem();

						}

						playGameVars.player.kill();
						playGame_ResetGame();
						playGame_ChangeViewState(ViewState::InTown);

					}
					break;

			}
			break;

		case ViewState::InDungeon_EnemyDead:

			if (!playGameVars.inventory.show) {

				playGame_handleLeftRightMovement(justPressed, pressed, Constants::Castle_LeftLimit, playGameVars.gameOver ? Constants::Castle_RightLimit_Monster : WIDTH);
	
				if (playGameVars.counter > 0) {
					playGame_HandleMessages(justPressed);
				}
				else {

					if (justPressed & B_BUTTON) { playGameVars.inventory.show = true; }

					if (justPressed & UP_BUTTON) { playGameVars.player.setJumping(true); }
					
					if (playGameVars.player.getX() >= 128) {

						playGameVars.world.dungeon++;
						uint8_t dungeon = playGameVars.world.dungeon - 1;
						playGame_InitEnenmy(dungeon);
						playGameVars.player.setX(0);
						playGameVars.player.setY(Constants::Player_Ground_Level);
						playGame_ChangeViewState(ViewState::InDungeon_Entry);

					}

					if (playGameVars.victoryGP > 0 && playGameVars.player.getX() + Constants::Player_Width == Constants::Castle_GoldPosition) {

						playGameVars.player.incGP(playGameVars.victoryGP);
						playGameVars.message.victoryGP = playGameVars.victoryGP;
						playGameVars.message.victorySP = playGameVars.victorySP;
						playGameVars.victoryGP = 0;
						playGameVars.counter = Constants::DialogueDelay;
						sound.tones(Sounds::hpOrGPGained);

					}

				}

			}
			else {

				playGame_UpdateInventory(justPressed);

			}
			break;

		default: break;

	}



	// Update player position (if jumping) ..

	playGameVars.player.update();

}


void playGame_handleLeftRightMovement(uint8_t justPressed, uint8_t pressed, int8_t leftLimit, uint8_t rightLimit) {

	// Left ..

	if (justPressed & LEFT_BUTTON) {
		playGameVars.player.setPlayerDirection(PlayerDirection::IdleLeft);
	}
	else if (pressed & LEFT_BUTTON) {

		if (playGameVars.player.getX() > Constants::Castle_LeftLimit) { 
			if (playGameVars.player.getX() > leftLimit) {
				playGameVars.player.setPlayerDirection(PlayerDirection::WalkingLeft);
				playGameVars.player.decX();
				playGame_setPlayerYPos();
			}
		}
		else {
			playGameVars.player.setX(127);
			playGame_ChangeViewState(ViewState::InTown);
		}
	}
	else if (!(pressed & LEFT_BUTTON) && playGameVars.player.getPlayerDirection() == PlayerDirection::WalkingLeft) {
		playGameVars.player.setPlayerDirection(PlayerDirection::IdleLeft);
	}


	// Right ..

	if (justPressed & RIGHT_BUTTON) {
		playGameVars.player.setPlayerDirection(PlayerDirection::IdleRight);
	}
	else if (pressed & RIGHT_BUTTON && playGameVars.player.getX() <= rightLimit) {
		playGameVars.player.setPlayerDirection(PlayerDirection::WalkingRight);
		playGameVars.player.incX();
		playGame_setPlayerYPos();
	}
	else if (!(pressed & RIGHT_BUTTON) && playGameVars.player.getPlayerDirection() == PlayerDirection::WalkingRight) {
		playGameVars.player.setPlayerDirection(PlayerDirection::IdleRight);
	}

}


void playGame_setPlayerYPos() {

	switch (playGameVars.player.getX()) {

		case Constants::Player_Stairs ... WIDTH:
			if (playGameVars.viewState == ViewState::InDungeon_EnemyDead) {
				uint8_t yOffset = ((playGameVars.player.getX() - Constants::Player_Stairs) / 3) * 2;
				playGameVars.player.setY(Constants::Player_Ground_Level + yOffset);
			}
			else {
				playGameVars.player.setY(Constants::Player_Ground_Level);
			}
			break;

		default:
			playGameVars.player.setY(Constants::Player_Ground_Level);
	}

}

void playGame_InitEnenmy(uint8_t dungeon) {

	LevelData levelData;
	memcpy_P(&levelData, &Levels[dungeon * Constants::LevelData_RecordSize], Constants::LevelData_RecordSize);

	playGameVars.enemy.setEnemyType(arduboy, levelData.enemyType);
	playGameVars.enemy.setDEF(levelData.def);
	playGameVars.enemy.setHP(levelData.hp);
	// //playGameVars.enemy.setHP(5);						// SJH

}

void playGame_SaveGame() {

	playGameVars.showSaveDialogue = false;
	playGameVars.hasBeenSaved = 'c';
	EEPROM.put(101, playGameVars);
  EEPROM.commit();
	playGame_SaveMessage(Game_Saved_Idx, 1, 88, Constants::DialogueDelay);

};

bool playGame_CloseTo(int16_t v) {
  
  return ((v < 0) ? -v : v) < 10;

}

bool playGame_ProcessAorUpButton() {

	auto x = (playGameVars.world.buildingXPos - playGameVars.player.getX());

	if (!playGameVars.gameOver) {
		
		if (playGame_CloseTo(playGameVars.world.buildings[0] + x)) {

			playGame_ChangeViewState(ViewState::InShopItems);

		}

		if (playGame_CloseTo(playGameVars.world.buildings[1] + x)) {

			playGame_ChangeViewState(ViewState::InShopRunes);

		}

	}

	if (playGameVars.world.townItems[Constants::HP_Index].getEnabled() && playGame_CloseTo(playGameVars.world.townItems[Constants::HP_Index].getX() + x - 6)) {

		playGame_SaveMessage(Pickup_HP_Idx, 1, 94, Constants::DialogueDelay);
		playGameVars.world.townItems[Constants::HP_Index].setEnabled(false);
		playGameVars.player.incHP(arduboy.randomLFSR(5, 15));
		sound.tones(Sounds::hpOrGPGained);
		return true;

	}

	if (playGame_CloseTo(-155 + x - 6)) {

		mrBlinky = 40;
		return true;

	}

	for (uint8_t y = 0; y < 3; y++) {

		if (playGame_CloseTo(Constants::Tombstone_Locations[y] + x - 6)) {

			playGame_SaveMessage(Tombstone_Inscription_00_Idx + y, 2, 94, Constants::DialogueDelay);
			return true;

		}

		if (playGame_CloseTo(Constants::NPC_Locations[y] + x)) {

			if (playGameVars.gameOver) {
				playGame_SaveMessage(Thank_You_Idx, 2, 104, Constants::DialogueDelay);
			}
			else {
				playGame_SaveMessage_MultiPart(pgm_read_byte(&multiPart_ArrayIndex[y]), 2, pgm_read_byte(&multiPart_LinesOverall[y]));
			}

			return true;

		}

	}

	if (playGame_CloseTo(Constants::SavingPost_Location + x)) {
		
		playGameVars.showSaveDialogue = true;
		playGameVars.counter = 0;
		return true;

	}

	return false;

}
