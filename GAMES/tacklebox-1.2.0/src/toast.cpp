#include "toast.h"
#include "renderer.h"

extern Renderer renderer;

uint8_t Toast::toastCount = 0;
const uint8_t* Toast::msg = NULL;

void Toast::toast(const uint8_t* newMsg, uint8_t count) {
    Toast::toastCount = count;
    Toast::msg = newMsg;
}

void Toast::render() {
    if (toastCount > 0) {
        toastCount -= 1;

        renderer.translateX = 0;
        renderer.translateY = 0;
        renderer.fillRect(0, HEIGHT - 5, WIDTH, 5, BLACK);
        renderer.drawString(0, HEIGHT - 4, msg);
    }
}
