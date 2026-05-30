#include "saving.h"

#include "paint.h"

const char* Saving::getName(Paint& paint) {
    return "Saving";
}

void Saving::setup(Paint& paint) {
    line = 0;
    loading = false;
    saving = false;
    savingExport = false;
    updateDrawTool = true;
}

void Saving::update(Paint& paint) {
    if (paint.fileSystemInit) {
        if (loading) {
            loadPaint(paint);
            loading = false;
            doneTimer = 60;
        }
        if (saving) {
            savePaint(paint);
            saving = false;
            doneTimer = 60;
        }
        if (savingExport) {
            savePaw(paint);
            savingExport = false;
            doneTimer = 60;
        }
    }

    int maxLine = 6;

    if ((keysD & KEY_UP) && (line - 1 >= 0)) {
        line--;
        updateDrawTool = true;
    }
    if ((keysD & KEY_DOWN) && (line + 1 < maxLine)) {
        line++;
        updateDrawTool = true;
    }

    if (keysD & KEY_A) {
        if (line == 0) {
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                for (int y = 0; y < SCREEN_HEIGHT; y++) {
                    pixelBufferSubLayer0[x + (y * SCREEN_WIDTH)] = paint.selectedColorSub;
                    pixelBufferSubLayer1[x + (y * SCREEN_WIDTH)] = alphaColor;
                    pixelBufferSubLayer2[x + (y * SCREEN_WIDTH)] = alphaColor;
                    pixelBufferSubLayer3[x + (y * SCREEN_WIDTH)] = alphaColor;
                    paint.blendSubLayers(x, y);
                }
            }
            paint.setPaintName("Unnamed");
        }

        if (paint.fileSystemInit) {
            switch (line) {
                case 1: {
                    loading = true;
                    paint.updateDrawTools = true;
                    break;
                }
                case 2: {
                    saving = true;
                    paint.updateDrawTools = true;
                    break;
                }
                case 4: {
                    pawName = paint.getPaintName();
                    savingExport = true;
                    paint.updateDrawTools = true;
                    break;
                }
            }
        }
    }

    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();

    if (keysD & KEY_TOUCH && paint.reverseScreens) {
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset + 8 && touchY >= yOffset && touchY < yOffset + 8) {
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                for (int y = 0; y < SCREEN_HEIGHT; y++) {
                    pixelBufferSubLayer0[x + (y * SCREEN_WIDTH)] = paint.selectedColorSub;
                    pixelBufferSubLayer1[x + (y * SCREEN_WIDTH)] = alphaColor;
                    pixelBufferSubLayer2[x + (y * SCREEN_WIDTH)] = alphaColor;
                    pixelBufferSubLayer3[x + (y * SCREEN_WIDTH)] = alphaColor;
                    paint.blendSubLayers(x, y);
                }
            }
            paint.setPaintName("Unnamed");
        }
        yOffset += 10;
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset + 8 && touchY >= yOffset && touchY < yOffset + 8) {
            loading = true;
            paint.updateDrawTools = true;
        }
        yOffset += 10;
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset + 8 && touchY >= yOffset && touchY < yOffset + 8) {
            saving = true;
            paint.updateDrawTools = true;
        }
        yOffset += 10;
        yOffset += 10;
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset + 8 && touchY >= yOffset && touchY < yOffset + 8) {
            pawName = paint.getPaintName();
            savingExport = true;
            paint.updateDrawTools = true;
        }
    }

    if (doneTimer > 0)  {
        doneTimer--;
        paint.updateDrawTools = true;
    }
}

void Saving::updateTool(Paint& paint) {
    if (updateDrawTool) {
        drawTool(paint);
        updateDrawTool = false;
    }
}

void Saving::open(Paint& paint) {
    line = 0;
    updateDrawTool = true;
}

void Saving::close(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 2, SCREEN_WIDTH, 62, pixelBufferMain, paint.getSelectedThemeColor());
    doneTimer = 0;
}

void Saving::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    const unsigned int* iconSprite = paint.fileSystemInit ? saving_iconBitmap : saving_error_iconBitmap;
    if (loading || saving || savingExport) {
        iconSprite = saving_loading_iconBitmap;
    } else if (doneTimer > 0) {
        iconSprite = saving_done_iconBitmap;
    }
    paint.drawSprite(x, y, 16, 16, iconSprite, buffer);
}

void Saving::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 2, SCREEN_WIDTH, 62, pixelBufferMain, paint.getSelectedThemeColor());

    string newString = string((line == 0) ? ">" : "") + "New";
    paint.drawText(3, yOffset, newString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string loadString = string((line == 1) ? ">" : "") + "Load";
    paint.drawText(3, yOffset += 10, loadString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string saveString = string((line == 2) ? ">" : "") + "Save";
    paint.drawText(3, yOffset += 10, saveString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string saveAsString = string((line == 3) ? ">" : "") + "Save As";
    paint.drawText(3, yOffset += 10, saveAsString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string exportString = string((line == 4) ? ">" : "") + "Export";
    paint.drawText(3, yOffset += 10, exportString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string exportAsString = string((line == 5) ? ">" : "") + "Export As";
    paint.drawText(3, yOffset += 10, exportAsString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);
}

void Saving::createPaintDirectory(Paint& paint, const char* paintName) {
    string path = string(pawPaintPath);
    if (!paint.directoryExist(path.c_str())) paint.makeDirectory(path.c_str());

    path = string(pawPaintPath) + "/" + paintsPath;
    if (!paint.directoryExist(path.c_str())) paint.makeDirectory(path.c_str());

    path = string(pawPaintPath) + "/" + paintsPath + "/" + paint.getPaintName();
    if (!paint.directoryExist(path.c_str())) paint.makeDirectory(path.c_str());
}

void Saving::createPawDirectory(Paint& paint) {
    string path = string(pawPaintPath);
    if (!paint.directoryExist(path.c_str())) paint.makeDirectory(path.c_str());

    path = string(pawPaintPath) + "/" + pawsPath;
    if (!paint.directoryExist(path.c_str())) paint.makeDirectory(path.c_str());
}

void Saving::savePaint(Paint& paint) {
    createPaintDirectory(paint, paint.getPaintName());
    string pathString = string(pawPaintPath) + "/" + paintsPath + "/" + paint.getPaintName();
    saveLayer(paint, pathString.c_str(), "layer0.png", pixelBufferSubLayer0);
    saveLayer(paint, pathString.c_str(), "layer1.png", pixelBufferSubLayer1);
    saveLayer(paint, pathString.c_str(), "layer2.png", pixelBufferSubLayer2);
    saveLayer(paint, pathString.c_str(), "layer3.png", pixelBufferSubLayer3);
}

void Saving::loadPaint(Paint& paint) {
    createPawDirectory(paint);
    string pathString = string(pawPaintPath) + "/" + paintsPath + "/" + paint.getPaintName();
    loadLayer(paint, pathString.c_str(), "layer0.png", pixelBufferSubLayer0);
    loadLayer(paint, pathString.c_str(), "layer1.png", pixelBufferSubLayer1);
    loadLayer(paint, pathString.c_str(), "layer2.png", pixelBufferSubLayer2);
    loadLayer(paint, pathString.c_str(), "layer3.png", pixelBufferSubLayer3);
    paint.blendSubLayers(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Saving::savePaw(Paint& paint) {
    createPaintDirectory(paint, paint.getPaintName());
    string pathString = string(pawPaintPath) + "/" + pawsPath;
    string pawString = string(pawName) + ".png";
    saveLayer(paint, pathString.c_str(), pawString.c_str(), pixelBufferSub);
}

bool Saving::saveLayer(Paint& paint, const char* path, const char* layerName, u16* buffer) {
    string pathString = string(path) + "/" + layerName;
    return paint.saveFileBuffer(pathString.c_str(), buffer);
}

bool Saving::loadLayer(Paint& paint, const char* path, const char* layerName, u16* buffer) {
    string pathString = string(path) + "/" + layerName;
    return paint.loadFileBuffer(pathString.c_str(), buffer);
}