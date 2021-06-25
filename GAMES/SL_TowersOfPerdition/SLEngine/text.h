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

#ifndef TEXT_H
#define TEXT_H

#define MESSAGE_BUFFER_SIZE	255

static const uint8_t *huffmanNode = huffmanNodes;

uint8_t messageBuffer[MESSAGE_BUFFER_SIZE];
uint8_t bufferCursor = 0;

uint8_t nextCharInCompressedMessage()
{
	uint8_t ch;
	do
	{
		huffmanNode += pgm_read_byte(huffmanNode + bitReaderRead8(1)) & 127;//globalBitReader/.read8(1)) & 127;
		ch = pgm_read_byte(huffmanNode);
	} while (ch & 128);
	huffmanNode = huffmanNodes;
	return ch;
}

void drawChar(uint8_t ch)
{
	if (ch == CHAR_NULL)
		return;
	else if (flags & FLAG_USE_MESSAGE_BUFFER)
	{
		messageBuffer[bufferCursor] = ch;
		++bufferCursor;
		return;
	}
	else if (ch == CHAR_LINE)
	{
		cursorX = lineStartX;
		cursorY += 8;
		return;
	}
	else if (ch == CHAR_NONE)
		arduboy.drawBitmap(cursorX, cursorY, font_ + (CHAR_NONE - 1) * 5, 5, 8, INVERT);
	else if (ch > 0)
		arduboy.drawBitmap(cursorX, cursorY, font_ + (ch - 1) * 5, 5, 8, INVERT);
	cursorX += 6;
}

void drawNumber(int16_t number)
{
	bool drawMinus = false;
	if (number == 0)
		drawChar(CHAR_0);
	if (number < 0)
	{
		drawMinus = true;
		number *= -1;
	}
	while (number > 0)
	{
		drawChar((number % 10) + CHAR_0);
		cursorX -= 12;
		number /= 10;
	}
	if (drawMinus)
		drawChar(CHAR_DASH);
}

void initMessageRead(const uint8_t *sentence, uint8_t &chars)
{
	bitReaderInit(sentence, 0);
	if (chars == 0)
		chars = bitReaderRead8(8);
}

void drawName(const uint8_t *name, uint8_t numChars)
{
	for (uint8_t i = 0; i < numChars; ++i)
		drawChar(pgm_read_byte(name + i));
}

void drawMessageCompressed(const uint8_t *sentence, uint8_t chars=0)
{
	uint8_t numChars = chars;
	initMessageRead(sentence, numChars);
	do
	{
		drawChar(nextCharInCompressedMessage());
		--numChars;
	} while(numChars > 0);
}

void fillMessageBuffer()
{
	for (uint8_t i = bufferCursor;i < MESSAGE_BUFFER_SIZE; ++i)
		messageBuffer[i] = CHAR_NULL;
	bufferCursor = 0;
	flags &= ~FLAG_USE_MESSAGE_BUFFER;
}

void drawSmallMessageBox()
{
	for (int16_t i = 896; i < 1024; ++i)
		arduboy.sBuffer[i] = 0x00;
}

void drawMessageBox()
{
	for (int16_t i = 768; i < 1024; ++i)
		arduboy.sBuffer[i] = 0x00;
}

#endif //TEXT_H
