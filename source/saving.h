#pragma once

#include <string>

using namespace std;

#include "tool.h"

class Saving : public Tool {
    public:
        int line = 0;
        bool active = false;
        bool loading = false;
        bool saving = false;
        bool savingExport = false;
        int doneTimer = 0;
        string pawDirectory = "";
        string pawName = "Unnamed";
        bool updateDrawTool = true;

        virtual ~Saving() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void setup(Paint& paint) override;
        virtual void update(Paint& paint) override;
        virtual void updateTool(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;
        virtual void redraw(Paint& paint) override;

        virtual void drawIcon(Paint& paint, int x, int y, u16* buffer) override;
        virtual void drawHints(Paint& paint, int x, int y, u16* buffer) override;

        virtual void drawTool(Paint& paint);

        virtual void createPawDirectory(Paint& paint, const char* paintName);
        virtual void createPaintDirectory(Paint& paint);
        virtual void savePaw(Paint& paint);
        virtual void loadPaw(Paint& paint);
        virtual void savePaint(Paint& paint);
        virtual bool saveLayer(Paint& paint, const char* path, const char* layerName, u16* buffer);
        virtual bool loadLayer(Paint& paint, const char* path, const char* layerName, u16* buffer);
};