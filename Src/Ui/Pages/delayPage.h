#ifndef DelayPage_h
#define DelayPage_h

#include "listPage.h"
#include "delay.h"
#include "delayList.h"

namespace DelayPage {

	using TopPage::settings_;
	using TopPage::engine_;

	bool pasteable_;
	Delay delay_;
	DelayList delayList_;

	void clear() {
		settings_->delay().init();
	}

	void copy() {
		delay_.paste(&settings_->delay());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings_->delay().paste(&delay_);
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		delay_.init();
		delayList_.init(engine_, settings_);
	}

	void enter() {
		ListPage::set_list(&delayList_);
		ListPage::set_clear_callback(&clear);
		ListPage::set_copy_callback(&copy);
		ListPage::set_paste_callback(&paste);
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
