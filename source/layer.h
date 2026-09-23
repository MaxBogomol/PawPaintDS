#pragma once

#include <nds.h>

class Layer {
    public:
        u16 pixelBufferLayer[SCREEN_WIDTH * SCREEN_HEIGHT];
        bool active = true;

        virtual ~Layer() {} 

        virtual void fillLayer(u16 color);
        virtual void clearLayer();
};