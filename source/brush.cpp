#include "brush.h"

#include "paint.h"

void Brush::update(Paint& paint) {
    if (touchCount > 1) {
        paint.drawLine(touchXOld, touchYOld, touchX, touchY, paint.getSelectedLayer(), paint.getSelectedColor());
    }

    if (keysD & KEY_Y) {
        u16 color = paint.selectedColorSub;
        paint.selectedColorSub = paint.selectedColor;
        paint.selectedColor = color;
    }
}

void Brush::open(Paint& paint) {

}


void Brush::close(Paint& paint) {

}