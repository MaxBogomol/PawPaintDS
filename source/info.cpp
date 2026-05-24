#include "info.h"

#include "paint.h"

const char* Info::getName(Paint& paint) {
    return "Info";
}

void Info::setup(Paint& paint) {
    updateDrawTool = true;
}

void Info::update(Paint& paint) {

}

void Info::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void Info::open(Paint& paint) {
    updateDrawTool = true;
}

void Info::close(Paint& paint) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 54; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }
}

void Info::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, info_iconBitmap, pixelBufferMain);
}

void Info::drawTool(Paint& paint) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 54; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }
}