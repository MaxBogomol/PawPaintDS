#include "paint.h"

Paint paint;

int main(void) {
	if (!fatInitDefault()) {
		return 0;
    }

	paint.setup();
	paint.setupVideo();
	paint.setupLayers();
	paint.setupTools();

	while (pmMainLoop()) {
		paint.updateInputs();
		paint.updateTools();
		paint.updateVideo();
	}

    return 0;
}
