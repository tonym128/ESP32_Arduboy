#pragma once

#include <Arduboy2.h>
#include <Arduino.h>
#include "direction.h"
#include "worm.h"
#include "fishType.h"
#include "fish.h"
#include "baitType.h"

struct FishDiceRoll {
    FishType type;
    uint8_t points;
};

const int8_t MAX_FISH_IN_ROLL = static_cast<int8_t>(FishType::COUNT);

const int16_t STARTING_X = 25 * 16;
const int16_t STARTING_Y = 17 * 16;

enum class MenuRow: int8_t {
    COLLECTION,
    SAVE,
    SFX,
    DELETE,
    COUNT
};

enum class CollectionColumn: int8_t {
    Quantity,
    Length,
    COUNT
};

class Player {
    typedef void (Player::*UpdatePtr)(uint8_t);
    typedef void (Player::*RenderPtr)(uint8_t);

    public:
        int16_t x;
        int16_t y;
        int16_t prevX;
        int16_t prevY;
        Direction dir;
        bool movedThisFrame;
        bool scanning;
        int16_t cursorX;
        int16_t cursorY;
        uint8_t castCount;
        uint8_t reelLevel;
        MenuRow menuRow;
        bool areYouSure;
        int8_t currentCollectionRow;
        CollectionColumn currentCollectionColumn;

        Fish currentFish;
        BaitType currentBait;
        FishDiceRoll fishDiceRoll[MAX_FISH_IN_ROLL];
        uint8_t announceFishCount;

        uint8_t inProWindowCount;
        uint8_t proReelTime;

        UpdatePtr currentUpdate;
        RenderPtr currentRender;

        Player():
            x(0),
            y(0),
            prevX(0),
            prevY(0),
            dir(DOWN),
            movedThisFrame(false),
            scanning(false),
            castCount(0),
            reelLevel(0),
            menuRow(MenuRow::COLLECTION),
            currentCollectionRow(0),
            currentCollectionColumn(CollectionColumn::Quantity),
            areYouSure(false),
            currentUpdate(&Player::updateWalk),
            currentRender(&Player::renderWalk)
        {
            reset();
        }

        void reset();

        void placeCursorBasedOnDir();

        void render(uint8_t frame);
        void update(uint8_t frame);

        void updateWalk(uint8_t frame);
        void renderWalk(uint8_t frame);
        
        void updateMenu(uint8_t frame);
        void renderMenu(uint8_t frame);

        void updateAreYouSure(uint8_t frame);
        void renderAreYouSure(uint8_t frame);

        void updateCollection(uint8_t frame);
        void renderCollection(uint8_t frame);

        void updateScanning(uint8_t frame);
        void renderScanning(uint8_t frame);

        void updateCast(uint8_t frame);
        void renderCast(uint8_t frame);

        void updateReel(uint8_t frame);
        void renderReel(uint8_t frame);

        void updateReelProMode(uint8_t frame);
        void renderReelProMode(uint8_t frame);

        void updateGetFish(uint8_t frame);
        void renderGetFish(uint8_t frame);

        bool isOnSolidTile(void);
        void onGetWorm(Worm& worm);

        Direction determineDirection(int16_t px, int16_t py, int16_t x, int16_t y, Direction prevDir);
        void moveTo(int16_t newX, int16_t newY, boolean resetPrev = false);
        void undoMove();

        FishType getFishThatBit(bool isDeepWater);
        uint8_t getPointsForFish(Fish& fish, bool isDeepWater);
};

