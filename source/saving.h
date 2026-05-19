#pragma once

#include "tool.h"

class Saving : public Tool {
    public:
        virtual ~Saving() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void update(Paint& paint) override;

        virtual void drawIcon(Paint& paint, int x, int y, u16* buffer) override;
};