#include "engine.h"

void Engine::init(Settings *settings, Uart *uart, Usb *usb) {
	settings_ = settings;

	midiEngine_.init(uart, usb);
	//midiClockEngine_.init(&settings->midi());
}

void Engine::start() {

}

void Engine::stop() {

}

void Engine::note_on(MidiEngine::Event &e) {

}

void Engine::note_off(MidiEngine::Event &e) {

}

void Engine::pitch_bend(MidiEngine::Event &e) {
	float data = (1.f / 16383.f) * MidiEngine::read_14_bit(e);
}

void Engine::cc(MidiEngine::Event &e) {
	uint8_t number = e.data[0];
	float data = (1.f / 127.f) * e.data[1];
}

// low priority
void Engine::process_midi() {
	MidiEngine::Event e;

	while (midiEngine_.pull(e)) {
		switch (e.message & 0xF0)
		{
		case MidiEngine::NOTE_ON:
			note_on(e);
			break;
		case MidiEngine::NOTE_OFF:
			note_off(e);
			break;
		case MidiEngine::PITCH_BEND:
			pitch_bend(e);
			break;
		case MidiEngine::CONTROLLER_CHANGE:
			cc(e);
			break;
		default:
			break;
		}
	}
}

void Engine::process_requests() {

}