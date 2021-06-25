struct Cop : Police {
    Cop() = default;

    Cop(Point spawn) {
        x = spawn.x, y = spawn.y;
        width = 20, height = 16;
        cbox_conf = {.x = 5, .y = 4, .width = 10, .height = 10};

        accel = 0.01 + (1 / (5 + random(5)));
        followTurnRate = followTurnRate + (1 / (40 + random(10)));

        frameCount = ANGLES - 1;

        type = 'E';
        damage = 3;
    }

    void update() {
        physics();
        updateCbox();
        updateAngle();
        accelerate();
    }

    void draw() {
        // ghetto mask
        arduboy.fillRect(int16_t(x - camera.x + 4), int16_t(y - camera.y + 4), width - 8, height - 7, BLACK);
        sketch(COP, curFrame);
        flash(-4, -6);
    }
};
