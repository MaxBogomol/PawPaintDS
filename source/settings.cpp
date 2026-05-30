#include "settings.h"

#include "paint.h"

const char* Settings::getName(Paint& paint) {
    return "Settings";
}

void Settings::setup(Paint& paint) {
    line = 0;
    updateDrawTool = true;
}

void Settings::update(Paint& paint) {
    int maxLine = 3;

    if ((keysD & KEY_UP) && (line - 1 >= 0)) {
        line--;
        updateDrawTool = true;
    }
    if ((keysD & KEY_DOWN) && (line + 1 < maxLine)) {
        line++;
        updateDrawTool = true;
    }

    switch (line) {
        case 0: {
            if (keysD & KEY_LEFT) {
                paint.selectedTheme--;
                if (paint.selectedTheme < 0) paint.selectedTheme = maxPaintThemes - 1;
                paint.updateDrawAll = true;
            }
            if (keysD & KEY_RIGHT) {
                paint.selectedTheme++;
                if (paint.selectedTheme > maxPaintThemes - 1) paint.selectedTheme = 0;
                paint.updateDrawAll = true;
            }
            break;
        }
        case 1: {
            if (keysD & KEY_LEFT) {
                paint.selectedIcon--;
                if (paint.selectedIcon < 0) paint.selectedIcon = maxPaintIcons - 1;
                paint.updateDrawPaintIcon = true;
                updateDrawTool = true;
            }
            if (keysD & KEY_RIGHT) {
                paint.selectedIcon++;
                if (paint.selectedIcon > maxPaintIcons - 1) paint.selectedIcon = 0;
                paint.updateDrawPaintIcon = true;
                updateDrawTool = true;
            }
            break;
        }
    }
}

void Settings::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void Settings::open(Paint& paint) {
    line = 0;
    updateDrawTool = true;
}

void Settings::close(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 2, SCREEN_WIDTH, 32, pixelBufferMain);
}

void Settings::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    paint.drawSprite(x, y, 16, 16, settings_iconBitmap, pixelBufferMain);
}

void Settings::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 2, SCREEN_WIDTH, 32, pixelBufferMain);

    string themeString = string((line == 0) ? ">" : "") + "Theme: " + getThemeName(paint, paint.selectedTheme);
    paint.drawText(3, yOffset, themeString.c_str(), pixelBufferMain, blackColor);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 16 - 5, yOffset, 32, 32, 24, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 8, yOffset, 32, 32, 8, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);

    string iconString = string((line == 1) ? ">" : "") + "Icon: " + getIconName(paint, paint.selectedIcon);
    paint.drawText(3, yOffset += 10, iconString.c_str(), pixelBufferMain, blackColor);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 16 - 5, yOffset, 32, 32, 24, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 8, yOffset, 32, 32, 8, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);

    string languageString = string((line == 2) ? ">" : "") + "Language: " + "English";
    paint.drawText(3, yOffset += 10, languageString.c_str(), pixelBufferMain, blackColor);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 16 - 5, yOffset, 32, 32, 24, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);
    paint.drawSprite(SCREEN_WIDTH - bOffset - 8, yOffset, 32, 32, 8, 0, 8, 8, buttons_iconBitmap, pixelBufferMain);
}

const char* Settings::getThemeName(Paint& paint, int theme) {
    switch (theme) {
        case 0: return "White"; break;
        case 1: return "Pink Fox"; break;
        case 2: return "Maid"; break;
        case 3: return "Ace"; break;
    }
    return "White";
}

const char* Settings::getIconName(Paint& paint, int icon) {
    switch (icon) {
        case 0: return "Pride"; break;
        case 1: return "Monochrome"; break;
    }
    return "Pride";
}