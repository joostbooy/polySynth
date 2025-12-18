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
		set_font(Font::SMALL);
	}

	void clear() {
		frameBuffer_.clear();
	}

	void draw_pixel(int x, int y, Color color) {
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
			frameBuffer_.write(x, y, SettingsUtils::clip_max(0xF, frameBuffer_.read(x, y) * 3));
			break;
		default:
			break;
		}
	}

	void horizontal_line(int x, int y, int w, Color color) {
		int x2 = x + w;
		for (int xi = x; xi < x2; ++xi) {
			draw_pixel(xi, y, color);
		}
	}

	void vertical_line(int x, int y, int h, Color color) {
		int y2 = y + h;
		for (int yi = y; yi < y2; ++yi) {
			draw_pixel(x, yi, color);
		}
	}

	void fill(int x, int y, int w, int h, Color color) {
		int y2 = y + h;
		for (int yi = y; yi < y2; ++yi) {
			horizontal_line(x, yi, w, color);
		}
	}

	void frame(int x, int y, int w, int h, Color color, int border = 1) {
		int x2 = SettingsUtils::clip_min(0, x + w - 1);
		int y2 = SettingsUtils::clip_min(0, y + h - 1);

		for (int i = 0; i < border; ++i) {
			horizontal_line(x, y + i, w, color);
			horizontal_line(x, y2 - i, w, color);
			vertical_line(x + i, y, h, color);
			vertical_line(x2 - i, y, h, color);
		}
	}

	void get_xy_allignment(int *x, int *y, int w, int h, int frame_w, int frame_h, Allign x_allign, Allign y_allign) {
		int x_ = *x;
		int y_ = *y;

		switch (x_allign)
		{
		case LEFT:
			break;
		case CENTER:
			x_ += (w - frame_w) / 2;
			break;
		case RIGHT:
			x_ += (w - frame_w);
			break;
		default:
			break;
		}

		switch (y_allign)
		{
		case TOP:
			break;
		case CENTER:
			y_ += (h - frame_h) / 2;
			break;
		case BOTTOM:
			y_ += (h - frame_h);
			break;
		default:
			break;
		}

		*x = SettingsUtils::clip(0, width() - frame_w, x_);
		*y = SettingsUtils::clip(0, height() - frame_h, y_);
	}

	// bitmap
	Bitmap &bitmap() {
		return bitmap_;
	}

	void set_bitmap(Bitmap::Id id, Color color = BLACK) {
		bitmap_color = color;
		bitmap_.set(id);
	}

	// text
	Font &font() {
		return font_;
	}

	void set_font(Font::Type type, Color color = BLACK) {
		font_color = color;
		font_.set_type(type);
	}

	int text_cursor() {
		return text_cursor_;
	}

	void draw_text(int x, int y, const char *str, Color color = BLACK) {
		if (str) {
			font_color = color;
			while (*str) {
				draw_char(x, y, *str++);
				x = text_cursor();
			}
		}
	}

	void draw_text(int x, int y, int w, int h, const char *str, Allign x_allign, Allign y_allign, Color color = BLACK) {
		if (str) {
			int x_ = x;
			int y_ = y;
			get_xy_allignment(&x_, &y_, w, h, font_.string_width(str), font_.height(), x_allign, y_allign);
			draw_text(x_, y_, str, color);
		}
	}

	void draw_bitmap(int x, int y, int w, int h, Bitmap::Id id, Allign x_allign, Allign y_allign) {
		int x_ = x;
		int y_ = y;
		set_bitmap(id);
		get_xy_allignment(&x_, &y_, w, h, bitmap_.width, bitmap_.height, x_allign, y_allign);
		draw_bitmap(x_, y_, id);
	}

	void draw_char(int x, int y, char c);
	void draw_bitmap(int x, int y, Bitmap::Id id, int frame = 0);

private:
	int text_cursor_ = 0;
	Color font_color = BLACK;
	Color bitmap_color = BLACK;

	Font font_;
	Bitmap bitmap_;
	FrameBuffer frameBuffer_;
};

#endif
