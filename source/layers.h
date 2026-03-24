#pragma once

#include "tool.h"

class Layers : public Tool {
    public:
        int line = 0;
        bool updateDrawTool = true;

        virtual ~Layers() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void setup(Paint& paint) override;
        virtual void update(Paint& paint) override;
        virtual void updateTool(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;

        virtual void drawIcon(Paint& paint, int x, int y, u16* buffer) override;

        virtual void drawTool(Paint& paint);
};