#include "settings.h"

#include "paint.h"

const char* Settings::getName(Paint& paint) {
    return "Settings";
}

void Settings::setup(Paint& paint) {
    line = 0;
    updateDrawTool = true;
}

void Settings::update(Paint& paint) {

}

void Settings::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void Settings::open(Paint& paint) {
    line = 0;
    updateDrawTool = true;
}

void Settings::close(Paint& paint) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 54; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }
}

void Settings::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, settings_iconBitmap, pixelBufferMain);
}

void Settings::drawTool(Paint& paint) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 54; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }
}