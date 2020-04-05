#include "sign.h"
#include "renderer.h"
#include "nonMaskBitmaps.h"
#include "state.h"

extern Renderer renderer;

void Sign::render() {
    DrawMode drawMode = State::isDay() ? Normal : Invert;

    renderer.drawOverwrite(x, y, sign_tiles, 0, 0, drawMode);
}
