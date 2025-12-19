#ifndef SystemPage_h
#define SystemPage_h

#include "topPage.h"

namespace SystemPage {

	using TopPage::disk_;
	using TopPage::canvas_;
	using TopPage::settings_;
	using TopPage::engine_;
	using TopPage::str_;
	using TopPage::leds_;
	using TopPage::pages_;

	uint32_t sd_size_total;
	uint32_t sd_size_free;

	enum FooterOptions {
		VERSION,
		CLEAR,
		NUM_OPTIONS
	};

	const char *const footer_text[NUM_OPTIONS] = { "VERSION", "CLEAR" };

	void init() {

	}

	void enter() {
		if (disk_->mounted()) {
			disk_->available_kb(&sd_size_total, &sd_size_free);
		} else {
			sd_size_total = 0;
			sd_size_free = 0;
		}
	}

	void exit()  {

	}

	void on_button(int id, int state) {
		if (state) {
			switch (Controller::button_to_function(id))
			{
			case VERSION:
				MessagePainter::show(str_.write("FIRMWARE V", settings_->current_version()));
				break;
			case CLEAR:
				ConfirmationPage::set("CLEAR SETTINGS ?", [](int option) {
					if (option == ConfirmationPage::CONFIRM) {
						engine_->add_request_blocking(Engine::STOP);
						settings_->init();
						MessagePainter::show("SETTINGS CLEARED");
					}
				});
				pages_->open(Pages::CONFIRMATION_PAGE);
				break;
			default:
				break;
			}
		}
	}

	void on_encoder(int id, int state) {

	}

	void refresh_leds() {
		leds_->set_footer_buttons(NUM_OPTIONS);
	}

	void draw_graph(int x, int y, int w, int h, float value, const char* title, const char* value_text) {
		int fill_h = value * h;
		int fill_y = (h - fill_h) + y;
		canvas_->fill(x, y, w, h, Canvas::GRAY);
		canvas_->fill(x, fill_y, w, fill_h, Canvas::BLACK);
		canvas_->draw_text(x + w + 2, y + h - 14, title);
		canvas_->draw_text(x + w + 2, y + h - 6, value_text);
	}

	void draw_processing_time(int x, int y, int w, int h) {
		float time = (1.f / (1000000.f / CONTROL_RATE)) * engine_->processing_time_uS();
		draw_graph(x, y, w, h, time, "CPU", str_.write(100 * time, "%"));
	}

	void draw_available_ram(int x, int y, int w, int h) {
		uint32_t total_ram = settings_->sampleAllocator().max_ram() * 2;		// 2 bytes per sample
		uint32_t free_ram = settings_->sampleAllocator().available_ram() * 2;	// 2 bytes per sample
		uint32_t used_ram = total_ram - free_ram;
		float value = used_ram / total_ram;

		str_.write(SettingsText::kb_to_mem_size_text(free_ram / 1000));
		str_.append("/");
		str_.append(SettingsText::kb_to_mem_size_text(total_ram / 1000));

		draw_graph(x, y, w, h, value, "RAM", str_.read());
	}

	void draw_sd_card_available_kb(int x, int y, int w, int h) {
		uint32_t sd_used = sd_size_total - sd_size_free;
		float value = sd_used / sd_size_total;

		str_.write(SettingsText::kb_to_mem_size_text(sd_size_free));
		str_.append("/");
		str_.append(SettingsText::kb_to_mem_size_text(sd_size_total));

		draw_graph(x, y, w, h, value, "SD CARD", str_.read());
	}

	void draw() {
		canvas_->set_font(Font::SMALL);

		const int y = 15;
		const int w = 10;
		const int h = 30;

		draw_available_ram(37, y, w, h);
		draw_sd_card_available_kb(122, y, w, h);
		draw_processing_time(207, y, w, h);

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
