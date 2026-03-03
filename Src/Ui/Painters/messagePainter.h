#ifndef MessagePainter_h
#define MessagePainter_h

#include "ui.h"
#include "stringBuilder.h"

class MessagePainter {

public:

	static void init(Canvas *canvas) {
		canvas_ = canvas;
	}

	static void show(const char* text, uint16_t millis = 1200) {
		str.write(text);
		canvas_->setFont(Font::SMALL);
		w = canvas_->font().string_width(str.read()) + 20;
		x = (canvas_->width() - w) / 2;
		millis_ = millis;
	}

	static void draw(uint32_t millis) {
		if (millis_ >= millis) {
			millis_ -= millis;
			canvas_->setFont(Font::SMALL);
			canvas_->fill(x + 4, y + 4, w, h, Canvas::SUBTRACTED);
			canvas_->frame(x, y, w, h, Canvas::WHITE);
			canvas_->fill(x + 1, y + 1, w - 2, h - 2, Canvas::BLACK);
			canvas_->frame(x + 4, y + 4, w - 8, h - 8, Canvas::WHITE);
			canvas_->drawText(x, y, w, h, str.read(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);
		}
	}

private:
	static inline int x = 0;
	static inline int y = 16;
	static inline int w = 0;
	static inline int h = 32;
	static inline Canvas *canvas_;
	static inline uint16_t millis_ = 0;
	static inline StringBuilderBase<32>str;
};

#endif
