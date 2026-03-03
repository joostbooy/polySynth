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

	void onButton(int id, int state) {

	}

	void onEncoder(int id, int state) {

	}

	void refreshLeds() {

	}

	void draw() {
		
	}

	const size_t targetFps() {
		return 1000 / 16;
	}

	Pages::Page page = {
		&init,
		&enter,
		&exit,
		&draw,
		&refreshLeds,
		&onButton,
		&onEncoder,
		&targetFps
	};

};

#endif
