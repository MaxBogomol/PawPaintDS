#pragma once

#include "tool.h"

class Saving : public Tool {
    public:
        int line = 0;
        bool loading = false;
        bool saving = false;
        int doneTimer = 0;
        bool updateDrawTool = true;

        virtual ~Saving() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void setup(Paint& paint) override;
        virtual void update(Paint& paint) override;
        virtual void updateTool(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;

        virtual void drawIcon(Paint& paint, int x, int y, u16* buffer) override;

        virtual void drawTool(Paint& paint);

        virtual void createPaintDirectory(Paint& paint, const char* paintName);
        virtual void savePaint(Paint& paint);
        virtual void loadPaint(Paint& paint);
        virtual bool saveLayer(Paint& paint, const char* path, const char* layerName, u16* buffer);
        virtual bool loadLayer(Paint& paint, const char* path, const char* layerName, u16* buffer);
};