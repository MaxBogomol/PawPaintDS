#pragma once

#include "tool.h"

class Layers : public Tool {
    public:
        virtual ~Layers() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void update(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;
};