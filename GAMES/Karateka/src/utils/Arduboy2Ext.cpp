#include "Arduboy2Ext.h"

Arduboy2Ext::Arduboy2Ext() : Arduboy2Base() { }

/*uint8_t Arduboy2Ext::curButtonState() const {

  return currentButtonState;

}
*/
struct BitStreamReader {

  const uint8_t *source;
  uint16_t sourceIndex;
  uint8_t bitBuffer;
  uint8_t byteBuffer;
  
  BitStreamReader(const uint8_t *source)
    : source(source), sourceIndex(), bitBuffer(), byteBuffer()
  {
  }
  
  uint16_t readBits(uint16_t bitCount)
  {
    uint16_t result = 0;
    for (uint16_t i = 0; i < bitCount; i++)
    {
      if (this->bitBuffer == 0)
      {
        this->bitBuffer = 0x1;
        this->byteBuffer = pgm_read_byte(&this->source[this->sourceIndex]);
        ++this->sourceIndex;
      }
      
      if ((this->byteBuffer & this->bitBuffer) != 0)
        result |= (1 << i); // result |= bitshift_left[i];
        
      this->bitBuffer <<= 1;
    }
    return result;
  }
  
};


void Arduboy2Ext::drawCompressedMirror(int16_t sx, int16_t sy, const uint8_t *bitmap, uint8_t color, bool mirror)
{
  // set up decompress state

  BitStreamReader cs = BitStreamReader(bitmap);

  // read header

  int width = (int)cs.readBits(8) + 1;
  int height = (int)cs.readBits(8) + 1;
  uint8_t spanColour = (uint8_t)cs.readBits(1); // starting colour

  // no need to draw at all if we're offscreen
  if ((sx + width < 0) || (sx > WIDTH - 1) || (sy + height < 0) || (sy > HEIGHT - 1))
    return;

  // sy = sy - (frame * height);

  int yOffset = abs(sy) % 8;
  int startRow = sy / 8;
  if (sy < 0) {
    startRow--;
    yOffset = 8 - yOffset;
  }
  int rows = height / 8;
  if ((height % 8) != 0)
    ++rows;

  int rowOffset = 0; // +(frame*rows);
  int columnOffset = 0;
  
  if (mirror) {
    columnOffset = width - 1;
  }
  else {
    columnOffset = 0;
  }

  uint8_t byte = 0x00;
  uint8_t bit = 0x01;
  while (rowOffset < rows) // + (frame*rows))
  {
    uint16_t bitLength = 1;
    while (cs.readBits(1) == 0)
      bitLength += 2;

    uint16_t len = cs.readBits(bitLength) + 1; // span length

    // draw the span
    for (uint16_t i = 0; i < len; ++i)
    {
      if (spanColour != 0)
        byte |= bit;
      bit <<= 1;

      if (bit == 0) // reached end of byte
      {
        // draw
        int bRow = startRow + rowOffset;

        //if (byte) // possible optimisation
        if ((bRow <= (HEIGHT / 8) - 1) && (bRow > -2) && (columnOffset + sx <= (WIDTH - 1)) && (columnOffset + sx >= 0))
        {
          int16_t offset = (bRow * WIDTH) + sx + columnOffset;
          if (bRow >= 0)
          {
            int16_t index = offset;
            uint8_t value = byte << yOffset;
            
            if (color != 0)
              sBuffer[index] |= value;
            else
              sBuffer[index] &= ~value;
          }
          if ((yOffset != 0) && (bRow < (HEIGHT / 8) - 1))
          {
            int16_t index = offset + WIDTH;
            uint8_t value = byte >> (8 - yOffset);
            
            if (color != 0)
              sBuffer[index] |= value;
            else
              sBuffer[index] &= ~value;
          }
        }


        // iterate
        if (!mirror) {
          ++columnOffset;
          if (columnOffset >= width)
          {
            columnOffset = 0;
            ++rowOffset;
          }
        }
        else {
          --columnOffset;
          if (columnOffset < 0) {
            columnOffset = width - 1;
            ++rowOffset;
          }
        }


        // reset byte
        byte = 0x00;
        bit = 0x01;
      }
    }

    spanColour ^= 0x01; // toggle colour bit (bit 0) for next span
  }
}

