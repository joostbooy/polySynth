#ifndef LfoEngine_h
#define LfoEngine_h

#include "engineUtils.h"
#include "lfo.h"
#include "rng.h"

class LfoEngine {
 public:
  enum Stage {
    FALLING = 0,
    RISING = 1,
  };

  void init(Lfo* lfo) {
    lfo_ = lfo;
    reset();

    if (stage_ == RISING) {
      lastValue_ = lfo_->min();
    } else {
      lastValue_ = lfo_->max();
    }
  }

  void reset() {
    phase_ = 0.f;
    setStage(nextPhase() < lfo_->skew() ? RISING : FALLING, true);
  }

  void retrigger(int playOrder) {
    playOrder_ = playOrder;
    if (lfo_->retrigger()) {
      reset();
    }
  }

  float phase() {
    return phase_;
  }

  float value() {
    return value_;
  }

  float next() {
    float skewPhase;
    float skewAmount = EngineUtils::spread(lfo_->skew(), lfo_->skewSpread(), playOrder_);
    float phase = nextPhase();

    if (phase < skewAmount) {
      setStage(RISING);
      skewPhase = phase * (1.0f / skewAmount);
    } else {
      setStage(FALLING);
      skewPhase = (phase - skewAmount) * (1.0f / (1.0f - skewAmount));
    }

    float x = 0.f;

    switch (lfo_->type()) {
      case Lfo::LINEAR:
        x = skewPhase;
        break;
      case Lfo::EXP_LOG:
        x = LookupTablesUtils::readInterpolated(lut_exp, skewPhase);
        break;
      case Lfo::LOG_EXP:
        x = LookupTablesUtils::readInterpolated(lut_inv_exp, skewPhase);
        break;
      case Lfo::SQUARE:
        x = phase_ < skewAmount ? 1.f : 0.f;
        break;
      default:
        break;
    }

    value_ = Dsp::cross_fade(lastValue_, targetValue_, x);
    return value_;
  }

 private:
  Lfo* lfo_;
  Stage stage_ = RISING;
  float phase_ = 0.f;
  float value_ = 0.f;
  float lastValue_ = 0.f;
  float targetValue_ = 1.f;
  int playOrder_ = 0;

  inline void setStage(Stage stage, bool force = false) {
    if (stage_ != stage || force == true) {
      stage_ = stage;

      lastValue_ = value_;

      if (lfo_->randomise()) {
        targetValue_ = Rng::reciprocal(lfo_->min(), lfo_->max());
      } else {
        targetValue_ = (stage_ == RISING) ? lfo_->max() : lfo_->min();
      }
    }
  }

  float readInc() {
    if (lfo_->clockSync()) {
      return MidiClockEngine::readInc(lfo_->speed());
    } else {
      return LookupTablesUtils::read(lut_phase_inc, lfo_->speed());
    }
  }

  float nextPhase() {
    float offsetPhase;

    float offset = EngineUtils::spread(lfo_->phaseOffset(), lfo_->phaseOffsetSpread(), playOrder_);
    if (phase_ + offset >= 1.f) {
      offsetPhase = phase_ - (1.f - offset);
    } else {
      offsetPhase = phase_ + offset;
    }

    phase_ += readInc();
    if (phase_ >= 1.f) {
      phase_ = lfo_->oneShot() ? 1.f : 0.f;
    }

    return offsetPhase;
  }
};

#endif
