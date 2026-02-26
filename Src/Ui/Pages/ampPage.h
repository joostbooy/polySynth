#ifndef AmpPage_h
#define AmpPage_h

#include "listPage.h"
#include "amp.h"
#include "ampList.h"

namespace AmpPage {

	using TopPage::settings_;
	using TopPage::engine_;
	using TopPage::ui_;

	bool pasteable_;
	Amp amp_;
	AmpList ampList_;

	void clear() {
		settings_->amp().init();
		ui_->resetAllPots();
	}

	void copy() {
		amp_.paste(&settings_->amp());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings_->amp().paste(&amp_);
			ui_->resetAllPots();
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		amp_.init();
		ampList_.init(engine_, settings_, ui_);
	}

	void enter() {
		ListPage::setList(&ampList_);
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
