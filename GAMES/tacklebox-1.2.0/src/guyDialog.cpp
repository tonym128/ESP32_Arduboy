#include "guyDialog.h"
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
const uint8_t NEEDED_BOOT_COUNT = 4;

extern Renderer renderer;
extern Arduboy2Base arduboy;

void GuyDialog::update() {
    if (
        !State::gameState.canChooseProMode &&
        arduboy.justPressed(A_BUTTON)
    ) {
        if (State::gameState.currentFishCount[static_cast<int8_t>(FishType::OLD_BOOT)] >= NEEDED_BOOT_COUNT) {
            State::decreaseCurrentCount(FishType::OLD_BOOT, NEEDED_BOOT_COUNT);
            State::gameState.canChooseProMode = true;
        } else {
            Sfx::buzz();
        }
    }
    else if (
        State::gameState.canChooseProMode &&
        arduboy.justPressed(A_BUTTON)
    ) {
        State::gameState.useProMode = !State::gameState.useProMode;
    }
}

void GuyDialog::render() {
    DialogUtils::renderFrame(fishingGuyDialog_tiles);

    const uint8_t* str = guyQuest_string;

    if (State::gameState.canChooseProMode) {
        if (State::gameState.useProMode) {
            str = guyTurnOffProMode_string;
        } else {
            str = guyTurnOnProMode_string;
        }
    } else {
        // user's current boot count for reference
        renderer.drawOverwrite(WIDTH - 48, 8, OLD_BOOT_tiles, 0);
        renderer.drawNumber(WIDTH - 36, 10, State::gameState.currentFishCount[static_cast<int8_t>(FishType::OLD_BOOT)]);
    }

    renderer.drawString(17, 28, str);

    renderer.drawOverwrite(54, 50, squareIcon_tiles, 0);
    renderer.drawString(60, 50, ok_string);
}

