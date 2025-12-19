#ifndef InstrumentSampleListPage_h
#define InstrumentSampleListPage_h

#include "instrument.h"

namespace InstrumentSampleListPage {

	using TopPage::settings_;
	using TopPage::pages_;
	using TopPage::canvas_;
	using TopPage::leds_;
	using TopPage::engine_;


	enum FooterOptions {
		ADD,
		REMOVE,
		AUDITION,
		CLOSE,
		NUM_OPTIONS
	};

	const char* const footer_option_text[NUM_OPTIONS] = { "ADD", "REMOVE", "AUDITION", "CLOSE" };

	bool pasteable_ = false;
	Instrument instrument_;

	int sample_top_row_ = 0;
	int instrument_sample_top_row_ = 0;
	const int kMaxVisibleRows = 4;

	void scroll_to_row(int row, int *top_row) {
		if (row < *top_row) {
			*top_row = row;
		} else if (row >= (*top_row + kMaxVisibleRows)) {
			*top_row = row - (kMaxVisibleRows - 1);
		}
	}

	void scroll_to_selected_sample() {
		scroll_to_row(settings_->selected_sample_index(), &sample_top_row_);
	}

	void scroll_to_selected_instrument_sample() {
		scroll_to_row(settings_->selected_instrument_sample_index(), &instrument_sample_top_row_);
	}

	void copy() {
		instrument_.paste_sample_list(&settings_->selected_instrument());
		MessagePainter::show("LIST COPIED");
		pasteable_ = true;
	}

	void paste() {
		if (pasteable_) {
			settings_->selected_instrument().paste_sample_list(&instrument_);
			settings_->select_instrument_sample_index(settings_->selected_instrument_sample_index());
			scroll_to_selected_instrument_sample();
			MessagePainter::show("LIST PASTED");
		}
	}

	void remove() {
		if (settings_->selected_instrument().remove_sample(settings_->selected_instrument_sample_index())) {
			settings_->select_instrument_sample_index(settings_->selected_instrument_sample_index());
			scroll_to_selected_instrument_sample();
			MessagePainter::show("SAMPLE REMOVED");
		}
	}

	void add() {
		if (settings_->selected_instrument().add_sample(settings_->selected_sample())) {
			settings_->select_instrument_sample_index(settings_->selected_instrument().num_samples());
			scroll_to_selected_instrument_sample();
		}
	}

	void clear() {
		settings_->selected_instrument().clear_samples();
		settings_->select_instrument_sample_index(0);
		scroll_to_selected_instrument_sample();
	}

	void audition(bool state) {
		if (settings_->num_samples() > 0) {
			if (state) {
				engine_->set_sample_to_audition(&settings_->selected_instrument(), settings_->selected_sample());
				engine_->add_request_blocking(Engine::STOP);
				engine_->add_request_blocking(Engine::AUDITION);
			} else {
				engine_->add_request_blocking(Engine::STOP);
			}
		}
	}

	void init() {

	}

	void enter() {
		scroll_to_selected_sample();
		scroll_to_selected_instrument_sample();
	}

	void exit()  {

	}

	void on_encoder(int id, int state) {
		if (id == Controller::FUNCTION_ENC_A) {
			settings_->select_sample_index(settings_->selected_sample_index() + state);
			scroll_to_selected_sample();
		} else if (id == Controller::FUNCTION_ENC_D) {
			settings_->select_instrument_sample_index(settings_->selected_instrument_sample_index() + state);
			scroll_to_selected_instrument_sample();
		}
	}

	void on_function_button(int function, bool state) {
		switch (function)
		{
		case ADD:
			if (state) {
				add();
			}
			break;
		case REMOVE:
			if (state) {
				ConfirmationPage::set("REMOVE FROM LIST ?", [](int option) {
					if (option == ConfirmationPage::CONFIRM) {
						remove();
					}
				});
				pages_->open(Pages::CONFIRMATION_PAGE);
			}
			break;
		case AUDITION:
			audition(state);
			break;
		case CLOSE:
			if (state) {
				pages_->close(Pages::INSTRUMENT_SAMPLE_LIST_PAGE);
			}
			break;
		default:
			break;
		}
	}

	void on_button(int id, int state) {
		if (state) {
			bool shifted = Controller::is_pressed(Controller::SHIFT_BUTTON);
			int enc_id = shifted ? Controller::FUNCTION_ENC_D : Controller::FUNCTION_ENC_A;

			switch (id)
			{
			case Controller::UP_BUTTON:
				on_encoder(enc_id, -1);
				break;
			case Controller::DOWN_BUTTON:
				on_encoder(enc_id, 1);
				break;
			case Controller::RIGHT_BUTTON:
				on_function_button(ADD, 1);
				break;
			case Controller::LEFT_BUTTON:
				on_function_button(REMOVE, 1);
				break;
			case Controller::COPY_BUTTON:
				copy();
				break;
			case Controller::PASTE_BUTTON:
				if (pasteable_) {
					ConfirmationPage::set("PASTE LIST ?", [](int option) {
						if (option == ConfirmationPage::CONFIRM) {
							paste();
						}
					});
				}
				break;
			case Controller::CLEAR_BUTTON:
				ConfirmationPage::set("CLEAR LIST ?", [](int option) {
					if (option == ConfirmationPage::CONFIRM) {
						clear();
					}
				});
				pages_->open(Pages::CONFIRMATION_PAGE);
				break;
			default:
				break;
			}
		}

		on_function_button(Controller::button_to_function(id), state);
	}

	void refresh_leds() {
		leds_->set_footer_encoder(0, Leds::RED);
		leds_->set_footer_encoder(1, Leds::BLACK);
		leds_->set_footer_encoder(2, Leds::BLACK);
		leds_->set_footer_encoder(3, Leds::RED);
		leds_->set_footer_buttons(NUM_OPTIONS);
	}

	void draw() {
		const int x = 0;
		const int y = 0;
		const int w = canvas_->width() / 2;
		const int h = canvas_->height() - 10;
		const int row_h = h / kMaxVisibleRows;

		canvas_->fill(0, 0, canvas_->width(), canvas_->height(), Canvas::WHITE);

		int sample_count = settings_->num_samples();
		int instrument_sample_count = settings_->selected_instrument().num_samples();

		for (int i = 0; i < kMaxVisibleRows; ++i) {
			int row_y = (i * row_h) + y;

			// sample list
			int sample = i + sample_top_row_;
			if (sample < sample_count) {
				const char *text = settings_->sample(sample)->name();
				canvas_->draw_text(x + 4, row_y, w - 8, row_h, text, Canvas::LEFT, Canvas::CENTER);
				if (sample == settings_->selected_sample_index()) {
					canvas_->fill(x, row_y, w, row_h, Canvas::INVERTED);
				}
			}

			// instrument sample list
			int instrument_sample = i + instrument_sample_top_row_;
			if (instrument_sample < instrument_sample_count) {
				const char *text = settings_->selected_instrument().sample(instrument_sample)->name();
				canvas_->draw_text(x + w + 4, row_y, w - 8, row_h, text, Canvas::LEFT, Canvas::CENTER);
				if (instrument_sample == settings_->selected_instrument_sample_index()) {
					canvas_->fill(x + w, row_y, w, row_h, Canvas::INVERTED);
				}
			}
		}

		const int bar_w = 8;
		const int bar_x = w - bar_w;
		WindowPainter::draw_vertical_scollbar(bar_x, y, bar_w, h, sample_top_row_, sample_count, kMaxVisibleRows);
		WindowPainter::draw_vertical_scollbar(bar_x + w, y, bar_w, h, instrument_sample_top_row_, instrument_sample_count, kMaxVisibleRows);

		WindowPainter::draw_footer(footer_option_text, NUM_OPTIONS);
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
