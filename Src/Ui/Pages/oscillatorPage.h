#ifndef OscillatorPage_h
#define OscillatorPage_h

#include "listPage.h"
#include "oscillator.h"
#include "oscillatorList.h"

namespace OscillatorPage {

	using TopPage::settings_;
	using TopPage::engine_;
	using TopPage::ui_;

	bool pasteable_;
	Oscillator oscillator_;
	OscillatorList oscillatorList_;

	void clear() {
		settings_->oscillator().init();
	}

	void copy() {
		oscillator_.paste(&settings_->oscillator());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings_->oscillator().paste(&oscillator_);
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		oscillator_.init();
		oscillatorList_.init(engine_, settings_, ui_);
	}

	void enter() {
		ListPage::setList(&oscillatorList_);
		ListPage::setClearCallback(&clear);
		ListPage::setCopyCallback(&copy);
		ListPage::setPasteCallback(&paste);
		ListPage::enter();
	}

	void exit()  {
		ListPage::exit();
	}

	void on_button(int id, int state) {
		ListPage::on_button(id, state);
	}

	void on_encoder(int id, int state) {
		ListPage::on_encoder(id, state);
	}

	void refresh_leds() {
		ListPage::refresh_leds();
	}

	void draw() {
		ListPage::draw();
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
