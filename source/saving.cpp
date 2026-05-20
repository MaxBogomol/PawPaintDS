#include "saving.h"

#include "paint.h"

const char* Saving::getName(Paint& paint) {
    return "Saving";
}

void Saving::setup(Paint& paint) {
    line = 0;
    updateDrawTool = true;
}

void Saving::update(Paint& paint) {
    bool updateSubLayers = false;
    int maxLine = 4;

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
        }

        if (line == 1) {
            /*
            string path = string(pawPaintPath);
            mkdir(path.c_str(), 0777);

            path = string(pawPaintPath) + "/" + paintsPath;
            mkdir(path.c_str(), 0777);

            path = string(pawPaintPath) + "/" + paintsPath + "/Unnamed";
            mkdir(path.c_str(), 0777);*/

            string path0String = string(pawPaintPath) + "/" + paintsPath + "/Unnamed/layer0.png";
            paint.loadFileBuffer(path0String.c_str(), pixelBufferSubLayer0);
            string path1String = string(pawPaintPath) + "/" + paintsPath + "/Unnamed/layer1.png";
            paint.loadFileBuffer(path1String.c_str(), pixelBufferSubLayer1);
            string path2String = string(pawPaintPath) + "/" + paintsPath + "/Unnamed/layer2.png";
            paint.loadFileBuffer(path2String.c_str(), pixelBufferSubLayer2);
            string path3String = string(pawPaintPath) + "/" + paintsPath + "/Unnamed/layer3.png";
            paint.loadFileBuffer(path3String.c_str(), pixelBufferSubLayer3);

            for (int x = 0; x < SCREEN_WIDTH; x++) {
                for (int y = 0; y < SCREEN_HEIGHT; y++) {
                    paint.blendSubLayers(x, y);
                }
            }
        }
        if (line == 2) {
            string path = string(pawPaintPath);
            mkdir(path.c_str(), 0777);

            path = string(pawPaintPath) + "/" + paintsPath;
            mkdir(path.c_str(), 0777);

            path = string(pawPaintPath) + "/" + paintsPath + "/Unnamed";
            mkdir(path.c_str(), 0777);

            string path0String = string(pawPaintPath) + "/" + paintsPath + "/Unnamed/layer0.png";
            paint.saveFileBuffer(path0String.c_str(), pixelBufferSubLayer0);
            string path1String = string(pawPaintPath) + "/" + paintsPath + "/Unnamed/layer1.png";
            paint.saveFileBuffer(path1String.c_str(), pixelBufferSubLayer1);
            string path2String = string(pawPaintPath) + "/" + paintsPath + "/Unnamed/layer2.png";
            paint.saveFileBuffer(path2String.c_str(), pixelBufferSubLayer2);
            string path3String = string(pawPaintPath) + "/" + paintsPath + "/Unnamed/layer3.png";
            paint.saveFileBuffer(path3String.c_str(), pixelBufferSubLayer3);
        }
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
    paint.drawSprite(x, y, 16, 16, paint.fileSystemInit ? saving_iconBitmap : saving_error_iconBitmap, pixelBufferMain);
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
}