#ifndef EmptyPage_h
#define EmptyPage_h

#include "topPage.h"

namespace EmptyPage {

	void init() {

	}

	void enter() {

	}

	void exit()  {

	}

	void on_button(int id, int state) {

	}

	void on_encoder(int id, int state) {

	}

	void refresh_leds() {

	}

	void draw() {
		
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
