#ifndef ConfirmationPage_h
#define ConfirmationPage_h

namespace ConfirmationPage {

	using TopPage::canvas_;
	using TopPage::pages_;
	using TopPage::leds_;

	enum Option {
		CONFIRM,
		CANCEL,
		APPLY_TO_ALL,
		NUM_OPTIONS
	};

	const char* const option_text[NUM_OPTIONS] = {
		"CONFIRM",
		"CANCEL",
		"APPLY TO ALL"
	};


	typedef void(*Callback)(int);
	Callback callback_;
	StringBuilderBase<32>str;

	int x = 0;
	int y = 16;
	int w = 0;
	int h = 32;

	int num_options_;

	void set(const char* message, Callback callback, bool request_apply_to_all = false) {
		str.write(message);
		callback_ = callback;
		w = canvas_->font().string_width(str.read()) + 20;
		x = (canvas_->width() - w) / 2;
		num_options_ = request_apply_to_all ? 3 : 2;
	}

	void init() {

	}

	void enter() {
		if (callback_ == nullptr) {
			pages_->close(Pages::CONFIRMATION_PAGE);
		}
	}

	void exit() {
		callback_ = nullptr;
	}

	void on_button(int id, int state) {
		int selected_option = Controller::buttonToFunction(id);
		if (state >= 1 && selected_option >= 0 && selected_option < num_options_) {
			callback_(selected_option);
			pages_->close(Pages::CONFIRMATION_PAGE);
		}
	}

	void on_encoder(int id, int state) {

	}

	void refresh_leds() {

	}

	void draw() {
		canvas_->setFont(Font::SMALL);
		canvas_->fill(x + 4, y + 4, w, h, Canvas::SUBTRACTED);
		canvas_->frame(x, y, w, h, Canvas::WHITE);
		canvas_->fill(x + 1, y + 1, w - 2, h - 2, Canvas::BLACK);
		canvas_->frame(x + 4, y + 4, w - 8, h - 8, Canvas::WHITE);
		canvas_->drawText(x, y, w, h, str.read(), Canvas::CENTER, Canvas::CENTER, Canvas::WHITE);

		WindowPainter::draw_footer(option_text, num_options_);
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
