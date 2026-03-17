#include "eraser.h"

#include "paint.h"

const char* Eraser::getName(Paint& paint) {
    return "Eraser";
}

void Eraser::update(Paint& paint) {
    if (touchCount > 1) {
        paint.drawLine(touchXOld, touchYOld, touchX, touchY, paint.getSelectedLayer(), alphaColor);
    }
}

void Eraser::open(Paint& paint) {

}


void Eraser::close(Paint& paint) {

}