#ifndef FooterPage_h
#define FooterPage_h

#include "controller.h"

namespace FooterPage {

	int numOptions_ = 0;
	const char* const *text_;
	void(*callback_)(int) = nullptr;

	void init() {
	
	}

	void setText(const char* const *text) {
		text_ = text;
	}

	void setCallback(void(*callback)(int)) {
		callback_ = callback;
	}

	void setNumOptions(int value) {
		numOptions_ = value;
		if (numOptions_ >= 8) {
			numOptions_ = 8;
		}
	}

	void enter() {

	}

	void exit()  {
		callback_ = nullptr;
		numOptions_ = 0;
	}

	void on_button(int id, int state) {
		int index = Controller::buttonToFunction(id);

		if (index < 0) {
			return;
		}

		if (Controller::isPressed(Controller::SHIFT) && numOptions_ > 4) {
			index += 4;
		} 
		
		if (callback_) {
			callback_(index);
		}
	}

	void on_encoder(int id, int state) {
		
	}

	void refresh_leds() {
		
	}

	void draw() {
		if (Controller::isPressed(Controller::SHIFT) && numOptions_ > 4) {
			WindowPainter::draw_footer(&text_[4], numOptions_ - 4);
		} else {
			WindowPainter::draw_footer(text_, numOptions_ >= 4 ? 4 : numOptions_);
		}
	}

	const size_t target_fps() {
		return 1000 / 16;
	}

	Pages::Page page = {
		&init,
		&enter,
		&exit,
		&draw,
		&refresh_leds,
		&on_button,
		&on_encoder,
		&target_fps
	};

};

#endif
