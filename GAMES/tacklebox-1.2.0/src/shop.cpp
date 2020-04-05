#include "shop.h"
#include "renderer.h"
#include "strings.h"
#include "nonMaskBitmaps.h"
#include "maskBitmaps.h"
#include "fishType.h"
#include "fish.h"
#include "state.h"
#include "enumUtils.h"
#include "baitType.h"
#include "sfx.h"
#include "dialogUtils.h"

extern Renderer renderer;
extern Arduboy2Base arduboy;

ShopMainMenu Shop::mainMenuCurrentRow = ShopMainMenu::Buy;
BuyMenu Shop::buyMenuCurrentRow = BuyMenu::Grub;
Shop::UpdatePtr Shop::currentUpdate = &Shop::updateMainMenu;
Shop::RenderPtr Shop::currentRender = &Shop::renderMainMenu;
bool Shop::showAdvice = false;


const uint16_t GRUB_PRICE = 1;
const uint16_t SHRIMP_PRICE = 3;
const uint16_t PRO_POLE_PRICE = 200;
const uint16_t OARS_PRICE = 400;
const int8_t MAX_ADVICE_LEVEL = 3;

const uint8_t BUY_MENU_ITEMS_COUNT = 4;
const uint8_t BUY_MENU_ITEM_PROPS_COUNT = 3;

const uint32_t PROGMEM buyMenuItems[BUY_MENU_ITEMS_COUNT * BUY_MENU_ITEM_PROPS_COUNT] = {
    reinterpret_cast<uint32_t>(grub_tiles),
    reinterpret_cast<uint32_t>(grub_string),
    GRUB_PRICE,
    reinterpret_cast<uint32_t>(shrimp_tiles),
    reinterpret_cast<uint32_t>(shrimp_string),
    SHRIMP_PRICE,
    reinterpret_cast<uint32_t>(proPole_tiles),
    reinterpret_cast<uint32_t>(proPole_string),
    PRO_POLE_PRICE,
    reinterpret_cast<uint32_t>(boatOar_tiles),
    reinterpret_cast<uint32_t>(boatOars_string),
    OARS_PRICE
};

const uint8_t* const PROGMEM adviceStrings[] = {
    NULL,
    advice1_string,
    advice2_string,
    advice3_string
};

bool Shop::isOpen() {
    const uint8_t hour = State::getCurrentHour();

    return hour >= 4 && hour <= 24;
}

void Shop::update(uint8_t frame) {
    Shop::currentUpdate(frame);
}

void Shop::render(uint8_t frame) {
    Shop::currentRender(frame);

    // user's current money in upper corner
    DialogUtils::renderMoneyInCorner();

    // user's current grub and shrimp count for reference
    renderer.drawOverwrite(WIDTH - 70, 8, grub_tiles, 0);
    renderer.drawNumber(WIDTH - 58, 10, State::gameState.baitCounts[static_cast<int8_t>(BaitType::Grub)]);

    renderer.drawOverwrite(WIDTH - 40, 8, shrimp_tiles, 0);
    renderer.drawNumber(WIDTH - 28, 10, State::gameState.baitCounts[static_cast<int8_t>(BaitType::Shrimp)]);
}

void Shop::updateMainMenu(uint8_t frame) {
    if (arduboy.justPressed(UP_BUTTON)) {
        Shop::mainMenuCurrentRow = prev(Shop::mainMenuCurrentRow);
    }

    if (arduboy.justPressed(DOWN_BUTTON)) {
        Shop::mainMenuCurrentRow = next(Shop::mainMenuCurrentRow);
    }

    if (arduboy.justPressed(A_BUTTON)) {
        if (mainMenuCurrentRow == ShopMainMenu::Buy) {
            currentUpdate = &Shop::updateBuy;
            currentRender = &Shop::renderBuy;
        } else if (mainMenuCurrentRow == ShopMainMenu::Sell) {
            currentUpdate = &Shop::updateSell;
            currentRender = &Shop::renderSell;
        } else if (mainMenuCurrentRow == ShopMainMenu::Advice) {
            showAdvice = false;
            currentUpdate = &Shop::updateAdvice;
            currentRender = &Shop::renderAdvice;
        } else {
            currentUpdate = &Shop::updateHours;
            currentRender = &Shop::renderHours;
        }
    }
}

const uint8_t MAIN_MENU_STRINGS_COUNT = 4;

const uint8_t* const PROGMEM mainMenuStrings[MAIN_MENU_STRINGS_COUNT] = {
    buy_string,
    sell_string,
    advice_string,
    hours_string
};

void Shop::renderMainMenu(uint8_t frame) {
    DialogUtils::renderFrame(shopOwner_tiles);

    const uint8_t startY = 25;
    const uint8_t x = 48;
    const uint8_t spacing = 9;

    renderer.drawOverwrite(x - 6, startY + static_cast<uint8_t>(Shop::mainMenuCurrentRow) * spacing, squareIcon_tiles, 0);

    for (uint8_t m = 0; m < MAIN_MENU_STRINGS_COUNT; ++m) {
        renderer.drawString(x, startY + spacing * m, static_cast<const uint8_t*>(pgm_read_ptr(mainMenuStrings + m)));
    }
}

void buy(BuyMenu itemToBuy) {
    bool buzz = false;

    switch (itemToBuy) {
        case BuyMenu::Grub:
            if (State::gameState.money >= GRUB_PRICE &&
                State::gameState.baitCounts[static_cast<int8_t>(BaitType::Grub)] < 99) {
                State::gameState.money -= GRUB_PRICE;
                State::gameState.baitCounts[static_cast<int8_t>(BaitType::Grub)] += 1;
            } else {
                buzz = true;
            }
            break;
        case BuyMenu::Shrimp:
            if (State::gameState.money >= SHRIMP_PRICE &&
                State::gameState.baitCounts[static_cast<int8_t>(BaitType::Shrimp)] < 99) {
                State::gameState.money -= SHRIMP_PRICE;
                State::gameState.baitCounts[static_cast<int8_t>(BaitType::Shrimp)] += 1;
            } else {
                buzz = true;
            }
            break;
        case BuyMenu::ProPole:
            if (State::gameState.money >= PRO_POLE_PRICE && !State::gameState.hasProPole) {
                State::gameState.money -= PRO_POLE_PRICE;
                State::gameState.hasProPole = true;
            } else {
                buzz = true;
            }
            break;
        case BuyMenu::Oars:
            if (State::gameState.money >= OARS_PRICE && !State::gameState.hasOars) {
                State::gameState.money -= OARS_PRICE;
                State::gameState.hasOars = true;
            } else {
                buzz = true;
            }
            break;
    }

    if (buzz) {
        Sfx::buzz();
    } else {
        Sfx::purchase();
    }
}

void Shop::updateBuy(uint8_t frame) {
    if (arduboy.justPressed(B_BUTTON)) {
        Shop::currentUpdate = &Shop::updateMainMenu;
        Shop::currentRender = &Shop::renderMainMenu;
    }

    if (arduboy.justPressed(DOWN_BUTTON)) {
        buyMenuCurrentRow = next(buyMenuCurrentRow);
    }

    if (arduboy.justPressed(UP_BUTTON)) {
        buyMenuCurrentRow = prev(buyMenuCurrentRow);
    }

    if (arduboy.justPressed(A_BUTTON)) {
        buy(buyMenuCurrentRow);
    }
}



void Shop::renderBuy(uint8_t frame) {
    DialogUtils::renderFrame(shopOwner_tiles);

    const uint8_t startY = 22;
    
    const uint8_t startX = 24;
    const uint8_t spacing = 9;

    for (uint8_t i = 0; i < BUY_MENU_ITEMS_COUNT; ++i) {
        const uint8_t y = startY + spacing * i;

        // icon
        const uint8_t* bmp = reinterpret_cast<const uint8_t*>(pgm_read_ptr(buyMenuItems + i * BUY_MENU_ITEM_PROPS_COUNT));
        renderer.drawOverwrite(startX, y, bmp, 0);

        // name
        const uint8_t* str = reinterpret_cast<const uint8_t*>(pgm_read_ptr(buyMenuItems + i * BUY_MENU_ITEM_PROPS_COUNT + 1));
        renderer.drawString(startX + 21, y + 1, str);

        
        // price
        const int16_t price = static_cast<uint32_t>(pgm_read_word(buyMenuItems + i * BUY_MENU_ITEM_PROPS_COUNT + 2));

        if ((i == 2 && State::gameState.hasProPole) || (i == 3 && State::gameState.hasOars)) {
        } else {
            // currency symbol
            renderer.drawPlusMask(startX + 68, y, currencySymbol_plus_mask, 0);
            // then price
            renderer.drawNumber(startX + 74, y + 1, price);
        }
    }

    renderer.drawOverwrite(startX - 7, startY + 2 + spacing * static_cast<int8_t>(buyMenuCurrentRow),  squareIcon_tiles, 0);
}

void Shop::updateSell(uint8_t frame) {
    if (arduboy.justPressed(B_BUTTON)) {
        Shop::currentUpdate = &Shop::updateMainMenu;
        Shop::currentRender = &Shop::renderMainMenu;
    }

    if (arduboy.justPressed(A_BUTTON)) {
        bool soldSomething = State::sellAllFish();

        if (soldSomething) {
            Sfx::purchase();
        }

        Shop::currentUpdate = &Shop::updateMainMenu;
        Shop::currentRender = &Shop::renderMainMenu;
    }
}

void Shop::renderSell(uint8_t frame) {
    DialogUtils::renderFrame(shopOwner_tiles);

    uint16_t moneyAmount = 0;

    Fish fish;
    for (uint8_t f = 0; f < static_cast<int8_t>(FishType::OLD_BOOT); ++f) {
        Fish::loadFish(static_cast<FishType>(f), fish);
        moneyAmount += State::gameState.currentFishCount[f] * fish.value;
    }

    if (moneyAmount == 0) {
        const uint8_t* str = youHaveNoFish_string;

        if (State::gameState.currentFishCount[static_cast<int8_t>(FishType::OLD_BOOT)] > 0) {
            str = notBuyingBoots_string;
        }

        renderer.drawString(24, 30, str);
    } else {
        renderer.drawString(20, 30, sellFor_string);

        renderer.drawPlusMask(43, 39, currencySymbol_plus_mask, 0);
        renderer.drawNumber(49, 40, moneyAmount);
        renderer.drawRect(38, 37, 50, 10, WHITE);
    }

    renderer.drawOverwrite(54, 50, squareIcon_tiles, 0);
    renderer.drawString(60, 50, ok_string);
}

const uint8_t PROGMEM advicePrices[] = {
    3,
    30,
    150
};

void Shop::updateAdvice(uint8_t frame) {
    if (arduboy.justPressed(B_BUTTON)) {
        Shop::currentUpdate = &Shop::updateMainMenu;
        Shop::currentRender = &Shop::renderMainMenu;
    }

    uint8_t advicePrice = static_cast<uint8_t>(pgm_read_byte(advicePrices + State::gameState.adviceLevel));

    if (!showAdvice && arduboy.justPressed(A_BUTTON)) {
        if (
            State::gameState.adviceLevel < MAX_ADVICE_LEVEL &&
            State::gameState.money >= advicePrice
        ) {
            State::gameState.money -= advicePrice;

            State::gameState.adviceLevel += 1;
            showAdvice = true;

            Sfx::purchase();
        } else {
            Sfx::buzz();
        }
    }
}

void Shop::renderAdvice(uint8_t frame) {
    DialogUtils::renderFrame(shopOwner_tiles);

    if (State::gameState.adviceLevel == MAX_ADVICE_LEVEL && !showAdvice) {
        renderer.drawString(20, 30, noMoreAdvice_string);
    } else if (showAdvice) {
        const uint8_t* str = static_cast<const uint8_t*>(pgm_read_ptr(adviceStrings + State::gameState.adviceLevel));
        renderer.drawString(20, 30, str);
    } else {
        uint8_t advicePrice = static_cast<uint8_t>(pgm_read_byte(advicePrices + State::gameState.adviceLevel));

        renderer.drawString(20, 30, adviceFor_string);

        renderer.drawPlusMask(43, 39, currencySymbol_plus_mask, 0);
        renderer.drawNumber(49, 40, advicePrice);
        renderer.drawRect(38, 37, 50, 10, WHITE);

        renderer.drawOverwrite(54, 50, squareIcon_tiles, 0);
        renderer.drawString(60, 50, ok_string);
    }
}

void Shop::updateHours(uint8_t frame) {
    if (arduboy.justPressed(B_BUTTON)) {
        Shop::currentUpdate = &Shop::updateMainMenu;
        Shop::currentRender = &Shop::renderMainMenu;
    }
}

void Shop::renderHours(uint8_t frame) {
    DialogUtils::renderFrame(shopOwner_tiles);

    renderer.drawString(25, 36, openFromTo_string);
}
