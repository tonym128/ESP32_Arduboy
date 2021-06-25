#ifndef tileFloor_h
#define tileFloor_h

#include <Arduino.h>
#include <Arduboy2.h>

enum class TileDef: uint8_t {
    // begin - walkable tiles
    Blank = 0,
    FloorFlavor1,
    FloorFlavor2,
    ShopDoor,
    Sand,
    SandGrassBoundary,
    IslandLowerLeftCorner,
    IslandLowerRightCorner,
    IslandUpperLeftCorner,
    IslandUpperRightCorner,
    IslandRight,
    IslandLeft,
    IslandUpper,
    IslandLower,
    Pier,
    // end - walkable tiles

    // begin- solid tiles
    // begin -fishable tiles
    PondLowerLeftCorner,
    PondLowerRightCorner,
    PondUpperLeftCorner,
    PondUpperRightCorner,
    PondMiddle,
    PondMiddleDeep,
    PondLowerBoundary,
    PondUpperBoundary,
    PondLeftBoundary,
    PondRightBoundary,
    PondInnerCorner,
    Ocean,
    OceanSandBoundary,

    // end - fishable tiles
    BushWallLeft,
    BushWallRight,
    BushWallTop,
    BushWallBottom,
    BushWallLowerLeftCorner,
    BushWallLowerRightCorner,
    BushWallUpperLeftCorner,
    BushWallUpperRightCorner,
    BushLeft,
    BushRight,
    ShopLeftWall,
    ShopRightWall,
    ShopLeftSign,
    ShopCenterSign,
    ShopRightSign
    // end - solid tiles
};

struct TileFloor {
    static uint8_t waterAnimationOffset;
    static void renderTile(int16_t x, int16_t y, uint8_t tileId);
    static void renderCenteredOn(int16_t x, int16_t y, uint8_t frame);

    static TileDef getTileAt(int16_t x, int16_t y);

    static bool isWalkable(TileDef tile) {
        return tile <= TileDef::Pier;
    }

    static bool isFishable(TileDef tile) {
        return tile >= TileDef::PondLowerLeftCorner && tile <= TileDef::OceanSandBoundary;
    }
};

#endif

