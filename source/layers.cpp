#include "layers.h"

#include "paint.h"

const char* Layers::getName(Paint& paint) {
    return "Layers";
}

void Layers::setup(Paint& paint) {
    line = 0;
    updateDrawTool = true;
}

void Layers::update(Paint& paint) {
    bool updateSubLayers = false;
    int maxLine = 2;

    if ((keysD & KEY_UP) && (line - 1 >= 0)) {
        line--;
        updateDrawTool = true;
    }
    if ((keysD & KEY_DOWN) && (line + 1 < maxLine)) {
        line++;
        updateDrawTool = true;
    }

    switch (line) {
        case 0: {
            if ((keysD & KEY_LEFT) && (paint.selectedLayer - 1 >= 0)) {
                paint.selectedLayer--;
                updateDrawTool = true;
            }
            if ((keysD & KEY_RIGHT) && (paint.selectedLayer + 1 <= 3)) {
                paint.selectedLayer++;
                updateDrawTool = true;
            }
            break;
        }
        case 1: {
            if ((keysD & KEY_LEFT) && (paint.selectedLayer - 1 >= 0)) {
                paint.swapSubLayers(paint.selectedLayer, paint.selectedLayer - 1);
                paint.selectedLayer--;
                updateSubLayers = true;
                updateDrawTool = true;
            }
            if ((keysD & KEY_RIGHT) && (paint.selectedLayer + 1 <= 3)) {
                paint.swapSubLayers(paint.selectedLayer, paint.selectedLayer + 1);
                paint.selectedLayer++;
                updateSubLayers = true;
                updateDrawTool = true;
            }
            break;
        }
    }

    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();

    if (keysD & KEY_TOUCH && paint.reverseScreens) {
        if (touchX >= SCREEN_WIDTH - bOffset - 16 - 5 && touchX < SCREEN_WIDTH - bOffset - 16 - 5 + 8 && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.selectedLayer - 1 >= 0) {
                paint.selectedLayer--;
                updateDrawTool = true;
            }
        }
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset + 8 && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.selectedLayer + 1 <= 3) {
                paint.selectedLayer++;
                updateDrawTool = true;
            }
        }
        yOffset += 10;
        if (touchX >= SCREEN_WIDTH - bOffset - 16 - 5 && touchX < SCREEN_WIDTH - bOffset - 16 - 5 + 8 && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.selectedLayer - 1 >= 0) {
                paint.swapSubLayers(paint.selectedLayer, paint.selectedLayer - 1);
                paint.selectedLayer--;
                updateSubLayers = true;
                updateDrawTool = true;
            }
        }
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset + 8 && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.selectedLayer + 1 <= 3) {
                paint.swapSubLayers(paint.selectedLayer, paint.selectedLayer + 1);
                paint.selectedLayer++;
                updateSubLayers = true;
                updateDrawTool = true;
            }
        }
    }

    if (updateSubLayers) {
        paint.blendSubLayers(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}

void Layers::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void Layers::open(Paint& paint) {
    line = 0;
    updateDrawTool = true;
}

void Layers::close(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 2, SCREEN_WIDTH, 22, pixelBufferMain);
}

void Layers::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, layers_iconBitmap, buffer);
}

void Layers::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 2, SCREEN_WIDTH, 22, pixelBufferMain);

    string moveString = string((line == 0) ? ">" : "") + "Layer: " + paint.intToChars(paint.selectedLayer + 1);
    paint.drawText(3, yOffset, moveString.c_str(), pixelBufferMain, blackColor);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 16 - 5, yOffset, 32, 32, 24, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 8, yOffset, 32, 32, 8, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);

    string typeString = string((line == 1) ? ">" : "") + "Move";
    paint.drawText(3, yOffset += 10, typeString.c_str(), pixelBufferMain, blackColor);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 16 - 5, yOffset, 32, 32, 24, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 8, yOffset, 32, 32, 8, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);
}