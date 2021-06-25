struct Car : Solid {
    float turnSpeed = 0.3; // the speed you must reach before turning is allowed
    float accel = 0.02;
    uint8_t health = 3;

    void accelerate() {
        speed += accel;

        if (speed > maxSpeed) speed = maxSpeed;
    }

    void updateAngle() {
        angle = normalizeAngle(angle);
        curFrame = angleToFrame(angle, frameCount);
    }
};
