#include "screen_keyboard.h"

#include "paint.h"

const char* ScreenKeyboard::getName(Paint& paint) {
    return "Keyboard";
}

void ScreenKeyboard::update(Paint& paint) {

}

void ScreenKeyboard::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, eyedropper_iconBitmap, pixelBufferMain);
}