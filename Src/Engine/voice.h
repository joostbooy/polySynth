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

    envelopeEngine_[0].init(&settings->envelope(0));
    envelopeEngine_[1].init(&settings->envelope(1));

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

  EnvelopeEngine& envelopeEngine(int index) {
    return envelopeEngine_[index];
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

    envelopeEngine_[0].attack();
    envelopeEngine_[1].attack();

    state_ = ACTIVE;
  }

  void noteOff() {
    keyPressed_ = false;
    envelopeEngine_[0].release();
    envelopeEngine_[1].release();
  }

  void update(int index) {
    if (stopRequested_ == true && fadePhase_ > 0.f) {
        fadePhase_ -= 1000.f / (SAMPLE_RATE * 24.f);
        if (fadePhase_ < 0.f) {
         fadePhase_ = 0.f; 
        }
    }

    modMatrixEngine_->setMidiVelocity(velocity_);
    modMatrixEngine_->setEnvelope(0, envelopeEngine_[0].next());
    modMatrixEngine_->setEnvelope(1, envelopeEngine_[1].next());
    modMatrixEngine_->setLfo(0, lfoEngine_[0].next());
    modMatrixEngine_->setLfo(1, lfoEngine_[1].next());
    float *data = modMatrixEngine_->process();

    if (fadePhase_ == 0.f || envelopeEngine_[0].stage() == EnvelopeEngine::IDLE) {
      state_ = IDLE;
    }

    Patch& p = settings_->selectedPatch();
    dac_->set(index, 0, (p.oscillator().shape2() * data[ModMatrix::SHAPE_2]) * 65535);
    dac_->set(index, 1, (p.oscillator().modDepth() * data[ModMatrix::VCO_MOD_DEPTH]) * 65535);
    dac_->set(index, 2, (p.oscillator().shape1() * data[ModMatrix::SHAPE_1]) * 65535);
    dac_->set(index, 3, (calculatePitchOsc1() * data[ModMatrix::TUNE_1]) * 65535);
    dac_->set(index, 4, (p.amp().drive() * data[ModMatrix::DRIVE]) * 65535);
    dac_->set(index, 5, (fadePhase_ * data[ModMatrix::GAIN]) * 65535);
    dac_->set(index, 6, (p.filter().resonance2() * data[ModMatrix::RESONANCE_2]) * 65535);
    dac_->set(index, 7, (calculatePitchOsc2() * data[ModMatrix::TUNE_2]) * 65535);
    dac_->set(index, 8, (p.filter().cutoff1() * data[ModMatrix::CUTOFF_1]) * 65535);
    dac_->set(index, 9, (p.filter().cutoff2() * data[ModMatrix::CUTOFF_2]) * 65535);
    dac_->set(index, 10, (p.amp().pan() * data[ModMatrix::PAN]) * 65535);
    dac_->set(index, 11, (p.filter().resonance1() * data[ModMatrix::RESONANCE_1]) * 65535);

    if (settings_->calibration().enabled()) {
        dac_->set(index, 0, settings_->calibration().min());
        dac_->set(index, 1, settings_->calibration().max());
    }
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
  EnvelopeEngine envelopeEngine_[2];
  LfoEngine lfoEngine_[Settings::kNumLfos];

  float calculatePitchOsc1() {
    //  if (settings_->oscillator().trackNote1()) {
    // return settings_->calibration().note(note_);
   // }
    return 0.f;
  }

  float calculatePitchOsc2() {
    //  if (settings_->oscillator().trackNote2()) {
    // return settings_->calibration().note(note_);
   // }
    return 0.f;
  }
};

#endif
