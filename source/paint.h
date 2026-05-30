#pragma once

#include <nds.h>
#include <filesystem.h>
#include <fat.h>
#include <png.h>
#include <string>
#include <vector>

using namespace std;

#include "tool.h"
#include "brush.h"
#include "eraser.h"
#include "eyedropper.h"
#include "color_picker.h"
#include "layers.h"
#include "saving.h"
#include "settings.h"
#include "info.h"
#include "language.h"

#include "pawscript_font.h"
#include "pawscript_font_test.h"
#include "paint_icon.h"
#include "paint_monochrome_icon.h"
#include "buttons_icon.h"
#include "brush_icon.h"
#include "brush_square_icon.h"
#include "brush_circle_icon.h"
#include "brush_dot_icon.h"
#include "brush_square_noise_icon.h"
#include "brush_circle_noise_icon.h"
#include "brush_dot_noise_icon.h"
#include "eraser_icon.h"
#include "eraser_square_icon.h"
#include "eraser_circle_icon.h"
#include "eraser_dot_icon.h"
#include "eraser_square_noise_icon.h"
#include "eraser_circle_noise_icon.h"
#include "eraser_dot_noise_icon.h"
#include "eyedropper_icon.h"
#include "color_picker_icon.h"
#include "layers_icon.h"
#include "saving_icon.h"
#include "saving_error_icon.h"
#include "saving_loading_icon.h"
#include "saving_done_icon.h"
#include "settings_icon.h"
#include "info_icon.h"

inline u16 alphaColor = ARGB16(0, 0, 0, 0);
inline u16 whiteColor = ARGB16(1, 31, 31, 31);
inline u16 blackColor = ARGB16(1, 0, 0, 0);
inline u16 redColor = ARGB16(1, 31, 0, 0);
inline u16 greenColor = ARGB16(1, 0, 31, 0);
inline u16 blueColor = ARGB16(1, 0, 0, 31);
inline u16 pinkColor = ARGB16(1, 31, 0, 31);

inline u16 pinkFoxThemeColor = ARGB16(1, 31, 24, 25); //#fec8cf
inline u16 maidThemeColor = ARGB16(1, 6, 5, 7); //#2e2939
inline u16 aceThemeColor = ARGB16(1, 20, 6, 20); //#a231a2

inline u16 pixelBufferMain[SCREEN_WIDTH * SCREEN_HEIGHT];

inline u16 pixelBufferSub[SCREEN_WIDTH * SCREEN_HEIGHT];
inline u16 pixelBufferSubLayer0[SCREEN_WIDTH * SCREEN_HEIGHT];
inline u16 pixelBufferSubLayer1[SCREEN_WIDTH * SCREEN_HEIGHT];
inline u16 pixelBufferSubLayer2[SCREEN_WIDTH * SCREEN_HEIGHT];
inline u16 pixelBufferSubLayer3[SCREEN_WIDTH * SCREEN_HEIGHT];

inline u16* bgMainDest;
inline u16* bgSubDest;

inline touchPosition touch;
inline int keysD;
inline int keysH;
inline int keysR;
inline int keysU;

inline int touchX = 0;
inline int touchY = 0;
inline int touchXOld = 0;
inline int touchYOld = 0;
inline int touchCount = 0;

inline Brush brush;
inline Eraser eraser;
inline Eyedropper eyedropper;
inline ColorPicker colorPicker;
inline Layers layers;
inline Saving saving;
inline Settings settings;
inline Info info;

inline bool activeSubLayer0 = true;
inline bool activeSubLayer1 = true;
inline bool activeSubLayer2 = true;
inline bool activeSubLayer3 = true;

inline const char* pawPaintPath = "fat:/PawPaintDS";
inline const char* paintsPath = "Paints";
inline const char* pawsPath = "Paws";
inline const char* languagesPath = "nitro:/languages";

inline int maxLanguages = 2;
inline int maxPaintThemes = 4;
inline int maxPaintIcons = 2;

inline const char* languageCodes[2] = {
    "en_us",
    "ru_ru"
};

struct HSV {
    int h;
    int s;
    int v;
};

class Paint {
    private:
        bool firstFrameTool = true;
        bool updateSubLayers = false;
        bool updateBlendSubLayers = false;

        const char* paintName = "Unnamed";

    public:
        int selectedLanguage = 0;
        int selectedTheme = 0;
        int selectedIcon = 0;
        int selectedLayer = 0;
        int selectedTool = 0;
        u16 selectedColor = blackColor;
        u16 selectedColorSub = whiteColor;
        bool reverseScreens = false;

        bool updateDrawAll = false;
        bool updateDrawSelectedColor = false;
        bool updateDrawTools = true;
        bool updateDrawColors = true;
        bool updateDrawPaintName = true;
        bool updateDrawPaintIcon = true;

        bool fileSystemInit = false;

        vector<Tool*> tools;

        void setupNitroFS();
        void setup();
        void setupVideo();
        void setupLayers();
        void setupTools();

        void updateInputs();
        void updateTools();
        void updateVideo();

        void drawTools();
        void drawColors();
        void drawPaintName();
        void drawPaintIcon();

        void blendSubLayers(int x, int y);
        void blendSubLayers(int x, int y, u16* buffer);
        void swapSubLayers(int l0, int l1);
        void updateSubLayersEnable();
        void updateSubLayersDisable();
        u16 *getLayer(int layer);

        u16 *getSelectedLayer();
        u16 getSelectedColor();

        u16 getPixel(int x, int y, u16* buffer);

        void drawPixel(int x, int y, u16* buffer, u16 color);
        void drawSquare(int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        void drawSquareOutline(int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        void drawSquareNoise(int x0, int y0, int x1, int y1, u16* buffer, u16 color, int xSize, int ySize, int xShift, int yShift, int xOffset, int yOffset);
        void drawCircleRadius(int xc, int yc, int r, u16* buffer, u16 color);
        void drawCircleRadiusNoise(int xc, int yc, int r, u16* buffer, u16 color, int xSize, int ySize, int xShift, int yShift, int xOffset, int yOffset);
        void drawCircleDiameter(int xc, int yc, int d, u16* buffer, u16 color);
        void drawCircleDiameterNoise(int xc, int yc, int d, u16* buffer, u16 color, int xSize, int ySize, int xShift, int yShift, int xOffset, int yOffset);
        u32 decodeChar(const char** s);
        int getCharLength(u32 c);
        int getTextLength(const char* text);
        void drawLine(int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        void drawChar(int x, int y, u32 c, u16* buffer, u16 color);
        void drawText(int x, int y, const char* text, u16* buffer, u16 color);
        void drawCharOutline(int x, int y, u32 c, u16* buffer, u16 color, u16 outlineColor);
        void drawTextOutline(int x, int y, const char* text, u16* buffer, u16 color, u16 outlineColor);
        void drawSprite(int x0, int y0, int x1, int y1, int xShift, int yShift, int xSize, int ySize, const unsigned int* spriteBitmap, u16* buffer);
        void drawSprite(int x0, int y0, int x1, int y1, const unsigned int* spriteBitmap, u16* buffer);

        u16 blendColors(u16 src, u16 dst);
        u16 HSVtoRGB(int h, int s, int v);
        u16 HSVtoRGB(HSV hsv);
        HSV RGBtoHSV(u16 color);
        int getDitherThreshold(int x, int y, int xSize, int ySize, int xShift, int yShift);
        const char* intToChars(int val);

        void setPaintName(const char* name);
        const char* getPaintName();
        bool saveFileBuffer(const char* path, u16* buffer);
        bool loadFileBuffer(const char* path, u16* buffer);
        bool makeDirectory(const char* path);
        bool directoryExist(const char* path);

        int getToolYOffset();
        int getToolsYOffset();
        int getToolsButtonsOffset();

        void drawYButton(int x, int y, u16* buffer);
        void drawXButton(int x, int y, u16* buffer);
        void drawBButton(int x, int y, u16* buffer);
        void drawAButton(int x, int y, u16* buffer);
        void drawScrollBox(int x, int y, int size, int scroll, u16* buffer);
        void clearBuffer(int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        void clearBuffer(int x0, int y0, int x1, int y1, u16* buffer);
        void blendSubLayers(int x0, int y0, int x1, int y1);

        u16 getThemeColor(int theme);
        u16 getSelectedThemeColor();

        const unsigned int* getIconSprite(int icon);
        const unsigned int* getSelectedIconSprite();

        const char* getLanguageCode(int language);
        const char* getSelectedLanguageCode();

        void clearSubLayers();

        bool readSelectedLanguage();
};