#ifndef Midi_h
#define Midi_h

#include "fileWriter.h"
#include "fileReader.h"
#include "lookupTables.h"
#include "settingsText.h"
#include "settingsUtils.h"

class Midi {

public:

	enum Port {
		UART,
		USB,

		NUM_PORTS
	};

	static const char *portText(int port) {
		switch (port)
		{
		case UART:	return "UART";
		case USB:	return "USB";
		default:
			break;
		}
		return nullptr;
	}


	enum ClockSource {
		EXTERNAL = NUM_PORTS - 1,
		INTERNAL,

		NUM_CLOCK_SOURCES
	};

	static const char* clockSourceText(int value) {
		if (value <= EXTERNAL) {
			return Midi::portText(value);
		} else {
			return "INTERNAL";
		}
	}

	void init() {
		setBpm(120);
		setClockSource(INTERNAL);

		for (int i = 0; i < NUM_PORTS; ++i){
			setSendClock(i, true);
		}
	}

	// bpm
	uint16_t bpm() {
		return bpm_;
	}

	void setBpm(int value) {
		bpm_ = SettingsUtils::clip(MIN_BPM, MAX_BPM, value);
	}

	const char *bpmText() {
		return SettingsText::str.write(bpm(), " BPM");
	}

	// clock source
	uint8_t clockSource() {
		return clockSource_;
	}

	void setClockSource(int value) {
		clockSource_ = SettingsUtils::clip(0, NUM_CLOCK_SOURCES - 1, value);
	}

	const char* clockSourceText() {
		return clockSourceText(clockSource());
	}

	// clock source
	bool sendClock(int port) {
		return sendClock_[port];
	}

	void setSendClock(int port, bool value) {
		sendClock_[port] = value;
	}

	const char* sendClock_text(int port) {
		return SettingsText::boolToOnOff(sendClock(port));
	}

	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(bpm_);
		fileWriter.write(clockSource_);

		for (int i = 0; i < NUM_PORTS; ++i) {
			fileWriter.write(sendClock_[i]);
		}
	}

	void load(FileReader &fileReader) {
		fileReader.read(bpm_);
		fileReader.read(clockSource_);

		for (int i = 0; i < NUM_PORTS; ++i) {
			fileReader.read(sendClock_[i]);
		}
	}

	void paste(Midi *midi) {
		bpm_ = midi->bpm();
		clockSource_ = midi->clockSource();
		
		for (int i = 0; i < NUM_PORTS; ++i) {
			sendClock_[i] = midi->sendClock(i);
		}
	}

private:
	uint16_t bpm_;
	uint8_t clockSource_;
	//uint8_t midiChannel_;
	bool sendClock_[NUM_PORTS];
};

#endif
