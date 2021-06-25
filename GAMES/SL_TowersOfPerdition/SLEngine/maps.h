/*************************************************************************
 * Shattered Lands: Towers of Perdition                                  *
 * C opyright (C) 2019  Justin (tuxinator2009) Davis                     *
 *                                                                       *
 * This program is free software: you can redistribute it and/or modify  *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * This program is distributed in the hope that it will be useful,       *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have received a copy of the GNU General Public License     *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *************************************************************************/

#ifndef MAPS_H
#define MAPS_H

#define EVENT_TELEPORT		0
#define EVENT_INN					1
#define EVENT_SHOP				2
#define EVENT_LARGECHEST	3
#define EVENT_MESSAGE			4
#define EVENT_BOSSBATTLE	5

static const uint16_t  tilesetPassability[8] = {0x6E2F, 0x4100, 0x50C4, 0x7211};//0101000011000100

//uint8_t getMapNumChunks(const uint8_t *map)
//{
//	return (pgm_read_byte(map) & 0x3F) + 1;
//}

uint8_t getMapTileset(const uint8_t *map)
{
	return pgm_read_byte(map) & 3;
	//return (pgm_read_byte(map) >> 6) & 3;
}

uint8_t getMapSong(const uint8_t *map)
{
	return pgm_read_byte(map) >> 5;
}

/*uint8_t getMapMonsters(const uint8_t *map)
{
	return pgm_read_byte(map + 10);
}*/

uint8_t getChunkTileID(uint16_t tiles, int8_t metaID)
{
	uint8_t tileID = 0;
	while (metaID >= 0)
	{
		if (tiles == 0)
			return 0;
		if (tiles & 0x8000)
			--metaID;
		++tileID;
		tiles *= 2;
	}
	return tileID - 1;
}

uint8_t getChunkMetaTileID(const uint8_t *chunk, uint8_t x, uint8_t y, uint8_t bitsPerTile)
{
	uint8_t bitOffset = (y * 8 + x) * bitsPerTile;
	bitReaderInit(chunk + 5 + (bitOffset / 8), bitOffset % 8);
	//globalBitReader/.init(chunk + 5 + (bitOffset / 8), bitOffset % 8);
	return bitReaderRead8(bitsPerTile);//globalBitReader/.read8(bitsPerTile);
}

uint8_t getChunkBits(uint16_t tiles)
{
	uint8_t count = 0;
	while (tiles)
	{
		count += tiles & 1;
		tiles >>= 1;
	}
	if (count <= 1)
		return 0;
	else if (count <= 2)
		return 1;
	else if (count <= 4)
		return 2;
	else if (count <= 8)
		return 3;
	return 4;
}

int16_t getChunkBytes(uint16_t tiles)
{
	return 5 + getChunkBits(tiles) * 8;
}

const uint8_t *getChunk(const uint8_t *map, uint8_t chunkID)
{
	const uint8_t *chunk = map + 1;
	if (chunkID == 0x3F)
		return NULL;
	while (chunkID > 0)
	{
		chunk += getChunkBytes(READ_WORD(chunk));
		--chunkID;
	}
	return chunk;
}

uint8_t getNeighborChunk(const uint8_t *chunk, uint8_t dir)
{
	if (chunk == NULL)
		return 0x3F;
	else if (dir == DIR_UP)
		return (uint8_t)((pgm_read_byte(chunk + 2) >> 2) & 0x3F);
	else if (dir == DIR_DOWN)
		return (uint8_t)((READ_WORD(chunk + 2) >> 4) & 0x3F);
	else if (dir == DIR_LEFT)
		return (uint8_t)((READ_WORD(chunk + 3) >> 6) & 0x3F);
	return pgm_read_byte(chunk + 4) & 0x3F;
}

bool canMove(const uint8_t *chunk, int8_t x, int8_t y, uint8_t direction)
{
	uint16_t tiles;
	uint8_t chunkID = player.chunkID;
	bool updateChunk = false;
	if (direction == DIR_UP)
		--y;
	else if (direction == DIR_DOWN)
		++y;
	else if (direction == DIR_LEFT)
		--x;
	else if (direction == DIR_RIGHT)
		++x;
	if (x == -1)
	{
		x = 7;
		updateChunk = true;
	}
	else if (x == 8)
	{
		x = 0;
		updateChunk = true;
	}
	if (y == -1)
	{
		y = 7;
		updateChunk = true;
	}
	else if (y == 8)
	{
		y = 0;
		updateChunk = true;
	}
	if (updateChunk)
	{
		chunkID = getNeighborChunk(chunk, direction);
		chunk = getChunk(pMap, chunkID);
	}
	if (chunk == NULL)
		return false;
	tiles = READ_WORD(chunk);
	return (((uint16_t)pgm_read_word(tilesetPassability + tilesetID) >> (15 - getChunkTileID(tiles, getChunkMetaTileID(chunk, x, y, getChunkBits(tiles)))))) & 1;
}

void drawChunk(const uint8_t *chunk, int16_t xOffset, int16_t yOffset)
{
  if (chunk == NULL)
  {
    for (uint8_t y = 0; y < 8; ++y)
    {
      for (uint8_t x = 0; x < 8; ++x)
        arduboy.drawBitmap(x * 16 + xOffset, y * 16 + yOffset, tileBitmaps + (tilesetID * 16) * 32, 16, 16);
    }
    return;
  }
  uint16_t tiles = READ_WORD(chunk);
  int8_t bitsPerTile = getChunkBits(tiles);
  int8_t tileID = getChunkTileID(tiles, 0);
  bitReaderInit(chunk + 5, 0);
  for (uint8_t y = 0; y < 8; ++y)
  {
    for (uint8_t x = 0; x < 8; ++x)
    {
      if (bitsPerTile != 0)
        tileID = getChunkTileID(tiles, bitReaderRead8(bitsPerTile));
      arduboy.drawBitmap(x * 16 + xOffset, y * 16 + yOffset, tileBitmaps + (tilesetID * 16 + tileID) * 32, 16, 16);
    }
  }
}

void drawChests(uint8_t chunkID, int16_t xOffset, int16_t yOffset)
{
	uint16_t loc;
	uint8_t num = pgm_read_byte(pMapEvents);
	uint8_t chestID;
	uint8_t x, y;
	for (uint8_t i = 0; i < num; ++i)
	{
		loc = READ_WORD(pMapEvents + i * 4 + 1);
		chestID = pgm_read_byte(pMapEvents + i * 4 + 3);
		if (((loc >> 9) & 0x3F) == chunkID && (loc & 7) == EVENT_LARGECHEST)
		{
			x = ((loc >> 6) & 0x7);
			y = ((loc >> 3) & 0x7);
			arduboy.fillRect(x * 16 + xOffset, y * 16 + yOffset, 16, 16, BLACK);
			arduboy.drawBitmap(x * 16 + xOffset, y * 16 + yOffset, tileBitmaps + LARGE_CHEST_TILE + ((player.largeChests & ((uint32_t)1 << chestID)) ? 32:0), 16, 16);
		}
	}
}

void drawMap()
{
	int16_t startX = player.x * -16 + 56 + playerXOff;
	int16_t startY = player.y * -16 + 24 + playerYOff;
	int16_t xOff, yOff;
	uint8_t starti = 0;
	uint8_t stopi = 5;
	if (player.y >= 5)
	{
		starti = 3;
		stopi = 8;
	}
	for (uint8_t i = starti; i <= stopi; ++i)
	{
		xOff = startX + (i % 3) * 128 - 128;
		yOff = startY + (i / 3) * 128 - 128;
		drawChunk(getChunk(pMap, chunkIDs[i]), xOff, yOff);
		drawChests(chunkIDs[i], xOff, yOff);
	}
	arduboy.drawBitmap(56, 24, playerMapSpriteMask + player.dir * 32, 16, 16, BLACK);
	arduboy.drawBitmap(56, 24, playerMapSprite + player.dir * 32, 16, 16);
}


int8_t getTileID(const uint8_t *chunk, int8_t x, int8_t y)
{
	uint16_t tiles = READ_WORD(chunk);
	return getChunkTileID(tiles, getChunkMetaTileID(chunk, x, y, getChunkBits(tiles)));
}

void loadChunkIDs()
{
	const uint8_t *subChunk;
	chunkIDs[1] = getNeighborChunk(pChunk, DIR_UP);
	chunkIDs[3] = getNeighborChunk(pChunk, DIR_LEFT);
	chunkIDs[4] = player.chunkID;
	chunkIDs[5] = getNeighborChunk(pChunk, DIR_RIGHT);
	chunkIDs[7] = getNeighborChunk(pChunk, DIR_DOWN);
	subChunk = getChunk(pMap, chunkIDs[1]);
	chunkIDs[0] = getNeighborChunk(subChunk, DIR_LEFT);
	chunkIDs[2] = getNeighborChunk(subChunk, DIR_RIGHT);
	subChunk = getChunk(pMap, chunkIDs[7]);
	chunkIDs[6] = getNeighborChunk(subChunk, DIR_LEFT);
	chunkIDs[8] = getNeighborChunk(subChunk, DIR_RIGHT);
	subChunk = getChunk(pMap, chunkIDs[3]);
	chunkIDs[0] &= getNeighborChunk(subChunk, DIR_UP);
	chunkIDs[6] &= getNeighborChunk(subChunk, DIR_DOWN);
	subChunk = getChunk(pMap, chunkIDs[5]);
	chunkIDs[2] &= getNeighborChunk(subChunk, DIR_UP);
	chunkIDs[8] &= getNeighborChunk(subChunk, DIR_DOWN);
}

void loadMap()
{
	pMap = (uint8_t*)pgm_read_dword(maps+player.map);
	pMapEvents = (uint8_t*)pgm_read_dword(mapEvents+player.map);
	tilesetID = getMapTileset(pMap);
	pChunk = getChunk(pMap, player.chunkID);
	loadChunkIDs();
}

#endif //MAPS_H
