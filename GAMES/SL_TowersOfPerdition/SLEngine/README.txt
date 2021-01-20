to use this simply copy this folder into the same folder as your .ino file
then in the .ino file add the following four lines

#define ENABLE_MUSIC
#define ENABLE_NAME_ENTRY
#include "data.h" //generated from the editor
#include "SLEngine/game.h"

That's all there is to it
If you don't want any background music comment out the ENABLE_MUSIC line
If you don't want to use the NEW game option (if this is a sequal) comment out the ENABLE_NAME_ENTRY line
