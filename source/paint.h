#pragma once

#include <nds.h>
#include <string>
#include <vector>

using namespace std;

#include "tool.h"
#include "brush.h"
#include "eraser.h"
#include "eyedropper.h"
#include "color_picker.h"
#include "layers.h"

inline u16 alphaColor = ARGB16(0, 0, 0, 0);
inline u16 whiteColor = ARGB16(1, 31, 31, 31);
inline u16 blackColor = ARGB16(1, 0, 0, 0);
inline u16 redColor = ARGB16(1, 31, 0, 0);
inline u16 greenColor = ARGB16(1, 0, 31, 0);
inline u16 blueColor = ARGB16(1, 0, 0, 31);

alignas(4) inline u16 pixelBufferMain[SCREEN_WIDTH * SCREEN_HEIGHT];

alignas(4) inline u16 pixelBufferSub[SCREEN_WIDTH * SCREEN_HEIGHT];
alignas(4) inline u16 pixelBufferSubLayer0[SCREEN_WIDTH * SCREEN_HEIGHT];
alignas(4) inline u16 pixelBufferSubLayer1[SCREEN_WIDTH * SCREEN_HEIGHT];
alignas(4) inline u16 pixelBufferSubLayer2[SCREEN_WIDTH * SCREEN_HEIGHT];
alignas(4) inline u16 pixelBufferSubLayer3[SCREEN_WIDTH * SCREEN_HEIGHT];

inline u16* bgMainDest;
inline u16* bgSubDest;

extern const u8 default_font_bin[];

inline touchPosition touch;
inline int keysD;
inline int keysH;
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

struct HSV {
    int h;
    int s;
    int v;
};

class Paint {
    private:

    public:
        int selectedLayer = 0;
        int selectedTool = 0;
        u16 selectedColor = blackColor;
        u16 selectedColorSub = whiteColor;

        bool updateSubLayers = true;

        vector<Tool*> tools;

        void setupVideo();
        void setupLayers();
        void setupTools();

        void updateInputs();
        void updateTools();
        void updateVideo();

        void blendSubLayers(int x, int y);

        u16 *getSelectedLayer();
        u16 getSelectedColor();

        u16 getPixel(int x, int y, u16* buffer);

        void drawPixel(int x, int y, u16* buffer, u16 color);
        void drawSquare(int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        void drawSquareOutline(int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        void drawCircleRadius(int xc, int yc, int r, u16* buffer, u16 color);
        void drawCircleDiameter(int xc, int yc, int d, u16* buffer, u16 color);
        void drawLine(int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        void drawChar(int x, int y, char c, u16* buffer, u16 color);
        void drawText(int x, int y, const char* text, u16* buffer, u16 color);
        void drawCharOutline(int x, int y, char c, u16* buffer, u16 color, u16 outlineColor);
        void drawTextOutline(int x, int y, const char* text, u16* buffer, u16 color, u16 outlineColor);

        u16 blendColors(u16 src, u16 dst);
        u16 HSVtoRGB(int h, int s, int v);
        HSV RGBtoHSV(u16 color);
        const char* intToChars(int val);
};