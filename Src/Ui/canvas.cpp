#include "canvas.h"

void Canvas::draw_bitmap(int x, int y, Bitmap::Id id, int frame) {
	setBitmap(id);

	int shift = 0;
	int x2 = x + bitmap_.width;
	int y2 = y + bitmap_.height;
	const uint8_t* ptr = bitmap_.pointer(frame);

	for (int xi = x; xi < x2; ++xi) {
		for (int yi = y; yi < y2; ++yi) {
			uint8_t pixel = (*ptr >> shift) & 1;
			drawPixel(xi, yi, pixel ? BLACK : WHITE);
			if (++shift >= 8) {
				++ptr;
				shift = 0;
			}
		}

		if(shift != 0) {
			++ptr;
			shift = 0;
		}
	}
}

void Canvas::drawChar(int x, int y, char c) {
	const uint8_t* ptr = font_.data(c);
	int char_width = font_.width(c);
	textCursor_ = x;

	for (int i = 0; i < char_width; ++i) {
		uint8_t data = *ptr++;
		for (int pixel = 0; pixel < font_.height(); ++pixel) {
			if (data & (1 << pixel)) {
				drawPixel(textCursor_, y + pixel, fontColor_);
			}
		}
		++textCursor_;
	}
}
