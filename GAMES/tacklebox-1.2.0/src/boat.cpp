#include "boat.h"
#include "renderer.h"
#include "maskBitmaps.h"
#include "state.h"
#include "enumUtils.h"

extern Renderer renderer;


int16_t Boat::x = BEACH_DOCK_X;
int16_t Boat::y = 16 * 12 - 7;
int8_t Boat::frame = 0;
bool Boat::arrived = false;
bool Boat::traveling = false;
Dock Boat::currentDock = Dock::Beach;

const uint8_t BOAT_VELOCITY = 1;

void Boat::onEnter() {
    traveling = true;
    arrived = false;
}

void Boat::update(uint8_t frame) {
    if (frame & 1) {
        return;
    }

    if (traveling && !arrived) {
        if (currentDock == Dock::Beach) {
            x += BOAT_VELOCITY;
        } else {
            x -= BOAT_VELOCITY;
        }
    }

    arrived = x <= BEACH_DOCK_X || x >= ISLAND_DOCK_X;

    if (arrived) {
        currentDock = next(currentDock);
        traveling = false;
    }
}

void Boat::render(uint8_t renderFrame) {
    if (renderFrame == 60) {
        frame = 1 - frame;
    }

    uint8_t frameOffset = traveling ? 2 : 0;
    MirrorMode mirror = currentDock == Dock::Beach ? 0 : MIRROR_HORIZONTAL;

    DrawMode drawMode = State::isDay() ? Normal : Invert;

    renderer.drawPlusMask(x, y, boat_plus_mask, frame + frameOffset, mirror, drawMode);
}

