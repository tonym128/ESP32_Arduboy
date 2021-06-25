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

#ifndef MUSIC_H
#define MUSIC_H

#define SONG_TITLE			0
#define SONG_OVERWORLD	1
#define SONG_CASTLE			2
#define SONG_TOWN				3
#define SONG_TOWER			4
#define SONG_CAVE				5
#define SONG_BATTLE			6

uint8_t song = 0;
uint16_t songT = 0;

#include <ArduboyTones.h>
//#include "sltones.h"

ArduboyTones sound(arduboy.audio.enabled);
//SLTones sound(arduboy.audio.enabled);
uint16_t notes[] = {208,220,233,247,262,277,294,311,330,349,370,392,415,440,466,494,523,554,587,622,659,698,740,784,831,880,932,988,1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,1865,1976,2093,2218,2349,2489,2637,2794,2960,3136,3322,3520,3729,3951,4186,4435,4699,4978,5274,5588,5920,6272,6645,7040,7459,7902};
//uint16_t notes[] = {32976,32988,33001,33015,33030,33045,33062,33079,33098,33117,33138,33160,33183,33208,33234,33262,33291,33322,33355,33390,33427,33466,33508,33552,33599,33648,33700,33756,33815,33877,33943,34013,34087,34165,34248,34336,34429,34528,34633,34744,34861,34986,35117,35257,35405,35562,35728,35904,36090,36288,36497,36719,36954,37203,37467,37746,38042,38356,38688,39040,39413,39808,40227,40670};

void nextNote()
{
	register uint16_t t = songT;
	uint8_t note = 0;
	if (song == 0)
		note = song0(t);
	else if (song == 1)
		note = song1(t);
	else if (song == 2)
		note = song2(t);
	else if (song == 3)
		note = song3(t);
	else if (song == 4)
		note = song4(t);
	else if (song == 5)
		note = song5(t);
	else if (song == 6)
		note = song6(t);
	sound.tone(notes[note&0x3F], songTempo[song]<<5);
	++songT;
}

#endif // MUSIC_H
