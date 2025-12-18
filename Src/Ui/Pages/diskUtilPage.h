#ifndef DiskUtilPage_h
#define DiskUtilPage_h

#include "topPage.h"

namespace DiskUtilPage {

	using TopPage::str_;
	using TopPage::disk_;
	using TopPage::pages_;

	enum EditOptions {
		ADD_FOLDER,
		RENAME,
		REMOVE,
		CANCEL,
		NUM_OPTIONS
	};

	const char* const option_text[NUM_OPTIONS] = { "ADD FOLDER", "RENAME", "REMOVE", "CANCEL" };

	Entry::List* entry_;
	void (*refresh_dir_callback_)();
	int num_options_;

	void set_entry(Entry::List* entry) {
		entry_ = entry;

		// only allow add folder if its a directory with no entries
		if (entry_ == nullptr) {
			num_options_ = ADD_FOLDER;
		} else {
			num_options_ = NUM_OPTIONS;
		}
	}

	void set_callback(void (*callback)()) {
		refresh_dir_callback_ = callback;
	}

	void remove(int option) {
		if (option == ConfirmationPage::CONFIRM) {
			if (disk_->directory().remove(entry_->name.read())) {
				MessagePainter::show(entry_->is_dir ? "FOLDER DELETED" : "FILE DELETED");
			} else {
				MessagePainter::show("FAILED");
			}
		}
		pages_->close(Pages::DISK_UTIL_PAGE);
	}

	void add_folder(bool confirmed) {
		if (confirmed) {
			if (disk_->directory().make(str_.read())) {
				MessagePainter::show("FOLDER CREATED");
			} else {
				MessagePainter::show("FAILED !");
			}
		}
		pages_->close(Pages::DISK_UTIL_PAGE);
	}

	void rename(bool confirmed) {
		if (confirmed) {
			if (disk_->entry().rename(entry_->name.read(), str_.read())) {
				MessagePainter::show("RENAMED");
			} else {
				MessagePainter::show("FAILED !");
			}
		}
		pages_->close(Pages::DISK_UTIL_PAGE);
	}

	void edit(int option) {
		switch (option)
		{
		case ADD_FOLDER:
			str_.clear();
			TextInputPage::set(str_.read(), 16, "ENTER FOLDER NAME", &add_folder);
			pages_->open(Pages::TEXT_INPUT_PAGE);
			break;
		case RENAME:
			str_.write(entry_->name.read());
			TextInputPage::set(str_.read(), 16, "ENTER NEW NAME", &rename);
			pages_->open(Pages::TEXT_INPUT_PAGE);
			break;
		case REMOVE:
			str_.write("REMOVE ", entry_->name.read(), " ?");
			ConfirmationPage::set(str_.read(), &remove);
			pages_->open(Pages::CONFIRMATION_PAGE);
			break;
		case CANCEL:
			pages_->close(Pages::DISK_UTIL_PAGE);
			break;
		default:
			break;
		}
	}

	void init() {
		OptionListPage::init();
	}

	void enter() {
		OptionListPage::set_count(NUM_OPTIONS);
		OptionListPage::set_text(option_text);
		OptionListPage::set_callback(edit);
		OptionListPage::enter();
	}

	void exit() {
		OptionListPage::exit();
		refresh_dir_callback_();
	}

	void on_encoder(int id, int inc) {
		OptionListPage::on_encoder(id, inc);
	}

	void on_button(int id, int value) {
		OptionListPage::on_button(id, value);
	}

	void refresh_leds() {
		OptionListPage::refresh_leds();
	}

	void draw() {
		OptionListPage::draw();
	}

	const size_t target_fps() {
		return OptionListPage::target_fps();
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
