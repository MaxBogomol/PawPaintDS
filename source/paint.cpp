#include "paint.h"

void Paint::setup() {
    firstFrameTool = true;
    updateSubLayers = false;
    updateBlendSubLayers = false;

    selectedLayer = 0;
    selectedTool = 0;
    selectedColor = blackColor;
    selectedColorSub = whiteColor;
    reverseScreens = false;

    updateDrawSelectedColor = false;
    updateDrawTools = true;
    updateDrawColors = true;

    keysSetRepeat(10, 2);
}

void Paint::setupVideo() {
    videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);

    int bg3Main = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    bgMainDest = (u16*) bgGetGfxPtr(bg3Main);

    videoSetModeSub(MODE_5_2D);
    vramSetBankC(VRAM_C_SUB_BG);

    int bg3sub = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    bgSubDest = (u16*) bgGetGfxPtr(bg3sub);
}

void Paint::setupLayers() {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			pixelBufferMain[x + (y * SCREEN_WIDTH)] = whiteColor;

			pixelBufferSubLayer0[x + (y * SCREEN_WIDTH)] = whiteColor;
			pixelBufferSubLayer1[x + (y * SCREEN_WIDTH)] = alphaColor;
			pixelBufferSubLayer2[x + (y * SCREEN_WIDTH)] = alphaColor;
			pixelBufferSubLayer3[x + (y * SCREEN_WIDTH)] = alphaColor;
			blendSubLayers(x, y);
		}
	}
}

void Paint::setupTools() {
    brush.setup(*this);
    eraser.setup(*this);
    eyedropper.setup(*this);
    colorPicker.setup(*this);
    layers.setup(*this);

    tools.push_back(&brush);
    tools.push_back(&eraser);
    tools.push_back(&eyedropper);
    tools.push_back(&colorPicker);
    tools.push_back(&layers);
}

void Paint::updateInputs() {
    scanKeys();
    keysD = keysDown();
    keysH = keysHeld();
    keysR = keysDownRepeat();
    keysU = keysUp();
    touchRead(&touch);

    if (keysH & KEY_TOUCH) {
        touchXOld = touchX;
        touchYOld = touchY;

        touchX = touch.px;
        touchY = touch.py;

        touchCount++;
    } else {
        touchCount = 0;
    }
}

void Paint::updateTools() {
    int selectedToolOld = selectedTool;
    bool toolChanged = false;

    if (updateDrawSelectedColor) updateDrawSelectedColor = false;

    if (keysD & KEY_START) {
        reverseScreens = !reverseScreens;
        if (reverseScreens) {
            lcdMainOnBottom();
        } else {
            lcdMainOnTop();
        }
        tools[selectedTool]->reverse(*this);
    }
    if (keysD & KEY_L) {
        selectedTool--;
        if (selectedTool < 0) selectedTool = tools.size() - 1;
        toolChanged = true;
    }
    if (keysD & KEY_R) {
        selectedTool++;
        if (selectedTool > tools.size() - 1) selectedTool = 0;
        toolChanged = true;
    }
    if (keysD & KEY_TOUCH && reverseScreens) {
        int i = 0;
        int j = 0;
        for (int t = 0; t < tools.size(); t++) { 
            if (touchX >= 3 + (i * 18) && touchX < 3 + (i * 18) + 18 && touchY >= 3 + (j * 18) && touchY < 3 + (j * 18) + 18) {
                selectedTool = t;
                toolChanged = true;
                break;
            }
            i++;
            if (i >= 14) {
                i = 0;
                j++;
            }
        }

        if (touchX >= 3 && touchX < 19 && touchY >= 157 && touchY < 189) {
            u16 color = selectedColorSub;
            selectedColorSub = selectedColor;
            selectedColor = color;
            updateDrawColors = true;
            updateDrawSelectedColor = true;
        }
}
    if (toolChanged) {
        tools[selectedToolOld]->close(*this);
        tools[selectedTool]->open(*this);
        updateDrawTools = true;
    }

    if (firstFrameTool) {
        tools[selectedTool]->open(*this);
        firstFrameTool = false;
    }

    tools[selectedTool]->update(*this);
}

void Paint::updateVideo() {
    if (updateDrawTools) {
        drawTools();
        updateDrawTools = false;
    }

    if (updateDrawColors) {
        drawColors();
        updateDrawColors = false;
    }

    tools[selectedTool]->updateTool(*this);

    swiWaitForVBlank();
    DC_FlushAll();
    dmaCopy(pixelBufferMain, bgMainDest, sizeof(pixelBufferMain));
    dmaCopy(pixelBufferSub, bgSubDest, sizeof(pixelBufferSub));
}

void Paint::drawTools() {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 48; y++) {
            drawPixel(x, y, pixelBufferMain, whiteColor);
        }
    }

    int i = 0;
    int j = 0;
    for (int t = 0; t < tools.size(); t++) {
        tools[t]->drawIcon(*this, 3 + (i * 18), 3 + (j * 18), pixelBufferMain);
        if (t == selectedTool) drawSquareOutline(2 + (i * 18), 2 + (j * 18), 18, 18, pixelBufferMain, blackColor);
        i++;
        if (i >= 14) {
            i = 0;
            j++;
        }
    }

    string toolString = string("Tool: ") + tools[selectedTool]->getName(*this); 
    drawText(3, 39, toolString.c_str(), pixelBufferMain, blackColor);
}

void Paint::drawColors() {
    for (int x = 0; x < 128; x++) {
        for (int y = 0; y < 34; y++) {
            drawPixel(x, y + 156, pixelBufferMain, whiteColor);
        }
    }

    drawSquareOutline(2, 156, 18, 34, pixelBufferMain, blackColor);

    drawSquare(3, 157, 16, 16, pixelBufferMain, selectedColor);
    int r = (selectedColor) & 31;
    int g = (selectedColor >> 5) & 31;
    int b = (selectedColor >> 10) & 31;
    string colorString = string("RGB: ") + intToChars(r) + " " + intToChars(g) + " " + intToChars(b); 
    drawText(20, 161, colorString.c_str(), pixelBufferMain, blackColor);

    drawSquare(3, 173, 16, 16, pixelBufferMain, selectedColorSub);
    int rs = (selectedColorSub) & 31;
    int gs = (selectedColorSub >> 5) & 31;
    int bs = (selectedColorSub >> 10) & 31;
    string colorSubString = string("RGB: ") + intToChars(rs) + " " + intToChars(gs) + " " + intToChars(bs); 
    drawText(20, 177, colorSubString.c_str(), pixelBufferMain, blackColor);
}

void Paint::blendSubLayers(int x, int y) {
    updateBlendSubLayers = false;
    if (activeSubLayer0) blendSubLayers(x, y, pixelBufferSubLayer0);
    if (activeSubLayer1) blendSubLayers(x, y, pixelBufferSubLayer1);
    if (activeSubLayer2) blendSubLayers(x, y, pixelBufferSubLayer2);
    if (activeSubLayer3) blendSubLayers(x, y, pixelBufferSubLayer3);
    if (!updateBlendSubLayers) pixelBufferSub[x + (y * SCREEN_WIDTH)] = blackColor;
}

void Paint::blendSubLayers(int x, int y, u16* buffer) {
    if (updateBlendSubLayers) {
        pixelBufferSub[x + (y * SCREEN_WIDTH)] = blendColors(pixelBufferSub[x + (y * SCREEN_WIDTH)], buffer[x + (y * SCREEN_WIDTH)]);
    } else {
        pixelBufferSub[x + (y * SCREEN_WIDTH)] = buffer[x + (y * SCREEN_WIDTH)];
        updateBlendSubLayers = true;
    }
}

void Paint::swapSubLayers(int l0, int l1) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
            u16 color = getLayer(l0)[x + (y * SCREEN_WIDTH)];
            getLayer(l0)[x + (y * SCREEN_WIDTH)] = getLayer(l1)[x + (y * SCREEN_WIDTH)];
            getLayer(l1)[x + (y * SCREEN_WIDTH)] = color;
        }
    }
}

void Paint::updateSubLayersEnable() {
    updateSubLayers = true;
}

void Paint::updateSubLayersDisable() {
    updateSubLayers = false;
}

u16 *Paint::getLayer(int layer) {
	switch (layer) {
    	case 0: return pixelBufferSubLayer0;
    	case 1: return pixelBufferSubLayer1;
		case 2: return pixelBufferSubLayer2;
		case 3: return pixelBufferSubLayer3;
    }
	return pixelBufferSubLayer0;
}

u16 *Paint::getSelectedLayer() {
	return getLayer(selectedLayer);
}

u16 Paint::getSelectedColor() {
	return selectedColor;
}

u16 Paint::getPixel(int x, int y, u16* buffer) {
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
		return buffer[x + (y * SCREEN_WIDTH)];
	}
    return blackColor;
}

void Paint::drawPixel(int x, int y, u16* buffer, u16 color) {
	if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
		if (getPixel(x, y, buffer) != color) {
            buffer[x + (y * SCREEN_WIDTH)] = color;
		    if (updateSubLayers) blendSubLayers(x, y);
        }
	}
}

void Paint::drawSquare(int x0, int y0, int x1, int y1, u16* buffer, u16 color) {
	for (int x = 0; x < x1; x++) {
		for (int y = 0; y < y1; y++) {
			drawPixel(x0 + x, y0 + y, buffer, color);
		}
	}
}

void Paint::drawSquareOutline(int x0, int y0, int x1, int y1, u16* buffer, u16 color) {
    x1 = x1 - 1;
    y1 = y1 - 1;
	drawLine(x0, y0, x0 + x1, y0, buffer, color);
    drawLine(x0, y0 + y1, x0 + x1, y0 + y1, buffer, color);
    drawLine(x0, y0, x0, y0 + y1, buffer, color);
    drawLine(x0 + x1, y0, x0 + x1, y0 + y1, buffer, color);
}

void Paint::drawSquareNoise(int x0, int y0, int x1, int y1, u16* buffer, u16 color, int xSize, int ySize, int xShift, int yShift, int xOffset, int yOffset) {
	for (int x = 0; x < x1; x++) {
		for (int y = 0; y < y1; y++) {
            int threshold = getDitherThreshold(x0 + x + xOffset, y0 + y + yOffset, xSize, ySize, xShift, yShift);
			if (threshold <= 0) drawPixel(x0 + x, y0 + y, buffer, color);
		}
	}
}

void Paint::drawCircleRadius(int xc, int yc, int r, u16* buffer, u16 color) {
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x*x + y*y <= r*r) {
                drawPixel(xc + x, yc + y, buffer, color);
            }
        }
    }
}

void Paint::drawCircleRadiusNoise(int xc, int yc, int r, u16* buffer, u16 color, int xSize, int ySize, int xShift, int yShift, int xOffset, int yOffset) {
    for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x*x + y*y <= r*r) {
                int threshold = getDitherThreshold(xc + x + xOffset, yc + y + yOffset, xSize, ySize, xShift, yShift);
                if (threshold <= 0) drawPixel(xc + x, yc + y, buffer, color);
            }
        }
    }
}

void Paint::drawCircleDiameter(int xc, int yc, int d, u16* buffer, u16 color) {
    int d2 = d * d;
    int offset = (d - 1);
    int center = d / 2; 
    for (int y = 0; y < d; y++) {
        for (int x = 0; x < d; x++) {
            int dx = 2 * x - offset;
            int dy = 2 * y - offset;
            if (dx * dx + dy * dy <= d2) {
                drawPixel(xc + x - center, yc + y - center, buffer, color);
            }
        }
    }
}

void Paint::drawCircleDiameterNoise(int xc, int yc, int d, u16* buffer, u16 color, int xSize, int ySize, int xShift, int yShift, int xOffset, int yOffset) {
    int d2 = d * d;
    int offset = (d - 1);
    int center = d / 2; 
    for (int y = 0; y < d; y++) {
        for (int x = 0; x < d; x++) {
            int dx = 2 * x - offset;
            int dy = 2 * y - offset;
            if (dx * dx + dy * dy <= d2) {
                int threshold = getDitherThreshold(xc + x - center + xOffset, yc + y - center + yOffset, xSize, ySize, xShift, yShift);
                if (threshold <= 0) drawPixel(xc + x - center, yc + y - center, buffer, color);
            }
        }
    }
}

void Paint::drawLine(int x0, int y0, int x1, int y1, u16* buffer, u16 color) {
    int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
		drawPixel(x0, y0, buffer, color);

        if (x0 == x1 && y0 == y1) break;
        
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void Paint::drawChar(int x, int y, char c, u16* buffer, u16 color) {
    const u8* charData = &default_font_bin[c * 8];

    for (int row = 0; row < 8; row++) {
        u8 bits = charData[row];
        for (int col = 0; col < 8; col++) {
            if (bits & (1 << col)) { 
                int px = x + col;
                int py = y + row;
                
                if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
                    buffer[py * SCREEN_WIDTH + px] = color;
                }
            }
        }
    }
}

void Paint::drawText(int x, int y, const char* text, u16* buffer, u16 color) {
    while (*text) {
        drawChar(x, y, *text++, buffer, color);
        x += 8;
    }
}

void Paint::drawCharOutline(int x, int y, char c, u16* buffer, u16 color, u16 outlineColor) {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            drawChar(x + dx, y + dy, c, buffer, outlineColor);
        }
    }
    drawChar(x, y, c, buffer, color);
}

void Paint::drawTextOutline(int x, int y, const char* text, u16* buffer, u16 color, u16 outlineColor) {
    while (*text) {
        drawCharOutline(x, y, *text++, buffer, color, outlineColor);
        x += 8;
    }
}

void Paint::drawSprite(int x0, int y0, int x1, int y1, const unsigned int* spriteBitmap, const unsigned short* spritePal, u16* buffer) {
    const u8* pixels = (const u8*)spriteBitmap; 

    for (int y = 0; y < y1; y++) {
        for (int x = 0; x < x1; x++) {
            u8 colorIndex = pixels[x + (y * x1)];
            u16 color = spritePal[colorIndex] | BIT(15);

            if (color != pinkColor) {
                drawPixel(x0 + x, y0 + y, buffer, color);
            }
        }
    }
}

u16 Paint::blendColors(u16 src, u16 dst) {
	u8 alpha = (dst >> 15) & 1;
	if (alpha == 1) {
		return dst;
	}
	return src;
}

u16 Paint::HSVtoRGB(int h, int s, int v) {
	int r = 0, g = 0, b = 0;
    
    if (s == 0) {
        r = g = b = v;
    } else {
        int region = h / 60;
        int remainder = (h % 60) * 255 / 60;

        int p = (v * (255 - s)) >> 8;
        int q = (v * (255 - ((s * remainder) >> 8))) >> 8;
        int t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

        switch (region) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            default: r = v; g = p; b = q; break;
        }
    }

    return ARGB16(1, r >> 3, g >> 3, b >> 3);
}

u16 Paint::HSVtoRGB(HSV hsv) {
    return HSVtoRGB(hsv.h, hsv.s, hsv.v);
}

HSV Paint::RGBtoHSV(u16 color) {
    int r5 = (color) & 0x1F;
    int g5 = (color >> 5) & 0x1F;
    int b5 = (color >> 10) & 0x1F;

    int r = (r5 * 255) / 31;
    int g = (g5 * 255) / 31;
    int b = (b5 * 255) / 31;

    int maxV = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);
    int minV = (r < g) ? ((r < b) ? r : b) : ((g < b) ? g : b);
    int delta = maxV - minV;

    HSV res;
    res.v = maxV;

    if (maxV == 0) {
        res.s = 0;
        res.h = 0;
        return res;
    }
    res.s = (255 * delta) / maxV;

    if (delta == 0) {
        res.h = 0;
    } else {
        if (maxV == r) {
            res.h = 60 * (g - b) / delta;
        } else if (maxV == g) {
            res.h = 120 + 60 * (b - r) / delta;
        } else {
            res.h = 240 + 60 * (r - g) / delta;
        }

        if (res.h < 0) res.h += 360;
    }

    return res;
}

int Paint::getDitherThreshold(int x, int y, int xSize, int ySize, int xShift, int yShift) {
    int xOffset = ((y / ySize) % 2 == 1) ? xShift : 0;
    int yOffset = ((x / xSize) % 2 == 1) ? yShift : 0;
    return ((x + xOffset) % xSize) + ((y + yOffset) % ySize);
}

const char* Paint::intToChars(int val) {
    static char buf[12];
    sprintf(buf, "%d", val);
    return buf;
}