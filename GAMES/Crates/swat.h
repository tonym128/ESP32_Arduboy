struct Swat : Police {
    Swat() = default;

    Swat(Point spawn) {
        x = spawn.x, y = spawn.y;
        width = 20, height = 16;
        cbox_conf = {.x = 5, .y = 4, .width = 10, .height = 10};

        accel = 0.01 + (1 / (5 + random(5)));
        followTurnRate = followTurnRate + (1 / (40 + random(10)));

        frameCount = ANGLES - 1;

        type = 'E';
        damage = 4;
        health = 10;
        maxSpeed = 2;
    }

    void update() {
        physics();
        updateCbox();
        updateAngle();
        accelerate();

        if (random(10) < 3) {
            if (!spikes.full()) spikes.add(Spike(x, y));
        }
    }

    void draw() {
        // ghetto mask
        arduboy.fillRect(int16_t(x - camera.x + 4), int16_t(y - camera.y + 4), width - 8, height - 7, BLACK);
        sketch(SWAT, curFrame);
        flash(-4, -8);
    }
};
