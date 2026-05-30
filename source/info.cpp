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
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 2, SCREEN_WIDTH, 92, pixelBufferMain, whiteColor);
}

void Info::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, info_iconBitmap, pixelBufferMain);
}

void Info::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 2, SCREEN_WIDTH, 92, pixelBufferMain, whiteColor);

    paint.drawText(3, yOffset, "Paw Paint DS", pixelBufferMain, blackColor);
    paint.drawText(3, yOffset += 10, "Paint with your paws on Nintendo DS", pixelBufferMain, blackColor);
    paint.drawText(3, yOffset += 10, "By MaxBogomol (Pink Joke)", pixelBufferMain, blackColor);
    
    paint.drawText(3, yOffset += 20, "https://github.com/MaxBogomol/PawPaintDS", pixelBufferMain, blackColor);
    paint.drawText(3, yOffset += 10, "https://fluffy-village.dev/pages/eng/creations/", pixelBufferMain, blackColor);
    paint.drawText(3, yOffset += 10, "paw_paint.html", pixelBufferMain, blackColor);
    paint.drawText(3, yOffset += 20, "UwU", pixelBufferMain, blackColor);
}