#ifndef SamplePage_h
#define SamplePage_h

#include "listPage.h"
#include "sample.h"
#include "sampleList.h"

namespace SamplePage {

	using TopPage::settings_;
	using TopPage::engine_;
	using TopPage::pages_;
	using TopPage::canvas_;
	using TopPage::str_;

	bool pasteable_;
	Sample sample_;
	SampleList sampleList_;

	int x_zoom_ = 1;
	int y_zoom_ = 1;
	int sample_x_ = 0;
	int selected_option_;
	Voice *voice_;

	enum ListOptions {
		IMPORT,
		REMOVE,
		CONVERT_TO_MONO,
		MAP_NAME_TO_ROOT_NOTE,
		MAP_NAME_TO_KEY_RANGE,
		MAP_NAME_TO_ROOT_AND_KEY,
		CANCEL,

		NUM_LIST_OPTIONS
	};

	const char* const list_option_text[NUM_LIST_OPTIONS] = {
		"IMPORT",
		"REMOVE",
		"CONVERT TO MONO",
		"MAP NAME TO ROOT NOTE",
		"MAP NAME TO KEY RANGE",
		"MAP NAME TO ROOT & KEY",
		"CANCEL"
	};

	// FIXME : this also removes the sample data, which we dont want
	void clear() {
		// settings_->selected_sample()->init();
	}

	void copy() {
		sample_.paste(settings_->selected_sample());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings_->selected_sample()->paste(&sample_);
			return true;
		}
		return false;
	}

	void remove(int index) {
		engine_->add_request_blocking(Engine::STOP);

		if (settings_->sampleAllocator().remove(index)) {
			settings_->refresh_sample_selection();
			MessagePainter::show("SAMPLE REMOVED");
		}
	}

	void remove_all() {
		engine_->add_request_blocking(Engine::STOP);

		size_t count = settings_->num_samples();
		for (size_t i = 0; i < count; ++i) {
			settings_->sampleAllocator().remove(i);
		}

		settings_->refresh_sample_selection();
		MessagePainter::show("SAMPLES REMOVED");
	}

	void convert_to_mono(int index) {
		engine_->add_request_blocking(Engine::STOP);

		if (settings_->sampleAllocator().convert_to_mono(index)) {
			MessagePainter::show("SAMPLE CONVERTED TO MONO");
		}
	}

	void convert_all_to_mono() {
		engine_->add_request_blocking(Engine::STOP);

		size_t count = settings_->num_samples();
		for (size_t i = 0; i < count; ++i) {
			settings_->sampleAllocator().convert_to_mono(i);
		}
		MessagePainter::show("SAMPLES CONVERTED TO MONO");
	}

	void map_name_to_root_note(int index) {
		Sample *sample = settings_->sample(index);
		if (sample->map_name_to_root_note()) {
			MessagePainter::show("SUCCES");
		}
	}

	void map_all_names_to_root_note() {
		size_t count = settings_->num_samples();
		for (size_t i = 0; i < count; ++i) {
			settings_->sample(i)->map_name_to_root_note();
		}
		MessagePainter::show("FINISHED");
	}

	void map_name_to_key_range(int index) {
		Sample *sample = settings_->sample(index);
		if (sample->map_name_to_key_range()) {
			MessagePainter::show("SUCCES");
		}
	}

	void map_all_names_to_key_range() {
		size_t count = settings_->num_samples();
		for (size_t i = 0; i < count; ++i) {
			settings_->sample(i)->map_name_to_key_range();
		}
		MessagePainter::show("FINISHED");
	}

	void enter();

	void edit(int option) {
		if (option == ConfirmationPage::CANCEL) {
			return;
		}

		bool apply_to_all = (option == ConfirmationPage::APPLY_TO_ALL);

		switch (selected_option_)
		{
		case REMOVE:
			if (apply_to_all) {
				remove_all();
			} else {
				remove(settings_->selected_sample_index());
			}
			enter(); // check if num samples is greater than 0
			break;
		case CONVERT_TO_MONO:
			if (apply_to_all) {
				convert_all_to_mono();
			} else {
				convert_to_mono(settings_->selected_sample_index());
			}
			break;
		case MAP_NAME_TO_ROOT_NOTE:
			if (apply_to_all) {
				map_all_names_to_root_note();
			} else {
				map_name_to_root_note(settings_->selected_sample_index());
			}
			break;
		case MAP_NAME_TO_KEY_RANGE:
			if (apply_to_all) {
				map_all_names_to_key_range();
			} else {
				map_name_to_key_range(settings_->selected_sample_index());
			}
			break;
		case MAP_NAME_TO_ROOT_AND_KEY:
			if (apply_to_all) {
				map_all_names_to_root_note();
				map_all_names_to_key_range();
			} else {
				map_name_to_root_note(settings_->selected_sample_index());
				map_name_to_key_range(settings_->selected_sample_index());
			}
			break;
		case CANCEL:
			break;
		default:
			break;
		}
	}

	void select_edit_option(int option) {
		if (option == IMPORT) {
			pages_->open(Pages::WAV_IMPORT_PAGE);
		} else if (option != CANCEL) {
			selected_option_ = option;
			ConfirmationPage::set("CONTINUE ?", edit, true);
			pages_->open(Pages::CONFIRMATION_PAGE);
		}
	}

	void init() {
		pasteable_ = false;
		sample_.init();
		sampleList_.init(engine_, settings_);
		voice_ = &engine_->voiceEngine().voice(0);
	}

	void enter() {
		if (settings_->num_samples() == 0) {
			pages_->close(Pages::SAMPLE_PAGE);
			pages_->open(Pages::WAV_IMPORT_PAGE);
		} else {
			ListPage::set_list(&sampleList_);
			ListPage::set_clear_callback(&clear);
			ListPage::set_copy_callback(&copy);
			ListPage::set_paste_callback(&paste);
			ListPage::enter();
		}
	}

	void exit()  {
		ListPage::exit();
	}

	void on_encoder(int id, int state) {
		const int kMilliSecond =  SAMPLE_RATE / 1000;
		const int kQuarterSecond = SAMPLE_RATE / 4;

		if (Controller::is_pressed(Controller::SHIFT_BUTTON)) {
			sample_x_ += Controller::encoder_is_pressed(id) ? kQuarterSecond * state : kMilliSecond * state;
			return;
		}

		ListPage::on_encoder(id, state);
	}

	void on_button(int id, int state) {
		bool shift = Controller::is_pressed(Controller::SHIFT_BUTTON);

		if (!shift) {
			ListPage::on_button(id, state);
		}

		if (state) {
			if (id == Controller::MENU_BUTTON) {
				OptionListPage::set_text(list_option_text);
				OptionListPage::set_callback(select_edit_option);
				OptionListPage::set_count(NUM_LIST_OPTIONS);
				pages_->open(Pages::OPTION_LIST_PAGE);
				return;
			}

			if (shift) {
				switch (id)
				{
				case Controller::LEFT_BUTTON:
					x_zoom_ = SettingsUtils::clip(1, 8, x_zoom_ - 1);
					break;
				case Controller::RIGHT_BUTTON:
					x_zoom_ = SettingsUtils::clip(1, 8, x_zoom_ + 1);
					break;
				case Controller::DOWN_BUTTON:
					y_zoom_ = SettingsUtils::clip(1, 8, y_zoom_ - 1);
					break;
				case Controller::UP_BUTTON:
					y_zoom_ = SettingsUtils::clip(1, 8, y_zoom_ + 1);
					break;
				default:
					break;
				}
			}
		}
	}

	void refresh_leds() {
		ListPage::refresh_leds();
	}

	void draw_point(int x, int y, int w, int h, int visible_size, int point) {
		if (point >= sample_x_ && point <= (sample_x_ + visible_size)) {
			float point_ = ((point - sample_x_) / float(visible_size)) * w;
			canvas_->vertical_line(x + point_, y, h, Canvas::BLACK);
		}
	}

	void draw_sample(int x, int y, int h, int sample_min, int sample_max) {
		int h_center = h / 2;
		int h_min = (abs(sample_min) * (1.f / 32768.f)) * h_center * y_zoom_;
		int h_max = (abs(sample_max) * (1.f / 32768.f)) * h_center * y_zoom_;
		h_min = SettingsUtils::clip_max(h, h_min);
		h_max = SettingsUtils::clip_max(h, h_max);
		canvas_->vertical_line(x, y + h_center, h_min, Canvas::GRAY);
		canvas_->vertical_line(x, y + (h_center - h_max), h_max, Canvas::GRAY);
	}

	void draw() {
		ListPage::draw();

		const int x = 0;
		const int y = 0;
		const int w = 255;
		const int h = 41;
		const int wave_h = h / 2;
		const size_t readings = 32;

		Sample *sample = settings_->selected_sample();
		size_t size = sample->size_samples();
		size_t visible_size = size / x_zoom_;
		size_t inc = (visible_size / w);
		inc = (readings >= inc) ? 1 : inc / readings;

		sample_x_ = SettingsUtils::clip(0, (size - visible_size), sample_x_);
		size_t index = sample_x_;

		// draw sample
		for (int x2 = 0; x2 < w; ++x2) {
			int16_t l_min = 0, l_max = 0;
			int16_t r_min = 0, r_max = 0;

			for (size_t i = 0; i < readings; ++i) {
				int16_t l, r;
				sample->read(index, &l, &r);

				if (l > l_max) { l_max = l; }
				if (r > r_max) { r_max = r; }
				if (l < l_min) { l_min = l; }
				if (r < r_min) { r_min = r; }

				index += inc;
			}

			draw_sample(x + x2, y, wave_h, l_min, l_max);
			draw_sample(x + x2, y + wave_h + 1, wave_h, r_min, r_max);
		}

		const int bar_h = 6;
		const int bar_y = h;
		WindowPainter::draw_horizontal_scollbar(x, bar_y, w, bar_h, sample_x_, size, visible_size);

		// draw start & end points
		draw_point(x, y, w, h, visible_size, sample->start());
		draw_point(x, y, w, h, visible_size, sample->end());
		draw_point(x, y, w, h, visible_size, sample->loop_start());
		draw_point(x, y, w, h, visible_size, sample->loop_end());

		// update last voice that used the selected sample
		Voice &most_recent_voice = engine_->voiceEngine().most_recent_voice();
		if (sample == most_recent_voice.sample()) {
			voice_ = &most_recent_voice;
		}

		// draw phase of the last voice
		if (sample == voice_->sample() && voice_->state() != Voice::IDLE) {
			draw_point(x, y, w, h, visible_size, voice_->phase());
		}

		// draw zoom text
		canvas_->draw_text(x, y, w, h, str_.write("Xx", x_zoom_, " Yx", y_zoom_), Canvas::RIGHT, Canvas::TOP);
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
