#pragma once

#include "tool.h"

#include <cstdint>

typedef uint16_t u16;

class ColorPicker : public Tool {
    public:
        int hue = 0;
        int hueOld = 0;
        int colorX = 0;
        int colorY = 0;
        int colorXOld = 0;
        int colorYOld = 0;
        bool updatePicker = false;
        bool updateHue = false;
        bool updateSelected = false;
        bool updateNewSelected = false;
        u16 selectedColor = 0;
        u16 newSelectedColor = 0;

        virtual ~ColorPicker() {} 

        virtual void setup(Paint& paint) override;
        virtual void update(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;

        u16 *getDrawLayer(Paint& paint);

        virtual void drawPicker(Paint& paint);
        virtual void drawHue(Paint& paint);
        virtual void drawSelectedColor(Paint& paint);
        virtual void drawNewSelectedColor(Paint& paint);
        virtual void drawOutlines(Paint& paint);
        virtual void drawPickerPointers(Paint& paint);
        virtual void drawHuePointer(Paint& paint);
};