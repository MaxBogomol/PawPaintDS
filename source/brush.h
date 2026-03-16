#pragma once

#include "tool.h"

class Brush : public Tool {
    public:
        virtual ~Brush() {} 

        virtual void update(Paint& paint) override;
        virtual void open(Paint& paint) override;
        virtual void close(Paint& paint) override;
};