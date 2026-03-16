#include "color_picker.h"

#include "paint.h"

void ColorPicker::setup(Paint& paint) {
    hue = 0;
    updatePicker = false;
    updateSelected = false;
    selectedColor = blackColor;
    newSelectedColor = blackColor;
}

void ColorPicker::update(Paint& paint) {
    if (touchCount > 0) {
        if (touchX >= 64 && touchX < 192 && touchY >= 32 && touchY < 160) {
            newSelectedColor = pixelBufferSub[touchX + (touchY * 256)];
            updateSelected = true;
        }
        if (touchX >= 208 && touchX < 224 && touchY >= 8 && touchY < 188) {
            hue = touchY - 8;
            updatePicker = true;
        }
        if (touchX >= 224 && touchX < 240 && touchY >= 8 && touchY < 188) {
            hue = touchY - 8 + 180;
            updatePicker = true;
        }
    }

    if (keysD & KEY_A) {
        selectedColor = newSelectedColor;
        paint.selectedColor = newSelectedColor;

        paint.updateSubLayers = false;
        paint.drawSquare(24, 96, 16, 16, pixelBufferSub, selectedColor);

        for (int x = 0; x < 144; x++) {
            for (int y = 0; y < 8; y++) {
                paint.blendSubLayers(x + 24, y + 168);
            }
        }

        int r = (selectedColor) & 31;
        int g = (selectedColor >> 5) & 31;
        int b = (selectedColor >> 10) & 31;

        string colorString = string("RGB: (") + paint.intToChars(r) + ", " + paint.intToChars(g) + ", " + paint.intToChars(b) + ")"; 
        paint.drawText(24, 168, colorString.c_str(), pixelBufferSub, blackColor);

        paint.updateSubLayers = true;
    }

    if (updatePicker) {
        paint.updateSubLayers = false;
        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 32; x++) {
                u16 color = paint.HSVtoRGB(hue, x * 8, 255 - (y * 8));
                updatePicker = false;
                paint.drawSquare(x * 4 + 64, y * 4 + 32, 4, 4, pixelBufferSub, color);
            }
        }
        paint.updateSubLayers = true;
        updatePicker = false;
    }

    if (updateSelected) {
        paint.updateSubLayers = false;
        paint.drawSquare(24, 80, 16, 16, pixelBufferSub, newSelectedColor);

        for (int x = 0; x < 144; x++) {
            for (int y = 0; y < 8; y++) {
                paint.blendSubLayers(x + 24, y + 16);
            }
        }

        int r = (newSelectedColor) & 31;
        int g = (newSelectedColor >> 5) & 31;
        int b = (newSelectedColor >> 10) & 31;

        string colorString = string("RGB: (") + paint.intToChars(r) + ", " + paint.intToChars(g) + ", " + paint.intToChars(b) + ")"; 
        paint.drawText(24, 16, colorString.c_str(), pixelBufferSub, blackColor);

        paint.updateSubLayers = true;
        updateSelected = false;
    }
}

void ColorPicker::open(Paint& paint) {
    selectedColor = paint.selectedColor;
    newSelectedColor = paint.selectedColor;

    paint.updateSubLayers = false;
    for (int y = 0; y < 180; y++) {
        u16 color = paint.HSVtoRGB(y, 255, 255);
        paint.drawSquare(208, y + 8, 16, 1, pixelBufferSub, color);
    }
    for (int y = 0; y < 180; y++) {
        u16 color = paint.HSVtoRGB(y + 180, 255, 255);
        paint.drawSquare(224, y + 8, 16, 1, pixelBufferSub, color);
    }

    paint.drawSquare(24, 96, 16, 16, pixelBufferSub, selectedColor);
    int r = (selectedColor) & 31;
    int g = (selectedColor >> 5) & 31;
    int b = (selectedColor >> 10) & 31;

    string colorString = string("RGB: (") + paint.intToChars(r) + ", " + paint.intToChars(g) + ", " + paint.intToChars(b) + ")"; 
    paint.drawText(24, 168, colorString.c_str(), pixelBufferSub, blackColor);

    paint.updateSubLayers = true;
    updatePicker = true;
    updateSelected = true;
}


void ColorPicker::close(Paint& paint) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            paint.blendSubLayers(x, y);
        }
    }
}