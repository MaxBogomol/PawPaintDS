#pragma once

#include "tool.h"

class ScreenKeyboard : public Tool {
    public:
        virtual ~ScreenKeyboard() {} 

        virtual const char* getName(Paint& paint) override;

        virtual void update(Paint& paint) override;
};