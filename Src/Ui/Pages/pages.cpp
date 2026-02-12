#include "pages.h"
#include "topPage.h"
#include "emptyPage.h"
#include "listPage.h"
#include "confirmationPage.h"
#include "optionListPage.h"
#include "hardwareTestPage.h"
#include "textInputPage.h"
#include "diskRetryPage.h"
#include "diskUtilPage.h"
#include "lfoPage.h"

Pages::Page* page_[Pages::NUM_PAGES] = {
	[Pages::EMPTY_PAGE]						= &EmptyPage::page,
	[Pages::CONFIRMATION_PAGE]				= &ConfirmationPage::page,
	[Pages::OPTION_LIST_PAGE]				= &OptionListPage::page,
	[Pages::HARDWARE_TEST_PAGE]				= &HardwareTestPage::page,
	[Pages::TEXT_INPUT_PAGE]				= &TextInputPage::page,
	[Pages::DISK_RETRY_PAGE]				= &DiskRetryPage::page,
	[Pages::DISK_UTIL_PAGE]					= &DiskUtilPage::page,
	[Pages::LFO_PAGE]						= &LfoPage::page,
};

void Pages::init(Settings *settings, Engine *engine, Ui *ui) {
	page_stack_.clear();
	TopPage::init(settings, engine, ui);

	for (int i = 0; i < NUM_PAGES; ++i) {
		page_[i]->init();
	}

	//open(MIDI_PAGE);
}

void Pages::open(int id) {
	if (page_stack_.find(id) < 0 && page_stack_.writeable() == true) {
		curr_page_ = id;
		page_stack_.push(id);
		page_[id]->enter();
	}
}

void Pages::close(int id) {
	if (page_stack_.remove_by_value(id)) {
		if (page_stack_.readable()) {
			curr_page_ = page_stack_.read(page_stack_.size() - 1);
		} else {
			curr_page_ = EMPTY_PAGE;
		}
		page_[id]->exit();
	}
}

void Pages::close_all() {
	while (page_stack_.readable()) {
		int id = page_stack_.pop();
		page_[id]->exit();
	}
	curr_page_ = EMPTY_PAGE;
}

void Pages::on_button(int id, int state) {
	page_[curr_page_]->on_button(id, state);
	TopPage::on_button(id, state);
}

void Pages::on_encoder(int id, int state) {
	page_[curr_page_]->on_encoder(id, state);
	TopPage::on_encoder(id, state);
}

void Pages::refresh_leds() {
	for (int i = 0; i < page_stack_.size(); ++i) {
		int id = page_stack_.read(i);
		page_[id]->refresh_leds();
	}
	TopPage::refresh_leds();
}

void Pages::draw() {
	for (int i = 0; i < page_stack_.size(); ++i) {
		int id = page_stack_.read(i);
		page_[id]->draw();
	}
	TopPage::draw();
}

const size_t Pages::target_fps() {
	return page_[curr_page_]->target_fps();
}
