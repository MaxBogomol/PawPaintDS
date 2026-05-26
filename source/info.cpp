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
        for (int y = 0; y < 54 + 27; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }
}

void Info::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, info_iconBitmap, pixelBufferMain);
}

void Info::drawTool(Paint& paint) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 54 + 27; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }

    paint.drawText(3, 48, "Paw Paint DS", pixelBufferMain, blackColor);
    paint.drawText(3, 57, "Paint with your paws on Nintendo DS", pixelBufferMain, blackColor);
    paint.drawText(3, 66, "By MaxBogomol (Pink Joke)", pixelBufferMain, blackColor);
    
    paint.drawText(3, 84, "https://github.com/MaxBogomol/PawPaintDS", pixelBufferMain, blackColor);
    paint.drawText(3, 93, "https://fluffy-village.dev/pages/eng/creations/", pixelBufferMain, blackColor);
    paint.drawText(3, 102, "paw_paint.html", pixelBufferMain, blackColor);
    paint.drawText(3, 120, "UwU", pixelBufferMain, blackColor);
}