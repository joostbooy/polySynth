#ifndef DiskRetryPage_h
#define DiskRetryPage_h

#include "topPage.h"

namespace DiskRetryPage {

	using TopPage::settings_;
	using TopPage::canvas_;
	using TopPage::pages_;
	using TopPage::leds_;
	using TopPage::disk_;

	typedef void (*Callback)();
	Callback refresh_dir_callback;

	enum FooterOptions {
		RETRY,
		NUM_OPTIONS
	};

	const char* const footer_text[NUM_OPTIONS] = {
		"RETRY"
	};

	void set_callback(Callback callback) {
		refresh_dir_callback = callback;
	}

	void init() {

	}

	void enter() {

	}

	void exit() {

	}

	void on_encoder(int id, int inc) {

	}

	void on_button(int id, int state) {
		if (!state) {
			return;
		}

		switch (Controller::buttonToFunction(id))
		{
		case RETRY:
			disk_->mount();

			if (disk_->mounted()) {
				disk_->reset();
				pages_->close(Pages::DISK_RETRY_PAGE);	// close page before calling refresh_dir_callback() !
				MessagePainter::show("CARD DETECTED");
				refresh_dir_callback();
			} else {
				MessagePainter::show("FAILED !");
			}
			break;
		default:
			break;
		}
	}

	void refresh_leds() {
		leds_->setFooterButtons(NUM_OPTIONS);
	}

	void msTick(uint16_t ticks) {

	}

	// Bottom to top
	void draw() {
		canvas_->setFont(Font::SMALL);
		canvas_->fill(0, 12, 256, 56, Canvas::WHITE);
		canvas_->drawText(0, 12, 256, 56, "NO CARD FOUND", Canvas::CENTER, Canvas::CENTER);
		WindowPainter::draw_footer(footer_text, NUM_OPTIONS);
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
