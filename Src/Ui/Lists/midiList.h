#ifndef MidiList_h
#define MidiList_h

#include "settings.h"
#include "settingsText.h"

class MidiList : public SettingsList {

public:

	enum Item {
		BPM,
		CLOCK_SOURCE,
		SEND_CLOCK_UART,
		SEND_CLOCK_USB,

		NUM_ITEMS
	};


	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case BPM:				return "BPM";
		case CLOCK_SOURCE:		return "CLOCK SOURCE";
		case SEND_CLOCK_UART:	return "SEND CLOCK UART";
		case SEND_CLOCK_USB:	return "SEND CLOCK USB";
		default:
			break;
		}
		return nullptr;
	}

	const char* value_text(int item) override {
		Midi &midi = settings_->midi();

		switch (item)
		{
		case BPM:				return midi.bpm_text();
		case CLOCK_SOURCE:		return midi.clock_source_text();
		case SEND_CLOCK_UART:	return midi.send_clock_text(Midi::UART);
		case SEND_CLOCK_USB:	return midi.send_clock_text(Midi::USB);
		default:
			break;
		}
		return nullptr;
	}

	void edit(int item, int inc, bool shifted) override {
		Midi &midi = settings_->midi();

		switch (item)
		{
		case BPM:				return midi.set_bpm(midi.bpm() + (inc * (shifted ? 10 : 1)));
		case CLOCK_SOURCE:		return midi.set_clock_source(midi.clock_source() + inc);
		case SEND_CLOCK_UART:	return midi.set_send_clock(Midi::UART, inc > 0);
		case SEND_CLOCK_USB:	return midi.set_send_clock(Midi::USB, inc > 0);
		default:
			break;
		}
	}

private:

};

#endif
