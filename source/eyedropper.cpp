#include "eyedropper.h"

#include "paint.h"
#include "language.h"

#include "eyedropper_icon.h"

const char* Eyedropper::getName(Paint& paint) {
    return STR_EYEDROPPER.c_str();
}

void Eyedropper::setup(Paint& paint) {
    line = 0;
    cursorX = 0;
    cursorY = 0;
    cursorXOld = 0;
    cursorYOld = 0;
    active = false;
    updateDrawTool = true;
    updateDrawCursor = false;
}

void Eyedropper::update(Paint& paint) {
    if (touchCount >= 1 && !paint.reverseScreens) {
        if (line == 0 && active) {
            updateDrawCursor = true;
        } else {
            u16 color = paint.getPixel(touchX, touchY, pixelBufferSub);
            if (((color >> 15) & 1) == 0) color = blackColor;
            paint.selectedColor = color;
            paint.updateDrawColors = true;
        }

        cursorX = touchX;
        cursorY = touchY;
    }

    if (keysD & KEY_Y) {
        u16 color = paint.selectedColorSub;
        paint.selectedColorSub = paint.selectedColor;
        paint.selectedColor = color;
        paint.updateDrawColors = true;
    }

    if (keysD & KEY_A) {
        if (line == 0) {
            active = !active;
            updateDrawTool = true;
            updateDrawCursor = true;
            paint.updateDrawHints = true;
        }
    }

    if (active) {
        if (line == 0) {
            if ((keysR & KEY_LEFT) && (cursorX - 1 >= 0)) {
                cursorX--;
                updateDrawCursor = true;
            }
            if ((keysR & KEY_RIGHT) && (cursorX + 1 < SCREEN_WIDTH)) {
                cursorX++;
                updateDrawCursor = true;
            }
            if ((keysR & KEY_UP) && (cursorY - 1 >= 0)) {
                cursorY--;
                updateDrawCursor = true;
            }
            if ((keysR & KEY_DOWN) && (cursorY + 1 < SCREEN_HEIGHT)) {
                cursorY++;
                updateDrawCursor = true;
            }
            if (keysH & KEY_B) {
                u16 color = paint.getPixel(cursorX, cursorY, pixelBufferSub);
                if (((color >> 15) & 1) == 0) color = blackColor;
                paint.selectedColor = color;
                paint.updateDrawColors = true;
            }
        }
    }

    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();

    if (keysD & KEY_TOUCH && paint.reverseScreens) {
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
            active = !active;
            line = 0;
            updateDrawTool = true;
            updateDrawCursor = true;
            paint.updateDrawHints = true;
        }
    }

    if (updateDrawCursor) {
        drawCursor(paint);
        updateDrawCursor = false;
    }
}

void Eyedropper::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void Eyedropper::open(Paint& paint) {
    line = 0;
    active = false;
    updateDrawTool = true;
    updateDrawCursor = false;
}

void Eyedropper::close(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 14, pixelBufferMain);

    active = false;
    drawCursor(paint);
}

void Eyedropper::redraw(Paint& paint) {
    updateDrawTool = true;
}

void Eyedropper::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, eyedropper_iconBitmap, pixelBufferMain);
}

void Eyedropper::drawHints(Paint& paint, int x, int y, u16* buffer) {
    int xOffset = -10;
    int yOffset = 0;
    paint.drawAButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    xOffset = -10;
    yOffset += 10;
    if (!paint.reverseScreens) paint.drawTouchButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    if (active && line == 0) {
        paint.drawAllButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
        paint.drawBButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    }
    paint.drawYButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
}

void Eyedropper::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 14, pixelBufferMain);

    string moveString = string((line == 0) ? ">" : "") + STR_EYEDROPPER_MOVE + ": " + ((line == 0 && active) ? "+" : "-"); 
    paint.drawText(3, yOffset, moveString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);
}

void Eyedropper::drawCursor(Paint& paint, bool clear) {
    paint.blendLayers(cursorXOld - 2, cursorYOld - 2, 5, 5);
    if (active && !clear) {
        paint.drawSquareOutline(cursorX - 2, cursorY - 2, 5, 5, pixelBufferSub, blackColor);
        paint.drawSquareOutline(cursorX - 1, cursorY - 1, 3, 3, pixelBufferSub, whiteColor);
    }

    cursorXOld = cursorX;
    cursorYOld = cursorY;
}

void Eyedropper::drawCursor(Paint& paint) {
    drawCursor(paint, false);
}