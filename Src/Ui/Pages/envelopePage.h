#ifndef EnvelopePage_h
#define EnvelopePage_h

#include "listPage.h"
#include "envelope.h"
#include "envelopeList.h"

namespace EnvelopePage {

	using TopPage::engine_;
	using TopPage::settings_;
	using TopPage::canvas_;
	using TopPage::ui_;

	bool pasteable_;
	Envelope envelope_;
	EnvelopeList envelopeList_;

	void clear() {
		settings_->selectedEnvelope().init();
		ui_->resetAllPots();
	}

	void copy() {
		envelope_.paste(&settings_->selectedEnvelope());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings_->selectedEnvelope().paste(&envelope_);
			ui_->resetAllPots();
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		envelope_.init();
		envelopeList_.init(engine_, settings_);
	}

	void enter() {
		ListPage::setList(&envelopeList_);
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
	/*
		Envelope envelope;
		EnvelopeEngine envelopeEngine;

		envelope.paste(&settings_->selected_envelope());

		// 0.623f results in 32.25 hz (control rate / (w / 4) = 31.25 hz)
		envelope.set_attack_time(Dsp::cross_fade(0.623f, 1.0f, envelope.attack_time()));
		envelope.set_decay_time(Dsp::cross_fade(0.623f, 1.0f, envelope.decay_time()));
		envelope.set_release_time(Dsp::cross_fade(0.623f, 1.0f, envelope.release_time()));

		int mode = envelope.mode();
		envelope.set_mode(Envelope::TRIGGER);

		if (mode == Envelope::TRIGGER) {
			envelope.set_hold_time(Dsp::cross_fade(0.623f, 1.0f, envelope.hold_time()));
		} else {
			envelope.set_hold_time(0.623f);
		}

		envelope.set_clock_sync(false);

		envelopeEngine.init(&envelope);
		envelopeEngine.attack();

		const int x = 64;
		const int y = 5;
		const int w = 128;
		const int h = 32;

		for (int x2 = 0; x2 < w; ++x2) {
			int y2 = h * (1.f - envelopeEngine.next());
			canvas_->draw_pixel(x + x2, y + y2, Canvas::BLACK);
		}

		int index = settings_->selected_envelope_index();
		float phase = engine_->voiceEngine().most_recent_voice().envelopeEngine(index).phase();
		canvas_->verticalLine(x + (phase * w), y, h, Canvas::BLACK);
		*/
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
