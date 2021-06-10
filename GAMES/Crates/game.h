typedef struct Game {
    void play() {
        drawScreenFlash();
        drawGrid();

        if (!player.hasCrate) {
            crate.update();
            crate.draw();
            drawLocator(crate, 2, false);
        } else {
            drop.update();
            drop.draw();
            drawLocator(drop, 2, false);
        }

        player.update();
        player.draw();

        // make the camera follow the player
        camera.follow(player.x + player.width / 2, player.y + player.height / 2, 54, 27);

        if (!player.hasCrate && collide(player.cbox, crate.cbox) && player.callback(crate)) {
            // TODO make this more exciting for the player
            drop = Drop(randomPointOffCamera(LEVEL_SIZE)); // move the drop to a new spot
        } else if (player.hasCrate && collide(player.cbox, drop.cbox) && player.callback(drop)) {
            // TODO make this more exciting for the player
            WANTED = true;
            if (LEVEL < MAX_LEVEL) { LEVEL++; }
            crate = Crate(randomPointOffCamera(LEVEL_SIZE)); // move the crate to a new spot
        }

        // increase the score for surviving when wanted
        if (WANTED && arduboy.everyXFrames(60)) {
            player.increaseScore(1);
        }

        // handle dust
        for (uint8_t i = 0; i < dust.size(); i++) {
            if (dust[i].ttl) {
                dust[i].update();
                dust[i].draw();
            } else {
                dust.erase(i);
                if (i) i--;
            }
        }

        // TODO move setup elsewhere
        while (!cacti.full()) cacti.add(Cactus());

        // draw cacti
        for (uint8_t i = 0; i < cacti.size(); i++) {
            cacti[i].draw();
        }

        // handle spikes
        for (uint8_t i = 0; i < spikes.size(); i++) {
            if (spikes[i].ttl) {
                // delete the spike if the player collides with it
                // TODO make the player react to hitting a spike
                if (collide(player.cbox, spikes[i].cbox) && player.callback(spikes[i])) {
                    spikes.erase(i);
                    if (i) i--;
                    continue;
                }

                spikes[i].update();
                spikes[i].draw();
            } else {
                spikes.erase(i);
                if (i) i--;
            }
        }

        // TODO implement levels
        while (cops.size() < COP_COUNT[LEVEL]) cops.add(Cop(randomPointOffCamera(LEVEL_SIZE)));
        while (swat.size() < SWAT_COUNT[LEVEL]) swat.add(Swat(randomPointOffCamera(LEVEL_SIZE)));

        // handle cops
        for (uint8_t i = 0; i < cops.size(); i++) {
            // only collide with the player if it still exists
            if (player.health) {
                if (collide(player.cbox, cops[i].cbox)) player.callback(cops[i]);
            }

            // check for collisions with other cops
            for (uint8_t j = 0; j < cops.size(); j++) {
                if (j != i) {
                    if (collide(cops[i].cbox, cops[j].cbox)) {
                        if (cops[i].callback(cops[j])) {
                            sound.tone(50, 200);
                            if (!dust.full())
                                dust.add(
                                        Dust(cops[i].x + cops[i].width / 2 - 4, cops[i].y + cops[i].height / 2 - 4,
                                             cops[i].angle, cops[i].speed / 2)); // generate a dust cloud
                            player.increaseScore(3); // award the player for destroying a cop
                            cops.erase(i);
                            break; // break instead of continuing because we don't want the nested for loops to get out of sync
                        } else {
                            separate(cops[i], cops[j]);
                        }
                    }
                }
            }

            cops[i].update();
            cops[i].draw();
            cops[i].follow(player.x, player.y);

            // the magic number here represents the distance at which the indicator starts to grow
            uint8_t locatorRadius = 200 / distanceBetween(player.x, player.y, cops[i].x, cops[i].y);
            drawLocator(cops[i], locatorRadius);
        }

        // handle swat
        for (uint8_t i = 0; i < swat.size(); i++) {
            // only collide with the player if it still exists
            if (player.health) {
                if (collide(player.cbox, swat[i].cbox)) player.callback(swat[i]);
            }

            // check for collisions with other swat
            for (uint8_t j = 0; j < swat.size(); j++) {
                if (j != i) {
                    if (collide(swat[i].cbox, swat[j].cbox)) {
                        if (swat[i].callback(swat[j])) {
                            if (!dust.full())
                                dust.add(
                                        Dust(swat[i].x + swat[i].width / 2 - 4, swat[i].y + swat[i].height / 2 - 4,
                                             swat[i].angle, swat[i].speed / 2)); // generate a dust cloud
                            player.increaseScore(5); // award the player for destroying a cop
                            // swat.erase(i);
                            break; // break instead of continuing because we don't want the nested for loops to get out of sync
                        } else {
                            separate(swat[i], swat[j]);
                        }
                    }
                }
            }

            swat[i].update();
            swat[i].draw();
            swat[i].follow(player.x, player.y);

            // the magic number here represents the distance at which the indicator starts to grow
            uint8_t locatorRadius = 200 / distanceBetween(player.x, player.y, swat[i].x, swat[i].y);
            drawLocator(swat[i], locatorRadius);
        }

        drawNumber(2, 2, SCORE);

        // game over
        if (player.health == 0) {
            // TODO save score
            busted.gameOver(); // show "busted" and return to the menu
        }

        arduboy.display(CLEAR_BUFFER); // draw everything to the screen
    }
} Game;
