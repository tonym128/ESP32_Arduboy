struct Police : Car {
    bool braking = false;
    bool flashing = false;
    float followTurnRate = 0.05;

    void follow(uint16_t const &X, uint16_t const &Y) {
        if (speed > turnSpeed) {
            // randomly toggle brake on occasion
            if (arduboy.everyXFrames(10)) {
                if (random(10) < 3) {
                    braking = !braking;
                }
            }

            if (braking && speed > maxSpeed / 2) {
                speed -= accel * 2; // multiply to override the constant police acceleration
            }

            // TODO this can be cleaned up
            int16_t angleToTarget = findAngle(x, y, X, Y) * 57296 / 1000 + 180;
            float shortest_angle = ((((angleToTarget - int16_t(angle)) % 360) + 540) % 360) - 180;
            angle += shortest_angle * (followTurnRate + speed / 50);
        }
    }

    bool callback(Solid const &other) {
        if (other.type == 'E') // only take damage from other cops
        {
            if (health > other.damage) {
                health -= other.damage;
                return false;
            }

            health = 0;
            return true; // delete if the cop has taken all the damage it can
        }

        return false;
    }

    // flash the lights
    void flash(int8_t const &xOffset, int8_t const &yOffset) {
        if (arduboy.everyXFrames(10)) flashing = !flashing;
        if (flashing)
            arduboy.fillRect(x - camera.x + width / 2 + xOffset, y - camera.y + height / 2 + yOffset, 7, 3, WHITE);
    }
};
