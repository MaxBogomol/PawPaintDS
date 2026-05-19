#include "saving.h"

#include "paint.h"

const char* Saving::getName(Paint& paint) {
    return "Saving";
}

void Saving::update(Paint& paint) {

}

void Saving::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, saving_iconBitmap, pixelBufferMain);
}