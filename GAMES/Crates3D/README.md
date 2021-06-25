# Crates3D

Based on the Ti83 game 'Crates 3D':
http://tiwizard.com/downloads/crates-3d/

Full TI83 version including Z80 source and level editor:
http://www.ticalc.org/archives/files/fileinfo/214/21464.html

Guide the black block to the other black block.

Keys: 
Direction keys = move

A = rotate view 90 degrees

B = 'push' a crate (in conjunction with direction key)

Hold-A = quit / retart a level (e.g. if a mistake made)

On the title screen you can choose which level to play
using the left and right keys, then press 'B' to begin.

There is a 'save progress' option available depending on whether
ALLOW_SAVE is set in the Crates3D.ino file. If it is set, then
while in a level, hold 'B' and tap 'A' to save progress. You
can then resume by choosing to play level '0' on the title screen.
(This uses the EEPROM space between addresses 150 and 923)

(Extra levels made with the Microsoft Windows level editor should work
OK here in theory if the file format is converted)

Please see the 'crates3d_TI83.txt' file for more detaled instructions.

(If you download a ZIP file you might need to rename it from Crates3D-master.zip to just Crates3D.zip)
