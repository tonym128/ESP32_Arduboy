#include "girl.h"
#include "renderer.h"
#include "state.h"
#include "maskBitmaps.h"
#include "nonMaskBitmaps.h"

extern Renderer renderer;

int16_t Girl::x = 16 * 4;
int16_t Girl::y = 16 * 1 + 6;

bool Girl::isActive = false;
uint8_t Girl::startingActiveHour = 4;
uint8_t Girl::endingActiveHour = 10;

void Girl::update(bool isActive, bool justBecameActive) {
    uint8_t hour = State::getCurrentHour();

    Girl::isActive = (isActive && Girl::isActive) || (justBecameActive && hour >= startingActiveHour && hour <= endingActiveHour);
}

void Girl::render(uint8_t frame) {
    if (isActive) {
        renderer.drawPlusMask(x, y, fishingGirl_plus_mask, 0);
        renderer.drawPlusMask(x - 10, y, fishingPole_plus_mask, 0, MIRROR_HORIZONTAL);
        renderer.drawOverwrite(x - 24, y + 8, bobber_tiles, static_cast<uint8_t>(frame > 30), 0, Xor);
    }
}
