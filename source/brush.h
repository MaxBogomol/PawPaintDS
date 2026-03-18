#pragma once

#include "tool.h"

#include <cstdint>

typedef uint16_t u16;

class Brush : public Tool {
    public:
        virtual ~Brush() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void update(Paint& paint) override;

        virtual void drawLine(Paint& paint, int x0, int y0, int x1, int y1, u16* buffer, u16 color);

        virtual u16 *getSelectedLayer(Paint& paint);
        virtual u16 getSelectedColor(Paint& paint);
};