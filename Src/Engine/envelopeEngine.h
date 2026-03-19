#ifndef EnvelopeEngine_h
#define EnvelopeEngine_h

#include "envelope.h"
#include "lookupTablesUtils.h"

class EnvelopeEngine {
 public:
  enum Stage {
    ATTACK,
    DECAY,
    SUSTAIN,
    HOLD,
    RELEASE,
    IDLE,
  };

  void init(Envelope* envelope) {
    envelope_ = envelope;
    stage_ = IDLE;
    phase_ = 0.f;
    sample_ = 0.f;
  }

  void attack() {
    phase_ = 0.0f;
    sample_ = 0.0f;
    stage_ = ATTACK;
    mode_ = envelope_->mode();
  }

  void release() {
    if (mode_ == Envelope::GATE) {
      releaseLevel_ = sample_;
      phase_ = 0.0f;
      stage_ = RELEASE;
    }
  }

  Stage stage() {
    return stage_;
  }

  float phase() {
    return phase_;
  }

  float sample() {
    if (stage_ == IDLE) {
      return 0.f;
    } else {
      return envelope_->invert() ? 1.f - sample_ : sample_;
    }
  }

  float next() {
    switch (stage_) {
      case IDLE:
        sample_ = 0.f;
        break;
      case ATTACK:
        phase_ += readInc(envelope_->attackTime());
        if (phase_ < 1.f) {
          sample_ = readCurve(envelope_->attackShape());
        } else {
          phase_ = 0.f;
          stage_ = DECAY;
        }
        break;
      case DECAY:
        phase_ += readInc(envelope_->decayTime());
        if (phase_ < 1.f) {
          sample_ = Dsp::cross_fade(1.f, envelope_->sustainLevel(), readCurve(envelope_->decayShape()));
        } else {
          if (mode_ == Envelope::GATE) {
            phase_ = 0.f;
            stage_ = SUSTAIN;
          } else if (mode_ == Envelope::TRIGGER) {
            phase_ = 0.f;
            stage_ = HOLD;
          }
        }
        break;
      case SUSTAIN:
        sample_ = envelope_->sustainLevel();
        break;
      case HOLD:
        phase_ += readInc(envelope_->holdTime());
        if (phase_ < 1.f) {
          sample_ = envelope_->sustainLevel();
        } else {
          phase_ = 0.f;
          releaseLevel_ = sample_;
          stage_ = RELEASE;
        }
        break;
      case RELEASE:
        phase_ += readInc(envelope_->releaseTime());
        if (phase_ < 1.f) {
          sample_ = Dsp::cross_fade(releaseLevel_, 0.f, readCurve(envelope_->releaseShape()));
        } else {
          phase_ = 0.f;
          stage_ = envelope_->loop() ? ATTACK : IDLE;
        }
        break;
      default:
        break;
    }

    return sample();
  }

 private:
  Stage stage_;
  float phase_;
  float sample_;
  float releaseLevel_;

  uint8_t mode_;
  Envelope* envelope_;

  float readCurve(float shape) {
    return LookupTablesUtils::readInterpolated(lut_exp, lut_inv_exp, phase_, shape);
  }

  float readInc(float value) {
    if (envelope_->clockSync()) {
      return MidiClockEngine::readInc(value);
    } else {
      return LookupTablesUtils::read(lut_phase_inc, value);
    }
  }

};

#endif
