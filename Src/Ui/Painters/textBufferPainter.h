#ifndef TextBufferPainter_h
#define TextBufferPainter_h

#include "ui.h"
#include "StringBuilder.h"

class TextBufferPainter {

public:

	static void init(Canvas *canvas) {
		canvas_ = canvas;
	}

	static void clear() {
		entries_ = 0;
		top_row_ = 0;
		for (int i = 0; i < kNumRows; ++i) {
			text_[i].clear();
		}
	}

	static void write(const char* text) {
		text_[entries_ % kNumRows].write(text);

		++entries_;
		if (entries_ >= kNumRows) {
			top_row_ = (entries_ - kNumRows) % kNumRows;
		}
	}

	static void draw() {
		int x = 0;
		int y = 10;
		int w = canvas_->width();
		int h = (canvas_->height() - 20) / kNumRows;

		canvas_->setFont(Font::SMALL);

		for (int i = 0; i < kNumRows; ++i) {
			int text__row = (top_row_ + i) % kNumRows;
			canvas_->drawText(x, y + (h * i), w, h, text_[text__row].read(), Canvas::LEFT, Canvas::CENTER);
		}
	}

private:
	static int entries_;
	static int top_row_;
	static const int kNumRows = 6;
	static StringBuilderBase<32>text_[kNumRows];
	static Canvas *canvas_;
};

#endif
