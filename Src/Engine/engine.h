#ifndef Engine_h
#define Engine_h

#include "uart.h"
#include "micros.h"
#include "midiEngine.h"
#include "midiClockEngine.h"
#include "settings.h"

class Engine {

public:

	enum Request {
		STOP				= (1 << 0),
		KILL_MIDI_CHANNEL	= (1 << 1),
		AUDITION			= (1 << 2),
	};

	void init(Settings*, Uart*, Usb*);

	void add_request_blocking(Request type) {
		add_request(type);
		while (requests_ & type);
	}

	uint32_t processing_time_uS() {
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

	void start();
	void stop();
	void process_requests();
	void process_midi();
	void process_gates();
	void note_on(MidiEngine::Event &e);
	void note_off(MidiEngine::Event &e);
	void pitch_bend(MidiEngine::Event &e);
	void cc(MidiEngine::Event &e);

	void send_midi_clock_start();
	void send_midi_clock_stop();
	void send_midi_clock_continue();
	void send_midi_24PPQN_clock_tick();

	void add_request(Request type) {
		uint8_t flags = requests_;
		requests_ = flags | type;
	}

	void clear_request(Request type) {
		uint8_t flags = requests_;
		requests_ = flags & ~type;
	}
};

#endif
