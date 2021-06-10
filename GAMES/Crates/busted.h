typedef struct Busted {
    uint8_t const x = 31;
    int8_t y = -46;
    uint8_t counter = 0;

    void gameOver() {
        if (y < 25) {
            y++;
        } else {
            // after a pause, return to the menu and reset the player to play again
            if (counter < 250) {
                counter++;
            } else {
                counter = 0;
                y = -46; // reset the BUSTED logo's position
                WANTED = false;
                LEVEL = 0;
                SCORE = 0;
                menu = Menu();
                cops.clear();
                swat.clear();
                spikes.clear();
                sound.noTone();
                crate = Crate(randomPointOffCamera(LEVEL_SIZE)); // move the crate to a new spot
                drop = Drop(randomPointOffCamera(LEVEL_SIZE));
                player = Player();
            }
        }

        sprites.drawPlusMask(x, y, BUSTED, 0);
    }
} Busted;
