#ifndef Voice_h
#define Voice_h

#include "dac.h"
#include "envelopeEngine.h"
#include "lfoEngine.h"
#include "modMatrixEngine.h"

class Voice {
 public:
  enum State { IDLE = 0, ACTIVE = 1 };

  void init(Settings* settings, ModMatrixEngine* modMatrixEngine, Dac *dac) {
    dac_ = dac;
    settings_ = settings;
    modMatrixEngine_ = modMatrixEngine;
    state_ = IDLE;
    keyPressed_ = false;
    stopRequested_ = false;
    lastNote_ = 60;

    ampEnvelopeEngine_.init(&settings->ampEnvelope());
    modEnvelopeEngine_.init(&settings->modEnvelope());

    lfoEngine_[0].init(&settings->lfo(0));
    lfoEngine_[1].init(&settings->lfo(1));
  }

  uint8_t port() {
    return port_;
  }

  uint8_t note() {
    return note_;
  }

  uint8_t channel() {
    return channel_;
  }

  bool keyPressed() {
    return keyPressed_;
  }

  State state() {
    return state_;
  }

  EnvelopeEngine& ampEnvelopeEnginex() {
    return ampEnvelopeEngine_;
  }

  EnvelopeEngine& modEnvelopeEnginex() {
    return modEnvelopeEngine_;
  }

  LfoEngine& lfoEngine(int index) {
    return lfoEngine_[index];
  }

  void requestStop() {
    stopRequested_ = true;
  }

  bool isAvailable() {
    return state_ == IDLE;
  }

  void noteOn(MidiEngine::Event& e) {
    lastNote_ = note_;
    port_ = e.port;
    note_ = e.data[0];
    channel_ = e.message & 0x0F;
    velocity_ = e.data[1] * (1.f / 127.f);

    keyPressed_ = true;
    stopRequested_ = false;
    fadePhase_ = 1.f;

    lfoEngine_[0].retrigger();
    lfoEngine_[1].retrigger();

    ampEnvelopeEngine_.attack();
    modEnvelopeEngine_.attack();

    state_ = ACTIVE;
  }

  void noteOff() {
    keyPressed_ = false;
    ampEnvelopeEngine_.release();
    modEnvelopeEngine_.release();
  }

  void update(int index) {
    if (stopRequested_) {
      if (fadePhase_ > 0.0f) {
        fadePhase_ -= 1000.f / (CONTROL_RATE * 4.f);
      } else {
        state_ = IDLE;
      }
    }

    if (ampEnvelopeEngine_.stage() == EnvelopeEngine::IDLE) {
      state_ = IDLE;
    }

    modMatrixEngine_->setMidiVelocity(velocity_);
    modMatrixEngine_->setAmpEnvelope(ampEnvelopeEngine_.next());
    modMatrixEngine_->setModEnvelope(modEnvelopeEngine_.next());
    modMatrixEngine_->setLfo(0, lfoEngine_[0].next());
    modMatrixEngine_->setLfo(1, lfoEngine_[1].next());
    ModMatrixEngine::Frame* frame = modMatrixEngine_->process();

    Patch& p = settings_->selectedPatch();
    dac_->set(index, 0, (calculatePitch() * frame->data[ModMatrix::PITCH]) * 65535);
    dac_->set(index, 1, (p.oscillator().shape1() * frame->data[ModMatrix::SHAPE_1]) * 65535);
    dac_->set(index, 2, (p.oscillator().shape2() * frame->data[ModMatrix::SHAPE_2]) * 65535);
    dac_->set(index, 3, (p.filter().cutoff1() * frame->data[ModMatrix::CUTOFF_1]) * 65535);
    dac_->set(index, 4, (p.filter().resonance1() * frame->data[ModMatrix::RESONANCE_1]) * 65535);
    dac_->set(index, 5, (p.filter().cutoff2() * frame->data[ModMatrix::CUTOFF_2]) * 65535);
    dac_->set(index, 6, (p.filter().resonance2() * frame->data[ModMatrix::RESONANCE_2]) * 65535);
    dac_->set(index, 7, (fadePhase_ * frame->data[ModMatrix::GAIN]) * 65535);
  }

 private:
  bool keyPressed_;
  bool stopRequested_;
  uint8_t note_;
  uint8_t lastNote_;
  uint8_t port_;
  uint8_t channel_;
  State state_;
  float velocity_;
  float fadePhase_;
  Dac* dac_;
  Settings* settings_;
  ModMatrixEngine* modMatrixEngine_;
  EnvelopeEngine ampEnvelopeEngine_;
  EnvelopeEngine modEnvelopeEngine_;
  LfoEngine lfoEngine_[Settings::kNumLfos];

  float calculatePitch() {
    // return settings_->calibration().note(note_);
    return 0.f;
  }
};

#endif
