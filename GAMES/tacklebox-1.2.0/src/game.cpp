#include <Arduboy2.h>
#include "game.h"
#include "renderer.h"
#include "nonMaskBitmaps.h"
#include "maskBitmaps.h"
#include "strings.h"
#include "tileFloor.h"
#include "world.h"
#include "hud.h"
#include "shop.h"
#include "girlDialog.h"
#include "guyDialog.h"
#include "worms.h"
#include "signs.h"
#include "girl.h"
#include "guy.h"
#include "boat.h"
#include "util.h"
#include "dialogUtils.h"
#include "toast.h"

extern Renderer renderer;
extern Arduboy2Base arduboy;

const uint8_t SECONDS_PER_TWO_MINUTES = 1;
const uint16_t MINUTES_IN_A_DAY = 24 * 60;

/**
 * push a game state onto the "stack". The stack can only go two deep.
 * This is used to go from play -> inGameMenu, for example.
 */
void Game::push(UpdatePtr newUpdate, RenderPtr newRender) {
    nextUpdate = newUpdate;
    nextRender = newRender;
}

/**
 * Go back one level in the "stack", ie from inGameMenu back to play
 */
void Game::pop() {
    nextUpdate = prevUpdate;
    nextRender = prevRender;
}

void Game::updateLogo(uint8_t frame) {
    if (frame == 60) {
        push(&Game::updateTitle, &Game::renderTitle);
    }
}

void Game::renderLogo(uint8_t frame) {
    renderer.drawOverwrite(WIDTH / 2 - 8, HEIGHT / 2 - 4, city41Logo_tiles, 0);
}

void Game::updateTitle(uint8_t frame) {
    titleCount += 1;
    if (titleCount == 120) {
        push(&Game::updatePlay, &Game::renderPlay);
    }
}

void Game::renderTitle(uint8_t frame) {
    renderer.drawOverwrite(17, 16, logo_tiles, 0);
}

bool isOnScreen(int16_t refX, int16_t refY, int16_t x, int16_t y) {
    return (refX - WIDTH / 2 <= x && refX + WIDTH / 2 >= x) &&
        (refY - HEIGHT / 2 <= y && refY + HEIGHT / 2 >= y);
}

bool overlap(Player& player, const int16_t otherX, const int16_t otherY, const int8_t otherW, const int8_t otherH) {
    return !(
        otherX          >= player.x + 16  ||
        otherX + otherW <= player.x       ||
        otherY          >= player.y + 16  ||
        otherY + otherH <= player.y
    );
}

bool Game::isOnShopDoor() {
    return (    
        player.y <= 20 &&
        (
            TileFloor::getTileAt(player.x + 8, player.y) == TileDef::ShopDoor
        )
    );
}

bool Game::isOnBoat() {
    if (Boat::currentDock == Dock::Beach) {
        return (
            player.dir == UP &&
            player.x >= Boat::x - 4 &&
            player.y >= Boat::y
        );
    } else {
        return (
            player.dir == UP &&
            player.x <= Boat::x + 4 &&
            player.y >= Boat::y
        );
    }
}

bool Game::isTalkingToGirl() {
    return Girl::isActive && overlap(player, Girl::x, Girl::y, 16, 16);
}

bool Game::isTalkingToGuy() {
    return Guy::isActive && overlap(player, Guy::x, Guy::y, 16, 16);
}

void Game::updatePlay(uint8_t frame) {
    if (frame == 60) {
        seconds += 1;

        if (seconds == SECONDS_PER_TWO_MINUTES) {
            seconds = 0;
            State::gameState.minute += 2;

            if (State::gameState.minute == MINUTES_IN_A_DAY) {
                State::gameState.minute = 0;
            }
        }
    }

    player.update(frame);

    bool isActive = isOnScreen(player.x, player.y, Girl::x, Girl::y);
    bool justBecameActive = isActive && !isOnScreen(player.prevX, player.prevY, Girl::x, Girl::y);
    Girl::update(isActive, justBecameActive);

    isActive = isOnScreen(player.x, player.y, Guy::x, Guy::y);
    justBecameActive = isActive && !isOnScreen(player.prevX, player.prevY, Guy::x, Guy::y);
    Guy::update(isActive, justBecameActive);

    if (isOnShopDoor()) {
        if (Shop::isOpen()) {
            push(&Game::updateShop, &Game::renderShop);
            return;
        } else {
            player.undoMove();
        }
    }

    if (isOnBoat() && State::gameState.hasOars) {
        Boat::onEnter();
        push(&Game::updateBoatTravel, &Game::renderBoatTravel);
        return;
    }

    if (isTalkingToGirl()) {
        player.undoMove();
        push(&Game::updateTalkToGirl, &Game::renderTalkToGirl);
        return;
    }

    if (isTalkingToGuy()) {
        player.undoMove();
        push(&Game::updateTalkToGuy, &Game::renderTalkToGuy);
        return;
    }

    for (uint8_t w = 0; w < MAX_WORMS; ++w) {
        isActive = isOnScreen(player.x, player.y, worms[w].x, worms[w].y);
        justBecameActive = isActive && !isOnScreen(player.prevX, player.prevY, worms[w].x, worms[w].y);
        worms[w].update(frame, isActive, justBecameActive);

        if (overlap(player, worms[w].x, worms[w].y, 8, 8)) {
            player.onGetWorm(worms[w]);
            worms[w].onGrabbedByPlayer();
        }
    }



    for (uint8_t s = 0; s < MAX_SIGNS; ++s) {
        if (overlap(player, signs[s].x, signs[s].y, 12, 6)) {
            player.undoMove();
            Toast::toast(signs[s].message);
        }
    }
}

void Game::renderPlay(uint8_t frame) {
    int16_t centerX = minVal(maxVal(WIDTH / 2, player.x), MAP_WIDTH_PX - WIDTH / 2);
    int16_t centerY = minVal(maxVal(HEIGHT / 2, player.y), MAP_HEIGHT_PX - HEIGHT / 2);

    TileFloor::renderCenteredOn(centerX, centerY, frame);

    // translate the renderer based on the player's location
    // essentially the same as translating a camera in a 3d game
    int16_t translateX;
    int16_t translateY;

    if (player.x < WIDTH / 2) {
        translateX = 0;
    } else if (player.x > MAP_WIDTH_PX - WIDTH / 2) {
        translateX = WIDTH - MAP_WIDTH_PX;
    } else {
        translateX = -player.x + WIDTH / 2;
    }

    if (player.y < HEIGHT / 2) {
        translateY = 0;
    } else if (player.y > MAP_HEIGHT_PX - HEIGHT / 2) {
        translateY = HEIGHT - MAP_HEIGHT_PX;
    } else {
        translateY = -player.y + HEIGHT / 2;
    }

    renderer.translateX = translateX;
    renderer.translateY = translateY;

    for (uint8_t w = 0; w < MAX_WORMS; ++w) {
        if (abs(worms[w].y - player.y) < HEIGHT) {
            worms[w].render(frame);
        }
    }

    for (uint8_t s = 0; s < MAX_SIGNS; ++s) {
        if (abs(signs[s].y - player.y) < HEIGHT) {
            signs[s].render();
        }
    }

    Girl::render(frame);

    if (player.y > 200) {
        Guy::render(frame);
    }

    Boat::render(frame);

    if (!Shop::isOpen() && player.y < HEIGHT) {
        renderer.drawOverwrite(16 * 24 - 5, 20, closed_tiles, 0);
    }

    player.render(frame);

    renderer.translateX = WIDTH - 64;
    renderer.translateY = 0;
    Hud::render(player);

    Toast::render();
}

void Game::updateShop(uint8_t frame) {
    if (arduboy.justPressed(B_BUTTON) && Shop::currentUpdate == &Shop::updateMainMenu) {
        player.moveTo(player.x, player.y + 16);
        pop();
    } else {
        Shop::update(frame);
    }
}

void Game::renderShop(uint8_t frame) {
    Shop::render(frame);
}

void Game::updateTalkToGirl(uint8_t frame) {
    if (arduboy.justPressed(B_BUTTON)) {
        pop();
    } else {
        GirlDialog::update();
    }
}

void Game::renderTalkToGirl(uint8_t frame) {
    GirlDialog::render();
}

void Game::updateTalkToGuy(uint8_t frame) {
    if (arduboy.justPressed(B_BUTTON)) {
        pop();
    } else {
        GuyDialog::update();
    }
}

void Game::renderTalkToGuy(uint8_t frame) {
    GuyDialog::render();
}

void Game::updateBoatTravel(uint8_t frame) {
    Boat::update(frame);

    if (Boat::arrived) {
        if (Boat::currentDock == Dock::Island) {
            player.moveTo(Boat::x + 8, Boat::y + 16, true);
            player.dir = RIGHT;
        } else {
            player.moveTo(Boat::x, Boat::y + 16, true);
            player.dir = LEFT;
        }
        pop();
    }
}

void Game::renderBoatTravel(uint8_t frame) {
    const int16_t boatX = Boat::x + 2;
    const int16_t boatY = Boat::y + 16;

    TileFloor::renderCenteredOn(boatX, boatY, frame);


    int16_t translateX = -boatX + WIDTH / 2;
    int16_t translateY = -boatY + HEIGHT / 2;
    renderer.translateX = translateX;
    renderer.translateY = translateY;

    for (uint8_t s = 0; s < MAX_SIGNS; ++s) {
        signs[s].render();
    }

    Boat::render(frame);
}

void Game::update(uint8_t frame) {
    (this->*currentUpdate)(frame);

    if (nextUpdate != NULL) {
        prevUpdate = currentUpdate;
        currentUpdate = nextUpdate;
        nextUpdate = NULL;
    }

}

void Game::render(uint8_t frame) {
    renderer.translateX = 0;
    renderer.translateY = 0;

    (this->*currentRender)(frame);

    if (nextRender != NULL) {
        prevRender = currentRender;
        currentRender = nextRender;
        nextRender = NULL;
    }
}

