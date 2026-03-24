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
    int maxLine = 4;

    if (keysD & KEY_UP) {
        if (line - 1 >= 0) {
            line--;
            updateDrawTool = true;
        }
    }
    if (keysD & KEY_DOWN) {
        if (line + 1 < maxLine) {
            line++;
            updateDrawTool = true;
        }
    }

    switch (line) {
        case 0: {
            if (keysD & KEY_LEFT) {
                if (paint.selectedLayer - 1 >= 0) {
                    paint.selectedLayer--;
                    updateDrawTool = true;
                }
            }
            if (keysD & KEY_RIGHT) {
                if (paint.selectedLayer + 1 <= 3) {
                    paint.selectedLayer++;
                    updateDrawTool = true;
                }
            }
            break;
        }
        case 1: {
            if (keysD & KEY_LEFT) {
                if (paint.selectedLayer - 1 >= 0) {
                    paint.swapSubLayers(paint.selectedLayer, paint.selectedLayer - 1);
                    paint.selectedLayer--;
                    updateSubLayers = true;
                    updateDrawTool = true;
                }
            }
            if (keysD & KEY_RIGHT) {
                if (paint.selectedLayer + 1 <= 3) {
                    paint.swapSubLayers(paint.selectedLayer, paint.selectedLayer + 1);
                    paint.selectedLayer++;
                    updateSubLayers = true;
                    updateDrawTool = true;
                }
            }
            break;
        }
    }

    if (updateSubLayers) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                paint.blendSubLayers(x, y);
            }
        }
        updateSubLayers = false;
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
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 54; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }
}

void Layers::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, layers_iconBitmap, layers_iconPal, buffer);
}

void Layers::drawTool(Paint& paint) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 54; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }

    string moveString = string((line == 0) ? ">" : "") + "Layer: " + paint.intToChars(paint.selectedLayer + 1);
    paint.drawText(3, 48, moveString.c_str(), pixelBufferMain, blackColor);
    paint.drawText(176, 48, "- +", pixelBufferMain, blackColor);

    string typeString = string((line == 1) ? ">" : "") + "Move";
    paint.drawText(3, 57, typeString.c_str(), pixelBufferMain, blackColor);
    paint.drawText(176, 57, "- +", pixelBufferMain, blackColor);
}