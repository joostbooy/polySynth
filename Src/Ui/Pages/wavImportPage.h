#ifndef WavImportPage_h
#define WavImportPage_h

#include "diskNavigatorPage.h"

namespace WavImportPage {

	using TopPage::pages_;
	using TopPage::settings_;
	using TopPage::canvas_;
	using TopPage::ui_;
	using TopPage::disk_;

	enum Options {
		IMPORT,
		IMPORT_AS_MONO,

		NUM_OPTIONS
	};

	bool import_as_mono_;
	StringBuilderBase<63>str_;

	const char* const footer_text[NUM_OPTIONS] = { "IMPORT", "IMPORT MONO" };

	void draw_progression(const char *file_name) {
		MessagePainter::show(str_.write("IMPORTING... ", file_name));
		ui_->send_display();
	}

	void import(int option) {
		int wavs_total_ = 0;
		int wavs_imported_ = 0;

		switch (option)
		{
		case ConfirmationPage::CONFIRM:
			draw_progression(DiskNavigatorPage::curr_entry_name());
			if (settings_->wavImporter().import(DiskNavigatorPage::curr_entry_path(), import_as_mono_)) {
				MessagePainter::show("FINISHED");
			} else {
				MessagePainter::show("FAILED");
			}
			break;
		case ConfirmationPage::APPLY_TO_ALL:
			disk_->entry().rewind();

			while (disk_->entry().next_visible()) {
				if (disk_->entry().is_dir() == false) {
					draw_progression(disk_->entry().name());

					++wavs_total_;
					str_.write(DiskNavigatorPage::curr_path(), "/", disk_->entry().name());
					if (settings_->wavImporter().import(str_.read(), import_as_mono_)) {
						++wavs_imported_;
					}
				}
			}

			MessagePainter::show(str_.write(wavs_imported_, "/", wavs_total_, " IMPORTED"));
			break;
		default:
			break;
		}
	}

	void footer_callback(int option) {
		import_as_mono_ = option == IMPORT_AS_MONO;
		ConfirmationPage::set("CONTINUE ?", import, true);
		pages_->open(Pages::CONFIRMATION_PAGE);
	}

	void init() {
		DiskNavigatorPage::init();
	}

	void enter() {
		DiskNavigatorPage::set_footer_text(footer_text);
		DiskNavigatorPage::set_num_footer_options(NUM_OPTIONS);
		DiskNavigatorPage::set_footer_callback(footer_callback);
		DiskNavigatorPage::set_list_filter(Entry::WAV);
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
