#pragma once

#include "tool.h"

#include <cstdint>

typedef uint16_t u16;

class ColorPicker : public Tool {
    public:
        int hue = 0;
        bool updatePicker = false;
        bool updateSelected = false;
        u16 selectedColor = 0;
        u16 newSelectedColor = 0;

        virtual ~ColorPicker() {} 

        virtual void setup(Paint& paint) override;
        virtual void update(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;
};