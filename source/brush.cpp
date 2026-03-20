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
        if (keysD & KEY_UP) {
            if (line - 1 >= 0) {
                line--;
                updateDrawTool = true;
            }
        }
        if (keysD & KEY_DOWN) {
            if (line + 1 < maxLine) {
                line++;
                updateDrawTool = true;paint.updateSubLayersDisable();
            }
        }
    } else {
        if (line == 0) {
            if (keysR & KEY_LEFT) {
                if (cursorX - 1 >= 0) {
                    cursorX--;
                    updateDrawCursor = true;
                }
            }
            if (keysR & KEY_RIGHT) {
                if (cursorX + 1 < SCREEN_WIDTH) {
                    cursorX++;
                    updateDrawCursor = true;
                }
            }
            if (keysR & KEY_UP) {
                if (cursorY - 1 >= 0) {
                    cursorY--;
                    updateDrawCursor = true;
                }
            }
            if (keysR & KEY_DOWN) {
                if (cursorY + 1 < SCREEN_HEIGHT) {
                    cursorY++;
                    updateDrawCursor = true;
                }
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
                    if (keysR & KEY_LEFT) {
                        if (squareSize - 1 >= 1) {
                            squareSize--;
                            updateDrawTool = true;
                        }
                    }
                    if (keysR & KEY_RIGHT) {
                        if (squareSize + 1 <= 64) {
                            squareSize++;
                            updateDrawTool = true;
                        }
                    }
                    break;
                }
                case 1:
                case 4: {
                    if (keysR & KEY_LEFT) {
                        if (circleDiameter - 1 >= 1) {
                            circleDiameter--;
                            updateDrawTool = true;
                        }
                    }
                    if (keysR & KEY_RIGHT) {
                        if (circleDiameter + 1 <= 64) {
                            circleDiameter++;
                            updateDrawTool = true;
                        }
                    }
                    break;
                }
                case 2:
                case 5: {
                    if (keysR & KEY_LEFT) {
                        if (dotRadius - 1 >= 1) {
                            dotRadius--;
                            updateDrawTool = true;
                        }
                    }
                    if (keysR & KEY_RIGHT) {
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
        if (touchX >= 176 && touchX < 176 + 8 && touchY >= 48 && touchY < 48 + 8) {
            active = !active;
            line = 0;
            updateDrawCursor = true;
            updateDrawTool = true;
        }
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
        case 0:
        case 3: {
            if (touchCount >= 1 && paint.reverseScreens) {
                if (touchX >= 176 && touchX < 176 + 64 && touchY >= 66 && touchY < 66 + 8) {
                    squareSize = touchX - 176 + 1;
                    updateDrawTool = true;
                }
            }
            break;
        }
        case 1:
        case 4: {
            if (touchCount >= 1 && paint.reverseScreens) {
                if (touchX >= 176 && touchX < 176 + 64 && touchY >= 66 && touchY < 66 + 8) {
                    circleDiameter = touchX - 176 + 1;
                    updateDrawTool = true;
                }
            }
            break;
        }
        case 2:
        case 5: {
            if (touchCount >= 1 && paint.reverseScreens) {
                if (touchX >= 176 && touchX < 176 + 32 && touchY >= 66 && touchY < 66 + 8) {
                    dotRadius = touchX - 176 + 1;
                    updateDrawTool = true;
                }
            }
            break;
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
                        if (keysD & KEY_LEFT) {
                            if (noiseXSize - 1 >= 1) {
                                noiseXSize--;
                                updateDrawTool = true;
                            }
                        }
                        if (keysD & KEY_RIGHT) {
                            if (noiseXSize + 1 <= 16) {
                                noiseXSize++;
                                updateDrawTool = true;
                            }
                        }
                    } else {
                        if (keysD & KEY_LEFT) {
                            if (noiseYSize - 1 >= 1) {
                                noiseYSize--;
                                updateDrawTool = true;
                            }
                        }
                        if (keysD & KEY_RIGHT) {
                            if (noiseYSize + 1 <= 16) {
                                noiseYSize++;
                                updateDrawTool = true;
                            }
                        }
                    }
                    break;
                }
                case 4: {
                    if (!activeNoise) {
                        if (keysD & KEY_LEFT) {
                            if (noiseXShift - 1 >= 0) {
                                noiseXShift--;
                                updateDrawTool = true;
                            }
                        }
                        if (keysD & KEY_RIGHT) {
                            if (noiseXShift + 1 < 16) {
                                noiseXShift++;
                                updateDrawTool = true;
                            }
                        }
                    } else {
                        if (keysD & KEY_LEFT) {
                            if (noiseYShift - 1 >= 0) {
                                noiseYShift--;
                                updateDrawTool = true;
                            }
                        }
                        if (keysD & KEY_RIGHT) {
                            if (noiseYShift + 1 < 16) {
                                noiseYShift++;
                                updateDrawTool = true;
                            }
                        }
                    }
                    break;
                }
                case 5: {
                    if (!activeNoise) {
                        if (keysD & KEY_LEFT) {
                            if (noiseXOffset - 1 >= 0) {
                                noiseXOffset--;
                                updateDrawTool = true;
                            }
                        }
                        if (keysD & KEY_RIGHT) {
                            if (noiseXOffset + 1 < 16) {
                                noiseXOffset++;
                                updateDrawTool = true;
                            }
                        }
                    } else {
                        if (keysD & KEY_LEFT) {
                            if (noiseYOffset - 1 >= 0) {
                                noiseYOffset--;
                                updateDrawTool = true;
                            }
                        }
                        if (keysD & KEY_RIGHT) {
                            if (noiseYOffset + 1 < 16) {
                                noiseYOffset++;
                                updateDrawTool = true;
                            }
                        }
                    }
                    break;
                }
            }
        }

        if (touchCount >= 1 && paint.reverseScreens) {
            if (touchX >= 176 && touchX < 176 + 64 && touchY >= 66 && touchY < 66 + 8) {
                squareSize = touchX - 176 + 1;
                updateDrawTool = true;
            }

            if (touchX >= 176 && touchX < 176 + 16 && touchY >= 75 && touchY < 75 + 8) {
                noiseXSize = touchX - 176 + 1;
                updateDrawTool = true;
            }
            if (touchX >= 176 + 24 && touchX < 176 + 24 + 16 && touchY >= 75 && touchY < 75 + 8) {
                noiseYSize = touchX - 176 + 1 - 24;
                updateDrawTool = true;
            }

            if (touchX >= 176 && touchX < 176 + 16 && touchY >= 84 && touchY < 84 + 8) {
                noiseXShift = touchX - 176;
                updateDrawTool = true;
            }
            if (touchX >= 176 + 24 && touchX < 176 + 24 + 16 && touchY >= 84 && touchY < 84 + 8) {
                noiseYShift = touchX - 176 - 24;
                updateDrawTool = true;
            }

            if (touchX >= 176 && touchX < 176 + 16 && touchY >= 93 && touchY < 93 + 8) {
                noiseXOffset = touchX - 176;
                updateDrawTool = true;
            }
            if (touchX >= 176 + 24 && touchX < 176 + 24 + 16 && touchY >= 93 && touchY < 93 + 8) {
                noiseYOffset = touchX - 176 - 24;
                updateDrawTool = true;
            }
        }
    }

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
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 54; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }

    active = false;
    drawCursor(paint);
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
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void Brush::drawTool(Paint& paint) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 54; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }

    string moveString = string((line == 0) ? ">" : "") + "Move: " + ((line == 0 && active) ? "+" : "-"); 
    paint.drawText(3, 48, moveString.c_str(), pixelBufferMain, blackColor);
    paint.drawText(176, 48, "+", pixelBufferMain, blackColor);

    string typeString = string((line == 1) ? ">" : "") + "Type: " + getTypeName(paint, type); 
    paint.drawText(3, 57, typeString.c_str(), pixelBufferMain, blackColor);
    paint.drawText(176, 57, "- +", pixelBufferMain, blackColor);

    switch (type) {
        case 0:
    	case 3: {
            string sizeString = string((line == 2) ? ">" : "") + "Size: " + paint.intToChars(squareSize);
            paint.drawText(3, 66, sizeString.c_str(), pixelBufferMain, blackColor);

            paint.drawLine(176, 67, 176 + 63, 67, pixelBufferMain, blackColor);
            paint.drawSquareOutline(176 - 1 + (squareSize - 1), 67 + 2, 3, 4, pixelBufferMain, blackColor);
            break;
        }
        case 1:
        case 4: {
            string diameterString = string((line == 2) ? ">" : "") + "Diameter: " + paint.intToChars(circleDiameter);
            paint.drawText(3, 66, diameterString.c_str(), pixelBufferMain, blackColor);

            paint.drawLine(176, 67, 176 + 63, 67, pixelBufferMain, blackColor);
            paint.drawSquareOutline(176 - 1 + (circleDiameter - 1), 67 + 2, 3, 4, pixelBufferMain, blackColor);
            break;
        }
        case 2:
        case 5: {
            string radiusString = string((line == 2) ? ">" : "") + "Radius: " + paint.intToChars(dotRadius);
            paint.drawText(3, 66, radiusString.c_str(), pixelBufferMain, blackColor);

            paint.drawLine(176, 67, 176 + 31, 67, pixelBufferMain, blackColor);
            paint.drawSquareOutline(176 - 1 + (dotRadius - 1), 67 + 2, 3, 4, pixelBufferMain, blackColor);
            break;
        }
    }

    if (type >= 3) {
        string noiseSizeString = string((line == 3 && !active) ? ">" : "") + "Noise Size: " + ((line == 3 && active && !activeNoise) ? ">" : "") + paint.intToChars(noiseXSize) + " " + ((line == 3 && active && activeNoise) ? ">" : "") + paint.intToChars(noiseYSize);
        paint.drawText(3, 75, noiseSizeString.c_str(), pixelBufferMain, blackColor);

        paint.drawLine(176, 76, 176 + 15, 76, pixelBufferMain, blackColor);
        paint.drawSquareOutline(176 - 1 + (noiseXSize - 1), 76 + 2, 3, 4, pixelBufferMain, blackColor);

        paint.drawLine(176 + 24, 76, 176 + 15 + 24, 76, pixelBufferMain, blackColor);
        paint.drawSquareOutline(176 - 1 + (noiseYSize - 1) + 24, 76 + 2, 3, 4, pixelBufferMain, blackColor);

        string noiseShiftString = string((line == 4 && !active) ? ">" : "") + "Noise Shift: " + ((line == 4 && active && !activeNoise) ? ">" : "") + paint.intToChars(noiseXShift) + " " + ((line == 4 && active && activeNoise) ? ">" : "") + paint.intToChars(noiseYShift);
        paint.drawText(3, 84, noiseShiftString.c_str(), pixelBufferMain, blackColor);

        paint.drawLine(176, 85, 176 + 15, 85, pixelBufferMain, blackColor);
        paint.drawSquareOutline(176 - 1 + noiseXShift, 85 + 2, 3, 4, pixelBufferMain, blackColor);

        paint.drawLine(176 + 24, 85, 176 + 15 + 24, 85, pixelBufferMain, blackColor);
        paint.drawSquareOutline(176 - 1 + noiseYShift + 24, 85 + 2, 3, 4, pixelBufferMain, blackColor);

        string noiseOffsetString = string((line == 5 && !active) ? ">" : "") + "Noise Offset: " + ((line == 5 && active && !activeNoise) ? ">" : "") + paint.intToChars(noiseXOffset) + " " + ((line == 5 && active && activeNoise) ? ">" : "") + paint.intToChars(noiseYOffset);
        paint.drawText(3, 95, noiseOffsetString.c_str(), pixelBufferMain, blackColor);

        paint.drawLine(176, 96, 176 + 15, 96, pixelBufferMain, blackColor);
        paint.drawSquareOutline(176 - 1 + noiseXOffset, 96 + 2, 3, 4, pixelBufferMain, blackColor);

        paint.drawLine(176 + 24, 96, 176 + 15 + 24, 96, pixelBufferMain, blackColor);
        paint.drawSquareOutline(176 - 1 + noiseYOffset + 24, 96 + 2, 3, 4, pixelBufferMain, blackColor);
    }
}

void Brush::drawCursor(Paint& paint) {
    int size = 0;
    switch (type) {
        case 0:
    	case 3: size = squareSize; break;
        case 1:
    	case 4: size = circleDiameter; break;
        case 2:
    	case 5: size = dotRadius; break;
    }
    size = size + 2;
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size; y++) {
            paint.blendSubLayers(cursorXOld + x - (size / 2), cursorYOld + y - (size / 2));
        }
    }

    if (active) drawLine(paint, cursorX, cursorY, cursorX, cursorY, pixelBufferSub, getSelectedColor(paint));

    cursorXOld = cursorX;
    cursorYOld = cursorY;
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