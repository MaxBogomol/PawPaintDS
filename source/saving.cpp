#include "saving.h"

#include "paint.h"
#include "language.h"

#include "saving_icon.h"
#include "saving_error_icon.h"
#include "saving_loading_icon.h"
#include "saving_done_icon.h"

const char* Saving::getName(Paint& paint) {
    return STR_SAVING.c_str();
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
            loadPaw(paint);
            loading = false;
            doneTimer = 60;
        }
        if (saving) {
            savePaw(paint);
            saving = false;
            doneTimer = 60;
        }
        if (savingExport) {
            savePaint(paint);
            savingExport = false;
            doneTimer = 60;
        }
    }

    int maxLine = 6;

    if ((keysD & KEY_UP) && (line - 1 >= 0)) {
        line--;
        updateDrawTool = true;
        paint.updateDrawHints = true;
    }
    if ((keysD & KEY_DOWN) && (line + 1 < maxLine)) {
        line++;
        updateDrawTool = true;
        paint.updateDrawHints = true;
    }

    if (keysD & KEY_A) {
        if (line == 0) {
            paint.selectedLayer = 0;
            paint.screenLayers.clear();
            Layer* newLayer = new Layer();
            paint.screenLayers.push_back(newLayer);
            paint.clearLayers();
            paint.setPaintName(STR_UNNAMED.c_str());
            paint.updateDrawPaintName = true;
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
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
            paint.selectedLayer = 0;
            paint.screenLayers.clear();
            Layer* newLayer = new Layer();
            paint.screenLayers.push_back(newLayer);
            paint.clearLayers();
            paint.setPaintName(STR_UNNAMED.c_str());
            paint.updateDrawPaintName = true;
        }
        yOffset += 13;
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.fileSystemInit) {
                loading = true;
                paint.updateDrawTools = true;
            }
        }
        yOffset += 13;
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.fileSystemInit) {
                saving = true;
                paint.updateDrawTools = true;
            }
        }
        yOffset += 13;
        yOffset += 13;
        if (touchX >= SCREEN_WIDTH - bOffset - 8 && touchX < SCREEN_WIDTH - bOffset && touchY >= yOffset && touchY < yOffset + 8) {
            if (paint.fileSystemInit) {
                pawName = paint.getPaintName();
                savingExport = true;
                paint.updateDrawTools = true;
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
    int yOffset = paint.getToolsYOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 6 * 13 + 3, pixelBufferMain);
    doneTimer = 0;
}

void Saving::redraw(Paint& paint) {
    updateDrawTool = true;
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

void Saving::drawHints(Paint& paint, int x, int y, u16* buffer) {
    int xOffset = -10;
    int yOffset = 0;
    if (line == 0 || paint.fileSystemInit) {
        paint.drawUpDownButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
        paint.drawAButton(x + (xOffset += 10), y + yOffset, pixelBufferMain);
    }
}

void Saving::drawTool(Paint& paint) {
    int yOffset = paint.getToolsYOffset();
    int bOffset = paint.getToolsButtonsOffset();
    paint.clearBuffer(0, yOffset - 3, SCREEN_WIDTH, 6 * 13 + 3, pixelBufferMain);

    string newString = string((line == 0) ? ">" : "") + STR_SAVING_NEW;
    paint.drawText(3, yOffset, newString.c_str(), pixelBufferMain, blackColor);
    paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string loadString = string((line == 1) ? ">" : "") + STR_SAVING_LOAD;
    paint.drawText(3, yOffset += 13, loadString.c_str(), pixelBufferMain, paint.fileSystemInit ? blackColor : grayColor);
    if (paint.fileSystemInit) paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string saveString = string((line == 2) ? ">" : "") + STR_SAVING_SAVE;
    paint.drawText(3, yOffset += 13, saveString.c_str(), pixelBufferMain, paint.fileSystemInit ? blackColor : grayColor);
    if (paint.fileSystemInit) paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string saveAsString = string((line == 3) ? ">" : "") + STR_SAVING_SAVE_AS;
    paint.drawText(3, yOffset += 13, saveAsString.c_str(), pixelBufferMain, paint.fileSystemInit ? blackColor : grayColor);
    if (paint.fileSystemInit) paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string exportString = string((line == 4) ? ">" : "") + STR_SAVING_EXPORT;
    paint.drawText(3, yOffset += 13, exportString.c_str(), pixelBufferMain, paint.fileSystemInit ? blackColor : grayColor);
    if (paint.fileSystemInit) paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);

    string exportAsString = string((line == 5) ? ">" : "") + STR_SAVING_EXPORT_AS;
    paint.drawText(3, yOffset += 13, exportAsString.c_str(), pixelBufferMain, paint.fileSystemInit ? blackColor : grayColor);
    if (paint.fileSystemInit) paint.drawAButton(SCREEN_WIDTH - bOffset - 8, yOffset, pixelBufferMain);
}

void Saving::createPawDirectory(Paint& paint, const char* paintName) {
    string path = string(paint.getWorkingDirectory());
    if (!paint.directoryExist(path.c_str())) paint.makeDirectory(path.c_str());

    path = string(paint.getWorkingDirectory()) + "/" + pawsPath;
    if (!paint.directoryExist(path.c_str())) paint.makeDirectory(path.c_str());

    path = string(paint.getWorkingDirectory()) + "/" + pawsPath + "/" + paint.getPaintName();
    if (!paint.directoryExist(path.c_str())) paint.makeDirectory(path.c_str());
}

void Saving::createPaintDirectory(Paint& paint) {
    string path = string(paint.getWorkingDirectory());
    if (!paint.directoryExist(path.c_str())) paint.makeDirectory(path.c_str());

    path = string(paint.getWorkingDirectory()) + "/" + paintsPath;
    if (!paint.directoryExist(path.c_str())) paint.makeDirectory(path.c_str());
}

void Saving::savePaw(Paint& paint) {
    createPawDirectory(paint, paint.getPaintName());
    string directoryPath = string(paint.getWorkingDirectory()) + "/" + pawsPath + "/" + paint.getPaintName();
    string pawPath = string(directoryPath) + "/" + pawFile;

    FILE* fp = fopen(pawPath.c_str(), "wb");
    if (!fp) return;

    int maxLayers = (int) paint.screenLayers.size();

    fprintf(fp, "[GENERAL]\n");
    fprintf(fp, "LAYERS=%d\n", maxLayers);

    for (int i = 0; i < (int) paint.screenLayers.size(); i++) {
        fprintf(fp, "\n");
        fprintf(fp, "[LAYER_%d]\n", i);
        fprintf(fp, "LAYER_%d_ACTIVE=%d\n", i, paint.screenLayers[i]->active);

        string fileName = string("layer") + paint.intToChars(i) + ".png";
        saveLayer(paint, directoryPath.c_str(), fileName.c_str(), paint.screenLayers[i]->pixelBufferLayer);
    }

    fclose(fp);
}

void Saving::loadPaw(Paint& paint) {
    createPawDirectory(paint, paint.getPaintName());
    string directoryPath = string(paint.getWorkingDirectory()) + "/" + pawsPath + "/" + paint.getPaintName();
    string pawPath = string(directoryPath) + "/" + pawFile;

    FILE* fp = fopen(pawPath.c_str(), "rb");
    if (!fp) return;

    int maxLayers = 1;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        char key[100], value[100];
        if (sscanf(line, "%99[^=]=%99[^\n]", key, value) == 2) {
            string k = key;
            string v = value;
            if (k == "LAYERS") maxLayers = paint.charsToInt(v.c_str());
        }
    }

    paint.screenLayers.clear();
    paint.selectedLayer = 0;

    if (maxLayers <=0 ) maxLayers = 1;
    for (int i = 0; i < maxLayers; i++) {
        Layer* newLayer = new Layer();
        paint.screenLayers.push_back(newLayer);

        string fileName = string("layer") + paint.intToChars(i) + ".png";
        loadLayer(paint, directoryPath.c_str(), fileName.c_str(), paint.screenLayers[i]->pixelBufferLayer);
    }

    fseek(fp, 0, SEEK_SET);
    while (fgets(line, sizeof(line), fp)) {
        char key[100], value[100];
        if (sscanf(line, "%99[^=]=%99[^\n]", key, value) == 2) {
            string k = key;
            string v = value;

            if (k.find("LAYER_") != string::npos) {
                size_t pos = k.find('_');
                string layerString = k.substr(pos + 1);
                int layer = paint.charsToInt(layerString.c_str());

                if (k.find("_ACTIVE") != string::npos) {
                    paint.screenLayers[layer]->active = paint.charsToInt(v.c_str());
                }
            }
        }
    }

    paint.blendLayers(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Saving::savePaint(Paint& paint) {
    createPaintDirectory(paint);
    string directoryPath = string(paint.getWorkingDirectory()) + "/" + paintsPath;
    string paintString = string(pawName) + ".png";
    saveLayer(paint, directoryPath.c_str(), paintString.c_str(), pixelBufferSub);
}

bool Saving::saveLayer(Paint& paint, const char* path, const char* layerName, u16* buffer) {
    string pathString = string(path) + "/" + layerName;
    return paint.saveFileBuffer(pathString.c_str(), buffer);
}

bool Saving::loadLayer(Paint& paint, const char* path, const char* layerName, u16* buffer) {
    string pathString = string(path) + "/" + layerName;
    return paint.loadFileBuffer(pathString.c_str(), buffer);
}