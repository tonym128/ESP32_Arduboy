#include "src/utils/Arduboy2Ext.h"

#define DISP_GRID_COL_WIDTH 12
#define DISP_GRID_ROW_HEIGHT 8
#define DISP_TITLE_OBJECTIVE_X 76
#define DISP_OBJECTIVES_LEFT DISP_TITLE_OBJECTIVE_X + 3
#define DISP_OBJECTIVES_TOP 11
#define DISP_OBJECTIVES_SPACING_X 10
#define DISP_OBJECTIVES_SPACING_Y 7
#define DISP_OBJECTIVES_COUNT 6

#define DISP_SCORES_SPACING_Y 8
#define DISP_SCORES_HACK_LEFT 57
#define DISP_SCORES_STATUS_LEFT 1
#define DISP_SCORES_TOP 10

const uint8_t hackGrid_X_Offsets[] = { 0, 0, 0, 0, DISP_GRID_COL_WIDTH, DISP_GRID_COL_WIDTH / 2, 0 };
const uint8_t hackGrid_Y_Offsets[] = { 0, 0, 0, 0, DISP_GRID_ROW_HEIGHT, DISP_GRID_ROW_HEIGHT / 2, 0 };

void renderInverseCell(const uint8_t x, const uint8_t y) {
    
    uint8_t xOffset = hackGrid_X_Offsets[hackGameVars.grid.getSize()];
    uint8_t yOffset = hackGrid_Y_Offsets[hackGameVars.grid.getSize()];

    uint16_t xPos = x * DISP_GRID_COL_WIDTH + xOffset;
    uint16_t yPos = y * DISP_GRID_ROW_HEIGHT + yOffset;

    arduboy.fillRect(xPos, yPos, DISP_GRID_COL_WIDTH + 1, DISP_GRID_ROW_HEIGHT + 1, WHITE);
    font3x5.setTextColor(BLACK);
    font3x5.setCursor(3 + xPos, 1 + yPos);
    printHex(hackGameVars.grid.getCell(x, y));
    font3x5.setTextColor(WHITE);

}

void renderInverseCell_Objectives(const uint8_t x, const uint8_t y, const bool inverse) {

    uint16_t xPos = DISP_OBJECTIVES_LEFT + (x * DISP_OBJECTIVES_SPACING_X);
    uint16_t yPos = DISP_OBJECTIVES_TOP + (y * DISP_OBJECTIVES_SPACING_Y);

    arduboy.fillRect(xPos - 1, yPos, DISP_OBJECTIVES_SPACING_X, DISP_OBJECTIVES_SPACING_Y, inverse ? WHITE : BLACK);
    font3x5.setTextColor(inverse ? BLACK : WHITE);
    font3x5.setCursor(xPos, yPos);
    printHex(hackGameVars.objectives.getObjective(y).getCell(x));
    font3x5.setTextColor(WHITE);

}

void renderBoard() {
    
    uint8_t gridSize = hackGameVars.grid.getSize();
    uint8_t xOffset = hackGrid_X_Offsets[gridSize];
    uint8_t yOffset = hackGrid_Y_Offsets[gridSize];


    // Render board ..
    
    for (uint8_t i = 0; i < gridSize + 1; i++) {

        arduboy.drawHorizontalDottedLine(xOffset, DISP_GRID_COL_WIDTH * gridSize + xOffset, (DISP_GRID_ROW_HEIGHT * i) + yOffset);
        arduboy.drawVerticalDottedLine(yOffset, DISP_GRID_ROW_HEIGHT * gridSize + yOffset, (DISP_GRID_COL_WIDTH * i) + xOffset);

    }

    for (uint8_t y = 0; y < gridSize; y++) {

        for (uint8_t x = 0; x < gridSize; x++) {

            font3x5.setCursor(3 + (x * DISP_GRID_COL_WIDTH) + xOffset, 1 + (y * DISP_GRID_ROW_HEIGHT) + yOffset);
            printHex(hackGameVars.grid.getCell(x, y));

            if (hackGameVars.grid.getCell_Selection(x, y) != Constants::Grid_Blank) {

                Sprites::drawSelfMasked(x * DISP_GRID_COL_WIDTH + xOffset, y * DISP_GRID_ROW_HEIGHT + yOffset, Images::GridCheck, 0);

            }
            
        }

    }


    // Rendered selected column / row ..

    switch (hackGameVars.selectionMode) {

        case SelectionMode::Grid:
            {
                uint8_t yVal = hackGameVars.grid.getY() * DISP_GRID_ROW_HEIGHT + yOffset;

                switch (hackGameVars.grid.getSelectionType()) {

                    case SelectionType::Cell:
                        arduboy.drawRect(xOffset, yVal, (DISP_GRID_COL_WIDTH * gridSize) + 1, DISP_GRID_ROW_HEIGHT + 1);
                        renderInverseCell(hackGameVars.grid.getX(), hackGameVars.grid.getY());
                        break;

                    case SelectionType::Column:
                        arduboy.drawRect(hackGameVars.grid.getX() * DISP_GRID_COL_WIDTH + xOffset, yOffset, DISP_GRID_COL_WIDTH + 1, (DISP_GRID_ROW_HEIGHT * gridSize) + 1);
                        renderInverseCell(hackGameVars.grid.getX(), hackGameVars.grid.getY());
                        break;

                    case SelectionType::Row:
                        arduboy.drawRect(xOffset, yVal, (DISP_GRID_COL_WIDTH * gridSize) + 1, DISP_GRID_ROW_HEIGHT + 1);
                        renderInverseCell(hackGameVars.grid.getX(), hackGameVars.grid.getY());
                        break;

                }

            }

            break;

        case SelectionMode::Objectives:

            for (uint8_t y = 0; y < gridSize; y++) {

                for (uint8_t x = 0; x < gridSize; x++) {

                    if (hackGameVars.grid.getCell(x, y) == hackGameVars.objectives.getObjective(hackGameVars.grid.getObjY()).getCell(hackGameVars.grid.getObjX())) {
                        
                        renderInverseCell(x, y);

                    }
                    
                }

            }

            break;

    }

}

void renderSelection() {

    // Render selection ..

    font3x5.setCursor(1, 50);
    font3x5.print(F("BUFFER"));

    Sprites::drawOverwrite(26, 51, Images::LeftFade, 0);    
    Sprites::drawOverwrite(65, 51, Images::RightLower_Fade, 0);  
    arduboy.drawFastHLine(34, 51, 31);  

    for (uint8_t i = 0; i < hackGameVars.grid.getBufferSize(); i++) {

        if (i < hackGameVars.grid.getSelectionCount()) {
            font3x5.setCursor(1 + (i * DISP_OBJECTIVES_SPACING_X), 56);
            printHex(hackGameVars.grid.getSelection(i));
        }

        arduboy.drawFastHLine(1 + (i * DISP_OBJECTIVES_SPACING_X), 63, 7);

    }

}

void printZero()
{
    font3x5.print("0");
}

void printTime(uint8_t minutes, uint8_t seconds)
{
    if (minutes < 10) printZero();
    font3x5.print(minutes);

    font3x5.print(".");

    if (seconds < 10) printZero();
    font3x5.print(seconds);
}

void renderObjectives() { 
    
    uint8_t availableHacks = hackGameVars.objectives.getCount();

    hackGameVars.installedHacks = 0;
    hackGameVars.failedHacks = 0;


    // Render hackGameVars.objectives ..

    arduboy.drawFastHLine(DISP_TITLE_OBJECTIVE_X + 4, 0, 55);
    arduboy.drawFastHLine(DISP_TITLE_OBJECTIVE_X, 8, 55);
    arduboy.drawFastHLine(DISP_TITLE_OBJECTIVE_X + 4, 55, 55);
    arduboy.drawFastHLine(DISP_TITLE_OBJECTIVE_X, 63, 55);
    arduboy.drawFastVLine(DISP_TITLE_OBJECTIVE_X, 9, 35);

    Sprites::drawOverwrite(WIDTH - 6, 0, Images::RightFade, 0);
    Sprites::drawOverwrite(WIDTH - 6, 55, Images::RightFade, 0);
    Sprites::drawOverwrite(DISP_TITLE_OBJECTIVE_X, 41, Images::DownFade, 0);
    Sprites::drawSelfMasked(DISP_TITLE_OBJECTIVE_X, 0, Images::LeftCorner, 0);
    Sprites::drawSelfMasked(DISP_TITLE_OBJECTIVE_X, 55, Images::LeftCorner, 0);

    font3x5.setCursor(DISP_TITLE_OBJECTIVE_X + 6, 1);
    font3x5.print(F("HACKS"));

    font3x5.setCursor(DISP_TITLE_OBJECTIVE_X + 6, 56);
    font3x5.print(F("TIME:"));

    if (hackGameVars.timer > 0 && (hackGameVars.timer > 5 || arduboy.getFrameCountHalf(15))) {

        printTime(hackGameVars.timer / 60, hackGameVars.timer % 60);

        #ifdef USE_LED_ANALOGUE
        arduboy.setRGBled(BLUE_LED, 0);
        #endif

    }
    else if (hackGameVars.timer <= 0) {

        font3x5.print(F("00.00"));

        #ifdef USE_LED_ANALOGUE
        arduboy.setRGBled(BLUE_LED, 0);
        #endif

    }
    else {

        #ifdef USE_LED_ANALOGUE
        arduboy.setRGBled(BLUE_LED, 32);
        #endif

    }


    // Render objective list ..

    uint8_t targetAchieved = 0;

    for (uint8_t j = 0; j < hackGameVars.objectives.getCount(); j++) {

        Objective objective = hackGameVars.objectives.getObjective(j);
        uint8_t highlightLength = isSubArray(hackGameVars.grid.getSelections(), objective.getCells(), hackGameVars.grid.getSelectionCount(), objective.getLength());

        if (highlightLength == objective.getLength() && hackGameVars.selectionMode == SelectionMode::Grid) {

            Sprites::drawSelfMasked(DISP_OBJECTIVES_LEFT - 1, DISP_OBJECTIVES_TOP + (j * DISP_OBJECTIVES_SPACING_Y), Images::Installed, 0);

            availableHacks--;
            hackGameVars.installedHacks++;
            targetAchieved = targetAchieved + objective.getScore();

        }
        else {

            if (objective.getLength() - highlightLength > hackGameVars.grid.getBufferSize() - hackGameVars.grid.getSelectionCount() && hackGameVars.selectionMode == SelectionMode::Grid) {

                Sprites::drawSelfMasked(DISP_OBJECTIVES_LEFT - 1, DISP_OBJECTIVES_TOP + (j * DISP_OBJECTIVES_SPACING_Y), Images::Failed, 0);
                availableHacks--;
                hackGameVars.failedHacks++;

            }
            else {

                for (uint8_t i = 0; i < Constants::Objective_Max_Length; i++) {

                    if (objective.getCell(i) != Constants::Grid_Blank) {

                        if (highlightLength > 0) {

                            arduboy.fillRect(DISP_OBJECTIVES_LEFT + (i * DISP_OBJECTIVES_SPACING_X) - 1, DISP_OBJECTIVES_TOP + (j * DISP_OBJECTIVES_SPACING_Y), 10, 7, WHITE);
                            font3x5.setTextColor(BLACK);

                        }
                        else {

                            font3x5.setTextColor(WHITE);

                        }

                        font3x5.setCursor(DISP_OBJECTIVES_LEFT + (i * DISP_OBJECTIVES_SPACING_X), DISP_OBJECTIVES_TOP + (j * DISP_OBJECTIVES_SPACING_Y));
                        printHex(objective.getCell(i));
                        if (highlightLength > 0) highlightLength--;

                    }

                }

            }
            
        }

    }


    // Render selected element ..

    if (hackGameVars.selectionMode == SelectionMode::Objectives) {

        renderInverseCell_Objectives(hackGameVars.grid.getObjX(), hackGameVars.grid.getObjY(), arduboy.getFrameCountHalf(32));

    }


    // Return the availableHacks ..

    hackGameVars.availableHacks = availableHacks;



    #ifdef SHOW_PUZZLE_NUMBER

        font3x5.setCursor(108, 1);
        font3x5.print("ID:");
        if (player.getHackIndex() < 10) printZero();
        font3x5.print(player.getHackIndex());
    
    #else

        Sprites::drawOverwrite(107, 0, Images::Target, 0);
        font3x5.setCursor(118, 1);

        uint8_t target = hackGameVars.grid.getTarget();
        uint8_t togo = target > targetAchieved ? target - targetAchieved : 0;
        if (togo < 10) printZero();
        font3x5.print(togo);

    #endif

}


uint8_t renderScoreObjectives() { // Returns number of active hacks available.

    uint8_t score = 0;


    // Render objectives ..

    #ifdef NEW_TOTALS

        Sprites::drawOverwrite(0, 0, Images::LeftFade2, 0);
        Sprites::drawOverwrite(46, 0, Images::RightFade, 0);
        Sprites::drawOverwrite(55, 0, Images::LeftFade2, 0);
        Sprites::drawOverwrite(96, 0, Images::RightFade, 0);
        Sprites::drawOverwrite(105, 0, Images::LeftFade2, 0);
        Sprites::drawOverwrite(121, 0, Images::RightFade, 0);

        Sprites::drawOverwrite(0, 57, Images::LeftFade, 0);
        Sprites::drawOverwrite(121, 57, Images::RightFade, 0);

        arduboy.drawFastHLine(5, 0, 41);
        arduboy.drawFastHLine(60, 0, 36);
        arduboy.drawFastHLine(112, 0, 9);
        
        arduboy.drawFastHLine(6, 8, 40);
        arduboy.drawFastHLine(61, 8, 35);
        arduboy.drawFastHLine(113, 8, 8);

        arduboy.drawFastHLine(6, 57, 115);

        font3x5.setCursor(13, 1);
        font3x5.print(F("STATUS        HACK"));
        font3x5.setCursor(106, 1);
        font3x5.print(F("MONEY"));

    #endif

    #ifdef OLD_TOTALS

        arduboy.drawHorizontalDottedLine(0, WIDTH, 0);
        arduboy.drawHorizontalDottedLine(0, WIDTH, 8);
        arduboy.drawHorizontalDottedLine(0, WIDTH, 57);

        font3x5.setCursor(1, 1);
        font3x5.print(F("STATUS        HACK"));
        font3x5.setCursor(107, 1);
        font3x5.print(F("SCORE"));

    #endif



    // Render objective list ..

    for (uint8_t j = 0; j < hackGameVars.objectives.getCount(); j++) {

        Objective objective = hackGameVars.objectives.getObjective(j);
        uint8_t highlightLength = isSubArray(hackGameVars.grid.getSelections(), objective.getCells(), hackGameVars.grid.getSelectionCount(), objective.getLength());

        for (uint8_t i = 0; i < Constants::Objective_Max_Length; i++) {

            if (objective.getCell(i) != Constants::Grid_Blank) {

                font3x5.setCursor(DISP_SCORES_HACK_LEFT + (i * DISP_OBJECTIVES_SPACING_X), DISP_SCORES_TOP + (j * DISP_SCORES_SPACING_Y));
                printHex(objective.getCell(i));

            }

        }

        if (highlightLength == objective.getLength()) {

            Sprites::drawSelfMasked(DISP_SCORES_STATUS_LEFT - 1, DISP_SCORES_TOP + (j * DISP_SCORES_SPACING_Y), Images::Installed, 0);

            font3x5.setCursor(119 + (objective.getScore() < 10 ? 4 : 0), DISP_SCORES_TOP + (j * DISP_SCORES_SPACING_Y));
            font3x5.print(objective.getScore());

            score = score + objective.getScore();

        }
        else {

            Sprites::drawSelfMasked(DISP_SCORES_STATUS_LEFT - 1, DISP_SCORES_TOP + (j * DISP_SCORES_SPACING_Y), Images::Failed, 0);

            font3x5.setCursor(123, DISP_SCORES_TOP + (j * DISP_SCORES_SPACING_Y));
            printZero();
            
        }

    }

    font3x5.setCursor(47 + (score < 10 ? 4 : 0) + (hackGameVars.grid.getTarget() < 10 ? 4 : 0), 58);
    font3x5.print(F("TARGET "));
    font3x5.print(hackGameVars.grid.getTarget());
    font3x5.print(" / TOTAL ");
    font3x5.print(score);

    return score;

}