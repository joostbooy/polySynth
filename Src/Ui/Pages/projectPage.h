#ifndef ProjectPage_h
#define ProjectPage_h

#include "diskNavigatorPage.h"

namespace ProjectPage {

	using TopPage::pages_;
	using TopPage::settings_;
	using TopPage::ui_;

	enum Options {
		LOAD,
		SAVE,

		NUM_OPTIONS
	};

	const char* const option_text[NUM_OPTIONS] = { "LOAD", "SAVE" };

	void save_as(bool confirmed) {
		if (confirmed) {
			if (settings_->save(DiskNavigatorPage::curr_path())) {
			//	MessagePainter::show("PROJECT SAVED");
			} else {
			//	MessagePainter::show("SAVE FAILED !");
			}
		}
	}

	void option_callback(int option) {
		switch (option)
		{
		case LOAD:
			ConfirmationPage::set("LOAD PROJECT ?", [](int option) {
				if (option == ConfirmationPage::CONFIRM) {

					// UNCOMMENT THIS LATER !!!
					//engine_->add_request_blocking(Engine::STOP);
					// UNCOMMENT THIS LATER !!!

					MessagePainter::show("LOADING...");
					ui_->send_display();

					if (settings_->load(DiskNavigatorPage::curr_entry_path())) {
						settings_->set_project_name(DiskNavigatorPage::curr_entry_name());
						MessagePainter::show("PROJECT LOADED");
					} else {
						MessagePainter::show("LOAD FAILED !");
					}
				}
			});
			pages_->open(Pages::CONFIRMATION_PAGE);
			break;
		case SAVE:
			if (settings_->has_valid_path()) {
				if (settings_->save()) {
					MessagePainter::show("PROJECT SAVED");
				} else {
					MessagePainter::show("SAVE FAILED !");
				}
			} else {
				TextInputPage::set(settings_->project_name(), settings_->max_name_length(), "SET PROJECT NAME", save_as);
				pages_->open(Pages::TEXT_INPUT_PAGE);
			}
			break;
		default:
			break;
		}
	}

	void init() {
		DiskNavigatorPage::init();
	}

	void enter() {
		DiskNavigatorPage::set_footer_text(option_text);
		DiskNavigatorPage::set_num_footer_options(NUM_OPTIONS);
		DiskNavigatorPage::set_footer_callback(option_callback);
		DiskNavigatorPage::set_list_filter(Entry::PROJECT);
		DiskNavigatorPage::enter();
	}

	void exit()  {
		DiskNavigatorPage::exit();
	}

	void on_button(int id, int state) {
		DiskNavigatorPage::on_button(id, state);
	}

	void on_encoder(int id, int state) {
		DiskNavigatorPage::on_encoder(id, state);
	}

	void refresh_leds() {
		DiskNavigatorPage::refresh_leds();
	}

	void draw() {
		DiskNavigatorPage::draw();
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
