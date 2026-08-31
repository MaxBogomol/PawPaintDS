#include "eyedropper.h"

#include "paint.h"
#include "language.h"

#include "eyedropper_icon.h"

const char* Eyedropper::getName(Paint& paint) {
    return STR_EYEDROPPER.c_str();
}

void Eyedropper::update(Paint& paint) {
    if (touchCount >= 1 && !paint.reverseScreens) {
        u16 color = paint.getPixel(touchX, touchY, pixelBufferSub);
        if (((color >> 15) & 1) == 0) color = blackColor;
        paint.selectedColor = color;
        paint.updateDrawColors = true;
    }

    if (keysD & KEY_Y) {
        u16 color = paint.selectedColorSub;
        paint.selectedColorSub = paint.selectedColor;
        paint.selectedColor = color;
        paint.updateDrawColors = true;
    }
}

void Eyedropper::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, eyedropper_iconBitmap, pixelBufferMain);
}

void Eyedropper::drawHints(Paint& paint, int x, int y, u16* buffer) {
    int xOffset = -10;
    int yOffset = 0;
    if (!paint.reverseScreens) paint.drawTouchButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    paint.drawYButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
}