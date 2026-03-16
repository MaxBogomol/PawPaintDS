#include "paint.h"

void Paint::setupVideo() {
    videoSetMode(MODE_5_2D);
    vramSetBankA(VRAM_A_MAIN_BG);

    int bg3Main = bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    bgMainDest = (u16*) bgGetGfxPtr(bg3Main);

    videoSetModeSub(MODE_5_2D);
    vramSetBankC(VRAM_C_SUB_BG);

    int bg3sub = bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
    bgSubDest = (u16*) bgGetGfxPtr(bg3sub);

    selectedLayer = 0;
    selectedTool = 0;
    selectedColor = blackColor;
    selectedColorSub = whiteColor;
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

    if (keysD & KEY_L) {
        selectedTool--;
        toolChanged = true;
    }
    if (keysD & KEY_R) {
        selectedTool++;
        toolChanged = true;
    }
    selectedTool = selectedTool % tools.size();
    if (toolChanged) {
        tools[selectedToolOld]->close(*this);
        tools[selectedTool]->open(*this);
    }

    tools[selectedTool]->update(*this);
}

void Paint::updateVideo() {
    dmaFillHalfWords(whiteColor, pixelBufferMain, sizeof(pixelBufferMain));

    drawSquare(0, 160, 16, 16, pixelBufferMain, selectedColor);
    int r = (selectedColor) & 31;
    int g = (selectedColor >> 5) & 31;
    int b = (selectedColor >> 10) & 31;

    string colorString = string("RGB: (") + intToChars(r) + ", " + intToChars(g) + ", " + intToChars(b) + ")"; 
    drawText(16, 164, colorString.c_str(), pixelBufferMain, blackColor);

    drawSquare(0, 176, 16, 16, pixelBufferMain, selectedColorSub);
    int rs = (selectedColorSub) & 31;
    int gs = (selectedColorSub >> 5) & 31;
    int bs = (selectedColorSub >> 10) & 31;

    string colorSubString = string("RGB: (") + intToChars(rs) + ", " + intToChars(gs) + ", " + intToChars(bs) + ")"; 
    drawText(16, 180, colorSubString.c_str(), pixelBufferMain, blackColor);

    string layerString = string("Layer: ") + intToChars(selectedLayer + 1); 
    drawText(2, 2, layerString.c_str(), pixelBufferMain, blackColor);
    string toolString = string("Tool: ") + intToChars(selectedTool + 1); 
    drawText(2, 2 + 16, toolString.c_str(), pixelBufferMain, blackColor);

    swiWaitForVBlank(); 
    DC_FlushRange(pixelBufferSub, sizeof(pixelBufferSub));
    DC_FlushRange(pixelBufferSubLayer0, sizeof(pixelBufferSubLayer0));
    DC_FlushRange(pixelBufferSubLayer1, sizeof(pixelBufferSubLayer1));
    DC_FlushRange(pixelBufferSubLayer2, sizeof(pixelBufferSubLayer2));
    DC_FlushRange(pixelBufferSubLayer3, sizeof(pixelBufferSubLayer3));
    dmaCopy(pixelBufferMain, bgMainDest, sizeof(pixelBufferMain));
    dmaCopy(pixelBufferSub, bgSubDest, sizeof(pixelBufferSub));
}

void Paint::blendSubLayers(int x, int y) {
	pixelBufferSub[x + (y * SCREEN_WIDTH)] = blendColors(pixelBufferSubLayer0[x + (y * SCREEN_WIDTH)], pixelBufferSubLayer1[x + (y * SCREEN_WIDTH)]);
	pixelBufferSub[x + (y * SCREEN_WIDTH)] = blendColors(pixelBufferSub[x + (y * SCREEN_WIDTH)], pixelBufferSubLayer2[x + (y * SCREEN_WIDTH)]);
	pixelBufferSub[x + (y * SCREEN_WIDTH)] = blendColors(pixelBufferSub[x + (y * SCREEN_WIDTH)], pixelBufferSubLayer3[x + (y * SCREEN_WIDTH)]);
}

u16 *Paint::getSelectedLayer() {
	switch (selectedLayer) {
    	case 0: return pixelBufferSubLayer0;
    	case 1: return pixelBufferSubLayer1;
		case 2: return pixelBufferSubLayer2;
		case 3: return pixelBufferSubLayer3;
    }
	return pixelBufferSubLayer0;
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
		buffer[x + (y * SCREEN_WIDTH)] = color;
		if (updateSubLayers) blendSubLayers(x, y);
	}
}

void Paint::drawSquare(int x0, int y0, int x1, int y1, u16* buffer, u16 color) {
	for (int x = 0; x < x1; x++) {
		for (int y = 0; y < y1; y++) {
			drawPixel(x0 + x, y0 + y, buffer, color);
		}
	}
}

void Paint::drawCircle(int xc, int yc, int r, u16* buffer, u16 color) {
	for (int y = -r; y <= r; y++) {
        for (int x = -r; x <= r; x++) {
            if (x*x + y*y <= r*r) {
                drawPixel(xc + x, yc + y, buffer, color);
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

const char* Paint::intToChars(int val) {
    static char buf[12];
    sprintf(buf, "%d", val);
    return buf;
}