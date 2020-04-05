#pragma once

#include "player.h"
#include "worm.h"
#include "state.h"

class Game {
    typedef void (Game::*UpdatePtr)(uint8_t);
    typedef void (Game::*RenderPtr)(uint8_t);

    public:
        Player player;

        UpdatePtr prevUpdate;
        UpdatePtr currentUpdate;
        UpdatePtr nextUpdate;

        RenderPtr prevRender;
        RenderPtr currentRender;
        RenderPtr nextRender;

        uint8_t titleCount;
        uint8_t seconds;     // actual seconds. 1 game hour = SECONDS_PER_HOUR real seconds

        bool isOnShopDoor();
        bool isOnBoat();
        bool isTalkingToGirl();
        bool isTalkingToGuy();

        void loadSave();

        void push(UpdatePtr newUpdate, RenderPtr newRender);
        void pop();

        void update(uint8_t frame);
        void render(uint8_t frame);

        void updateLogo(uint8_t frame);
        void renderLogo(uint8_t frame);

        void updateTitle(uint8_t frame);
        void renderTitle(uint8_t frame);

        void updatePlay(uint8_t frame);
        void renderPlay(uint8_t frame);

        void updateShop(uint8_t frame);
        void renderShop(uint8_t frame);

        void updateBoatTravel(uint8_t frame);
        void renderBoatTravel(uint8_t frame);

        void updateTalkToGirl(uint8_t frame);
        void renderTalkToGirl(uint8_t frame);

        void updateTalkToGuy(uint8_t frame);
        void renderTalkToGuy(uint8_t frame);

        Game(): 
            player(),
            prevUpdate(NULL),
            currentUpdate(&Game::updateLogo),
            nextUpdate(NULL),
            prevRender(NULL),
            currentRender(&Game::renderLogo),
            nextRender(NULL),
            titleCount(0),
            seconds(0)
        {
            State::load();
        }
};

