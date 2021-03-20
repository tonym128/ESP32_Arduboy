#include "src/utils/Arduboy2Ext.h"

void showMap_Init() {

    gameState = GameState::ShowMap;

}   


void showMap() {

    auto justPressed = arduboy.justPressedButtons();

    clearLEDs();

    if (mapGameVars.menu_Index == Constants::Map_Closed) {

        if (justPressed & LEFT_BUTTON) {
            mapGameVars.menu--;
        }

        else if (justPressed & RIGHT_BUTTON) {
            mapGameVars.menu++;
        }

        else if ((justPressed & DOWN_BUTTON) && mapGameVars.menu != MapMenu::SlumsDistrict) {
            mapGameVars.menu = MapMenu::SlumsDistrict;
        }

        else if ((justPressed & UP_BUTTON) && mapGameVars.menu == MapMenu::SlumsDistrict) {
            mapGameVars.menu = MapMenu::CommercialDistrict;
        }

        else if (justPressed & A_BUTTON) {
            mapGameVars.menu_Index = 0;
        }

    }
    else {

        if (justPressed & B_BUTTON) {

            switch (mapGameVars.menu) {

                case MapMenu::CyberDeck:
                    mapGameVars.menu = MapMenu::SafeHouse;
                    break;

                default:
                    mapGameVars.menu_Index = Constants::Map_Closed;
                    break;

            }

        }

        switch (mapGameVars.menu) {

            case MapMenu::CommercialDistrict:

                if (mapGameVars.currentLocation == MapMenu::CommercialDistrict) {

                    if ((justPressed & UP_BUTTON) && mapGameVars.menu_Index > 0) {
                        mapGameVars.menu_Index--;
                    }

                    else if ((justPressed & DOWN_BUTTON) && mapGameVars.menu_Index < 2) {
                        mapGameVars.menu_Index++;
                    }

                    else if (justPressed & A_BUTTON) {

                        if (player.getDeck(mapGameVars.menu_Index + 1)) {

                            #ifdef ARDUBOY_TONES_SOUNDS
                            sound.tones(Sounds::InvalidChoice);
                            #endif
   
                        }
                        else if (player.getDeckPrice(mapGameVars.menu_Index + 1) > player.getDollars()) {

                            #ifdef ARDUBOY_TONES_SOUNDS
                            sound.tones(Sounds::InvalidChoice);
                            #endif

                        }
                        else {

                            player.setDeck(mapGameVars.menu_Index + 1, true);
                            player.decDollars(player.getDeckPrice(mapGameVars.menu_Index + 1));

                            uint8_t i = player.getFirstAvailDeck(true);
                            if (i != Constants::No_Deck_Avail) mapGameVars.menu_Index = i - 1;

                        }

                    }

                }
                else {

                    if (justPressed & A_BUTTON) {
                    
                        gameState = GameState::Message_Move_To_CommercialDistrict_Init;
                        mapGameVars.nextLocation = MapMenu::CommercialDistrict;

                        switch (mapGameVars.commercialDistrictMessage) {

                            case Message::CommercialDistrict_1:
                                mapGameVars.commercialDistrictMessage = Message::CommercialDistrict_2;
                                break;

                            case Message::CommercialDistrict_2:
                                mapGameVars.commercialDistrictMessage = Message::CommercialDistrict_3;
                                break;

                            default:
                                mapGameVars.commercialDistrictMessage = Message::CommercialDistrict_1;
                                break;

                        }

                        mapGameVars.setMessage(mapGameVars.commercialDistrictMessage);

                    }

                }
                
                break;

            case MapMenu::SlumsDistrict:

                if (mapGameVars.currentLocation == MapMenu::SlumsDistrict) {

                    if ((justPressed & UP_BUTTON) && mapGameVars.menu_Index > 0) {
                        mapGameVars.menu_Index--;
                    }

                    else if ((justPressed & DOWN_BUTTON) && mapGameVars.menu_Index < 1) {
                        mapGameVars.menu_Index++;
                    }

                    else if (justPressed & A_BUTTON) {

                        switch (mapGameVars.menu_Index) {

                            case 0:
                                if (player.getDollars() < mapGameVars.threatPrice) {

                                    #ifdef ARDUBOY_TONES_SOUNDS
                                    sound.tones(Sounds::InvalidChoice);
                                    #endif

                                }
                                else {
                                    player.decDollars(mapGameVars.threatPrice);
                                    mapGameVars.threatPrice = 40 + (random(0, 5) * 5);
                                    player.decThreatLevel(10);
                                }
                                break;

                            case 1:
                                mapGameVars.menu_Index = Constants::Map_Closed;
                                break;

                        }

                    }

                }
                else {

                    if (justPressed & A_BUTTON) {
                    
                        gameState = GameState::Message_Move_To_SlumsDistrict_Init;
                        mapGameVars.nextLocation = MapMenu::SlumsDistrict;

                        switch (mapGameVars.slumsDistrictMessage) {

                            case Message::SlumsDistrict_1:
                                mapGameVars.slumsDistrictMessage = Message::SlumsDistrict_2;
                                break;

                            case Message::SlumsDistrict_2:
                                mapGameVars.slumsDistrictMessage = Message::SlumsDistrict_3;
                                break;

                            default:
                                mapGameVars.slumsDistrictMessage = Message::SlumsDistrict_1;
                                break;

                        }

                        mapGameVars.setMessage(mapGameVars.slumsDistrictMessage);

                    }

                }
                
                break;
                
            case MapMenu::SafeHouse:

                if (mapGameVars.currentLocation == MapMenu::SafeHouse) {
                    
                    bool gameSaved = EEPROM_Utils::isGameSaved();

                    if ((justPressed & UP_BUTTON) && mapGameVars.menu_Index > 0) {
                        mapGameVars.menu_Index--;
                    }

                    else if ((justPressed & DOWN_BUTTON) && mapGameVars.menu_Index < (gameSaved ? 3 : 2)) {
                        mapGameVars.menu_Index++;
                    }

                    else if (justPressed & A_BUTTON) {

                        switch (mapGameVars.menu_Index) {

                            case 0:
                                {
                                    uint8_t idx = player.getRandomPuzzle(false);

                                    switch (idx) {

                                        case Constants::Hack_None_Available:
                                            mapGameVars.hackingMessage = Message::Hack_None_Available;
                                            break;

                                        default:

                                            switch (mapGameVars.hackingMessage) {

                                                case Message::Hacking_1:
                                                    mapGameVars.hackingMessage = Message::Hacking_2;
                                                    break;

                                                case Message::Hacking_2:
                                                    mapGameVars.hackingMessage = Message::Hacking_3;
                                                    break;

                                                default:
                                                    mapGameVars.hackingMessage = Message::Hacking_1;
                                                    break;

                                            }

                                            break;

                                    }

                                    gameState = GameState::Message_Move_To_Hacking;
                                    mapGameVars.setMessage(mapGameVars.hackingMessage);

                                }
                                break;

                            case 1:
                                mapGameVars.menu = MapMenu::CyberDeck;
                                break;

                            case 2:
                                EEPROM_Utils::saveGame(player);
                                #ifdef ARDUBOY_TONES_SOUNDS
                                sound.tones(Sounds::LoadSaveGame);
                                #endif
                                break;

                            case 3:
                                EEPROM_Utils::loadGame(player);
                                #ifdef ARDUBOY_TONES_SOUNDS
                                sound.tones(Sounds::LoadSaveGame);
                                #endif
                                break;

                        }

                    }

                }
                else {

                    if (justPressed & A_BUTTON) {
                    
                        gameState = GameState::Message_Move_To_SafeHouse;
                        mapGameVars.nextLocation = MapMenu::SafeHouse;

                        switch (mapGameVars.safeHouseMessage) {

                            case Message::SafeHouse_1:
                                mapGameVars.safeHouseMessage = Message::SafeHouse_2;
                                break;

                            case Message::SafeHouse_2:
                                mapGameVars.safeHouseMessage = Message::SafeHouse_3;
                                break;

                            default:
                                mapGameVars.safeHouseMessage = Message::SafeHouse_1;
                                break;

                        }

                        mapGameVars.setMessage(mapGameVars.safeHouseMessage);

                    }

                }  

                break;


            case MapMenu::CyberDeck:

                if (justPressed & UP_BUTTON) {
                    player.decDeckIndex();
                }

                else if (justPressed & DOWN_BUTTON) {
                    player.incDeckIndex();
                }

                else if (justPressed & A_BUTTON) {
                    mapGameVars.menu = MapMenu::SafeHouse;
                }

                break;

        }

    }

    renderDistricts();

    switch (mapGameVars.menu) {

        case MapMenu::CommercialDistrict:

            if (arduboy.getFrameCountHalf(48)) Sprites::drawOverwrite(61, 13, Images::MapMenu_Arrow, 0);

            if (mapGameVars.menu_Index != Constants::Map_Closed) {

                if (mapGameVars.currentLocation != MapMenu::CommercialDistrict) {

                    renderPopup_Resizable(45, 20, 25, 53);
                    Sprites::drawSelfMasked(49, 25 + (mapGameVars.menu_Index * 6), Images::Pointer, 0);
                    print(font3x5, Words::Move_Here);

                }
                else {

                    renderPopup_Large(39, 20, false);
                    font3x5.setCursor(39 + 9, 20 + 3);

                    for (uint8_t i = 1; i < Constants::Max_Deck_Count; i++){
                        renderCommercialMenuOption(!player.getDeck(i), Words::Deck_V1 + 1, player.getDeckPrice(i));
                    }
                    
                    font3x5.setCursor(39 + 8, 20 + 25);
                    font3x5.print(F("CYBERDECKS"));

                    if (player.getAvailDeckCount() > 0) Sprites::drawSelfMasked(43, 25 + (mapGameVars.menu_Index * 6), Images::Pointer, 0);

                }

            }
            break;

        case MapMenu::SlumsDistrict:

            if (arduboy.getFrameCountHalf(48)) Sprites::drawOverwrite(87, 54, Images::MapMenu_Arrow, 0);

            if (mapGameVars.menu_Index != Constants::Map_Closed) {

                if (mapGameVars.currentLocation != MapMenu::SlumsDistrict) {

                    renderPopup_Resizable(17, 3, 25, 53);
                    Sprites::drawSelfMasked(21, 8 + (mapGameVars.menu_Index * 6), Images::Pointer, 0);
                    print(font3x5, Words::Move_Here);
                    mapGameVars.menu_Index = 0;

                }
                else {

                    renderPopup_Large(3, 3, true);
                    font3x5.setCursor(7, 7);
                    font3x5.print(F("LOWER~THREAT~10%\n~ YES ("));
                    font3x5.print(mapGameVars.threatPrice);
                    font3x5.print(F(")\n~ NO"));
                    font3x5.setCursor(24, 28);
                    font3x5.print(F("TERRY B."));

                    Sprites::drawSelfMasked(7, 16 + (mapGameVars.menu_Index * 6), Images::Pointer, 0);

                }

            }
            break;
            
        case MapMenu::SafeHouse:
        case MapMenu::CyberDeck:

            if (arduboy.getFrameCountHalf(48)) Sprites::drawOverwrite(91, 15, Images::MapMenu_Arrow, 0);

            if (mapGameVars.menu_Index != Constants::Map_Closed) {

                if (mapGameVars.currentLocation != MapMenu::SafeHouse) {

                    renderPopup_Resizable(36, 3, 26, 53);
                    print(font3x5, Words::Move_Here);

                }
                else {

                    bool gameSaved = EEPROM_Utils::isGameSaved();

                    renderPopup_Resizable(36, 3, 26 + (gameSaved ? 6 : 0), 53);
                    println(font3x5, Words::Safe_House_Opts);

                    if (gameSaved) {

                        font3x5.print(F("LOAD"));

                    }

                }

                if (mapGameVars.menu == MapMenu::SafeHouse) {

                    Sprites::drawSelfMasked(41, 8 + (mapGameVars.menu_Index * 6), Images::Pointer, 0);

                }
                else {

                    renderPopup_Resizable(45, 18, player.getAvailDeckCount() > 0 ? 25 : 31, 62);
                    for (uint8_t i = 0; i < Constants::Max_Deck_Count; i++) {
                        renderDeckName(i);
                    }

                    Sprites::drawSelfMasked(49, 23 + (player.getDeckIndex_ForDisplay() * 6), Images::Pointer, 0);

                }

            }
            break;
            
    }

    Sprites::drawOverwrite(1, 50, Images::MapMenu_Legend, 0);
    font3x5.setCursor(10, 50);
    font3x5.print(player.getDollars());
    font3x5.setCursor(10, 57);
    font3x5.print(player.getThreatLevel());
    font3x5.print("%");

}

void renderDeckName(uint8_t deckIdx) {

    if (player.getDeck(deckIdx)) {

        print(font3x5, Words::No_Deck + deckIdx);
        if (player.getHackSuccess(deckIdx) < 10)     font3x5.print(" ");
        if (player.getHackSuccess_Max(deckIdx) < 10) font3x5.print(" ");
        font3x5.print(player.getHackSuccess(deckIdx));
        font3x5.print("/");
        font3x5.println(player.getHackSuccess_Max(deckIdx));

    }

}

void renderPopup_Resizable(const uint8_t x, const uint8_t y, uint8_t height, uint8_t width) {

    arduboy.fillRect(x, y, width, height, BLACK);
    arduboy.drawFastHLine(x + 2, y + 1, width - 1);
    arduboy.drawFastHLine(x + 2, y + height - 2, width);
    Sprites::drawOverwrite(x, y + (height - 25), Images::MapMenu_L, 0);
    Sprites::drawOverwrite(x + width, y + (height - 25), Images::MapMenu_R, 0);
    arduboy.drawFastVLine(x + 1, y + 1, (height - 25));
    arduboy.drawFastVLine(x + width, y + 1, (height - 25));

    font3x5.setCursor(x+ 10, y + 3);

}

void renderPopup_Large(const uint8_t x, const uint8_t y, bool large) {

    arduboy.fillRect(x, y, 90 + (large ? 13 : 0), 37, BLACK);
    arduboy.drawFastHLine(x + 2, y + 1, 87 + (large ? 13 : 0));
    arduboy.drawFastHLine(x + 2, y + 23, 53 + (large ? 13 : 0));
    arduboy.drawFastHLine(x + 2, y + 33, 53 + (large ? 13 : 0));
    arduboy.drawFastHLine(x + 2, y + 35, 87 + (large ? 13 : 0));
    Sprites::drawOverwrite(x + 1, y + 1, Images::MapMenu_Lrg_L, 0);
    Sprites::drawOverwrite(x + 54 + (large ? 13 : 0), y + 1, Images::MapMenu_Lrg_M, 0);
    Sprites::drawOverwrite(x + 88 + (large ? 13 : 0), y + 1, Images::MapMenu_Lrg_R, 0);
    Sprites::drawOverwrite(x + 61 + (large ? 13 : 0), y + 4, Images::MapMenu_Face_00, 0);

}

void renderDistricts() {

    Sprites::drawOverwrite(3, 0, Images::CommercialDist, 0);
    Sprites::drawOverwrite(34, 23, Images::SlumsDist, 0);
    Sprites::drawOverwrite(94, 16, Images::SafeHouse, 0);

    font3x5.setCursor(28, 0); //6
    font3x5.print(F("COMMERCIAL\n  DISTRICT"));
    font3x5.setCursor(60, 41);
    font3x5.print(F("THE~SLUMS\nDISTRICT"));
    font3x5.setCursor(92, 2);
    font3x5.print(F(" SAFE\nHOUSE"));

}

void renderCommercialMenuOption(bool available, uint8_t wordIdx, uint16_t value) {

    if (available) {
        print(font3x5, wordIdx);
        font3x5.print(value);
        font3x5.print(F("\n"));
    }
    else {
        print(font3x5, Words::Sold_Out);
    }

}