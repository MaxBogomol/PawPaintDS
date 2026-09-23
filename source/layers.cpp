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
    int maxLine = 5;

    if ((keysD & KEY_UP) && (line - 1 >= 0)) {
        line--;
        updateDrawTool = true;
        paint.updateDrawHints = true;
    }
    if ((keysD & KEY_DOWN) && (line + 1 < maxLine)) {
        line++;
        updateDrawTool = true;
        paint.updateDrawHints = true;
    }

    int maxLayers = (int) paint.screenLayers.size();

    switch (line) {
        case 0: {
            if ((keysD & KEY_LEFT) && (paint.selectedLayer - 1 >= 0)) {
                paint.selectedLayer--;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
            if ((keysD & KEY_RIGHT) && (paint.selectedLayer + 1 < maxLayers)) {
                paint.selectedLayer++;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
            break;
        }
        case 1: {
            if ((keysD & KEY_LEFT) && (paint.selectedLayer - 1 >= 0)) {
                iter_swap(paint.screenLayers.begin() + paint.selectedLayer, paint.screenLayers.begin() + paint.selectedLayer - 1);
                paint.selectedLayer--;
                updateLayers = true;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
            if ((keysD & KEY_RIGHT) && (paint.selectedLayer + 1 < maxLayers)) {
                iter_swap(paint.screenLayers.begin() + paint.selectedLayer, paint.screenLayers.begin() + paint.selectedLayer + 1);
                paint.selectedLayer++;
                updateLayers = true;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
            break;
        }
        case 2: {
            if (keysD & KEY_A) {
                paint.screenLayers[paint.selectedLayer]->active = !paint.screenLayers[paint.selectedLayer]->active;
                updateLayers = true;
                updateDrawTool = true;
            }
            break;
        }
        case 3: {
            if (keysD & KEY_A) {
                paint.selectedLayer++;
                Layer* newLayer = new Layer();
                newLayer->clearLayer();
                paint.screenLayers.insert(paint.screenLayers.begin() + paint.selectedLayer, newLayer);
                updateLayers = true;
                updateDrawTool = true;
            }
            break;
        }
        case 4: {
            if ((keysD & KEY_A) && (maxLayers > 1)) {
                paint.screenLayers.erase(paint.screenLayers.begin() + paint.selectedLayer);
                if (paint.selectedLayer + 1 >= maxLayers) paint.selectedLayer--;
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
            if (paint.selectedLayer + 1 < maxLayers) {
                paint.selectedLayer++;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
        }
        yOffset += 13;
        if (touchX >= SCREEN_WIDTH - bOffset - 16 - 5 && touchX < SCREEN_WIDTH - bOffset - 8 - 5 && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.selectedLayer - 1 >= 0) {
                iter_swap(paint.screenLayers.begin() + paint.selectedLayer, paint.screenLayers.begin() + paint.selectedLayer - 1);
                paint.selectedLayer--;
                updateLayers = true;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
        }
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.selectedLayer + 1 < maxLayers) {
                iter_swap(paint.screenLayers.begin() + paint.selectedLayer, paint.screenLayers.begin() + paint.selectedLayer + 1);
                paint.selectedLayer++;
                updateLayers = true;
                updateDrawTool = true;
                paint.updateDrawHints = true;
            }
        }
        yOffset += 13;
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
            paint.screenLayers[paint.selectedLayer]->active = !paint.screenLayers[paint.selectedLayer]->active;
            updateLayers = true;
            updateDrawTool = true;
        }
        yOffset += 13;
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
            paint.selectedLayer++;
            Layer* newLayer = new Layer();
            newLayer->clearLayer();
            paint.screenLayers.insert(paint.screenLayers.begin() + paint.selectedLayer, newLayer);
            updateLayers = true;
            updateDrawTool = true;
        }
        yOffset += 13;
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
            if (maxLayers > 1) {
                paint.screenLayers.erase(paint.screenLayers.begin() + paint.selectedLayer);
                if (paint.selectedLayer + 1 >= maxLayers) paint.selectedLayer--;
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
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 5 * 13 + 3, pixelBufferMain);
}

void Layers::redraw(Paint& paint) {
    updateDrawTool = true;
}

void Layers::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, layers_iconBitmap, buffer);
}

void Layers::drawHints(Paint& paint, int x, int y, u16* buffer) {
    int xOffset = -10;
    int yOffset = 0;
    paint.drawUpDownButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    
    int maxLayers = (int) paint.screenLayers.size();
    if (line < 2) {
        if (maxLayers > 1) {
            if (paint.selectedLayer == 0) {
                paint.drawRightButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
            } else if (paint.selectedLayer + 1 == maxLayers) {
                paint.drawLeftButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
            } else {
                paint.drawLeftRightButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
            }
        }
    } else {
        bool active = true;
        if (line == 4) active = maxLayers > 1;
        if (active) paint.drawAButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    }
}

void Layers::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 5 * 13 + 3, pixelBufferMain);

    int maxLayers = (int) paint.screenLayers.size();

    string layerString = string((line == 0) ? ">" : "") + STR_LAYERS_LAYER + ": " + paint.intToChars(paint.selectedLayer + 1) + "/" + paint.intToChars(maxLayers);
    paint.drawText(3, yOffset, layerString.c_str(), pixelBufferMain, maxLayers > 1 ? blackColor : grayColor);
    if (paint.selectedLayer - 1 >= 0) paint.drawLeftButton(SCREEN_WIDTH - bOffset - 16 - 5, yOffset, pixelBufferMain);
    if (paint.selectedLayer + 1 < maxLayers) paint.drawRightButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string movetring = string((line == 1) ? ">" : "") + STR_LAYERS_MOVE;
    paint.drawText(3, yOffset += 13, movetring.c_str(), pixelBufferMain, maxLayers > 1 ? blackColor : grayColor);
    if (paint.selectedLayer - 1 >= 0) paint.drawLeftButton(SCREEN_WIDTH - bOffset - 16 - 5, yOffset, pixelBufferMain);
    if (paint.selectedLayer + 1 < maxLayers) paint.drawRightButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string activeString = string((line == 2) ? ">" : "") + STR_LAYERS_ACTIVE + ": " + ((paint.screenLayers[paint.selectedLayer]->active) ? "+" : "-"); 
    paint.drawText(3, yOffset += 13, activeString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string createString = string((line == 3) ? ">" : "") + STR_LAYERS_CREATE;
    paint.drawText(3, yOffset += 13, createString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string deleteString = string((line == 4) ? ">" : "") + STR_LAYERS_DELETE;
    paint.drawText(3, yOffset += 13, deleteString.c_str(), pixelBufferMain, maxLayers > 1 ? blackColor : grayColor);
    if (maxLayers > 1) paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);
}