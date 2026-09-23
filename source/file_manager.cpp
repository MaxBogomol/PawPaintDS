#include "file_manager.h"

#include "paint.h"
#include "language.h"

const char* FileManager::getName(Paint& paint) {
    return STR_FILES.c_str();
}

void FileManager::update(Paint& paint) {
    paint.drawSquareOutline(2, 2, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4, getDrawLayer(paint), blackColor);
    paint.drawSquare(3, 3, SCREEN_WIDTH - 6, SCREEN_HEIGHT - 6, getDrawLayer(paint), paint.getSelectedThemeColor());

    string path = string(paint.getPaintDirectory());
    vector<string> files = paint.getDirectoryFiles(path.c_str());

    for (int i = 0; i < (int) files.size(); i++) {
        paint.drawText(20, 8 + (i * 18), files[i].c_str(), getDrawLayer(paint), blackColor);
    }
}

u16 *FileManager::getDrawLayer(Paint& paint) {
    return pixelBufferSub;
}