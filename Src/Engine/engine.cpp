#include "engine.h"

void Engine::init(Settings* settings, Uart* uart, Usb* usb, Dac* dac, Switches* switches) {
  dac_ = dac;
  switches_ = switches;
  settings_ = settings;
  midiEngine_.init(uart, usb, &settings_->midi());
  modMatrixEngine_.init(settings_);
  voiceEngine_.init(settings_, &modMatrixEngine_, dac);
  midiClockEngine_.init(&settings->midi());
  state_ = RUNNING;
}

void Engine::start() {
  for (size_t i = 0; i < Settings::kNumVoices; i++) {
    for (size_t j = 0; j < Settings::kNumLfos; j++) {
      voiceEngine_.voice(i).lfoEngine(j).reset();
    }
  }
}

void Engine::stop() {
}

void Engine::noteOn(MidiEngine::Event& e) {
  if (midiEngine_.messageAccepted(e) && midiEngine_.withinKeyRange(e)) {
    noteQue_.write(e);
    voiceEngine_.requestVoice();
  }
}

void Engine::noteOff(MidiEngine::Event& e) {
  voiceEngine_.noteOff(e.port, e.message & 0x0F, e.data[0]);
}

void Engine::pitchBend(MidiEngine::Event& e) {
  float data = (1.f / 16383.f) * MidiEngine::read14Bit(e);
  modMatrixEngine_.setMidiBend(data);
}

void Engine::cc(MidiEngine::Event& e) {
  uint8_t number = e.data[0];
  float data = (1.f / 127.f) * e.data[1];
  modMatrixEngine_.setMidiCc(number, data);
}

// 8Khz, keep short !
void Engine::tick() {
  midiEngine_.poll();

  uint8_t data = 0;
  midiEngine_.getLastReceived(&data);
  if (data == MidiEngine::CLOCK_PULSE) {
    midiClockEngine_.syncBpm();
  }

  if (midiClockEngine_.tick()) {
    for (size_t i = 0; i < Midi::NUM_PORTS; i++) {
      if (settings_->midi().sendClock(i)) {
        //   midiEngine_.writeClock(MidiEngine::CLOCK_PULSE);
      }
    }
  }

  dac_->send();
}

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
      case MidiEngine::CLOCK_START:
        start();
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
      voiceEngine_.voice(i).requestStop();
    }
    noteQue_.clear();
    state_ = STOPPED;
    clearRequest(STOP);
  }

  if (requests_ & KILL_VOICES) {
    for (size_t i = 0; i < Settings::kNumVoices; i++) {
      voiceEngine_.voice(i).requestStop();
    }
    clearRequest(KILL_VOICES);
  }
}

void Engine::processSwitches() {
  Patch& p = settings_->selectedPatch();
  switches_->setAmEnable(p.amp().amEnable());
  switches_->setSync(p.oscillator().syncEnable());
  switches_->setOsc1(p.oscillator().type1());
  switches_->setOsc2(p.oscillator().type2());
  switches_->setFmEnable(p.oscillator().fmEnable());
  switches_->setMuteOsc1(p.oscillator().muteOsc1());
  switches_->setMuteOsc2(p.oscillator().muteOsc2());
  switches_->setVcoModSource(p.oscillator().modSource());
  switches_->setFmFilter1Enable(p.filter().fmEnable1());
  switches_->setFmFilter2Enable(p.filter().fmEnable2());
  switches_->setSelectedFilter(p.filter().type(), p.filter().routing());
}

// 1Khz
void Engine::update() {
  processRequests();

  if (state_ == RUNNING) {
    processMidi();
    processSwitches();

    while (voiceEngine_.available() && noteQue_.readable()) {
      voiceEngine_.assignVoice(noteQue_.read());
    }
    voiceEngine_.update();
  }
}