#include "brush.h"

#include "paint.h"

const char* Brush::getName(Paint& paint) {
    return "Brush";
}

void Brush::setup(Paint& paint) {
    type = 0;
    line = 0;
    squareSize = 1;
    circleDiameter = 1;
    dotRadius = 1;
    active = false;
    updateDrawTool = true;
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

    if (keysD & KEY_A) {
        active = !active;
        updateDrawTool = true;
    }

    if (active) {
        if (keysD & KEY_UP) {
            if (line - 1 >= 0) {
                line--;
                updateDrawTool = true;
            }
        }
        if (keysD & KEY_DOWN) {
            if (line + 1 < 2) {
                line++;
                updateDrawTool = true;
            }
        }

        switch (line) {
            case 0: {
                if (keysD & KEY_LEFT) {
                    type--;
                    if (type < 0) type = 3;
                    updateDrawTool = true;
                }
                if (keysD & KEY_RIGHT) {
                    type++;
                    if (type > 3) type = 0;
                    updateDrawTool = true;
                }
                break;
            }
            case 1: {
                switch (type) {
                    case 0: {
                        if (keysD & KEY_LEFT) {
                            if (squareSize - 1 >= 1) {
                                squareSize--;
                                updateDrawTool = true;
                            }
                        }
                        if (keysD & KEY_RIGHT) {
                            if (squareSize + 1 <= 64) {
                                squareSize++;
                                updateDrawTool = true;
                            }
                        }
                        break;
                    }
                    case 1: {
                        if (keysD & KEY_LEFT) {
                            if (circleDiameter - 1 >= 1) {
                                circleDiameter--;
                                updateDrawTool = true;
                            }
                        }
                        if (keysD & KEY_RIGHT) {
                            if (circleDiameter + 1 <= 64) {
                                circleDiameter++;
                                updateDrawTool = true;
                            }
                        }
                        break;
                    }
                    case 2: {
                        if (keysD & KEY_LEFT) {
                            if (dotRadius - 1 >= 1) {
                                dotRadius--;
                                updateDrawTool = true;
                            }
                        }
                        if (keysD & KEY_RIGHT) {
                            if (dotRadius + 1 <= 32) {
                                dotRadius++;
                                updateDrawTool = true;
                            }
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
}

void Brush::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void Brush::open(Paint& paint) {
    active = false;
    updateDrawTool = true;
}

void Brush::close(Paint& paint) {
    for (int x = 0; x < 144; x++) {
        for (int y = 0; y < 17; y++) {
            paint.drawPixel(x + 3, y + 48, pixelBufferMain, whiteColor);
        }
    }
}

void Brush::drawLine(Paint& paint, int x0, int y0, int x1, int y1, u16* buffer, u16 color) {
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        switch (type) {
            case 0: {
                int center = squareSize / 2;
                paint.drawSquare(x0 - center, y0 - center, squareSize, squareSize, buffer, color);
                break;
            }
            case 1: {
                paint.drawCircleDiameter(x0, y0, circleDiameter, buffer, color);
                break;
            }
            case 2: {
                paint.drawCircleRadius(x0, y0, dotRadius - 1, buffer, color);
                break;
            }
        }

        if (x0 == x1 && y0 == y1) break;
        
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void Brush::drawTool(Paint& paint) {
    for (int x = 0; x < 144; x++) {
        for (int y = 0; y < 17; y++) {
            paint.drawPixel(x + 3, y + 48, pixelBufferMain, whiteColor);
        }
    }

    string typeString = string((active && line == 0) ? ">" : "") + "Type: " + getTypeName(paint, type); 
    paint.drawText(3, 48, typeString.c_str(), pixelBufferMain, blackColor);

    switch (type) {
    	case 0: {
            string diameterString = string((active && line == 1) ? ">" : "") + "Size: " + paint.intToChars(squareSize);
            paint.drawText(3, 57, diameterString.c_str(), pixelBufferMain, blackColor);
            break;
        }
        case 1: {
            string diameterString = string((active && line == 1) ? ">" : "") + "Diameter: " + paint.intToChars(circleDiameter);
            paint.drawText(3, 57, diameterString.c_str(), pixelBufferMain, blackColor);
            break;
        }
        case 2: {
            string radiusString = string((active && line == 1) ? ">" : "") + "Radius: " + paint.intToChars(dotRadius);
            paint.drawText(3, 57, radiusString.c_str(), pixelBufferMain, blackColor);
            break;
        }
    }
}

const char* Brush::getTypeName(Paint& paint, int type) {
	switch (type) {
    	case 0: return "Square";
    	case 1: return "Circle";
		case 2: return "Dot";
		case 3: return "Noise";
    }
	return "Type";
}

u16 *Brush::getSelectedLayer(Paint& paint) {
    return paint.getSelectedLayer();
}

u16 Brush::getSelectedColor(Paint& paint) {
    return paint.getSelectedColor();
}