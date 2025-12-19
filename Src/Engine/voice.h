#ifndef Voice_h
#define Voice_h

#include "dac.h"
#include "envelopeEngine.h"
#include "lfoEngine.h"
#include "modMatrixEngine.h"

class Voice {
 public:
  enum State { IDLE = 0, ACTIVE = 1 };

  void init(Settings* settings, ModMatrixEngine* modMatrixEngine) {
    settings_ = settings;
    state_ = IDLE;
    key_pressed_ = false;
    stop_requested_ = false;
    lastNote_ = 60;
    modMatrixEngine_ = modMatrixEngine;

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

  bool key_pressed() {
    return key_pressed_;
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

  void request_stop() {
    stop_requested_ = true;
  }

  bool is_available() {
    return state_ == IDLE;
  }

  void note_on(MidiEngine::Event& e) {
    port_ = e.port;
    lastNote_ = note_;
    note_ = e.data[0];
    channel_ = e.message & 0x0F;
    velocity_ = e.data[1] * (1.f / 127.f);

    key_pressed_ = true;
    stop_requested_ = false;
    fade_phase_ = 1.f;

    lfoEngine_[0].retrigger();
    lfoEngine_[1].retrigger();

    ampEnvelopeEngine_.attack();
    modEnvelopeEngine_.attack();

    state_ = ACTIVE;
  }

  void note_off() {
    key_pressed_ = false;
    ampEnvelopeEngine_.release();
    modEnvelopeEngine_.release();
  }

  void update(Dac* dac) {
    if (stop_requested_) {
      if (fade_phase_ > 0.0f) {
        fade_phase_ -= 1000.f / (CONTROL_RATE * 4.f);
      } else {
        state_ = IDLE;
      }
    }

    if (ampEnvelopeEngine_.stage() == EnvelopeEngine::IDLE) {
      state_ = IDLE;
    }

    modMatrixEngine_->set_midi_velocity(velocity_);
    modMatrixEngine_->setAmpEnvelope(ampEnvelopeEngine_.next());
    modMatrixEngine_->setModEnvelope(modEnvelopeEngine_.next());
    modMatrixEngine_->setLfo(0, lfoEngine_[0].next());
    modMatrixEngine_->setLfo(1, lfoEngine_[1].next());
    ModMatrixEngine::Frame* frame = modMatrixEngine_->process();

    Patch &p = settings_->selectedPatch();
    dac->set(0, (fade_phase_ * frame->data[ModMatrix::GAIN]) * 65535);
    dac->set(1, (p.filter().resonance1() * frame->data[ModMatrix::RESONANCE_1]) * 65535);
    dac->set(2, (p.filter().resonance2() * frame->data[ModMatrix::RESONANCE_2]) * 65535);
    dac->set(3, (p.oscillator().shape1() * frame->data[ModMatrix::SHAPE_1]) * 65535);
    dac->set(3, (p.oscillator().shape2() * frame->data[ModMatrix::SHAPE_2]) * 65535);
    dac->set(5, (p.filter().cutoff1() * frame->data[ModMatrix::CUTOFF_1]) * 65535);
    dac->set(5, (p.filter().cutoff2() * frame->data[ModMatrix::CUTOFF_2]) * 65535);
    dac->set(7, (calculatePitch() * frame->data[ModMatrix::PITCH]) * 65535);
  }

 private:
  bool key_pressed_;
  bool stop_requested_;
  uint8_t note_;
  uint8_t lastNote_;
  uint8_t port_;
  uint8_t channel_;
  State state_;
  float velocity_;
  float fade_phase_;
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
