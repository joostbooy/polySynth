#ifndef Engine_h
#define Engine_h

#include "uart.h"
#include "micros.h"
#include "midiEngine.h"
#include "midiClockEngine.h"
#include "settings.h"
#include "voice.h"

class Engine {

public:

	enum Request {
		STOP				= (1 << 0),
		KILL_MIDI_CHANNEL	= (1 << 1),
		AUDITION			= (1 << 2),
	};

	void init(Settings*, Uart*, Usb*);

	void addReqestBlocking(Request type) {
		addRequest(type);
		while (requests_ & type);
	}

	uint32_t processingTimeUs() {
		return processing_time_uS_;
	}

private:
	volatile uint8_t requests_ = 0x00;

	uint8_t port_to_kill_;
	uint8_t channel_to_kill_;
	uint32_t processing_time_uS_;

	Settings *settings_;
	MidiEngine midiEngine_;
	MidiClockEngine midiClockEngine_;
	Voice voice_[Settings::kNumVoices];

	void start();
	void stop();
	void processRequests();
	void processMidi();
	void update();
	void noteOn(MidiEngine::Event &e);
	void noteOff(MidiEngine::Event &e);
	void pitchBend(MidiEngine::Event &e);
	void cc(MidiEngine::Event &e);

	void sendMidiClockStart();
	void sendMidiClockStop();
	void sendMidiClockContinue();
	void sendMidi24PPQNClock_tick();

	void addRequest(Request type) {
		uint8_t flags = requests_;
		requests_ = flags | type;
	}

	void clearRequest(Request type) {
		uint8_t flags = requests_;
		requests_ = flags & ~type;
	}
};

#endif
