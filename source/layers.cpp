#include "layers.h"

#include "paint.h"

const char* Layers::getName(Paint& paint) {
    return "Layers";
}

void Layers::update(Paint& paint) {
    if (keysD & KEY_LEFT) {
        if (paint.selectedLayer - 1 >= 0) {
            paint.selectedLayer--;
        }
    }
    if (keysD & KEY_RIGHT) {
        if (paint.selectedLayer + 1 <= 3) {
            paint.selectedLayer++;
        }
    }
}

void Layers::open(Paint& paint) {

}


void Layers::close(Paint& paint) {

}