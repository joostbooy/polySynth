#ifndef Canvas_h
#define Canvas_h

#include "font.h"
#include "bitmap.h"
#include "frameBuffer.h"
#include "settingsUtils.h"

class Canvas {

public:

	enum Color {
		WHITE,
		LIGHT_GRAY,
		GRAY,
		DARK_GRAY,
		BLACK,
		INVERTED,
		SUBTRACTED,
		MULTIPLIED,
	};

	enum Allign {
		LEFT,
		RIGHT,
		CENTER,
		TOP,
		BOTTOM,
	};

	// frameBuffer
	const size_t width() { return frameBuffer_.width(); }
	const size_t height() { return frameBuffer_.height(); }
	const size_t size() { return frameBuffer_.size(); }
	uint8_t *data() { return frameBuffer_.data(); }

	void init() {
		setFont(Font::SMALL);
	}

	void clear() {
		frameBuffer_.clear();
	}

	void drawPixel(int x, int y, Color color) {
		if (frameBuffer_.inside(x, y) == false) {
			return;
		}

		switch (color)
		{
		case WHITE:
			frameBuffer_.write(x, y, 0x00);
			break;
		case LIGHT_GRAY:
			frameBuffer_.write(x, y, 0x04);
			break;
		case GRAY:
			frameBuffer_.write(x, y, 0x07);
			break;
		case DARK_GRAY:
			frameBuffer_.write(x, y, 0x0C);
			break;
		case BLACK:
			frameBuffer_.write(x, y, 0x0F);
			break;
		case INVERTED:
			frameBuffer_.write(x, y, frameBuffer_.read(x, y) ^ 0xFF);
			break;
		case SUBTRACTED:
			frameBuffer_.write(x, y, frameBuffer_.read(x, y) / 3);
			break;
		case MULTIPLIED:
			frameBuffer_.write(x, y, SettingsUtils::clipMax(0xF, frameBuffer_.read(x, y) * 3));
			break;
		default:
			break;
		}
	}

	void horizontalLine(int x, int y, int w, Color color) {
		int x2 = x + w;
		for (int xi = x; xi < x2; ++xi) {
			drawPixel(xi, y, color);
		}
	}

	void verticalLine(int x, int y, int h, Color color) {
		int y2 = y + h;
		for (int yi = y; yi < y2; ++yi) {
			drawPixel(x, yi, color);
		}
	}

	void fill(int x, int y, int w, int h, Color color) {
		int y2 = y + h;
		for (int yi = y; yi < y2; ++yi) {
			horizontalLine(x, yi, w, color);
		}
	}

	void frame(int x, int y, int w, int h, Color color, int border = 1) {
		int x2 = SettingsUtils::clipMin(0, x + w - 1);
		int y2 = SettingsUtils::clipMin(0, y + h - 1);

		for (int i = 0; i < border; ++i) {
			horizontalLine(x, y + i, w, color);
			horizontalLine(x, y2 - i, w, color);
			verticalLine(x + i, y, h, color);
			verticalLine(x2 - i, y, h, color);
		}
	}

	void getXyAllignment(int *x, int *y, int w, int h, int frameW, int frameH, Allign xAllign, Allign yAllign) {
		int x_ = *x;
		int y_ = *y;

		switch (xAllign)
		{
		case LEFT:
			break;
		case CENTER:
			x_ += (w - frameW) / 2;
			break;
		case RIGHT:
			x_ += (w - frameW);
			break;
		default:
			break;
		}

		switch (yAllign)
		{
		case TOP:
			break;
		case CENTER:
			y_ += (h - frameH) / 2;
			break;
		case BOTTOM:
			y_ += (h - frameH);
			break;
		default:
			break;
		}

		*x = SettingsUtils::clip(0, width() - frameW, x_);
		*y = SettingsUtils::clip(0, height() - frameH, y_);
	}

	// bitmap
	Bitmap &bitmap() {
		return bitmap_;
	}

	void setBitmap(Bitmap::Id id, Color color = BLACK) {
		bitmap_color = color;
		bitmap_.set(id);
	}

	// text
	Font &font() {
		return font_;
	}

	void setFont(Font::Type type, Color color = BLACK) {
		fontColor_ = color;
		font_.set_type(type);
	}

	int textCursor() {
		return textCursor_;
	}

	void drawText(int x, int y, const char *str, Color color = BLACK) {
		if (str) {
			fontColor_ = color;
			while (*str) {
				drawChar(x, y, *str++);
				x = textCursor();
			}
		}
	}

	void drawText(int x, int y, int w, int h, const char *str, Allign xAllign, Allign yAllign, Color color = BLACK) {
		if (str) {
			int x_ = x;
			int y_ = y;
			getXyAllignment(&x_, &y_, w, h, font_.string_width(str), font_.height(), xAllign, yAllign);
			drawText(x_, y_, str, color);
		}
	}

	void draw_bitmap(int x, int y, int w, int h, Bitmap::Id id, Allign xAllign, Allign yAllign) {
		int x_ = x;
		int y_ = y;
		setBitmap(id);
		getXyAllignment(&x_, &y_, w, h, bitmap_.width, bitmap_.height, xAllign, yAllign);
		draw_bitmap(x_, y_, id);
	}

	void drawChar(int x, int y, char c);
	void draw_bitmap(int x, int y, Bitmap::Id id, int frame = 0);

private:
	int textCursor_ = 0;
	Color fontColor_ = BLACK;
	Color bitmap_color = BLACK;

	Font font_;
	Bitmap bitmap_;
	FrameBuffer frameBuffer_;
};

#endif
