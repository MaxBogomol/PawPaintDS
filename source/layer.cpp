#include "layer.h"

#include "paint.h"

void Layer::fillLayer(u16 color) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			pixelBufferLayer[x + (y * SCREEN_WIDTH)] = color;
		}
	}
}

void Layer::clearLayer() {
    fillLayer(alphaColor);
}