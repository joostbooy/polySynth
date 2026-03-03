#ifndef MidiPage_h
#define MidiPage_h

#include "listPage.h"
#include "midi.h"
#include "midiList.h"

namespace MidiPage {

	using TopPage::settings_;
	using TopPage::engine_;
	using TopPage::ui_;

	bool pasteable_;
	Midi midi_;
	MidiList midiList_;

	void clear() {
		settings_->midi().init();
	}

	void copy() {
		midi_.paste(&settings_->midi());
		pasteable_ = true;
	}

	bool paste() {
		if (pasteable_) {
			settings_->midi().paste(&midi_);
			return true;
		}
		return false;
	}

	void init() {
		pasteable_ = false;
		midi_.init();
		midiList_.init(engine_, settings_, ui_);
	}

	void enter() {
		ListPage::setList(&midiList_);
		ListPage::setClearCallback(&clear);
		ListPage::setCopyCallback(&copy);
		ListPage::setPasteCallback(&paste);
		ListPage::enter();
	}

	void exit()  {
		ListPage::exit();
	}

	void onButton(int id, int state) {
		ListPage::onButton(id, state);
	}

	void onEncoder(int id, int state) {
		ListPage::onEncoder(id, state);
	}

	void refreshLeds() {
		ListPage::refreshLeds();
	}

	void draw() {
		ListPage::draw();
	}

	const size_t targetFps() {
		return 1000 / 16;
	}

	Pages::Page page = {
		&init,
		&enter,
		&exit,
		&draw,
		&refreshLeds,
		&onButton,
		&onEncoder,
		&targetFps
	};

};

#endif
