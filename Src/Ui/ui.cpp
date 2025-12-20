#include "ui.h"
#include "que.h"
#include "controller.h"


static const int enc_a_row = 2;
static const int enc_b_row = 3;
static const int num_buttons_rows = 6;
static const int buttons_rows[num_buttons_rows] = { 0, 1, 4, 5, 6, 7 };

void Ui::addEvent(ControlType type, uint8_t id, int8_t value)  {
	Ui::Event e;
	e.type = type;
	e.id = id;
	e.value = value;
	uiQue.write(e);
}

void Ui::init(Settings *settings, Engine *engine, Matrix *matrix, Display *display) {
	matrix_ = matrix;
	display_ = display;

	last_interval = 0;
	display_interval = 0;

	canvas_.init();
	leds_.init();
	pages_.init(settings, engine, this);

	uiQue.clear();
}

void Ui::poll() {
	uint8_t reading[8];
	matrix_->refresh(&reading[0]);

	for (int x = 0; x < 8; ++x) {
		for (int y = 0; y < 8; ++y) {
			uint8_t sw = y + (x * 8);
			sw_raw[sw] <<= 1;
			if (reading[x] & (1 << y)) {
				sw_raw[sw] |= 1;
			}
		}
	}

	for (int x = 0; x < 8 * 8; x += 8) {
		for (int y = 0; y < num_buttons_rows; ++y) {
			uint8_t button = buttons_rows[y] + x;
			if (sw_raw[button] == 0x80) {
				addEvent(Ui::BUTTON, button, 1);
			} else if (sw_raw[button] == 0x01) {
				addEvent(Ui::BUTTON, button, 0);
			}
		}

		uint8_t a = enc_a_row + x;
		uint8_t b = enc_b_row + x;
		if ((sw_raw[b] & 0x03) == 0x02 && (sw_raw[a] & 0x03) == 0x00) {
			addEvent(Ui::ENCODER, a, -1);
		} else if ((sw_raw[a] & 0x03) == 0x02 && (sw_raw[b] & 0x03) == 0x00) {
			addEvent(Ui::ENCODER, a, 1);
		}
	}

}

void Ui::process() {
	while (uiQue.readable()) {
		Ui::Event e = uiQue.read();
		switch (e.type)
		{
		case Ui::BUTTON:
			pages_.on_button(e.id, e.value);
			break;
		case Ui::ENCODER:
			pages_.on_encoder(e.id, e.value);
			break;
		default:
			break;
		}
	}

	uint32_t interval = (Micros::read() / 1000) - last_interval;

	if (interval >= 1) {
		last_interval += interval;
		leds_.setAll(Leds::BLACK);
		pages_.refresh_leds();
		matrix_->setLeds(leds_.data());
	}

	display_interval += interval;
	if (display_interval >= pages_.target_fps()) {
		sendDisplay();
	}
}

void Ui::sendDisplay() {
	while (display_->dmaBusy());
	display_interval = 0;
	canvas_.clear();
	pages_.draw();
	display_->sendBuffer(canvas_.data(), canvas_.size());
}