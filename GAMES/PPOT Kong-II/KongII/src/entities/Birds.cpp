#include "Birds.h"

#include "../utils/Enums.h"

Birds::Birds() { }

Bird & Birds::getBird(uint8_t index) {

    return this->birds[index];

}

void Birds::updatePositions() {

    for (auto &bird : this->birds) {

        if (bird.isEnabled()) {

            bird.updatePosition();
        }

    }

    if (this->delay > 0) {
        
        this->delay--;

    }
    else {
            
        this->launchBird();

    }

}

void Birds::launchBird() {

    for (auto &bird : this->birds) {

        if (!bird.isEnabled()) {

            bird.setEnabled(true);
            this->delay = random(this->delayMax / 2, this->delayMax);
            return;

        }

    }

}

void Birds::setDelayMax(uint16_t delayMax, bool updateDelay) {

    if (updateDelay) this->delay = random(0, delayMax / 2);
    this->delayMax = delayMax;

}

void Birds::reset() {

    for (auto &bird : this->birds) {

        bird.setEnabled(false);

    }

}