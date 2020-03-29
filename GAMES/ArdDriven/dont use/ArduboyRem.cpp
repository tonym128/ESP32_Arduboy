#include "ArduboyRem.h"
//#include "glcdfont.c"
#include "pics/font.h"

//========================================
//========== class ArduboyBaseRem ==========
//========================================

uint8_t ArduboyBaseRem::sBuffer[];

ArduboyBaseRem::ArduboyBaseRem()
{
	currentButtonState = 0;
	previousButtonState = 0;
	nextFrameStart = 0;
}

void ArduboyBaseRem::flashlight()
{
	if (!pressed(UP_BUTTON)) {
		return;
	}

	sendLCDCommand(OLED_ALL_PIXELS_ON); // smaller than allPixelsOn()
	digitalWriteRGB(RGB_ON, RGB_ON, RGB_ON);

	while (!pressed(DOWN_BUTTON)) {
		idle();
	}

	digitalWriteRGB(RGB_OFF, RGB_OFF, RGB_OFF);
	sendLCDCommand(OLED_PIXELS_FROM_RAM);
}

void ArduboyBaseRem::systemButtons() {
	while (pressed(B_BUTTON)) {
		digitalWrite(BLUE_LED, RGB_ON); // turn on blue LED
		sysCtrlSound(UP_BUTTON + B_BUTTON, GREEN_LED, 0xff);
		sysCtrlSound(DOWN_BUTTON + B_BUTTON, RED_LED, 0);
		delay(200);
	}

	digitalWrite(BLUE_LED, RGB_OFF); // turn off blue LED
}

void ArduboyBaseRem::sysCtrlSound(uint8_t buttons, uint8_t led, uint8_t eeVal) {
	if (pressed(buttons)) {
		digitalWrite(BLUE_LED, RGB_OFF); // turn off blue LED
		delay(200);
		digitalWrite(led, RGB_ON); // turn on "acknowledge" LED
		EEPROM.update(EEPROM_AUDIO_ON_OFF, eeVal);
		delay(500);
		digitalWrite(led, RGB_OFF); // turn off "acknowledge" LED

		while (pressed(buttons)) {} // Wait for button release
	}
}

/* Frame management */

void ArduboyBaseRem::setFrameRate(uint16_t rate)
{
	eachFrameMicros = rate;
}

void ArduboyBaseRem::nextFrame()
{
	do {
		uint16_t now = micros();

		// if it's not time for the next frame yet
		int16_t diff = nextFrameStart - now;
		if (diff < 0) {
			// if we have more than 1ms to spare, lets sleep
			// we should be woken up by timer0 every 1ms, so this should be ok
			if (diff >= 1000)
				idle();
			continue;
		}

		nextFrameStart += eachFrameMicros;
		flicker++;
		return;
	} while (true);
}

void ArduboyBaseRem::initRandomSeed()
{
	power_adc_enable(); // ADC on
	randomSeed(~rawADC(ADC_TEMP) * ~rawADC(ADC_VOLTAGE) * ~micros() + micros());
	power_adc_disable(); // ADC off
}

uint16_t ArduboyBaseRem::rawADC(uint8_t adc_bits)
{
	ADMUX = adc_bits;
	// we also need MUX5 for temperature check
	if (adc_bits == ADC_TEMP) {
		ADCSRB = _BV(MUX5);
	}

	delay(2); // Wait for ADMUX setting to settle
	ADCSRA |= _BV(ADSC); // Start conversion
	while (bit_is_set(ADCSRA, ADSC)); // measuring

	return ADC;
}

/* Graphics */

void ArduboyBaseRem::clear()
{
	fillScreen(BLACK);
}

void ArduboyBaseRem::drawPixel(int16_t x, int16_t y, uint8_t color) const
{
#ifdef PIXEL_SAFE_MODE
	if (x < 0 || x >(WIDTH - 1) || y < 0 || y >(HEIGHT - 1))
	{
		return;
	}
#endif

	uint8_t row = (uint8_t)y / 8;
	if (color)
	{
		sBuffer[(row*WIDTH) + (uint8_t)x] |= _BV((uint8_t)y % 8);
	}
	else
	{
		sBuffer[(row*WIDTH) + (uint8_t)x] &= ~_BV((uint8_t)y % 8);
	}
}

uint8_t ArduboyBaseRem::getPixel(uint8_t x, uint8_t y) const
{
	uint8_t row = y / 8;
	uint8_t bit_position = y % 8;
	return (sBuffer[(row*WIDTH) + x] & _BV(bit_position)) >> bit_position;
}

void ArduboyBaseRem::drawCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0, y0 + r, color);
	drawPixel(x0, y0 - r, color);
	drawPixel(x0 + r, y0, color);
	drawPixel(x0 - r, y0, color);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y, color);
		drawPixel(x0 - x, y0 + y, color);
		drawPixel(x0 + x, y0 - y, color);
		drawPixel(x0 - x, y0 - y, color);
		drawPixel(x0 + y, y0 + x, color);
		drawPixel(x0 - y, y0 + x, color);
		drawPixel(x0 + y, y0 - x, color);
		drawPixel(x0 - y, y0 - x, color);
	}
}

void ArduboyBaseRem::drawCircleHelper
(int16_t x0, int16_t y0, uint8_t r, uint8_t cornername, uint8_t color)
{
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x4)
		{
			drawPixel(x0 + x, y0 + y, color);
			drawPixel(x0 + y, y0 + x, color);
		}
		if (cornername & 0x2)
		{
			drawPixel(x0 + x, y0 - y, color);
			drawPixel(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8)
		{
			drawPixel(x0 - y, y0 + x, color);
			drawPixel(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1)
		{
			drawPixel(x0 - y, y0 - x, color);
			drawPixel(x0 - x, y0 - y, color);
		}
	}
}

void ArduboyBaseRem::fillCircle(int16_t x0, int16_t y0, uint8_t r, uint8_t color)
{
	drawFastVLine(x0, y0 - r, 2 * r + 1, color);
	fillCircleHelper(x0, y0, r, 3, 0, color);
}

void ArduboyBaseRem::fillCircleHelper
(int16_t x0, int16_t y0, uint8_t r, uint8_t cornername, int16_t delta,
	uint8_t color)
{
	// used to do circles and roundrects!
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}

		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1)
		{
			drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
		}

		if (cornername & 0x2)
		{
			drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
			drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
		}
	}
}

void ArduboyBaseRem::drawLine
(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color)
{
	// bresenham's algorithm - thx wikpedia
	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int8_t ystep;

	if (y0 < y1)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	for (; x0 <= x1; x0++)
	{
		if (steep)
		{
			drawPixel(y0, x0, color);
		}
		else
		{
			drawPixel(x0, y0, color);
		}

		err -= dy;
		if (err < 0)
		{
			y0 += ystep;
			err += dx;
		}
	}
}

void ArduboyBaseRem::drawRect
(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
	drawFastHLine(x, y, w, color);
	drawFastHLine(x, y + h - 1, w, color);
	drawFastVLine(x, y, h, color);
	drawFastVLine(x + w - 1, y, h, color);
}

void ArduboyBaseRem::drawFastVLine
(int16_t x, int16_t y, uint8_t h, uint8_t color)
{
	int end = y + h;
	for (int a = max(0, y); a < min(end, HEIGHT); a++)
	{
		drawPixel(x, a, color);
	}
}

void ArduboyBaseRem::drawFastHLine(int16_t x, uint8_t y, int16_t w, uint8_t color)
{
	// Do y bounds checks
	if (y >= HEIGHT)
		return;

	int16_t xEnd = x + w; // last x point + 1

	// Check if the entire line is not on the display
	if (xEnd <= 0 || x >= WIDTH || w <= 0)
		return;

	// clip left edge
	if (x < 0)
		x = 0;

	// clip right edge
	if (xEnd > WIDTH)
		xEnd = WIDTH;

	// calculate final clipped width (even if unchanged)
	uint8_t clippedW = xEnd - x;

	// buffer pointer plus row offset + x offset
	// (y >> 3) * WIDTH, optimized to: ((y * 16) & 0xFF80) because gcc O3 is not smart enough to do it.
	uint8_t *pBuf = sBuffer + ((y * 16) & 0xFF80) + uint8_t(x);

	// pixel mask
	//uint8_t mask = 1 << (y & 7);
	uint8_t mask = pgm_read_byte(yMask + (y & 7));

	switch (color)
	{
	case WHITE:
		do
		{
			*pBuf++ |= mask;
		} while (--clippedW);
		break;

	case BLACK:
		mask = ~mask;
		do
		{
			*pBuf++ &= mask;
		} while (--clippedW);
		break;
	/* // GRAY is disabled because unused to remove useless code:
	case GRAY:
	{
		// TOOD: This could be optimized further, but unneeded since we clear display in gray now
		uint8_t alternate = (x + y + flicker) & 1;
		while (clippedW--)
		{
			if (alternate) {
				*pBuf |= mask;
			}
			pBuf++;
			alternate ^= 1;
		}
		break;
	}
	*/
	}
}

void ArduboyBaseRem::fillRect
(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t color)
{
	// stupidest version - update in subclasses if desired!
	for (int16_t i = x; i < x + w; i++)
	{
		drawFastVLine(i, y, h, color);
	}
}

void ArduboyBaseRem::fillScreen(uint8_t color)
{
	// C version :
	//
	// if (color) color = 0xFF;  //change any nonzero argument to b11111111 and insert into screen array.
	// for(int16_t i=0; i<1024; i++)  { sBuffer[i] = color; }  //sBuffer = (128*64) = 8192/8 = 1024 bytes.

	asm volatile
		(
			// load color value into r27
			"mov r27, %1 \n\t"
			// if value is zero, skip assigning to 0xff
			"cpse r27, __zero_reg__ \n\t"
			"ldi r27, 0xff \n\t"
			// load sBuffer pointer into Z
			"movw  r30, %0\n\t"
			// counter = 0
			"clr __tmp_reg__ \n\t"
			"loopto:   \n\t"
			// (4x) push zero into screen buffer,
			// then increment buffer position
			"st Z+, r27 \n\t"
			"st Z+, r27 \n\t"
			"st Z+, r27 \n\t"
			"st Z+, r27 \n\t"
			// increase counter
			"inc __tmp_reg__ \n\t"
			// repeat for 256 loops
			// (until counter rolls over back to 0)
			"brne loopto \n\t"
			// input: sBuffer, color
			// modified: Z (r30, r31), r27
			:
	: "r" (sBuffer), "r" (color)
		: "r30", "r31", "r27"
		);
}

void ArduboyBaseRem::drawRoundRect
(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
	// smarter version
	drawFastHLine(x + r, y, w - 2 * r, color); // Top
	drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
	drawFastVLine(x, y + r, h - 2 * r, color); // Left
	drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
	// draw four corners
	drawCircleHelper(x + r, y + r, r, 1, color);
	drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
	drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
	drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

void ArduboyBaseRem::fillRoundRect
(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t r, uint8_t color)
{
	// smarter version
	fillRect(x + r, y, w - 2 * r, h, color);

	// draw four corners
	fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
	fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

void ArduboyBaseRem::drawTriangle
(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

void ArduboyBaseRem::fillTriangle
(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color)
{

	int16_t a, b, y, last;
	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if (y0 > y1)
	{
		swap(y0, y1); swap(x0, x1);
	}
	if (y1 > y2)
	{
		swap(y2, y1); swap(x2, x1);
	}
	if (y0 > y1)
	{
		swap(y0, y1); swap(x0, x1);
	}

	if (y0 == y2)
	{ // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		if (x1 < a)
		{
			a = x1;
		}
		else if (x1 > b)
		{
			b = x1;
		}
		if (x2 < a)
		{
			a = x2;
		}
		else if (x2 > b)
		{
			b = x2;
		}
		drawFastHLine(a, y0, b - a + 1, color);
		return;
	}

	int16_t dx01 = x1 - x0,
		dy01 = y1 - y0,
		dx02 = x2 - x0,
		dy02 = y2 - y0,
		dx12 = x2 - x1,
		dy12 = y2 - y1,
		sa = 0,
		sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if (y1 == y2)
	{
		last = y1;   // Include y1 scanline
	}
	else
	{
		last = y1 - 1; // Skip it
	}


	for (y = y0; y <= last; y++)
	{
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;

		if (a > b)
		{
			swap(a, b);
		}

		drawFastHLine(a, y, b - a + 1, color);
	}

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);

	for (; y <= y2; y++)
	{
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;

		if (a > b)
		{
			swap(a, b);
		}

		drawFastHLine(a, y, b - a + 1, color);
	}
}
/*
void ArduboyBaseRem::drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h)
{
	// no need to draw at all if we're offscreen
	if (x + w < 0 || x >= WIDTH || y + h < 0 || y >= HEIGHT)
		return;

	int yOffset = abs(y) % 8;
	int sRow = y / 8;
	if (y < 0) {
		sRow--;
		yOffset = 8 - yOffset;
	}
	int rows = h / 8;
	if (h % 8 != 0) rows++;
	for (int a = 0; a < rows; a++) {
		int bRow = sRow + a;
		if (bRow > (HEIGHT / 8) - 1) break;
		if (bRow > -2) {
			for (int iCol = 0; iCol < w; iCol++) {
				if (iCol + x > (WIDTH - 1)) break;
				if (iCol + x >= 0) {
					uint8_t sourceByte = pgm_read_byte(bitmap + (a*w) + iCol);

					if (bRow >= 0) {
						sBuffer[(bRow*WIDTH) + x + iCol] |= sourceByte << yOffset;
					}
					if (yOffset && bRow<(HEIGHT / 8) - 1 && bRow > -2) {
						sBuffer[((bRow + 1)*WIDTH) + x + iCol] |= sourceByte >> (8 - yOffset);
					}
				}
			}
		}
	}
}
*/

#define pgm_read_byte_and_increment(addr)   \
(__extension__({                \
    uint16_t __addr16 = (uint16_t)(addr); \
    uint8_t __result;           \
    __asm__ __volatile__        \
    (                           \
        "lpm %0, Z+" "\n\t"            \
        : "=r" (__result)       \
        : "z" (__addr16)        \
        : "r0"                  \
    );                          \
    __result;                   \
}))

void ArduboyBaseRem::drawMaskBitmap(int8_t x, int8_t y, const uint8_t *bitmap, uint8_t hflip)
{
	// Read size
	uint8_t w = pgm_read_byte(bitmap++);
	uint8_t h = pgm_read_byte(bitmap++);
	// Read hotspot
	int8_t hx = pgm_read_byte(bitmap++);
	int8_t hy = pgm_read_byte(bitmap++);

	y -= hy;
	if (hflip == 0) {
		x -= hx;
	} else {
		x -= w - hx;
	}

	// no need to draw at all if we're offscreen
	int8_t endX = x + w;
	if ((endX < 0 && x < 0)/* || x >= WIDTH*/ || int8_t(y + h) < 0 || y >= HEIGHT)
		return;

	//uint8_t yOffset = 1 << (y & 7);
	uint8_t yOffset = pgm_read_byte(yMask + (y & 7));
	const uint8_t* sourceAddress = bitmap;

	int8_t sRow = y >> 3;
	uint8_t uRow = sRow; // Same as 'sRow' but unsigned: will be used after clipping
	uint8_t rows = (h + 7) >> 3;
	// Top clipping: skip 'n' rows if outside the screen
	if (sRow < 0) {
		sourceAddress += (-sRow * w) << 1;
		rows += sRow;
		uRow = 0;
	}
	// Bottom clipping: stop as soon as we reach bottom of screen
	if (uRow + rows > (HEIGHT >> 3)) {
		rows = (HEIGHT >> 3) - uRow;
	}
	// Optimization: Perform 'x' clipping outside of loop
	uint8_t startCol = 0;
	uint8_t endCol = w;
	if (x < 0) {
		startCol = -x;
	}
	if (x + w > WIDTH) {
		endCol = WIDTH - x;
	}

	if (hflip == 0) {
		sourceAddress += (startCol << 1);
	} else {
		sourceAddress += ((w - endCol) << 1);
	}
	uint8_t sourceAddressSkip = (w - (endCol - startCol)) << 1;

	if (hflip == 0) {
		uint16_t destAddress = (uRow * WIDTH) + x + startCol;
		uint8_t destAddressSkip = (WIDTH - (endCol - startCol));

		for (uint8_t bRow = uRow; bRow < uint8_t(uRow + rows); bRow++, sourceAddress += sourceAddressSkip, destAddress += destAddressSkip) {
			for (uint8_t iCol = startCol; iCol < endCol; iCol++, destAddress ++) {
				// Draw mask first with AND and Draw color with OR:
				// The GCC compiler is doing a poor job because of its constraint to keep 'r1' always 0 (two useless clr __zero_reg__ here)
				// and also because it can't accept to keep the last 'mul' result in R1:R0, so there's a useless 'movw'. Total: 3 wasted clock cycles in this loop
				uint16_t sourceMask = ~(pgm_read_byte_and_increment(sourceAddress) * yOffset);
				uint16_t sourceByte = pgm_read_byte_and_increment(sourceAddress) * yOffset;

				sBuffer[destAddress] = (sBuffer[destAddress] & uint8_t(sourceMask)) | uint8_t(sourceByte);
				sBuffer[destAddress + WIDTH] = (sBuffer[destAddress + WIDTH] & uint8_t(sourceMask >> 8)) | uint8_t(sourceByte >> 8);
			}
		}
	}
	else {
		uint16_t destAddress = (uRow * WIDTH) + x + endCol - 1;
		uint8_t destAddressSkip = (WIDTH + (endCol - startCol));

		for (uint8_t bRow = uRow; bRow < uint8_t(uRow + rows); bRow++, sourceAddress += sourceAddressSkip, destAddress += destAddressSkip) {
			for (uint8_t iCol = startCol; iCol < endCol; iCol++, destAddress--) {
				uint16_t sourceMask = ~(pgm_read_byte_and_increment(sourceAddress) * yOffset);
				uint16_t sourceByte = pgm_read_byte_and_increment(sourceAddress) * yOffset;

				sBuffer[destAddress] = (sBuffer[destAddress] & uint8_t(sourceMask)) | uint8_t(sourceByte);
				sBuffer[destAddress + WIDTH] = (sBuffer[destAddress + WIDTH] & uint8_t(sourceMask >> 8)) | uint8_t(sourceByte >> 8);
			}
		}
	}
}
/*
void ArduboyBaseRem::drawTurboBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h)
{
	// no need to draw at all if we're offscreen
	if (x + w < 0 || x >= WIDTH || y + h < 0 || y >= HEIGHT)
		return;

	int yOffset = abs(y) % 8;
	int sRow = y / 8;
	if (y < 0) {
		sRow--;
		yOffset = 8 - yOffset;
	}
	int rows = h / 8;
	if (h % 8 != 0) rows++;
	for (int a = 0; a < rows; a++) {
		int bRow = sRow + a;
		if (bRow > (HEIGHT / 8) - 1) break;
		if (bRow > -2) {
			for (int iCol = 0; iCol < w; iCol++) {
				if (iCol + x > (WIDTH - 1)) break;
				if (iCol + x >= 0) {
					if (bRow >= 0) {
						sBuffer[(bRow*WIDTH) + x + iCol] |= pgm_read_byte(bitmap + (a*w) + iCol) << yOffset;
						//sBuffer[(bRow*WIDTH) + x + iCol] &= ~(pgm_read_byte(bitmap+(a*w)+iCol) << yOffset);
					}
					if (yOffset && bRow<(HEIGHT / 8) - 1 && bRow > -2) {
						sBuffer[((bRow + 1)*WIDTH) + x + iCol] |= pgm_read_byte(bitmap + (a*w) + iCol) >> (8 - yOffset);
						//sBuffer[((bRow+1)*WIDTH) + x + iCol] &= ~(pgm_read_byte(bitmap+(a*w)+iCol) >> (8-yOffset));
					}
				}
			}
		}
	}
}

void ArduboyBaseRem::drawGrayBitmap(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h)
{
	// no need to draw at all if we're offscreen
	if (x + w < 0 || x >= WIDTH || y + h < 0 || y >= HEIGHT)
		return;

	uint8_t rows = h >> 2;

	for (uint8_t iCol = 0; iCol < w; iCol++) {
		if (iCol + x >= WIDTH) break;
		if (iCol + x < 0) continue;

		uint8_t page = y >> 2; // page has 1-bit of 'precision' that needs to shifted out

		for (uint8_t iRow = 0; iRow < rows; iRow++) {
			uint8_t b = pgm_read_byte(bitmap + iRow + (iCol * rows));
			uint16_t outputOffset = (((page + iRow) >> 1) << 7) + x + iCol;
			uint8_t screen = sBuffer[outputOffset];

			for (uint8_t p = 0; p < 4; p++) {
				uint8_t bit = (1 << (p + ((iRow & 1) << 2)));
				// Gray is replaced with alternating black/white
				uint8_t color = b & 3;
				if (color == 2) {
					color = (iCol + p + flicker) & 1 ? 0 : 3;
				}
				switch (color) {
				case 0: // black
					screen &= ~bit;
					break;
				case 1: // transparent
					break;
				case 3: // white
					screen |= bit;
					break;
				}
				b >>= 2;
			}
			sBuffer[outputOffset] = screen;

			//sBuffer[(bRow*WIDTH) + x + iCol] &= ~(pgm_read_byte(bitmap+(a*w)+iCol) << yOffset);
		}
	}
}

void ArduboyBaseRem::drawSlowXYBitmap
(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t w, uint8_t h, uint8_t color)
{
	// no need to draw at all of we're offscreen
	if (x + w < 0 || x > WIDTH - 1 || y + h < 0 || y > HEIGHT - 1)
		return;

	int16_t xi, yi, byteWidth = (w + 7) / 8;
	for (yi = 0; yi < h; yi++) {
		for (xi = 0; xi < w; xi++) {
			if (pgm_read_byte(bitmap + yi * byteWidth + xi / 8) & (128 >> (xi & 7))) {
				drawPixel(x + xi, y + yi, color);
			}
		}
	}
}

typedef struct CSESSION {
	int byte;
	int bit;
	const uint8_t *src;
	int src_pos;
} CSESSION;
static CSESSION cs;

static int getval(int bits)
{
	int val = 0;
	int i;
	for (i = 0; i < bits; i++)
	{
		if (cs.bit == 0x100)
		{
			cs.bit = 0x1;
			cs.byte = pgm_read_byte(&cs.src[cs.src_pos]);
			cs.src_pos++;
		}
		if (cs.byte & cs.bit)
			val += (1 << i);
		cs.bit <<= 1;
	}
	return val;
}

void ArduboyBaseRem::drawCompressed(int16_t sx, int16_t sy, const uint8_t *bitmap, uint8_t color)
{
	int bl, len;
	int col;
	int i;
	int a, iCol;
	int byte = 0;
	int bit = 0;
	int w, h;

	// set up decompress state

	cs.src = bitmap;
	cs.bit = 0x100;
	cs.byte = 0;
	cs.src_pos = 0;

	// read header

	w = getval(8) + 1;
	h = getval(8) + 1;

	col = getval(1); // starting colour

	// no need to draw at all if we're offscreen
	if (sx + w < 0 || sx > WIDTH - 1 || sy + h < 0 || sy > HEIGHT - 1)
		return;

	// sy = sy - (frame*h);

	int yOffset = abs(sy) % 8;
	int sRow = sy / 8;
	if (sy < 0) {
		sRow--;
		yOffset = 8 - yOffset;
	}
	int rows = h / 8;
	if (h % 8 != 0) rows++;

	a = 0; // +(frame*rows);
	iCol = 0;

	byte = 0; bit = 1;
	while (a < rows) // + (frame*rows))
	{
		bl = 1;
		while (!getval(1))
			bl += 2;

		len = getval(bl) + 1; // span length

		// draw the span


		for (i = 0; i < len; i++)
		{
			if (col)
				byte |= bit;
			bit <<= 1;

			if (bit == 0x100) // reached end of byte
			{
				// draw

				int bRow = sRow + a;

				//if (byte) // possible optimisation
				if (bRow <= (HEIGHT / 8) - 1)
					if (bRow > -2)
						if (iCol + sx <= (WIDTH - 1))
							if (iCol + sx >= 0) {

								if (bRow >= 0)
								{
									if (color)
										sBuffer[(bRow * WIDTH) + sx + iCol] |= byte << yOffset;
									else
										sBuffer[(bRow * WIDTH) + sx + iCol] &= ~(byte << yOffset);
								}
								if (yOffset && bRow < (HEIGHT / 8) - 1 && bRow > -2)
								{
									if (color)
										sBuffer[((bRow + 1)*WIDTH) + sx + iCol] |= byte >> (8 - yOffset);
									else
										sBuffer[((bRow + 1)*WIDTH) + sx + iCol] &= ~(byte >> (8 - yOffset));
								}
							}

				// iterate
				iCol++;
				if (iCol >= w)
				{
					iCol = 0;
					a++;
				}

				// reset byte
				byte = 0; bit = 1;
			}
		}

		col = 1 - col; // toggle colour for next span
	}
}
*/
void ArduboyBaseRem::display()
{
	paintScreen(sBuffer);
}

unsigned char* ArduboyBaseRem::getBuffer()
{
	return sBuffer;
}

bool ArduboyBaseRem::pressed(uint8_t buttons)
{
	return (buttonsState() & buttons) == buttons;
}

bool ArduboyBaseRem::notPressed(uint8_t buttons)
{
	return (buttonsState() & buttons) == 0;
}

void ArduboyBaseRem::pollButtons()
{
	previousButtonState = currentButtonState;
	currentButtonState = buttonsState();
}

bool ArduboyBaseRem::justPressed(uint8_t button)
{
	return (!(previousButtonState & button) && (currentButtonState & button));
}

bool ArduboyBaseRem::justReleased(uint8_t button)
{
	return ((previousButtonState & button) && !(currentButtonState & button));
}

bool ArduboyBaseRem::collide(Point point, Rect rect)
{
	return ((point.x >= rect.x) && (point.x < rect.x + rect.width) &&
		(point.y >= rect.y) && (point.y < rect.y + rect.height));
}

bool ArduboyBaseRem::collide(Rect rect1, Rect rect2)
{
	return !(rect2.x >= rect1.x + rect1.width ||
		rect2.x + rect2.width <= rect1.x ||
		rect2.y >= rect1.y + rect1.height ||
		rect2.y + rect2.height <= rect1.y);
}

void ArduboyBaseRem::swap(int16_t& a, int16_t& b)
{
	int16_t temp = a;
	a = b;
	b = temp;
}


//====================================
//========== class ArduboyRem ==========
//====================================

ArduboyRem::ArduboyRem()
{
	cursor_x = 0;
	cursor_y = 0;
	textColor = 1;
	textBackground = 0;
	textSize = 1;
	textWrap = 0;
}

size_t ArduboyRem::write(uint8_t c)
{
	if (c == '\n')
	{
		cursor_y += textSize * 8;
		cursor_x = 0;
	}
	else if (c == '\r')
	{
		// skip em
	}
	else
	{
		drawChar(cursor_x, cursor_y, c);
		cursor_x += textSize * 6;
		if (textWrap && (cursor_x > (WIDTH - textSize * 6)))
		{
			// calling ourselves recursively for 'newline' is
			// 12 bytes smaller than doing the same math here
			write('\n');
		}
	}
	return 1;
}

void ArduboyRem::drawChar(uint8_t x, uint8_t y, unsigned char c) const
{
	// RV optimization: get rid of 'size' scaling
	// Also remove transparent check
	// Force alignment on full byte in 'y'

	if ((x >= WIDTH) || (y >= HEIGHT))
	{
		return;
	}

	y >>= 3;

	for (uint8_t i = 0; i < 6; i++)
	{
		// use '~' to draw black on white text if font was created white on black
		// Note: Font has been changed to 6x8, at starts at character 32 instead of 0, and ends a char 127 (instead of 255)
		// so it occupies 6 bytes x 96 chars = 576 bytes (instead of the original arduboy font of 5 bytes x 256 chars = 1280 bytes)
		drawByte(x + i, y, pgm_read_byte(font + ((c - 32) * 6) + i));
	}
	//drawByte(x + 5, y, 0);
}

void ArduboyRem::printBytePadded(uint8_t x, uint8_t y, byte num) const
{
	int ten = num / 10;
	int unit = num % 10;
	drawChar(x, y, '0' + ten);
	drawChar(x + 6, y, '0' + unit);
}


void ArduboyRem::setCursor(uint8_t x, uint8_t y)
{
	cursor_x = x;
	cursor_y = y;
}

uint8_t ArduboyRem::getCursorX() const {
	return cursor_x;
}

uint8_t ArduboyRem::getCursorY() const {
	return cursor_y;
}

void ArduboyRem::setTextColor(uint8_t color)
{
	textColor = color;
}

void ArduboyRem::setTextBackground(uint8_t bg)
{
	textBackground = bg;
}

void ArduboyRem::setTextSize(uint8_t s)
{
	// size must always be 1 or higher
	textSize = max(1, s);
}

void ArduboyRem::setTextWrap(bool w)
{
	textWrap = w;
}

void ArduboyRem::clear() {
	ArduboyBaseRem::clear();
	cursor_x = cursor_y = 0;
}

