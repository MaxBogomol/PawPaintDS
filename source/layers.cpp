#include "layers.h"

#include "paint.h"
#include "language.h"

#include "buttons_icon.h"
#include "layers_icon.h"

const char* Layers::getName(Paint& paint) {
    return STR_LAYERS.c_str();
}

void Layers::setup(Paint& paint) {
    line = 0;
    updateDrawTool = true;
}

void Layers::update(Paint& paint) {
    bool updateLayers = false;
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
                paint.updateDrawHints = true;
            }
            if ((keysD & KEY_RIGHT) && (paint.selectedLayer + 1 <= 3)) {
                paint.selectedLayer++;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
            break;
        }
        case 1: {
            if ((keysD & KEY_LEFT) && (paint.selectedLayer - 1 >= 0)) {
                paint.swapLayers(paint.selectedLayer, paint.selectedLayer - 1);
                paint.selectedLayer--;
                updateLayers = true;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
            if ((keysD & KEY_RIGHT) && (paint.selectedLayer + 1 <= 3)) {
                paint.swapLayers(paint.selectedLayer, paint.selectedLayer + 1);
                paint.selectedLayer++;
                updateLayers = true;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
            break;
        }
    }

    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();

    if (keysD & KEY_TOUCH && paint.reverseScreens) {
        if (touchX >= SCREEN_WIDTH - bOffset - 16 - 5 && touchX < SCREEN_WIDTH - bOffset - 8 - 5 && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.selectedLayer - 1 >= 0) {
                paint.selectedLayer--;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
        }
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.selectedLayer + 1 <= 3) {
                paint.selectedLayer++;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
        }
        yOffset += 10;
        if (touchX >= SCREEN_WIDTH - bOffset - 16 - 5 && touchX < SCREEN_WIDTH - bOffset - 8 - 5 && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.selectedLayer - 1 >= 0) {
                paint.swapLayers(paint.selectedLayer, paint.selectedLayer - 1);
                paint.selectedLayer--;
                updateLayers = true;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
        }
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.selectedLayer + 1 <= 3) {
                paint.swapLayers(paint.selectedLayer, paint.selectedLayer + 1);
                paint.selectedLayer++;
                updateLayers = true;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
        }
    }

    if (updateLayers) {
        paint.blendLayers(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 24, pixelBufferMain);
}

void Layers::redraw(Paint& paint) {
    updateDrawTool = true;
}

void Layers::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, layers_iconBitmap, buffer);
}

void Layers::drawHints(Paint& paint, int x, int y, u16* buffer) {
    int xOffset = 0;
    int yOffset = 0;
    if (paint.selectedLayer == 0) {
        paint.drawRightButton(x + xOffset, y + yOffset, pixelBufferMain);
    } else if (paint.selectedLayer == 3) {
        paint.drawLeftButton(x + xOffset, y + yOffset, pixelBufferMain);
    } else {
        paint.drawLeftRightButton(x + xOffset, y + yOffset, pixelBufferMain);
    }
}

void Layers::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 24, pixelBufferMain);

    string moveString = string((line == 0) ? ">" : "") + STR_LAYERS_LAYER + ": " + paint.intToChars(paint.selectedLayer + 1);
    paint.drawText(3, yOffset, moveString.c_str(), pixelBufferMain, blackColor);
    if (paint.selectedLayer - 1 >= 0) paint.drawLeftButton(SCREEN_WIDTH - bOffset - 16 - 5, yOffset, pixelBufferMain);
    if (paint.selectedLayer + 1 <= 3) paint.drawRightButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string typeString = string((line == 1) ? ">" : "") + STR_LAYERS_MOVE;
    paint.drawText(3, yOffset += 10, typeString.c_str(), pixelBufferMain, blackColor);
    if (paint.selectedLayer - 1 >= 0) paint.drawLeftButton(SCREEN_WIDTH - bOffset - 16 - 5, yOffset, pixelBufferMain);
    if (paint.selectedLayer + 1 <= 3) paint.drawRightButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);
}