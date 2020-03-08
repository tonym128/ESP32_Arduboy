#include "TownItem.h"

#include "../utils/Arduboy2Ext.h"
#include "../ardBitmap/ArdBitMap.h"
#include "../utils/Constants.h"
#include "../utils/Utils.h"
#include "../utils/Enums.h"
#include "../images/Images.h"

TownItem::TownItem() { }


ItemType TownItem::getItemType() {

  return this->itemType;

}


void TownItem::setItemType(const ItemType itemType, const RenderPosition renderPosition) {

  this->itemType = itemType;
  this->renderPosition = renderPosition;

}


void TownItem::init(const ItemType itemType, int16_t xPos, const RenderPosition renderPosition) {

  this->itemType = itemType;
  this->x = xPos;
  this->renderPosition = renderPosition;

}

uint16_t TownItem::getX() {

  return this->x;

}

bool TownItem::getEnabled() {

  return this->enabled;

}

RenderPosition TownItem::getRenderPosition() {

  return this->renderPosition;

}


void TownItem::setX(const uint16_t x) {

  this->x = x;
  
}

void TownItem::setEnabled(const bool enabled) {

  this->enabled = enabled;
  
}

void TownItem::setRenderPosition(const RenderPosition renderPosition) {

  this->renderPosition = renderPosition;
  
}


void TownItem::render(Arduboy2Ext &arduboy, ArdBitmap<128, 64> &ardBitmap, const int16_t xPos, const RenderPosition renderPosition, const bool gameOver) {

  if (this->renderPosition == renderPosition) {

    switch (this->itemType) {

      case ItemType::Crates:
        ardBitmap.drawCompressed_WithMask(this->x + xPos - 6, 33, Images::Crates, Images::Crates_Mask, MIRROR_NONE);
        break;

      case ItemType::Castle_01:
        SpritesB::drawExternalMask(this->x + xPos - 21, 10, Images::Castle_LH_Top, Images::Castle_LH_Top_Mask, 0, 0);
        SpritesB::drawOverwrite(this->x + xPos - 13, 2, Images::Castle_RH_Top, 0);
        SpritesB::drawOverwrite(this->x + xPos - 21, 18, Images::Castle_LH_Mid, 0);
        SpritesB::drawOverwrite(this->x + xPos - 28, 34, Images::Castle_LH_Bottom, 0);

        if (gameOver) {
          SpritesB::drawOverwrite(this->x + xPos - 9, 36, Images::ClosedDoor, 0);
        }        

        break;

      case ItemType::Castle_02:
        SpritesB::drawExternalMask(this->x + xPos, 34, Images::Castle_RH_Bottom, Images::Castle_RH_Bottom_Mask, 0, 0);
        
        break;

      case ItemType::Tombstone:
        SpritesB::drawOverwrite(this->x + xPos - 4, 34, Images::Tombstone, 0);
        break;

      case ItemType::Pumpkin:
        SpritesB::drawExternalMask(this->x + xPos, 40, Images::Pumpkin, Images::Pumpkin_Mask, 0, 0);
        break;

      case ItemType::SavingPost:
        ardBitmap.drawCompressed(this->x + xPos, 34, Images::SavingPost, BLACK, MIRROR_NONE);
        break;

      case ItemType::NPC:
        
        if (gameOver) {

          SpritesB::drawErase(this->x + xPos - 4, 37, Images::NPC_EOG, this->mask);

        }
        else {

          SpritesB::drawErase(this->x + xPos - 5, 34, Images::NPC, this->mask);

        }
        
        if (arduboy.everyXFrames(8)) {
          this->mask++;
          if (this->mask == 3) { this->mask = 0; }

        }

        break;

      case ItemType::HP:

        if (this->enabled) {

          if (arduboy.getFrameCountHalf(32)) {
            SpritesB::drawErase(this->x + xPos - 5, 43, Images::HP, this->mask);
          }

        }

        break;

      default: break;
        
    }

  }

}
