#pragma once

#include <Arduino.h>

enum class ShopMainMenu: uint8_t {
    Buy,
    Sell,
    Advice,
    Hours,
    COUNT
};

enum class BuyMenu: uint8_t {
    Grub,
    Shrimp,
    ProPole,
    Oars,
    COUNT
};

struct Shop {
    typedef void (*UpdatePtr)(uint8_t);
    typedef void (*RenderPtr)(uint8_t);

    static bool showAdvice;

    static UpdatePtr currentUpdate;
    static RenderPtr currentRender;

    static bool isOpen();

    static void update(uint8_t frame);
    static void render(uint8_t frame);

    static ShopMainMenu mainMenuCurrentRow;
    static BuyMenu buyMenuCurrentRow;
    static void updateMainMenu(uint8_t frame);
    static void renderMainMenu(uint8_t frame);

    static void updateBuy(uint8_t frame);
    static void renderBuy(uint8_t frame);

    static void updateSell(uint8_t frame);
    static void renderSell(uint8_t frame);

    static void updateAdvice(uint8_t frame);
    static void renderAdvice(uint8_t frame);

    static void updateHours(uint8_t frame);
    static void renderHours(uint8_t frame);
};


