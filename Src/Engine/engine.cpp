#include "engine.h"

void Engine::init(Settings* settings, Uart* uart, Usb* usb, Dac* dac, Gpio* gpio) {
  dac_ = dac;
  gpio_ = gpio;
  settings_ = settings;
  midiEngine_.init(uart, usb, &settings_->midi());
  modMatrixEngine_.init(settings_);
  voiceEngine_.init(settings_, &modMatrixEngine_);
  // midiClockEngine_.init(&settings->midi());
  state_ = RUNNING;
}

void Engine::start() {
}

void Engine::stop() {
}

void Engine::noteOn(MidiEngine::Event& e) {
  if (midiEngine_.messageAccepted(e) && midiEngine_.withinKeyRange(e)) {
    noteQue_.write(e);
    voiceEngine_.request_voice();
  }
}

void Engine::noteOff(MidiEngine::Event& e) {
  voiceEngine_.note_off(e.port, e.message & 0x0F, e.data[0]);
}

void Engine::pitchBend(MidiEngine::Event& e) {
  float data = (1.f / 16383.f) * MidiEngine::read_14_bit(e);
  modMatrixEngine_.set_midi_bend(data);
}

void Engine::cc(MidiEngine::Event& e) {
  uint8_t number = e.data[0];
  float data = (1.f / 127.f) * e.data[1];
  modMatrixEngine_.set_midi_cc(number, data);
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
  if (!requests_) {
    return;
  }

  if (requests_ & START) {
    state_ = RUNNING;
    clearRequest(START);
  }

  if (requests_ & STOP) {
    for (size_t i = 0; i < Settings::kNumVoices; i++) {
      voiceEngine_.voice(i).request_stop();
    }
    noteQue_.clear();
    state_ = STOPPED;
    clearRequest(STOP);
  }

  if (requests_ & KILL_VOICES) {
    for (size_t i = 0; i < Settings::kNumVoices; i++) {
      voiceEngine_.voice(i).request_stop();
    }
    clearRequest(KILL_VOICES);
  }
}

void Engine::processSwitches() {
  gpio_->setFmEnable(settings_->oscillator().fmEnable());
  gpio_->setAmEnable(settings_->oscillator().amEnable());
  gpio_->setMuteOsc1(settings_->oscillator().muteOsc1());
  gpio_->setMuteOsc2(settings_->oscillator().muteOsc2());
  gpio_->setOsc1(settings_->oscillator().selectedOsc1());
  gpio_->setOsc2(settings_->oscillator().selectedOsc2());
  gpio_->setSelectedFilter(settings_->filter().selectedType(), settings_->filter().selectedRouting());
}

void Engine::update() {
  processRequests();

  if (state_ == RUNNING) {
    processMidi();
    processSwitches();

    while (voiceEngine_.available() && noteQue_.readable()) {
      voiceEngine_.assign_voice(noteQue_.read());
    }
    voiceEngine_.update(dac_);
  }
}