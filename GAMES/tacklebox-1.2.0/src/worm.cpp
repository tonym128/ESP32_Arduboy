#include "worm.h"
#include "renderer.h"
#include "maskBitmaps.h"
#include "state.h"

extern Renderer renderer;
extern Arduboy2Base arduboy;

void Worm::update(uint8_t frame, bool isActive, bool justBecameActive) {

    if (justBecameActive && !isSpawned) {
        const uint8_t randomRange = State::isDay() ? 4 : 1;

        isSpawned = random(0, randomRange) == 0;

        if (isSpawned) {
            duration = random(4 * 60, 10 * 60);
        }
    }

    if (isSpawned) {
        duration -= 1;

        justWentInGround = duration > 0 && duration < 14;
        isSpawned = duration > 0;
    }
}

void Worm::render(uint8_t frame) {
    if (isSpawned) {
        uint8_t index = justWentInGround ? 1 : 0;
        renderer.drawPlusMask(x, y, worm_plus_mask, index, static_cast<MirrorMode>(frame > 50), Xor);
    }
}

void Worm::onGrabbedByPlayer() {
    isSpawned = false;
}
