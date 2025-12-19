#ifndef Voice_h
#define Voice_h

#include "dac.h"
#include "envelopeEngine.h"
#include "lfoEngine.h"
#include "modulationMatrixEngine.h"

class Voice {
 public:
  enum State { BACKWARD = -1, IDLE = 0, FORWARD = 1 };

  void init(Settings* settings, ModulationMatrixEngine* modulationMatrixEngine) {
    settings_ = settings;
    state_ = IDLE;
    key_pressed_ = false;
    stop_requested_ = false;
    lastNote_ = 60;
    modulationMatrixEngine_ = modulationMatrixEngine;
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

  bool key_pressed() {
    return key_pressed_;
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

    lfoEngine_[0].reset();
    lfoEngine_[1].reset();

    envelopeEngine_[0].attack();
    envelopeEngine_[1].attack();
  }

  void note_off() {
    key_pressed_ = false;
    envelopeEngine_[0].release();
    envelopeEngine_[1].release();
  }

  void update(Dac* dac) {
    modulationMatrixEngine_->set_midi_velocity(velocity_);
    modulationMatrixEngine_->set_envelope(0, envelopeEngine_[0].next());
    modulationMatrixEngine_->set_envelope(1, envelopeEngine_[1].next());
    modulationMatrixEngine_->setLfo(0, lfoEngine_[0].next());
    modulationMatrixEngine_->setLfo(1, lfoEngine_[1].next());
    ModulationMatrixEngine::Frame* frame = modulationMatrixEngine_->process();

    if (stop_requested_) {
      if (fade_phase_ > 0.0f) {
        fade_phase_ -= 1000.f / (CONTROL_RATE * 4.f);
      } else {
        stop_requested_ = false;
        state_ = IDLE;
      }
    }
    dac->set(0, (fade_phase_ * frame->data[ModulationMatrix::GAIN]));
    // dac->set(1, (frame->data[ModulationMatrix::BEND]));
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
  ModulationMatrixEngine* modulationMatrixEngine_;
  EnvelopeEngine envelopeEngine_[Settings::kNumEnvelopes];
  LfoEngine lfoEngine_[Settings::kNumLfos];
};

#endif
