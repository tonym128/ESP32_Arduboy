# Blocks
A smple [Arduboy](https://www.arduboy.com) game.

# Description
Move boxes around until they're where they belong.

## Some notes about Blocks

- The current level you are on is saved if you power-cycle the game. (The 
level is stored in EEPROM.)
- You can always reset the level you're on, and you can also exit the game.
- Now with 31 levels of varying difficulty.

## Game level editor

The game level editor is now included in the "levels" directory. You don't 
need this if you wish to run the Arduboy game, but it helps if you wish to
build new levels. The source was built with Eclipse, and I've included the
Eclipse project files.

The game editor contains a panel which shows the code that needs to be cut
and pasted into the source code. You can also validate each level using the
"Test" button, which will recursively exhaust all possible moves until it
determines if the level can be played to conclusion correctly or not. (Note
that if you have more than 2 boxes, the search can take a long time.)

# License
    Blocks: A simple Arduboy game.
    
    Copyright © 2016 by William Edward Woody
    
    This program is free software: you can redistribute it and/or modify 
    it under the terms of the GNU General Public License as published by 
    the Free Software Foundation, either version 3 of the License, or 
    (at your option) any later version.

    This program is distributed in the hope that it will be useful, but 
    WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
    General Public License for more details.
    
    You should have received a copy of the GNU General Public License 
    along with this program. If not, see http://www.gnu.org/licenses/
    
    Contact information:
    
    William Edward Woody
    12605 Raven Ridge Rd
    Raleigh, NC 27614
    United States of America
    woody@alumni.caltech.edu

# Why?

Because sometimes I miss the old days when I did embedded software development. :-)
