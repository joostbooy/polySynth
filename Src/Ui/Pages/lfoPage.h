#ifndef LfoPage_h
#define LfoPage_h

#include "listPage.h"
#include "lfo.h"
#include "lfoList.h"

namespace LfoPage {

	using TopPage::engine_;
	using TopPage::settings_;
	using TopPage::canvas_;

	bool pasteable_;
	Lfo lfo_;
	LfoList lfoList_;

	void clear() {
		settings_->selected_lfo().init();
	}

	void copy() {
		lfo_.paste(&settings_->selected_lfo());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings_->selected_lfo().paste(&lfo_);
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		lfo_.init();
		lfoList_.init(engine_, settings_);
	}

	void enter() {
		ListPage::set_list(&lfoList_);
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

		Lfo lfo;
		LfoEngine lfoEngine;

		lfo.paste(&settings_->selected_lfo());
		lfo.set_clock_sync(false);
		lfo.set_randomise(false);
		lfo.set_speed(0.152f);	// 0.152f results in 7.8hz (control_rate / w = 7.8 hz)
		lfoEngine.init(&lfo);

		const int x = 64;
		const int y = 5;
		const int w = 128;
		const int h = 32;

		for (int x2 = 0; x2 < w; ++x2) {
			int y2 = h * (1.f - lfoEngine.next());
			canvas_->draw_pixel(x + x2, y + y2, Canvas::BLACK);
		}

		int index = settings_->selected_lfo_index();
		float phase = engine_->modulationEngine().lfoEngine(index).phase();
		canvas_->vertical_line(x + (phase * w), y, h, Canvas::BLACK);
	}

	const size_t target_fps() {
		return 1000 / 24;
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
