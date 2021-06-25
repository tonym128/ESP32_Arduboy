#include "src/utils/Arduboy2Ext.h"

void hackGame_Init() {

    #ifndef PUZZLE_LOAD_TEST
    const uint8_t *hack_in_play = (const uint8_t *)pgm_read_ptr(&Hacks::hacks[player.getHackIndex()]);  
    #endif
    
    uint8_t counter = 0;
    uint8_t contiguous = 0;

    clearLEDs();


    #ifdef PUZZLE_LOAD_TEST

    for (uint8_t a = 0; a < 20; a++) {
        for (uint16_t b = 0; b < 1000; b++) {
            
            const uint8_t *hack_in_play = (const uint8_t *)pgm_read_ptr(&Hacks::hacks[a]);  

            counter = 0;
            contiguous = 0;

            Serial.print(a);
            Serial.print(" ");
            Serial.println(b);

    #endif


    hackGameVars.grid.clear();
    hackGameVars.grid.setSize(pgm_read_byte(&hack_in_play[counter++]));
    hackGameVars.grid.setBufferSize(pgm_read_byte(&hack_in_play[counter++]));
    hackGameVars.timer = pgm_read_byte(&hack_in_play[counter++]);

    hackGameVars.objectives.clear();

    uint8_t clues = pgm_read_byte(&hack_in_play[counter++]);
    uint8_t maxNumber = pgm_read_byte(&hack_in_play[counter++]);  
    hackGameVars.grid.setTarget(pgm_read_byte(&hack_in_play[counter++]));  

    for (uint8_t i = 0; i < clues; i++) {

        uint8_t length = pgm_read_byte(&hack_in_play[counter++]);    
        uint8_t maxValue = pgm_read_byte(&hack_in_play[counter++]);    
        uint8_t overlap = pgm_read_byte(&hack_in_play[counter++]);    
        uint8_t overlapIdx = pgm_read_byte(&hack_in_play[counter++]);    

        if (i == 0 || overlap != NO_HACK_ID) {

            contiguous++;

        }

        hackGameVars.objectives.generate(i, length, maxValue, overlap, overlapIdx);

    }

    #ifndef PUZZLE_LOAD_TEST
    hackGameVars.objectives.print();
    #endif

    hackGameVars.grid.placeObjectives(hackGameVars.objectives, contiguous, maxNumber);

    #ifndef PUZZLE_LOAD_TEST
    hackGameVars.grid.print();
    #endif

    hackGameVars.objectives.shuffle();
    hackGameVars.renumber(arduboy);

    #ifndef PUZZLE_LOAD_TEST
    hackGameVars.objectives.print();
    #endif

    #ifdef PUZZLE_LOAD_TEST
    }}
    #endif

    #ifdef BYTE_BEAT_SOUNDS_3
    byteBeatIdx = ByteBeatIndex::Hack;
    #endif

    hackGameVars.timerStart = false;
    gameState = GameState::HackGame;

}   


void hackGame() {

    auto justPressed = arduboy.justPressedButtons();


    // House keeping ..

    if (arduboy.isFrameCount(30, 0) && hackGameVars.timerStart && hackGameVars.timer > 0) hackGameVars.timer--;

    #ifdef USE_LED_ANALOGUE
    
        if (hackGameVars.ledDelay > 0) {
            
            hackGameVars.ledDelay--;

        }
        else {
                
            clearLEDs();

        }
    
    #endif


    // Handle movements

    switch (hackGameVars.selectionMode) {

        case SelectionMode::Grid:

            switch (hackGameVars.grid.getSelectionType()) {

                case SelectionType::Cell:

                    if (justPressed & LEFT_BUTTON)       hackGameVars.grid.decX();
                    if (justPressed & RIGHT_BUTTON)      hackGameVars.grid.incX();

                    break;

                case SelectionType::Column:

                    if ((justPressed & UP_BUTTON) && hackGameVars.grid.getY() > 0) {

                        for (uint8_t y = hackGameVars.grid.getY(); y > 0; y--) {
                            if (hackGameVars.grid.getCell_Selection(hackGameVars.grid.getX(), y - 1) == Constants::Grid_Blank) {
                                hackGameVars.grid.setY(y - 1);
                                break;
                            }
                        }

                    }

                    else if ((justPressed & DOWN_BUTTON) && hackGameVars.grid.getY() < hackGameVars.grid.getSize() - 1) {

                        for (uint8_t y = hackGameVars.grid.getY() + 1; y < hackGameVars.grid.getSize(); y++) {
                            if (hackGameVars.grid.getCell_Selection(hackGameVars.grid.getX(), y) == Constants::Grid_Blank) {
                                hackGameVars.grid.setY(y);
                                break;
                            }
                        }

                    }

                    break;

                case SelectionType::Row:

                    if ((justPressed & LEFT_BUTTON) && hackGameVars.grid.getX() > 0) {

                        for (uint8_t x = hackGameVars.grid.getX(); x > 0; x--) {

                            if (hackGameVars.grid.getCell_Selection(x - 1, hackGameVars.grid.getY()) == Constants::Grid_Blank) {
                                hackGameVars.grid.setX(x - 1);
                                break;
                            }
                        }

                    }

                    else if ((justPressed & RIGHT_BUTTON) && hackGameVars.grid.getX() < hackGameVars.grid.getSize() - 1) {

                        for (uint8_t x = hackGameVars.grid.getX() + 1; x < hackGameVars.grid.getSize(); x++) {

                            if (hackGameVars.grid.getCell_Selection(x, hackGameVars.grid.getY()) == Constants::Grid_Blank) {
                                hackGameVars.grid.setX(x);
                                break;
                            }

                        }

                    }

                    break;

            }

            if ((justPressed & A_BUTTON) && hackGameVars.grid.getCell_Selection(hackGameVars.grid.getX(), hackGameVars.grid.getY()) == Constants::Grid_Blank) {

                uint8_t installedHacks = hackGameVars.installedHacks;
                uint8_t failedHacks = hackGameVars.failedHacks;

                hackGameVars.timerStart = true;
                hackGameVars.grid.select();
                renderObjectives(); // so we can see if a hack was made.

                if (installedHacks != hackGameVars.installedHacks) {

                    hackGameVars.ledDelay = 12;

                    #ifdef USE_LED_ANALOGUE
                        arduboy.setRGBled(GREEN_LED, 32);
                    #endif

                    #ifdef ARDUBOY_TONES_SOUNDS
                        sound.tones(Sounds::Success);
                    #endif
                    
                }
                else if (failedHacks != hackGameVars.failedHacks) {

                    hackGameVars.ledDelay = 12;

                    #ifdef USE_LED_ANALOGUE
                        arduboy.setRGBled(RED_LED, 32);
                    #endif

                    #ifdef ARDUBOY_TONES_SOUNDS
                        sound.tones(Sounds::Failure);
                    #endif

                }

            }

            else if (justPressed & B_BUTTON) {

                hackGameVars.selectionMode = SelectionMode::Objectives;

            }

            break;

        case SelectionMode::Objectives:

            if ((justPressed & LEFT_BUTTON) && hackGameVars.grid.getObjX() > 0) {

                hackGameVars.grid.setObjX(hackGameVars.grid.getObjX() - 1);
                
            }

            else if ((justPressed & RIGHT_BUTTON) && hackGameVars.grid.getObjX() < hackGameVars.objectives.getObjective(hackGameVars.grid.getObjY()).getLength() - 1) {
                hackGameVars.grid.setObjX(hackGameVars.grid.getObjX() + 1);

            }

            else if (justPressed & UP_BUTTON) {

                if (hackGameVars.grid.getObjY() > 0) {

                    hackGameVars.grid.setObjY(hackGameVars.grid.getObjY() - 1);
                 
                }

            }

            else if (justPressed & DOWN_BUTTON) {

                if (hackGameVars.grid.getObjY() < hackGameVars.objectives.getCount() - 1) {

                    hackGameVars.grid.setObjY(hackGameVars.grid.getObjY() + 1);
 
                }

            }

            else if (justPressed & B_BUTTON) {

                hackGameVars.selectionMode = SelectionMode::Grid;

            }

    }


    // Render screen ..

    renderBoard();
    renderObjectives();
    renderSelection();


    // Is the hack over?

    if (hackGameVars.timer == 0) {
        gameState = GameState::HackOver_Disconnected;
    }

    else if (hackGameVars.availableHacks == 0) {
        gameState = GameState::HackOver_AllHacksPlayed;
    }

    else if (hackGameVars.grid.getSelectionCount() == hackGameVars.grid.getBufferSize()) {
        gameState = GameState::HackOver_BufferFull;
    }

    /* Valid Move Testing
    bool validMove = false;

    switch (hackGameVars.grid.getSelectionType()) {

        case SelectionType::Cell:
            validMove = true;
            break;

        case SelectionType::Column:

            for (uint8_t y = 0; y < hackGameVars.grid.getSize(); y++) {

                if (hackGameVars.grid.getCell_Selection(hackGameVars.grid.getX(), y) == Constants::Grid_Blank) {

                    validMove = true;
                    break;

                }

            }

            break;

        case SelectionType::Row:

            for (uint8_t x = 0; x < hackGameVars.grid.getSize(); x++) {

                if (hackGameVars.grid.getCell_Selection(x, hackGameVars.grid.getY()) == Constants::Grid_Blank) {

                    validMove = true;
                    break;

                }

            }

            break;

    }

    if (!validMove) {

        gameState = GameState::HackOver_CannotMove;

    }
    */
 
}

void hackGame_HackOver() {

    clearLEDs();
    renderBoard();
    renderObjectives();
    renderSelection();

    Sprites::drawExternalMask(36, 23, Images::Dialogue, Images::Dialogue_Mask, 0, 0);
    font3x5.setCursor(40, 26);

    switch (gameState) {

        case GameState::HackOver_Disconnected:
            font3x5.print(F("DISCONNECTED"));
            break;

        case GameState::HackOver_BufferFull:
            font3x5.print(F("BUFFER FULL"));
            break;

        case GameState::HackOver_CannotMove:
            font3x5.print(F("CANNOT MOVE"));
            break;

        case GameState::HackOver_AllHacksPlayed:
            Sprites::drawExternalMask(25, 23, Images::Dialogue_Large, Images::Dialogue_Large_Mask, 0, 0);
            break;

        default: break;

    }

     if (arduboy.justPressed(A_BUTTON)) {

         gameState = GameState::HackScore;

     }

}

void hackGame_HackScore() {

    uint8_t thisScore = renderScoreObjectives();

    auto justPressed = arduboy.justPressedButtons();

    if (justPressed & A_BUTTON) {

        player.setDollars(player.getDollars() + thisScore);

        if (thisScore < hackGameVars.grid.getTarget()) {

            player.incThreatLevel(10);
            mapGameVars.setMessage(Message::Hack_Fail);

            #ifdef TV_TRANSITION
                gameState = GameState::TV_Init;
                nextState = GameState::Message_Hack_Target_Not_Achieved_Init;
            #else
                gameState = GameState::Message_Hack_Target_Not_Achieved_Init;
            #endif

        }
        else {

            uint8_t idx = player.getRandomPuzzle(true);
            player.incHackSuccess();

            #ifdef TV_TRANSITION
                gameState = GameState::TV_Init;
            #endif

            Message message = Message::Hack_Success;

            switch (idx) {

                case Constants::Hack_Game_Over:
                    message = Message::Game_Over_Success;
                    #ifdef TV_TRANSITION
                        nextState = GameState::Message_Game_Over_Success_Init;
                    #else
                        gameState = GameState::Message_Game_Over_Success_Init;
                    #endif
                    break;

                case Constants::Hack_None_Available:
                {
                    const bool playerCanPurchaseDeck = player.canPurchaseADeck();
                    message = playerCanPurchaseDeck ? Message::Hack_Success : Message::Hacks_Game_Over;
                    #ifdef TV_TRANSITION
                        nextState = playerCanPurchaseDeck ? GameState::Message_Hacks_Achieved_Init : GameState::Message_Hacks_Game_Over;
                    #else
                        gameState = playerCanPurchaseDeck ? GameState::Message_Hacks_Achieved_Init : GameState::Message_Hacks_Game_Over;
                    #endif
                    break;
                }

                default:
                    #ifdef TV_TRANSITION
                    nextState = GameState::Message_Hacks_Achieved_Init;
                    #else
                    gameState = GameState::Message_Hacks_Achieved_Init;
                    #endif
                    break;

            }

            mapGameVars.setMessage(message);

        }

    }

}
