#include "color_picker.h"

#include "paint.h"

void ColorPicker::setup(Paint& paint) {
    hue = 0;
    hueOld = 0;
    colorX = 0;
    colorY = 0;
    colorXOld = 0;
    colorYOld = 0;
    updatePicker = false;
    updateHue = false;
    updateSelected = false;
    updateNewSelected = false;
    selectedColor = blackColor;
    newSelectedColor = blackColor;
}

void ColorPicker::update(Paint& paint) {
    if (touchCount > 0) {
        bool setNewColor = false;
        if (touchX >= 64 && touchX < 192 && touchY >= 32 && touchY < 160) {
            colorX = ((touchX - 64) / 4);
            colorY = ((touchY - 32) / 4);
            updatePicker = true;
            updateNewSelected = true;
            setNewColor = true;
        }
        if (touchX >= 208 && touchX < 224 && touchY >= 8 && touchY < 188) {
            hue = touchY - 8;
            updatePicker = true;
            updateHue = true;
            updateNewSelected = true;
            setNewColor = true;
        }
        if (touchX >= 224 && touchX < 240 && touchY >= 8 && touchY < 188) {
            hue = touchY - 8 + 180;
            updatePicker = true;
            updateHue = true;
            updateNewSelected = true;
            setNewColor = true;
        }
        if (setNewColor) {
            newSelectedColor = paint.HSVtoRGB(hue, colorX * 8, 255 - (colorY * 8));
        }
    }

    if (keysD & KEY_A) {
        selectedColor = newSelectedColor;
        paint.selectedColor = newSelectedColor;
        updateSelected = true;
    }

    if (keysD & KEY_Y) {
        u16 color = paint.selectedColorSub;
        paint.selectedColorSub = paint.selectedColor;
        paint.selectedColor = color;
        selectedColor = paint.selectedColor;
        newSelectedColor = paint.selectedColor;
        updateSelected = true;
        updateNewSelected = true;
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

    drawHue(paint);
    drawOutlines(paint);

    updatePicker = true;
    updateHue = true;
    updateSelected = true;
    updateNewSelected = true;
}

void ColorPicker::close(Paint& paint) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            paint.blendSubLayers(x, y);
        }
    }
}

u16 *ColorPicker::getDrawLayer(Paint& paint) {
    return pixelBufferSub;
}

void ColorPicker::drawPicker(Paint& paint) {
    paint.updateSubLayers = false;
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) {
            u16 color = paint.HSVtoRGB(hue, x * 8, 255 - (y * 8));
            paint.drawSquare(x * 4 + 64, y * 4 + 32, 4, 4, getDrawLayer(paint), color);
        }
    }
    paint.updateSubLayers = true;
}

void ColorPicker::drawHue(Paint& paint) {
    paint.updateSubLayers = false;
    for (int y = 0; y < 180; y++) {
        u16 color = paint.HSVtoRGB(y, 255, 255);
        paint.drawSquare(208, y + 8, 16, 1, getDrawLayer(paint), color);
    }
    for (int y = 0; y < 180; y++) {
        u16 color = paint.HSVtoRGB(y + 180, 255, 255);
        paint.drawSquare(224, y + 8, 16, 1, getDrawLayer(paint), color);
    }
    paint.updateSubLayers = true;
}


void ColorPicker::drawSelectedColor(Paint& paint) {
    paint.updateSubLayers = false;
    paint.drawSquare(24, 96, 16, 16, getDrawLayer(paint), selectedColor);

    for (int x = 0; x < 144; x++) {
        for (int y = 0; y < 10; y++) {
            paint.blendSubLayers(x + 23, y + 175);
        }
    }

    int r = (selectedColor) & 31;
    int g = (selectedColor >> 5) & 31;
    int b = (selectedColor >> 10) & 31;

    string colorString = string("RGB: (") + paint.intToChars(r) + ", " + paint.intToChars(g) + ", " + paint.intToChars(b) + ")"; 
    paint.drawTextOutline(24, 176, colorString.c_str(), getDrawLayer(paint), blackColor, whiteColor);
    paint.updateSubLayers = true;
}

void ColorPicker::drawNewSelectedColor(Paint& paint) {
    paint.updateSubLayers = false;
    paint.drawSquare(24, 80, 16, 16, getDrawLayer(paint), newSelectedColor);

    for (int x = 0; x < 144; x++) {
        for (int y = 0; y < 10; y++) {
            paint.blendSubLayers(x + 23, y + 7);
        }
    }

    int r = (newSelectedColor) & 31;
    int g = (newSelectedColor >> 5) & 31;
    int b = (newSelectedColor >> 10) & 31;

    string colorString = string("RGB: (") + paint.intToChars(r) + ", " + paint.intToChars(g) + ", " + paint.intToChars(b) + ")"; 
    paint.drawTextOutline(24, 8, colorString.c_str(), getDrawLayer(paint), blackColor, whiteColor);
    paint.updateSubLayers = true;
}

void ColorPicker::drawOutlines(Paint& paint) {
    paint.updateSubLayers = false;
    paint.drawLine(23, 79, 40, 79, getDrawLayer(paint), blackColor);
    paint.drawLine(23, 112, 40, 112, getDrawLayer(paint), blackColor);
    paint.drawLine(23, 79, 23, 112, getDrawLayer(paint), blackColor);
    paint.drawLine(40, 79, 40, 112, getDrawLayer(paint), blackColor);

    paint.drawLine(63, 31, 192, 31, getDrawLayer(paint), blackColor);
    paint.drawLine(63, 160, 192, 160, getDrawLayer(paint), blackColor);
    paint.drawLine(63, 31, 63, 160, getDrawLayer(paint), blackColor);
    paint.drawLine(192, 31, 192, 160, getDrawLayer(paint), blackColor);

    paint.drawLine(207, 7, 240, 7, getDrawLayer(paint), blackColor);
    paint.drawLine(207, 188, 240, 188, getDrawLayer(paint), blackColor);
    paint.drawLine(207, 7, 207, 188, getDrawLayer(paint), blackColor);
    paint.drawLine(240, 7, 240, 188, getDrawLayer(paint), blackColor);
    paint.updateSubLayers = true;
}

void ColorPicker::drawPickerPointers(Paint& paint) {
    paint.updateSubLayers = false;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            paint.blendSubLayers(colorXOld * 4 + 63 + i, 23 + j);
        }
    }
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            paint.blendSubLayers(colorXOld * 4 + 63 + i, 162 + j);
        }
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            paint.blendSubLayers(55 + i, colorYOld * 4 + 31 + j);
        }
    }
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            paint.blendSubLayers(194 + i, colorYOld * 4 + 31 + j);
        }
    }

    paint.drawSquare(colorX * 4 + 63, 23, 6, 6, getDrawLayer(paint), blackColor);
    paint.drawSquare(colorX * 4 + 64, 24, 4, 4, getDrawLayer(paint), whiteColor);

    paint.drawSquare(colorX * 4 + 63, 162, 6, 6, getDrawLayer(paint), blackColor);
    paint.drawSquare(colorX * 4 + 64, 163, 4, 4, getDrawLayer(paint), whiteColor);

    paint.drawSquare(55, colorY * 4 + 31, 6, 6, getDrawLayer(paint), blackColor);
    paint.drawSquare(56, colorY * 4 + 32, 4, 4, getDrawLayer(paint), whiteColor);

    paint.drawSquare(194, colorY * 4 + 31, 6, 6, getDrawLayer(paint), blackColor);
    paint.drawSquare(195, colorY * 4 + 32, 4, 4, getDrawLayer(paint), whiteColor);

    paint.drawLine(colorX * 4 + 63, colorY * 4 + 31, colorX * 4 + 68, colorY * 4 + 31, getDrawLayer(paint), blackColor);
    paint.drawLine(colorX * 4 + 63, colorY * 4 + 36, colorX * 4 + 68, colorY * 4 + 36, getDrawLayer(paint), blackColor);
    paint.drawLine(colorX * 4 + 63, colorY * 4 + 31, colorX * 4 + 63, colorY * 4 + 36, getDrawLayer(paint), blackColor);
    paint.drawLine(colorX * 4 + 68, colorY * 4 + 31, colorX * 4 + 68, colorY * 4 + 36, getDrawLayer(paint), blackColor);

    paint.updateSubLayers = true;

    colorXOld = colorX;
    colorYOld = colorY;
}

void ColorPicker::drawHuePointer(Paint& paint) {
    paint.updateSubLayers = false;
    int x = 0;
    int y = hueOld;
    if (hueOld >= 180) {
        y = y - 180;
        x = x + 40;
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            paint.blendSubLayers(x + 202 + i, y + 7 + j);
        }
    }

    x = 0;
    y = hue;
    if (hue >= 180) {
        y = y - 180;
        x = x + 40;
    }
    paint.drawSquare(x + 202, y + 7, 4, 3, getDrawLayer(paint), blackColor);
    paint.drawSquare(x + 202 + 1, y + 8, 2, 1, getDrawLayer(paint), whiteColor);
    paint.updateSubLayers = true;

    hueOld = hue;
}