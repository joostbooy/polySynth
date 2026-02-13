#ifndef FilterPage_h
#define FilterPage_h

#include "listPage.h"
#include "filter.h"
#include "filterList.h"

namespace FilterPage {

	using TopPage::settings_;
	using TopPage::engine_;
	using TopPage::ui_;

	bool pasteable_;
	Filter filter_;
	FilterList filterList_;

	void clear() {
		settings_->filter().init();
		ui_->resetAllPots();
	}

	void copy() {
		filter_.paste(&settings_->filter());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings_->filter().paste(&filter_);
			ui_->resetAllPots();
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		filter_.init();
		filterList_.init(engine_, settings_);
	}

	void enter() {
		ListPage::setList(&filterList_);
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
