#pragma once

#include "tool.h"

class FileManager : public Tool {
    public:
        virtual ~FileManager() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void update(Paint& paint) override;

        virtual u16 *getDrawLayer(Paint& paint);
};