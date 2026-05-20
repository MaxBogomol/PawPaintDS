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

    bool updateSubLayers = false;
    int maxLine = 6;

    if (keysD & KEY_UP) {
        if (line - 1 >= 0) {
            line--;
            updateDrawTool = true;
        }
    }
    if (keysD & KEY_DOWN) {
        if (line + 1 < maxLine) {
            line++;
            updateDrawTool = true;
        }
    }

    if (keysD & KEY_A) {
        if (line == 0) {
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                for (int y = 0; y < SCREEN_HEIGHT; y++) {
                    pixelBufferSubLayer0[x + (y * SCREEN_WIDTH)] = whiteColor;
                    pixelBufferSubLayer1[x + (y * SCREEN_WIDTH)] = alphaColor;
                    pixelBufferSubLayer2[x + (y * SCREEN_WIDTH)] = alphaColor;
                    pixelBufferSubLayer3[x + (y * SCREEN_WIDTH)] = alphaColor;
                    paint.blendSubLayers(x, y);
                }
            }
            paint.setPaintName("Unnnamed");
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
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 54; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }
}

void Saving::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    if (loading || saving || savingExport) {
        paint.drawSprite(x, y, 16, 16, saving_loading_iconBitmap, pixelBufferMain);
    } else if (doneTimer > 0) {
        paint.drawSprite(x, y, 16, 16, saving_done_iconBitmap, pixelBufferMain);
    } else {
        paint.drawSprite(x, y, 16, 16, paint.fileSystemInit ? saving_iconBitmap : saving_error_iconBitmap, pixelBufferMain);
    }
}

void Saving::drawTool(Paint& paint) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < 54; y++) {
            paint.drawPixel(x, y + 48, pixelBufferMain, whiteColor);
        }
    }

    string newString = string((line == 0) ? ">" : "") + "New";
    paint.drawText(3, 48, newString.c_str(), pixelBufferMain, blackColor);
    paint.drawText(176, 48, "+", pixelBufferMain, blackColor);

    string loadString = string((line == 1) ? ">" : "") + "Load";
    paint.drawText(3, 57, loadString.c_str(), pixelBufferMain, blackColor);
    paint.drawText(176, 57, "+", pixelBufferMain, blackColor);

    string saveString = string((line == 2) ? ">" : "") + "Save";
    paint.drawText(3, 66, saveString.c_str(), pixelBufferMain, blackColor);
    paint.drawText(176, 66, "+", pixelBufferMain, blackColor);

    string saveAsString = string((line == 3) ? ">" : "") + "Save As";
    paint.drawText(3, 75, saveAsString.c_str(), pixelBufferMain, blackColor);
    paint.drawText(176, 75, "+", pixelBufferMain, blackColor);

    string exportString = string((line == 4) ? ">" : "") + "Export";
    paint.drawText(3, 84, exportString.c_str(), pixelBufferMain, blackColor);
    paint.drawText(176, 84, "+", pixelBufferMain, blackColor);

    string exportAsString = string((line == 5) ? ">" : "") + "Export As";
    paint.drawText(3, 93, exportAsString.c_str(), pixelBufferMain, blackColor);
    paint.drawText(176, 93, "+", pixelBufferMain, blackColor);
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

    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            paint.blendSubLayers(x, y);
        }
    }
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