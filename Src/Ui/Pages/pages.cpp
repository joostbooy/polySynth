#include "pages.h"
#include "topPage.h"
#include "emptyPage.h"
#include "listPage.h"
#include "confirmationPage.h"
#include "optionListPage.h"
#include "hardwareTestPage.h"
#include "textInputPage.h"
#include "lfoPage.h"
#include "midiPage.h"
#include "midiMonitorPage.h"
#include "envelopePage.h"
#include "ampPage.h"
#include "filterPage.h"
#include "oscillatorPage.h"
#include "modMatrixPage.h"
#include "modMatrixDepthPage.h"
#include "patchPage.h"
#include "calibrationPage.h"

Pages::Page* page_[Pages::NUM_PAGES] = {
	[Pages::EMPTY_PAGE]						= &EmptyPage::page,
	[Pages::CONFIRMATION_PAGE]				= &ConfirmationPage::page,
	[Pages::OPTION_LIST_PAGE]				= &OptionListPage::page,
	[Pages::HARDWARE_TEST_PAGE]				= &HardwareTestPage::page,
	[Pages::TEXT_INPUT_PAGE]				= &TextInputPage::page,
	[Pages::CALIBRATION_PAGE]				= &CalibrationPage::page,
	[Pages::LFO_PAGE]						= &LfoPage::page,
	[Pages::MIDI_PAGE]						= &MidiPage::page,
	[Pages::MIDI_MONITOR_PAGE]				= &MidiMonitorPage::page,
	[Pages::ENVELOPE_PAGE]					= &EnvelopePage::page,
	[Pages::AMP_PAGE]						= &AmpPage::page,
	[Pages::FILTER_PAGE]					= &FilterPage::page,
	[Pages::OSCILLATOR_PAGE]				= &OscillatorPage::page,
	[Pages::MOD_MATRIX_PAGE]				= &ModMatrixPage::page,
	[Pages::MOD_MATRIX_DEPTH_PAGE]			= &ModMatrixDepthPage::page,
	[Pages::PATCH_PAGE]						= &PatchPage::page,
};

void Pages::init(Settings *settings, Engine *engine, Ui *ui) {
	pageStack_.clear();
	TopPage::init(settings, engine, ui);

	for (int i = 0; i < NUM_PAGES; ++i) {
		page_[i]->init();
	}

	TopPage::selectedPage_ = PATCH_PAGE;
	open(TopPage::selectedPage_);
}

void Pages::open(int id) {
	if (isOpen(id) == false && pageStack_.writeable() == true) {
		currentPage_ = id;
		pageStack_.push(id);
		page_[id]->enter();
	}
}

bool Pages::isOpen(int id) {
	for (size_t i = 0; i < pageStack_.size(); i++){
		if (pageStack_.read(i) == id) {
			return true;
		}
	}
	return false;
}

void Pages::close(int id) {
	if (pageStack_.remove_by_value(id)) {
		if (pageStack_.readable()) {
			currentPage_ = pageStack_.read(pageStack_.size() - 1);
		} else {
			currentPage_ = EMPTY_PAGE;
		}
		page_[id]->exit();
	}
}

void Pages::closeAll() {
	while (pageStack_.readable()) {
		int id = pageStack_.pop();
		page_[id]->exit();
	}
	currentPage_ = EMPTY_PAGE;
}

void Pages::onButton(int id, int state) {
	page_[currentPage_]->onButton(id, state);
	TopPage::onButton(id, state);
}

void Pages::onEncoder(int id, int state) {
	page_[currentPage_]->onEncoder(id, state);
	TopPage::onEncoder(id, state);
}

void Pages::refreshLeds() {
	for (int i = 0; i < pageStack_.size(); ++i) {
		int id = pageStack_.read(i);
		page_[id]->refreshLeds();
	}
	TopPage::refreshLeds();
}

void Pages::draw() {
	for (int i = 0; i < pageStack_.size(); ++i) {
		int id = pageStack_.read(i);
		page_[id]->draw();
	}
	TopPage::draw();
}

const size_t Pages::targetFps() {
	return page_[currentPage_]->targetFps();
}
