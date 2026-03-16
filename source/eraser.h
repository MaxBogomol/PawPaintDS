#pragma once

#include "tool.h"

class Eraser : public Tool {
    public:
        virtual ~Eraser() {} 

        virtual void update(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;
};