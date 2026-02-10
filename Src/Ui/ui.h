#ifndef Ui_h
#define Ui_h

#include "que.h"
#include "canvas.h"
#include "settings.h"
#include "pages.h"
#include "engine.h"
#include "matrix.h"
#include "display.h"
#include "switches.h"
#include "adc.h"
#include "leds.h"
#include "pots.h"
#include "buttons.h"
#include <stdint.h>

class Ui {

public:

	Pots &pots() { return pots_; }
	Leds &leds() { return leds_; }
	Buttons &buttons() { return buttons_; }
	Pages &pages() { return pages_; }
	Canvas &canvas() { return canvas_; }

	void init(Settings*, Engine*, Matrix*, Display*, Switches*, Adc*);
	void poll();
	void process();
	void sendDisplay();

private:
	enum ControlType {
		BUTTON,
		ENCODER
	};

	struct Event {
		ControlType type;
		uint8_t id;
		int8_t value;
	};

	Canvas canvas_;
	Pages pages_;
	Leds leds_;
	Buttons buttons_;
	Pots pots_;

	Engine *engine_;
	Settings *settings_;
	Matrix *matrix_;
	Display *display_;
	Switches *switches_;
	Adc *adc_;

	uint32_t lastInterval_ = 0;
	uint32_t displayInterval_ = 0;
	uint8_t encoderRaw_[4];
	bool lastState_[8 * 6];

	Que<Ui::Event, 16> uiQue;
	void addEvent(ControlType, uint8_t, int8_t);
	void processSwitches();
};

#endif
