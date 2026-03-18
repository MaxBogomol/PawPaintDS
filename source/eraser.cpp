#include "eraser.h"

#include "paint.h"

const char* Eraser::getName(Paint& paint) {
    return "Eraser";
}

u16 Eraser::getSelectedColor(Paint& paint) {
    return alphaColor;
}