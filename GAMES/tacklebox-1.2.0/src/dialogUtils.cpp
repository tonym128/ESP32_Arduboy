#include "dialogUtils.h"
#include "renderer.h"
#include "state.h"
#include "maskBitmaps.h"
#include "baitBitmaps.h"

extern Renderer renderer;

void DialogUtils::renderFrame(const uint8_t* bmp) {
    renderer.drawRect(12, 19, 105, 43);
    renderer.drawOverwrite(20, 2, bmp, 0);
}

void DialogUtils::renderMoneyInCorner() {
    renderer.drawPlusMask(WIDTH - 42, 0, currencySymbol_plus_mask, 0);
    renderer.drawNumber(WIDTH - 36, 1, State::gameState.money);
}

