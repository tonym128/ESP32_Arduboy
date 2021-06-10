struct Player : Car {
    // List<Skid, 5> skids; // skid count * everyXFrame (below) should be skid ttl
    bool hasCrate = false;
    uint8_t hurt = 0;
    uint8_t max_health = 10; // max_health is the same as the pixel width of the health bar
    bool flashing = false;
    uint8_t spiked = 0;
    float turnRate = 3.5;
    float maxReverseSpeed = -1;

    Player() {
        angle = random(360);
        x = random(LEVEL_SIZE), y = random(LEVEL_SIZE);
        width = 20, height = 16;
        cbox_conf = {.x = 5, .y = 4, .width = 10, .height = 10};
        frameCount = ANGLES - 1;
        health = max_health;
        contain = true;
        type = 'P';
    }

    void increaseScore(uint8_t const &increase) {
        // TODO implement victory condition if you hit the max score
        if (health && abs(speed) > turnSpeed && SCORE < 55555) SCORE += increase;
        if (SCORE > 55555) SCORE = 55555;
        if (SCORE == 55555) {
            if (!sound.playing()) {
                sound.tones(SONG);
            }
        }
    }

    void control() {
        if (health) {
            if (abs(speed) > turnSpeed) {
                // TODO does this need to be a local variable?
                float turnRateModifier = speed / maxSpeed - 1;

                if (arduboy.pressed(LEFT_BUTTON)) {
                    angle += turnRate + turnRateModifier;
                } else if (arduboy.pressed(RIGHT_BUTTON)) {
                    angle -= turnRate + turnRateModifier;
                }
            }

            // the arduboy has its buttons flipped in my opinion
            if (arduboy.pressed(B_BUTTON)) {
                accelerate();
            } else if (arduboy.pressed(A_BUTTON)) {
                decelerate();
            }
        }
    }

    void decelerate() {
        if (speed > maxReverseSpeed) {
            speed -= accel * 2;

            /*
               // generate more skids
               if (arduboy.everyXFrames(2))
               {
                if (!skids.full())
                {
                    skids.add(Skid(x, y, curFrame));
                }
               }
             */
        }
    }

    bool callback(Solid const &other) {
        if (other.type == 'E' && !hurt) // normal police
        {
            if (health > other.damage) {
                health -= other.damage;
                // TODO make values like this configurable in globals.h
                // make the player flash 5 times (needs to be an even number)
                sound.tone(98, 100, 98, 100);
                hurt = 6;
            } else {
                sound.tone(174, 600, 164, 600, 155, 600);
                health = 0;
                speed = 0;
                // TODO maybe move this to .ino
                if (!dust.full()) dust.add(Dust(x + width / 2 - 4, y + height / 2 - 4, angle, speed / 2));
            }

            return false;
        } else if (other.type == 'C') { // crate
            sound.tone(784, 100, 1046, 100);
            hasCrate = true;
            increaseScore(5);
            return true; // delete the crate
        } else if (other.type == 'D') { // drop point
            sound.tone(1046, 100, 1046, 100, 1318, 100);
            hasCrate = false;
            increaseScore(15);
            return true; // delete the drop point
        } else if (other.type == 'S') // spikes
        {
            sound.tone(98, 100, 87, 100);
            spiked += 3; // seconds
            maxSpeed = 1; // cut max speed in half
            return true; // delete the spike
        }

        return false;
    }

    void update() {
        control();
        physics();
        updateCbox();
        updateAngle();

        // decrease the spike effect ttl 1 per second
        if (spiked && arduboy.everyXFrames(60)) {
            spiked--;
        } else {
            maxSpeed = 2;
        };
    }

    void draw() {
        /*
           // handle skids
           for (uint8_t i = 0; i < skids.size(); i++)
           {
            if (skids[i].ttl)
            {
                skids[i].update();
                skids[i].draw();
            }
            else
            {
                skids.erase(i);
                if (i) i--;
            }
           }
         */

        if (health) {
            // draw crate indicator
            if (hasCrate) {
                arduboy.drawCircle(x - camera.x + 9, y - camera.y - 5, 1);
                arduboy.drawCircle(x - camera.x + 10, y - camera.y - 5, 1);
            }

            // draw health bar
            arduboy.drawRect(x - camera.x + 4, y - camera.y - 2, 12, 3);
            arduboy.drawFastHLine(x - camera.x + 5, y - camera.y - 1, health, WHITE);

            // flash when hit by an enemy
            if (hurt && arduboy.everyXFrames(10)) {
                flashing = !flashing;
                hurt--;
            }

            if (!flashing) {
                // ghetto mask
                arduboy.fillRect(int16_t(x - camera.x + 4), int16_t(y - camera.y + 4), width - 8, height - 7,
                                 BLACK);
                sketch(PLAYER, curFrame);
            }
        }
    }
};
