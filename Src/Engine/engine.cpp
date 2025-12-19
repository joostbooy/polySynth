#include "engine.h"

void Engine::init(Settings* settings, Uart* uart, Usb* usb) {
  settings_ = settings;

  midiEngine_.init(uart, usb);
  // midiClockEngine_.init(&settings->midi());
}

void Engine::start() {
}

void Engine::stop() {
}

void Engine::noteOn(MidiEngine::Event& e) {
}

void Engine::noteOff(MidiEngine::Event& e) {
}

void Engine::pitchBend(MidiEngine::Event& e) {
  // float data = (1.f / 16383.f) * MidiEngine::read_14_bit(e);
}

void Engine::cc(MidiEngine::Event& e) {
  // uint8_t number = e.data[0];
  // float data = (1.f / 127.f) * e.data[1];
}

// low priority
void Engine::processMidi() {
  MidiEngine::Event e;

  while (midiEngine_.pull(e)) {
    switch (e.message & 0xF0) {
      case MidiEngine::NOTE_ON:
        noteOn(e);
        break;
      case MidiEngine::NOTE_OFF:
        noteOff(e);
        break;
      case MidiEngine::PITCH_BEND:
        pitchBend(e);
        break;
      case MidiEngine::CONTROLLER_CHANGE:
        cc(e);
        break;
      default:
        break;
    }
  }
}

void Engine::processRequests() {
}

void Engine::update() {
  processRequests();
  processMidi();
  // modulationMatrixEngine_.update();
}