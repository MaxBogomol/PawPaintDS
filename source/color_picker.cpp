#include "color_picker.h"

#include "paint.h"
#include "language.h"

#include "color_picker_icon.h"

const char* ColorPicker::getName(Paint& paint) {
    return STR_COLOR_PICKER.c_str();
}

void ColorPicker::setup(Paint& paint) {
    line = 0;
    hue = 0;
    hueOld = 0;
    colorX = 0;
    colorY = 0;
    colorXOld = 0;
    colorYOld = 0;
    selectedColor = blackColor;
    newSelectedColor = blackColor;
    active = false;
    updateDrawTool = true;
    updatePicker = false;
    updateHue = false;
    updateSelected = false;
    updateNewSelected = false;
}

void ColorPicker::update(Paint& paint) {
    if (!active) {
        if (keysD & KEY_A) {
            active = true;

            drawHue(paint);
            drawOutlines(paint);

            updateDrawTool = true;
            updatePicker = true;
            updateHue = true;
            updateSelected = true;
            updateNewSelected = true;
            paint.updateDrawHints = true;
        }

        if (keysD & KEY_Y) {
            u16 color = paint.selectedColorSub;
            paint.selectedColorSub = paint.selectedColor;
            paint.selectedColor = color;
            paint.updateDrawColors = true;
            selectedColor = paint.selectedColor;
            newSelectedColor = paint.selectedColor;

            HSV hsv = paint.RGBtoHSV(selectedColor);
            hue = hsv.h;
            colorX = hsv.s / 8;
            colorY = (255 - hsv.v) / 8;
        }
    } else {
        bool setNewColor = false;

        if (touchCount > 0 && !paint.reverseScreens) {
            if (touchX >= 64 && touchX < 192 && touchY >= 32 && touchY < 160) {
                colorX = ((touchX - 64) / 4);
                colorY = ((touchY - 32) / 4);
                updatePicker = true;
                updateNewSelected = true;
                setNewColor = true;
            }
            if (touchX >= 208 && touchX < 224 && touchY >= 6 && touchY < 186) {
                hue = touchY - 6;
                updatePicker = true;
                updateHue = true;
                updateNewSelected = true;
                setNewColor = true;
            }
            if (touchX >= 224 && touchX < 240 && touchY >= 6 && touchY < 186) {
                hue = touchY - 6 + 180;
                updatePicker = true;
                updateHue = true;
                updateNewSelected = true;
                setNewColor = true;
            }
        }

        if (keysD & KEY_A) {
            selectedColor = newSelectedColor;
            paint.selectedColor = newSelectedColor;
            paint.updateDrawColors = true;
            updateSelected = true;
        }

        if (keysD & KEY_Y) {
            u16 color = paint.selectedColorSub;
            paint.selectedColorSub = paint.selectedColor;
            paint.selectedColor = color;
            paint.updateDrawColors = true;
            selectedColor = paint.selectedColor;
            newSelectedColor = paint.selectedColor;
            updatePicker = true;
            updateHue = true;
            updateSelected = true;
            updateNewSelected = true;

            HSV hsv = paint.RGBtoHSV(selectedColor);
            hue = hsv.h;
            colorX = hsv.s / 8;
            colorY = (255 - hsv.v) / 8;
        }

        if (keysH & KEY_B) {
            if (keysR & KEY_UP) {
                hue--;
                if (hue < 0) hue = 359;
                updatePicker = true;
                updateHue = true;
                updateNewSelected = true;
                setNewColor = true;
            }
            if (keysR & KEY_DOWN) {
                hue++;
                if (hue >= 360) hue = 0;
                updatePicker = true;
                updateHue = true;
                updateNewSelected = true;
                setNewColor = true;
            }
        } else {
            if ((keysR & KEY_LEFT) && (colorX - 1 >= 0)) {
                colorX--;
                updatePicker = true;
                updateNewSelected = true;
                setNewColor = true;
            }
            if ((keysR & KEY_RIGHT) && (colorX + 1 < 32)) {
                colorX++;
                updatePicker = true;
                updateNewSelected = true;
                setNewColor = true;
            }
            if ((keysR & KEY_UP) && (colorY - 1 >= 0)) {
                colorY--;
                updatePicker = true;
                updateNewSelected = true;
                setNewColor = true;
            }
            if ((keysR & KEY_DOWN) && (colorY + 1 < 32)) {
                colorY++;
                updatePicker = true;
                updateNewSelected = true;
                setNewColor = true;
            }
        }

        if (setNewColor) {
            newSelectedColor = paint.HSVtoRGB(hue, colorX * 8, 255 - (colorY * 8));
        }

        if (keysD & KEY_X) {
            active = false;
            updateDrawTool = true;
            paint.updateDrawAll = true;
            clearPicker(paint);
            clearHue(paint);
            clearColors(paint);
            clearSelectedColor(paint);
            clearNewSelectedColor(paint);
            clearPickerPointers(paint);
            clearHuePointer(paint);
        }
    }

    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();

    if (keysD & KEY_TOUCH && paint.reverseScreens) {
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
            active = !active;

            if (active) {
                drawHue(paint);
                drawOutlines(paint);

                updateDrawTool = true;
                updatePicker = true;
                updateHue = true;
                updateSelected = true;
                updateNewSelected = true;
                paint.updateDrawHints = true;
            } else {
                updateDrawTool = true;
                paint.updateDrawAll = true;
                clearPicker(paint);
                clearHue(paint);
                clearColors(paint);
                clearSelectedColor(paint);
                clearNewSelectedColor(paint);
                clearPickerPointers(paint);
                clearHuePointer(paint);
            }
        }
    }

    if (paint.updateDrawSelectedColor) {
        selectedColor = paint.selectedColor;
        newSelectedColor = paint.selectedColor;

        HSV hsv = paint.RGBtoHSV(selectedColor);
        hue = hsv.h;
        colorX = hsv.s / 8;
        colorY = (255 - hsv.v) / 8;

        if (active) {
            updatePicker = true;
            updateHue = true;
            updateSelected = true;
            updateNewSelected = true;
        }
    }

    if (updatePicker) {
        drawPicker(paint);
        drawPickerPointers(paint);
        updatePicker = false;
    }

    if (updateHue) {
        drawHuePointer(paint);
        updateHue = false;
    }

    if (updateSelected) {
        drawSelectedColor(paint);
        updateSelected = false;
    }

    if (updateNewSelected) {
        drawNewSelectedColor(paint);
        updateNewSelected = false;
    }
}

void ColorPicker::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void ColorPicker::open(Paint& paint) {
    selectedColor = paint.selectedColor;
    newSelectedColor = paint.selectedColor;

    HSV hsv = paint.RGBtoHSV(selectedColor);
    hue = hsv.h;
    colorX = hsv.s / 8;
    colorY = (255 - hsv.v) / 8;
    hueOld = hue;
    colorXOld = colorX;
    colorYOld = colorY;

    updateDrawTool = true;
}

void ColorPicker::close(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 14, pixelBufferMain);

    if (active) {
        clearPicker(paint);
        clearHue(paint);
        clearColors(paint);
        clearSelectedColor(paint);
        clearNewSelectedColor(paint);
        clearPickerPointers(paint);
        clearHuePointer(paint);
    }

    active = false;
}

void ColorPicker::redraw(Paint& paint) {
    updateDrawTool = true;
    if (active) {
        updatePicker = true;
        updateHue = true;
        updateSelected = true;
        updateNewSelected = true;
    }
}

void ColorPicker::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, color_picker_iconBitmap, buffer);
}

void ColorPicker::drawHints(Paint& paint, int x, int y, u16* buffer) {
    int xOffset = -10;
    int yOffset = 0;
    if (!active) {
        paint.drawAButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    } else {
        if (!paint.reverseScreens) paint.drawTouchButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
        paint.drawAButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
        paint.drawYButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
        xOffset = 0;
        yOffset += 10;
        paint.drawAllButton(x + xOffset, y + yOffset, pixelBufferMain);
        paint.drawBButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
        paint.drawXButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    }
}

u16 *ColorPicker::getDrawLayer(Paint& paint) {
    return pixelBufferSub;
}

void ColorPicker::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 64, pixelBufferMain);

    string moveString = string((line == 0) ? ">" : "") + STR_COLOR_PICKER_COLOR + ": " + ((line == 0 && active) ? "+" : "-"); 
    paint.drawText(3, yOffset, moveString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);
}

void ColorPicker::drawPicker(Paint& paint) {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            u16 color = paint.HSVtoRGB(hue, x * 8, 255 - (y * 8));
            paint.drawSquare(x * 4 + 64, y * 4 + 32, 4, 4, getDrawLayer(paint), color);
        }
    }
}

void ColorPicker::drawHue(Paint& paint) {
    for (int y = 0; y < 180; y++) {
        u16 color = paint.HSVtoRGB(y, 255, 255);
        paint.drawSquare(208, y + 6, 16, 1, getDrawLayer(paint), color);
    }
    for (int y = 0; y < 180; y++) {
        u16 color = paint.HSVtoRGB(y + 180, 255, 255);
        paint.drawSquare(224, y + 6, 16, 1, getDrawLayer(paint), color);
    }
}

void ColorPicker::drawSelectedColor(Paint& paint) {
    paint.drawSquare(24, 96, 16, 16, getDrawLayer(paint), selectedColor);
    clearSelectedColor(paint);

    int r = (selectedColor) & 31;
    int g = (selectedColor >> 5) & 31;
    int b = (selectedColor >> 10) & 31;

    string colorString = string("RGB: ") + paint.intToChars(r) + " " + paint.intToChars(g) + " " + paint.intToChars(b); 
    paint.drawTextOutline(24, 176, colorString.c_str(), getDrawLayer(paint), blackColor, whiteColor);
}

void ColorPicker::drawNewSelectedColor(Paint& paint) {
    paint.drawSquare(24, 80, 16, 16, getDrawLayer(paint), newSelectedColor);
    clearNewSelectedColor(paint);

    int r = (newSelectedColor) & 31;
    int g = (newSelectedColor >> 5) & 31;
    int b = (newSelectedColor >> 10) & 31;

    string colorString = string("RGB: ") + paint.intToChars(r) + " " + paint.intToChars(g) + " " + paint.intToChars(b); 
    paint.drawTextOutline(24, 8, colorString.c_str(), getDrawLayer(paint), blackColor, whiteColor);
}

void ColorPicker::drawOutlines(Paint& paint) {
    paint.drawSquareOutline(23, 79, 18, 34, getDrawLayer(paint), blackColor);
    paint.drawSquareOutline(63, 31, 130, 130, getDrawLayer(paint), blackColor);
    paint.drawSquareOutline(207, 5, 34, 182, getDrawLayer(paint), blackColor);
}

void ColorPicker::drawPickerPointers(Paint& paint) {
    clearPickerPointers(paint);

    paint.drawSquare(colorX * 4 + 63, 24, 6, 6, getDrawLayer(paint), blackColor);
    paint.drawSquare(colorX * 4 + 64, 25, 4, 4, getDrawLayer(paint), whiteColor);

    paint.drawSquare(colorX * 4 + 63, 162, 6, 6, getDrawLayer(paint), blackColor);
    paint.drawSquare(colorX * 4 + 64, 163, 4, 4, getDrawLayer(paint), whiteColor);

    paint.drawSquare(56, colorY * 4 + 31, 6, 6, getDrawLayer(paint), blackColor);
    paint.drawSquare(57, colorY * 4 + 32, 4, 4, getDrawLayer(paint), whiteColor);

    paint.drawSquare(194, colorY * 4 + 31, 6, 6, getDrawLayer(paint), blackColor);
    paint.drawSquare(195, colorY * 4 + 32, 4, 4, getDrawLayer(paint), whiteColor);

    paint.drawSquareOutline(colorX * 4 + 63, colorY * 4 + 31, 6, 6, getDrawLayer(paint), blackColor);

    colorXOld = colorX;
    colorYOld = colorY;
}

void ColorPicker::drawHuePointer(Paint& paint) {
    clearHuePointer(paint);

    int x = 0;
    int y = hue;
    if (hue >= 180) {
        y = y - 180;
        x = x + 40;
    }
    paint.drawSquareOutline(x + 202, y + 5, 4, 3, getDrawLayer(paint), blackColor);
    paint.drawSquare(x + 202 + 1, y + 6, 2, 1, getDrawLayer(paint), whiteColor);

    hueOld = hue;
}

void ColorPicker::clearPicker(Paint& paint) {
    paint.blendLayers(207, 5, 34, 182);
}

void ColorPicker::clearHue(Paint& paint) {
    paint.blendLayers(63, 31, 130, 130);
}

void ColorPicker::clearColors(Paint& paint) {
    paint.blendLayers(23, 79, 34, 34);
}

void ColorPicker::clearSelectedColor(Paint& paint) {
    paint.blendLayers(23, 175, 73, 10);
}

void ColorPicker::clearNewSelectedColor(Paint& paint) {
    paint.blendLayers(23, 7, 73, 10);
}

void ColorPicker::clearPickerPointers(Paint& paint) {
    paint.blendLayers(colorXOld * 4 + 63, 24, 6, 6);
    paint.blendLayers(colorXOld * 4 + 63, 162, 6, 6);

    paint.blendLayers(56, colorYOld * 4 + 31, 6, 6);
    paint.blendLayers(194, colorYOld * 4 + 31, 6, 6);
}

void ColorPicker::clearHuePointer(Paint& paint) {
    int x = 0;
    int y = hueOld;
    if (hueOld >= 180) {
        y = y - 180;
        x = x + 40;
    }
    paint.blendLayers(x + 202, y + 5, 4, 3);
}