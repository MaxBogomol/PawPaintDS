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
    noiseXSize = 2;
    noiseYSize = 2;
    noiseXShift = 0;
    noiseYShift = 0;
    noiseXOffset = 0;
    noiseYOffset = 0;
    cursorX = 0;
    cursorY = 0;
    cursorXOld = 0;
    cursorYOld = 0;
    active = false;
    activeNoise = false;
    updateDrawTool = true;
    updateDrawCursor = false;
}

void Brush::update(Paint& paint) {
    if (touchCount > 1 && !paint.reverseScreens) {
        if (line == 0 && active) {
            updateDrawCursor = true;
        } else {
            paint.updateSubLayersEnable();
            drawLine(paint, touchXOld, touchYOld, touchX, touchY, getSelectedLayer(paint), getSelectedColor(paint));
            paint.updateSubLayersDisable();
        }

        cursorX = touchX;
        cursorY = touchY;
    }

    if (keysD & KEY_Y) {
        u16 color = paint.selectedColorSub;
        paint.selectedColorSub = paint.selectedColor;
        paint.selectedColor = color;
        paint.updateDrawColors = true;
        if (line == 0) updateDrawCursor = true;
    }

    if (keysD & KEY_A) {
        if (line == 0 || line >= 3) {
            active = !active;
            activeNoise = false;
            updateDrawTool = true;
        }
        if (line == 0) updateDrawCursor = true;
    }

    int maxLine = 3;
    if (type >= 3) maxLine = 6;

    if (!active) {
        if ((keysD & KEY_UP) && (line - 1 >= 0)) {
            line--;
            updateDrawTool = true;
        }
        if ((keysD & KEY_DOWN) && (line + 1 < maxLine)) {
            line++;
            updateDrawTool = true;
        }
    } else {
        if (line == 0) {
            if ((keysR & KEY_LEFT) && (cursorX - 1 >= 0)) {
                cursorX--;
                updateDrawCursor = true;
            }
            if ((keysR & KEY_RIGHT) && (cursorX + 1 < SCREEN_WIDTH)) {
                cursorX++;
                updateDrawCursor = true;
            }
            if ((keysR & KEY_UP) && (cursorY - 1 >= 0)) {
                cursorY--;
                updateDrawCursor = true;
            }
            if ((keysR & KEY_DOWN) && (cursorY + 1 < SCREEN_HEIGHT)) {
                cursorY++;
                updateDrawCursor = true;
            }
            if (keysH & KEY_B) {
                paint.updateSubLayersEnable();
                drawLine(paint, cursorX, cursorY, cursorX, cursorY, getSelectedLayer(paint), getSelectedColor(paint));
                paint.updateSubLayersDisable();
            }
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
                case 0:
                case 3: {
                    if ((keysR & KEY_LEFT) && (squareSize - 1 >= 1)) {
                        squareSize--;
                        updateDrawTool = true;
                    }
                    if ((keysR & KEY_RIGHT) && (squareSize + 1 <= 64)) {
                        squareSize++;
                        updateDrawTool = true;
                    }
                    break;
                }
                case 1:
                case 4: {
                    if ((keysR & KEY_LEFT) && (circleDiameter - 1 >= 1)) {
                        circleDiameter--;
                        updateDrawTool = true;
                    }
                    if ((keysR & KEY_RIGHT) && (circleDiameter + 1 <= 64)) {
                        circleDiameter++;
                        updateDrawTool = true;
                    }
                    break;
                }
                case 2:
                case 5: {
                    if ((keysR & KEY_LEFT) && (dotRadius - 1 >= 1)) {
                        dotRadius--;
                        updateDrawTool = true;
                    }
                    if ((keysR & KEY_RIGHT) && (dotRadius + 1 <= 32)) {
                        dotRadius++;
                        updateDrawTool = true;
                    }
                    break;
                }
            }
            break;
        }
    }

    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();

    if (keysD & KEY_TOUCH && paint.reverseScreens) {
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset + 8 && touchY >= yOffset && touchY < yOffset + 8) {
            active = !active;
            line = 0;
            updateDrawTool = true;
            updateDrawCursor = true;
        }
        yOffset += 10;
        if (touchX >= SCREEN_WIDTH - bOffset - 16 - 5 && touchX < SCREEN_WIDTH - bOffset - 16 - 5 + 8 && touchY >= yOffset && touchY < yOffset + 8) {
            drawCursor(paint, true);
            type--;
            if (type < 0) type = 5;
            updateDrawTool = true;
            updateDrawCursor = true;
            paint.updateDrawTools = true;
        }
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset + 8 && touchY >= yOffset && touchY < yOffset + 8) {
            drawCursor(paint, true);
            type++;
            if (type > 5) type = 0;
            updateDrawTool = true;
            updateDrawCursor = true;
            paint.updateDrawTools = true;
        }
    } else {
        yOffset += 10;
    }
    yOffset += 10;

    if (touchCount >= 1 && paint.reverseScreens) {
        switch (type) {
            case 0:
            case 3: {
                if (touchX >= SCREEN_WIDTH - bOffset - 64 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
                    drawCursor(paint, true);
                    squareSize = touchX - (SCREEN_WIDTH - bOffset - 64) + 1;
                    updateDrawTool = true;
                    updateDrawCursor = true;
                }
                break;
            }
            case 1:
            case 4: {
                if (touchX >= SCREEN_WIDTH - bOffset - 64 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
                    drawCursor(paint, true);
                    circleDiameter = touchX - (SCREEN_WIDTH - bOffset - 64) + 1;
                    updateDrawTool = true;
                    updateDrawCursor = true;
                }
                break;
            }
            case 2:
            case 5: {
                if (touchX >= SCREEN_WIDTH - bOffset - 32 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
                    drawCursor(paint, true);
                    dotRadius = touchX - (SCREEN_WIDTH - bOffset - 32) + 1;
                    updateDrawTool = true;
                    updateDrawCursor = true;
                }
                break;
            }
        }
    }

    if (type >= 3) {
        if (active) {
            if ((keysD & KEY_UP) || (keysD & KEY_DOWN)) {
                activeNoise = !activeNoise;
                updateDrawTool = true;
            }

            switch (line) {
                case 3: {
                    if (!activeNoise) {
                        if ((keysR & KEY_LEFT) && (noiseXSize - 1 >= 1)) {
                            noiseXSize--;
                            updateDrawTool = true;
                        }
                        if ((keysR & KEY_RIGHT) && (noiseXSize + 1 <= 16)) {
                            noiseXSize++;
                            updateDrawTool = true;
                        }
                    } else {
                        if ((keysR & KEY_LEFT) && (noiseYSize - 1 >= 1)) {
                            noiseYSize--;
                            updateDrawTool = true;
                        }
                        if ((keysR & KEY_RIGHT) && (noiseYSize + 1 <= 16)) {
                            noiseYSize++;
                            updateDrawTool = true;
                        }
                    }
                    break;
                }
                case 4: {
                    if (!activeNoise) {
                        if ((keysR & KEY_LEFT) && (noiseXShift - 1 >= 0)) {
                            noiseXShift--;
                            updateDrawTool = true;
                        }
                        if ((keysR & KEY_RIGHT) && (noiseXShift + 1 < 16)) {
                            noiseXShift++;
                            updateDrawTool = true;
                        }
                    } else {
                        if ((keysR & KEY_LEFT) && (noiseYShift - 1 >= 0)) {
                            noiseYShift--;
                            updateDrawTool = true;
                        }
                        if ((keysR & KEY_RIGHT) && (noiseYShift + 1 < 16)) {
                            noiseYShift++;
                            updateDrawTool = true;
                        }
                    }
                    break;
                }
                case 5: {
                    if (!activeNoise) {
                        if ((keysR & KEY_LEFT) && (noiseXOffset - 1 >= 0)) {
                            noiseXOffset--;
                            updateDrawTool = true;
                        }
                        if ((keysR & KEY_RIGHT) && (noiseXOffset + 1 < 16)) {
                            noiseXOffset++;
                            updateDrawTool = true;
                        }
                    } else {
                        if ((keysR & KEY_LEFT) && (noiseYOffset - 1 >= 0)) {
                            noiseYOffset--;
                            updateDrawTool = true;
                        }
                        if ((keysR & KEY_RIGHT) && (noiseYOffset + 1 < 16)) {
                            noiseYOffset++;
                            updateDrawTool = true;
                        }
                    }
                    break;
                }
            }
        }

        if (touchCount >= 1 && paint.reverseScreens) {
            yOffset += 10;
            if (touchX >= SCREEN_WIDTH - bOffset - 16 - 24 && touchX < SCREEN_WIDTH - bOffset - 24 && touchY >= yOffset && touchY < yOffset + 8) {
                drawCursor(paint, true);
                noiseXSize = touchX - (SCREEN_WIDTH - bOffset - 16 - 24) + 1;
                updateDrawTool = true;
                updateDrawCursor = true;
            }
            if (touchX >= SCREEN_WIDTH - bOffset - 16 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
                drawCursor(paint, true);
                noiseYSize = touchX - (SCREEN_WIDTH - bOffset - 16) + 1;
                updateDrawTool = true;
                updateDrawCursor = true;
            }
            yOffset += 10;
            if (touchX >= SCREEN_WIDTH - bOffset - 16 - 24 && touchX < SCREEN_WIDTH - bOffset - 24 && touchY >= yOffset && touchY < yOffset + 8) {
                drawCursor(paint, true);
                noiseXShift = touchX - (SCREEN_WIDTH - bOffset - 16 - 24);
                updateDrawTool = true;
                updateDrawCursor = true;
            }
            if (touchX >= SCREEN_WIDTH - bOffset - 16 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
                drawCursor(paint, true);
                noiseYShift = touchX - (SCREEN_WIDTH - bOffset - 16);
                updateDrawTool = true;
                updateDrawCursor = true;
            }
            yOffset += 10;
            if (touchX >= SCREEN_WIDTH - bOffset - 16 - 24 && touchX < SCREEN_WIDTH - bOffset - 24 && touchY >= yOffset && touchY < yOffset + 8) {
                drawCursor(paint, true);
                noiseXOffset = touchX - (SCREEN_WIDTH - bOffset - 16 - 24);
                updateDrawTool = true;
                updateDrawCursor = true;
            }
            if (touchX >= SCREEN_WIDTH - bOffset - 16 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
                drawCursor(paint, true);
                noiseYOffset = touchX - (SCREEN_WIDTH - bOffset - 16);
                updateDrawTool = true;
                updateDrawCursor = true;
            }
        }
    }

    if (paint.updateDrawSelectedColor && (line == 0)) updateDrawCursor = true;

    if (updateDrawCursor) {
        drawCursor(paint);
        updateDrawCursor = false;
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
    active = false;
    activeNoise = false;
    updateDrawTool = true;
    updateDrawCursor = false;
}

void Brush::close(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 2, SCREEN_WIDTH, 62, pixelBufferMain);

    active = false;
    drawCursor(paint);
}

void Brush::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    const unsigned int* iconSprite = brush_square_iconBitmap;
    switch (type) {
    	case 0: iconSprite = brush_square_iconBitmap; break;
    	case 1: iconSprite = brush_circle_iconBitmap; break;
		case 2: iconSprite = brush_dot_iconBitmap; break;
		case 3: iconSprite = brush_square_noise_iconBitmap; break;
    	case 4: iconSprite = brush_circle_noise_iconBitmap; break;
		case 5: iconSprite = brush_dot_noise_iconBitmap; break;
    }
    paint.drawSprite(x, y, 16, 16, iconSprite, buffer);
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
            case 3: {
                int center = squareSize / 2;
                paint.drawSquareNoise(x0 - center, y0 - center, squareSize, squareSize, buffer, color, noiseXSize, noiseYSize, noiseXShift, noiseYShift, noiseXOffset, noiseYOffset);
                break;
            }
            case 4: {
                paint.drawCircleDiameterNoise(x0, y0, circleDiameter, buffer, color, noiseXSize, noiseYSize, noiseXShift, noiseYShift, noiseXOffset, noiseYOffset);
                break;
            }
            case 5: {
                paint.drawCircleRadiusNoise(x0, y0, dotRadius - 1, buffer, color, noiseXSize, noiseYSize, noiseXShift, noiseYShift, noiseXOffset, noiseYOffset);
                break;
            }
        }

        if (x0 == x1 && y0 == y1) break;
        
        e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx; 
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void Brush::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 2, SCREEN_WIDTH, 62, pixelBufferMain);

    string moveString = string((line == 0) ? ">" : "") + "Move: " + ((line == 0 && active) ? "+" : "-"); 
    paint.drawText(3, yOffset, moveString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string typeString = string((line == 1) ? ">" : "") + "Type: " + getTypeName(paint, type); 
    paint.drawText(3, yOffset += 10, typeString.c_str(), pixelBufferMain, blackColor);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 16 - 5, yOffset, 32, 32, 24, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 8, yOffset, 32, 32, 8, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);

    yOffset += 10;

    switch (type) {
        case 0:
    	case 3: {
            string sizeString = string((line == 2) ? ">" : "") + "Size: " + paint.intToChars(squareSize);
            paint.drawText(3, yOffset, sizeString.c_str(), pixelBufferMain, blackColor);
            paint.drawScrollBox(SCREEN_WIDTH - bOffset - 64, yOffset + 1, 64, squareSize - 1, pixelBufferMain);
            break;
        }
        case 1:
        case 4: {
            string diameterString = string((line == 2) ? ">" : "") + "Diameter: " + paint.intToChars(circleDiameter);
            paint.drawText(3, yOffset, diameterString.c_str(), pixelBufferMain, blackColor);
            paint.drawScrollBox(SCREEN_WIDTH - bOffset - 64, yOffset + 1, 64, circleDiameter - 1, pixelBufferMain);
            break;
        }
        case 2:
        case 5: {
            string radiusString = string((line == 2) ? ">" : "") + "Radius: " + paint.intToChars(dotRadius);
            paint.drawText(3, yOffset, radiusString.c_str(), pixelBufferMain, blackColor);
            paint.drawScrollBox(SCREEN_WIDTH - bOffset - 32, yOffset + 1, 32, dotRadius - 1, pixelBufferMain);
            break;
        }
    }

    if (type >= 3) {
        string noiseSizeString = string((line == 3 && !active) ? ">" : "") + "Noise Size: " + ((line == 3 && active && !activeNoise) ? ">" : "") + paint.intToChars(noiseXSize) + " " + ((line == 3 && active && activeNoise) ? ">" : "") + paint.intToChars(noiseYSize);
        paint.drawText(3, yOffset += 10, noiseSizeString.c_str(), pixelBufferMain, blackColor);
        paint.drawScrollBox(SCREEN_WIDTH - bOffset - 40, yOffset + 1, 16, noiseXSize - 1, pixelBufferMain);
        paint.drawScrollBox(SCREEN_WIDTH - bOffset - 16, yOffset + 1, 16, noiseYSize - 1, pixelBufferMain);

        string noiseShiftString = string((line == 4 && !active) ? ">" : "") + "Noise Shift: " + ((line == 4 && active && !activeNoise) ? ">" : "") + paint.intToChars(noiseXShift) + " " + ((line == 4 && active && activeNoise) ? ">" : "") + paint.intToChars(noiseYShift);
        paint.drawText(3, yOffset += 10, noiseShiftString.c_str(), pixelBufferMain, blackColor);
        paint.drawScrollBox(SCREEN_WIDTH - bOffset - 40, yOffset + 1, 16, noiseXShift, pixelBufferMain);
        paint.drawScrollBox(SCREEN_WIDTH - bOffset - 16, yOffset + 1, 16, noiseYShift, pixelBufferMain);

        string noiseOffsetString = string((line == 5 && !active) ? ">" : "") + "Noise Offset: " + ((line == 5 && active && !activeNoise) ? ">" : "") + paint.intToChars(noiseXOffset) + " " + ((line == 5 && active && activeNoise) ? ">" : "") + paint.intToChars(noiseYOffset);
        paint.drawText(3, yOffset += 10, noiseOffsetString.c_str(), pixelBufferMain, blackColor);
        paint.drawScrollBox(SCREEN_WIDTH - bOffset - 40, yOffset + 1, 16, noiseXOffset, pixelBufferMain);
        paint.drawScrollBox(SCREEN_WIDTH - bOffset - 16, yOffset + 1, 16, noiseYOffset, pixelBufferMain);
    }
}

void Brush::drawCursor(Paint& paint, bool clear) {
    int size = 0;
    switch (type) {
        case 0:
    	case 3: size = squareSize; break;
        case 1:
    	case 4: size = circleDiameter; break;
        case 2:
    	case 5: size = dotRadius * 2; break;
    }
    size = size + 2;
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            paint.blendSubLayers(cursorXOld + x - (size / 2), cursorYOld + y - (size / 2));
        }
    }

    if (active && !clear) drawLine(paint, cursorX, cursorY, cursorX, cursorY, pixelBufferSub, getSelectedColor(paint));

    cursorXOld = cursorX;
    cursorYOld = cursorY;
}

void Brush::drawCursor(Paint& paint) {
    drawCursor(paint, false);
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