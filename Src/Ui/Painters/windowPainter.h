#ifndef WindowPainter_h
#define WindowPainter_h

#include "canvas.h"

class WindowPainter {

public:

	static void init(Canvas *canvas) {
		canvas_ = canvas;
	}

	static void draw_vertical_scollbar(int x, int y, int w, int h, int top_item, int num_items, int visible_items) {
		if (num_items > visible_items) {
			int bar_y = (top_item * h) / num_items;
			int bar_h = (visible_items * h) / num_items;

			canvas_->fill(x, y, w, h, Canvas::BLACK);
			canvas_->fill(x + 1, y + bar_y, w - 2, bar_h, Canvas::WHITE);
		}
	}

	static void draw_horizontal_scollbar(int x, int y, int w, int h, int top_item, int num_items, int visible_items) {
		if (num_items > visible_items) {
			int bar_x = (top_item * w) / num_items;
			int bar_w = (visible_items * w) / num_items;

			canvas_->fill(x, y, w, h, Canvas::BLACK);
			canvas_->fill(x + bar_x, y + 1, bar_w, h - 2, Canvas::WHITE);
		}
	}

	static void draw_footer(const char* const *text, int num_options) {
		const int h = 10;
		const int y = canvas_->height() - h;
		const int w = canvas_->width() / kMaxFooterOptions;

		if (num_options > kMaxFooterOptions) {
			num_options = kMaxFooterOptions;
		}

		canvas_->fill(0, y, canvas_->width(), h, Canvas::WHITE);
		canvas_->horizontalLine(0, y, canvas_->width(), Canvas::BLACK);

		for (int i = 0; i < kMaxFooterOptions; ++i) {
			int x = i * w;
			canvas_->verticalLine(x, y, h, Canvas::BLACK);
			if (i < num_options) {
				canvas_->drawText(x, y, w, h, text[i], Canvas::CENTER, Canvas::CENTER);
			}
		}
	}

private:
	static Canvas *canvas_;
	static const int kMaxFooterOptions = 4;
};

#endif
