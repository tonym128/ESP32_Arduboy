#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../ardBitmap/ArdBitmap.h"
#include "../utils/Constants.h"
#include "../utils/Enums.h"

  
class TownItem { 

  public:

    TownItem();
    constexpr TownItem(ItemType itemType, int16_t x, RenderPosition renderPosition = RenderPosition::Background) : 
      itemType(itemType), x(x), mask(0), renderPosition(renderPosition) { }

    ItemType getItemType();
    uint16_t getX();
    bool getEnabled();
    RenderPosition getRenderPosition();

    void setItemType(const ItemType itemType, const RenderPosition renderPosition = RenderPosition::Background);
    void setX(const uint16_t x);
    void setEnabled(const bool enabled);
    void setRenderPosition(const RenderPosition renderPosition);

    void render(Arduboy2Ext &arduboy, ArdBitmap<128, 64> &ardBitmap, const int16_t xPos, const RenderPosition renderPosition, const bool gameOver);
    void init(const ItemType itemType, const int16_t xPos, const RenderPosition renderPosition = RenderPosition::Background);


  private:


  private:

    ItemType itemType;
    uint16_t x;
    uint8_t mask;
    bool enabled = true;
    RenderPosition renderPosition = RenderPosition::Background;
    
};
