#include "brush.h"

#include "paint.h"

const char* Brush::getName(Paint& paint) {
    return "Brush";
}

void Brush::update(Paint& paint) {
    if (touchCount > 1) {
        paint.updateSubLayersEnable();
        drawLine(paint, touchXOld, touchYOld, touchX, touchY, getSelectedLayer(paint), getSelectedColor(paint));
        paint.updateSubLayersDisable();
    }

    if (keysD & KEY_Y) {
        u16 color = paint.selectedColorSub;
        paint.selectedColorSub = paint.selectedColor;
        paint.selectedColor = color;
        paint.updateDrawColors = true;
    }
}

void Brush::drawLine(Paint& paint, int x0, int y0, int x1, int y1, u16* buffer, u16 color) {
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
		paint.drawCircleDiameter(x0, y0, 3, buffer, color);

        if (x0 == x1 && y0 == y1) break;
        
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

u16 *Brush::getSelectedLayer(Paint& paint) {
    return paint.getSelectedLayer();
}

u16 Brush::getSelectedColor(Paint& paint) {
    return paint.getSelectedColor();
}