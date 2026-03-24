#include "eraser.h"

#include "paint.h"

const char* Eraser::getName(Paint& paint) {
    return "Eraser";
}

void Eraser::drawIcon(Paint& paint, int x, int y, u16* buffer) {
    switch (type) {
    	case 0: return paint.drawSprite(x, y, 16, 16, eraser_square_iconBitmap, eraser_square_iconPal, buffer); break;
    	case 1: return paint.drawSprite(x, y, 16, 16, eraser_circle_iconBitmap, eraser_circle_iconPal, buffer); break;
		case 2: return paint.drawSprite(x, y, 16, 16, eraser_dot_iconBitmap, eraser_dot_iconPal, buffer); break;
        case 3: return paint.drawSprite(x, y, 16, 16, eraser_square_noise_iconBitmap, eraser_square_noise_iconPal, buffer); break;
    	case 4: return paint.drawSprite(x, y, 16, 16, eraser_circle_noise_iconBitmap, eraser_circle_noise_iconPal, buffer); break;
		case 5: return paint.drawSprite(x, y, 16, 16, eraser_dot_noise_iconBitmap, eraser_dot_noise_iconPal, buffer); break;
    }
}

u16 Eraser::getSelectedColor(Paint& paint) {
    return alphaColor;
}