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
    updateDrawTool = true;
}

void Brush::update(Paint& paint) {
    if (touchCount > 1 && !paint.reverseScreens) {
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

    if (keysD & KEY_UP) {
        if (line - 1 >= 0) {
            line--;
            updateDrawTool = true;
        }
    }
    if (keysD & KEY_DOWN) {
        if (line + 1 < 3) {
            line++;
            updateDrawTool = true;
        }
    }

    switch (line) {
        case 1: {
            if (keysD & KEY_LEFT) {
                type--;
                if (type < 0) type = 5;
                updateDrawTool = true;
                paint.updateDrawTools = true;
            }
            if (keysD & KEY_RIGHT) {
                type++;
                if (type > 5) type = 0;
                updateDrawTool = true;
                paint.updateDrawTools = true;
            }
            break;
        }
        case 2: {
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

    if (keysD & KEY_TOUCH && paint.reverseScreens) {
        if (touchX >= 176 && touchX < 176 + 8 && touchY >= 57 && touchY < 57 + 8) {
            type--;
            if (type < 0) type = 5;
            updateDrawTool = true;
            paint.updateDrawTools = true;
        }
        if (touchX >= 192 && touchX < 192 + 8 && touchY >= 57 && touchY < 57 + 8) {
            type++;
            if (type > 5) type = 0;
            updateDrawTool = true;
            paint.updateDrawTools = true;
        }
    }

    switch (type) {
        case 0: {
            if (touchCount >= 1 && paint.reverseScreens) {
                if (touchX >= 176 && touchX < 176 + 64 && touchY >= 66 && touchY < 66 + 8) {
                    squareSize = touchX - 176 + 1;
                    updateDrawTool = true;
                }
            }
        }
        case 1: {
            if (touchCount >= 1 && paint.reverseScreens) {
                if (touchX >= 176 && touchX < 176 + 64 && touchY >= 66 && touchY < 66 + 8) {
                    circleDiameter = touchX - 176 + 1;
                    updateDrawTool = true;
                }
            }
        }
        case 2: {
            if (touchCount >= 1 && paint.reverseScreens) {
                if (touchX >= 176 && touchX < 176 + 32 && touchY >= 66 && touchY < 66 + 8) {
                    dotRadius = touchX - 176 + 1;
                    updateDrawTool = true;
                }
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
    line = 0;
    updateDrawTool = true;
}

void Brush::close(Paint& paint) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 26; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }
}

void Brush::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    switch (type) {
    	case 0: return paint.drawSprite(x, y, 16, 16, brush_square_iconBitmap, brush_square_iconPal, pixelBufferMain); break;
    	case 1: return paint.drawSprite(x, y, 16, 16, brush_circle_iconBitmap, brush_circle_iconPal, pixelBufferMain); break;
		case 2: return paint.drawSprite(x, y, 16, 16, brush_dot_iconBitmap, brush_dot_iconPal, pixelBufferMain); break;
		case 3: return paint.drawSprite(x, y, 16, 16, brush_square_noise_iconBitmap, brush_square_noise_iconPal, pixelBufferMain); break;
    	case 4: return paint.drawSprite(x, y, 16, 16, brush_circle_noise_iconBitmap, brush_circle_noise_iconPal, pixelBufferMain); break;
		case 5: return paint.drawSprite(x, y, 16, 16, brush_dot_noise_iconBitmap, brush_dot_noise_iconPal, pixelBufferMain); break;
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
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 26; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }

    string moveString = string((line == 0) ? ">" : "") + "Move: " + "-"; 
    paint.drawText(3, 48, moveString.c_str(), pixelBufferMain, blackColor);
    paint.drawText(176, 48, "+", pixelBufferMain, blackColor);

    string typeString = string((line == 1) ? ">" : "") + "Type: " + getTypeName(paint, type); 
    paint.drawText(3, 57, typeString.c_str(), pixelBufferMain, blackColor);
    paint.drawText(176, 57, "- +", pixelBufferMain, blackColor);

    switch (type) {
    	case 0: {
            string diameterString = string((line == 2) ? ">" : "") + "Size: " + paint.intToChars(squareSize);
            paint.drawText(3, 66, diameterString.c_str(), pixelBufferMain, blackColor);

            paint.drawLine(176, 67, 176 + 63, 67, pixelBufferMain, blackColor);
            paint.drawSquareOutline(176 - 1 + (squareSize - 1), 67 + 2, 3, 4, pixelBufferMain, blackColor);
            break;
        }
        case 1: {
            string diameterString = string((line == 2) ? ">" : "") + "Diameter: " + paint.intToChars(circleDiameter);
            paint.drawText(3, 66, diameterString.c_str(), pixelBufferMain, blackColor);

            paint.drawLine(176, 67, 176 + 63, 67, pixelBufferMain, blackColor);
            paint.drawSquareOutline(176 - 1 + (circleDiameter - 1), 67 + 2, 3, 4, pixelBufferMain, blackColor);
            break;
        }
        case 2: {
            string radiusString = string((line == 2) ? ">" : "") + "Radius: " + paint.intToChars(dotRadius);
            paint.drawText(3, 66, radiusString.c_str(), pixelBufferMain, blackColor);

            paint.drawLine(176, 67, 176 + 31, 67, pixelBufferMain, blackColor);
            paint.drawSquareOutline(176 - 1 + (dotRadius - 1), 67 + 2, 3, 4, pixelBufferMain, blackColor);
            break;
        }
    }
}

const char* Brush::getTypeName(Paint& paint, int type) {
	switch (type) {
    	case 0: return "Square";
    	case 1: return "Circle";
		case 2: return "Dot";
		case 3: return "Square Noise";
    	case 4: return "Circle Noise";
		case 5: return "Dot Noise";
    }
	return "Type";
}

u16 *Brush::getSelectedLayer(Paint& paint) {
    return paint.getSelectedLayer();
}

u16 Brush::getSelectedColor(Paint& paint) {
    return paint.getSelectedColor();
}