#pragma once

#include "tool.h"

class Eyedropper : public Tool {
    public:
        virtual ~Eyedropper() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void update(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;
};