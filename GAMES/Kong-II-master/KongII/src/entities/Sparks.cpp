#include "Sparks.h"

#include "../utils/Enums.h"

Sparks::Sparks() { }

Spark & Sparks::getSpark(uint8_t index) {

    return this->sparks[index];

}

void Sparks::updatePositions() {

    for (auto &spark : this->sparks) {

        if (spark.isEnabled()) {

            spark.updatePosition();
        }

    }

    if (this->delay > 0) {
        
        this->delay--;

    }
    else {
            
        this->launchSpark();

    }

}

void Sparks::launchSpark() {

    for (auto &spark : this->sparks) {

        if (!spark.isEnabled()) {

            spark.setEnabled(true);
            this->delay = random(this->delayMax / 2, this->delayMax);
            return;

        }

    }

}

void Sparks::setDelayMax(uint8_t delayMax, bool updateDelay) {

    if (updateDelay) this->delay = random(0, delayMax / 2);
    this->delayMax = delayMax;

}

void Sparks::reset(uint8_t const *path) {

    for (auto &spark : this->sparks) {

        spark.setEnabled(false);
        spark.setPath(path);

    }

}