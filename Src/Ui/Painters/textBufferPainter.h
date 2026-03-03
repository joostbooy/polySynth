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
		topRow_ = 0;
		for (int i = 0; i < kNumRows; ++i) {
			text_[i].clear();
		}
	}

	static void write(const char* text) {
		text_[entries_ % kNumRows].write(text);

		++entries_;
		if (entries_ >= kNumRows) {
			topRow_ = (entries_ - kNumRows) % kNumRows;
		}
	}

	static void draw() {
		int x = 0;
		int y = 10;
		int w = canvas_->width();
		int h = (canvas_->height() - 20) / kNumRows;

		canvas_->setFont(Font::SMALL);

		for (int i = 0; i < kNumRows; ++i) {
			int text__row = (topRow_ + i) % kNumRows;
			canvas_->drawText(x, y + (h * i), w, h, text_[text__row].read(), Canvas::LEFT, Canvas::CENTER);
		}
	}

private:
	static inline int entries_;
	static inline int topRow_;
	static inline const int kNumRows = 6;
	static inline StringBuilderBase<32>text_[kNumRows];
	static inline Canvas *canvas_;
};

#endif
