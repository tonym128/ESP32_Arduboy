#include "girlDialog.h"
#include <Arduboy2.h>
#include "renderer.h"
#include "nonMaskBitmaps.h"
#include "maskBitmaps.h"
#include "strings.h"
#include "state.h"
#include "fishType.h"
#include "baitType.h"
#include "sfx.h"
#include "dialogUtils.h"

const uint8_t MEAT_PRICE = 10;
const uint8_t NEEDED_LOBSTER_COUNT = 3;

extern Renderer renderer;
extern Arduboy2Base arduboy;

void GirlDialog::update() {
    if (
        !State::gameState.canBuyMeat &&
        arduboy.justPressed(A_BUTTON)
    ) {
        if (State::gameState.currentFishCount[static_cast<int8_t>(FishType::LOBSTER)] >= NEEDED_LOBSTER_COUNT) {
            State::decreaseCurrentCount(FishType::LOBSTER, NEEDED_LOBSTER_COUNT);
            State::gameState.canBuyMeat = true;
        } else {
            Sfx::buzz();
        }
    }
    else if (
        State::gameState.canBuyMeat &&
        arduboy.justPressed(A_BUTTON)
    ) {
        if (State::gameState.money >= MEAT_PRICE &&
            State::gameState.baitCounts[static_cast<int8_t>(BaitType::Meat)] < 99) {
            State::gameState.money -= MEAT_PRICE;
            State::gameState.baitCounts[static_cast<int8_t>(BaitType::Meat)] += 1;
            Sfx::purchase();
        } else {
            Sfx::buzz();
        }
    }
}

void GirlDialog::render() {
    DialogUtils::renderFrame(fishingGirlDialog_tiles);

    const uint8_t* str = girlQuest_string;

    if (State::gameState.canBuyMeat) {
        str = girlSellMeat_string;
    }

    renderer.drawString(17, 24, str);

    renderer.drawOverwrite(54, 52, squareIcon_tiles, 0);
    renderer.drawString(60, 52, ok_string);

    if (State::gameState.canBuyMeat) {
        renderer.drawPlusMask(43, 39, currencySymbol_plus_mask, 0);
        renderer.drawNumber(49, 40, MEAT_PRICE);
        renderer.drawRect(38, 37, 50, 10, WHITE);

        // user's current money in upper corner
        DialogUtils::renderMoneyInCorner();

        // user's current meat count for reference
        renderer.drawOverwrite(WIDTH - 48, 8, meat_tiles, 0);
        renderer.drawNumber(WIDTH - 36, 10, State::gameState.baitCounts[static_cast<int8_t>(BaitType::Meat)]);
    } else {
        renderer.drawOverwrite(WIDTH - 48, 8, LOBSTER_tiles, 0);
        renderer.drawNumber(WIDTH - 33, 10, State::gameState.currentFishCount[static_cast<int8_t>(FishType::LOBSTER)]);
    }
}

