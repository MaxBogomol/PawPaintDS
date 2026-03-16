#include "eyedropper.h"

#include "paint.h"

void Eyedropper::update(Paint& paint) {
    if (touchCount > 1) {
        u16 color = paint.getPixel(touchX, touchY, pixelBufferSub);
        if ((color >> 15) & 1 == 0) color = blackColor;
        paint.selectedColor = color;
    }
}

void Eyedropper::open(Paint& paint) {

}


void Eyedropper::close(Paint& paint) {

}