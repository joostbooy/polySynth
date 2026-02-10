#ifndef ModulationMatrixPage_h
#define ModulationMatrixPage_h

#include "topPage.h"

namespace ModulationMatrixPage {

	using TopPage::settings_;
	using TopPage::canvas_;
	using TopPage::leds_;
	using TopPage::pages_;

	int src_ = 0;
	int top_src_ = 0;
	const int kMaxVisibleSources = 4;

	int dest_ = 0;
	int top_dest_ = 0;
	const int kMaxVisibleDestinations = 4;

	bool pasteable_ = false;
	ModulationMatrix modulationMatrix_;

	void scroll_to_source(int src) {
		src_ = SettingsUtils::clip(0, ModulationMatrix::NUM_SOURCES - 1, src);

		if (src_ < top_src_) {
			top_src_ = src_;
		} else if (src_ >= (top_src_ + kMaxVisibleSources)) {
			top_src_ = src_ - (kMaxVisibleSources - 1);
		}
	}

	void scroll_to_destination(int dest) {
		dest_ = SettingsUtils::clip(0, ModulationMatrix::NUM_DESTINATIONS - 1, dest);

		if (dest_ < top_dest_) {
			top_dest_ = dest_;
		} else if (dest_ >= (top_dest_ + kMaxVisibleDestinations)) {
			top_dest_ = dest_ - (kMaxVisibleDestinations - 1);
		}
	}

	void init() {

	}

	void enter() {

	}

	void exit()  {

	}

	void on_encoder(int id, int state) {
		if (id == Controller::FUNCTION_ENC_A) {
			scroll_to_source(src_ + state);
		} else if (id == Controller::FUNCTION_ENC_D) {
			scroll_to_destination(dest_ + state);
		}
	}

	void on_button(int id, int state) {
		if (state) {
			switch (id)
			{
			case Controller::UP_BUTTON:
				on_encoder(Controller::FUNCTION_ENC_A, -1);
				break;
			case Controller::DOWN_BUTTON:
				on_encoder(Controller::FUNCTION_ENC_A, 1);
				break;
			case Controller::LEFT_BUTTON:
				on_encoder(Controller::FUNCTION_ENC_D, -1);
				break;
			case Controller::RIGHT_BUTTON:
				on_encoder(Controller::FUNCTION_ENC_D, 1);
				break;
			case Controller::FUNCTION_BUTTON_A:
			case Controller::FUNCTION_BUTTON_D:
			case Controller::FUNCTION_ENC_PUSH_A:
			case Controller::FUNCTION_ENC_PUSH_D:
				settings_->selected_modulation_matrix().toggle(src_, dest_);
				break;
			case Controller::COPY_BUTTON:
				modulationMatrix_.paste(&settings_->selected_modulation_matrix());
				pasteable_ = true;
				break;
			case Controller::PASTE_BUTTON:
				if (pasteable_) {
					ConfirmationPage::set("OVERWRITE MATRIX ?", [](int option) {
						if (option == ConfirmationPage::CONFIRM) {
							settings_->selected_modulation_matrix().paste(&modulationMatrix_);
						}
					});
					pages_->open(Pages::CONFIRMATION_PAGE);
				}
				break;
			case Controller::CLEAR_BUTTON:
				ConfirmationPage::set("CLEAR MATRIX ?", [](int option) {
					if (option == ConfirmationPage::CONFIRM) {
						settings_->selected_modulation_matrix().clear();
					}
				});
				pages_->open(Pages::CONFIRMATION_PAGE);
				break;
			default:
				break;
			}
		}
	}

	void refresh_leds() {

	}

	void draw_sources_text(int x, int y, int w, int h, int coll_w, int row_h) {
		for (int i = 0; i < kMaxVisibleSources; ++i) {
			int src = i + top_src_;
			int src_y = (i * row_h) + y;

			if (src < ModulationMatrix::NUM_SOURCES) {
				canvas_->drawText(x + 4, src_y, coll_w, row_h, ModulationMatrix::source_text(src), Canvas::CENTER, Canvas::CENTER);
				if (src == src_) {
					canvas_->fill(coll_w + 1, src_y + 1, w - 2, row_h - 2, Canvas::LIGHT_GRAY);
				}
			}
		}
	}

	void draw_destination_text(int x, int y, int w, int h, int coll_w, int row_h) {
		for (int i = 0; i < kMaxVisibleDestinations; ++i) {
			int dest = i + top_dest_;
			int dest_x = (i * coll_w) + x;

			if (dest < ModulationMatrix::NUM_DESTINATIONS) {
				canvas_->drawText(dest_x, y, coll_w, row_h, ModulationMatrix::destination_text(dest), Canvas::CENTER, Canvas::CENTER);
				if (dest == dest_) {
					canvas_->fill(dest_x + 1, row_h + 1, coll_w - 2, h - 2, Canvas::LIGHT_GRAY);
				}
			}
		}
	}

	void draw_matrix(int x, int y, int w, int h, int coll_w, int row_h) {
		for (int i = 0; i < kMaxVisibleDestinations; ++i) {
			int dest = i + top_dest_;
			int dest_x = (i * coll_w) + x;

			for (int j = 0; j < kMaxVisibleSources; ++j) {
				int src = j + top_src_;
				int src_y = (j * row_h) + y;

				if (settings_->selected_modulation_matrix().read(src, dest)) {
					canvas_->fill(dest_x + 1, src_y + 1, coll_w - 2, row_h - 2, Canvas::BLACK);
				}
			}
		}

		WindowPainter::draw_vertical_scollbar(256 - 8, y, 6, h, top_src_, ModulationMatrix::NUM_SOURCES, kMaxVisibleSources);
		WindowPainter::draw_horizontal_scollbar(coll_w, 64 - 8, w - coll_w, 6, top_dest_, ModulationMatrix::NUM_DESTINATIONS, kMaxVisibleDestinations);
	}

	void draw() {
		const int x = 0;
		const int y = 0;
		const int w = canvas_->width() - 10;
		const int h = canvas_->height() - 10;
		const int coll_w = w / (kMaxVisibleSources + 1);
		const int row_h = h / (kMaxVisibleDestinations + 1);

		canvas_->clear();

		draw_sources_text(x, row_h, w, h, coll_w, row_h);
		draw_destination_text(coll_w, y, w, h, coll_w, row_h);
		draw_matrix(x + coll_w, y + row_h, w - coll_w, h - row_h, coll_w, row_h);
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
