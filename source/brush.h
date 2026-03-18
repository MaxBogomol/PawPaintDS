#pragma once

#include "tool.h"

class Brush : public Tool {
    public:
        int type = 0;
        int line = 0;
        int squareSize = 1;
        int circleDiameter = 1;
        int dotRadius = 1;
        bool updateDrawTool = true;

        virtual ~Brush() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void setup(Paint& paint) override;
        virtual void update(Paint& paint) override;
        virtual void updateTool(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;

        virtual void drawIcon(Paint& paint, int x, int y, u16* buffer) override;

        virtual void drawLine(Paint& paint, int x0, int y0, int x1, int y1, u16* buffer, u16 color);
        virtual void drawTool(Paint& paint);

        virtual const char* getTypeName(Paint& paint, int type);

        virtual u16 *getSelectedLayer(Paint& paint);
        virtual u16 getSelectedColor(Paint& paint);
};