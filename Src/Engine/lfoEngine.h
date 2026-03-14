#ifndef LfoEngine_h
#define LfoEngine_h

#include "engineUtils.h"
#include "lfo.h"
#include "rng.h"

class LfoEngine {
 public:
  enum Stage {
    RISING,
    FALLING,
  };

  void init(Lfo* lfo) {
    lfo_ = lfo;
    reset();
  }

  void reset() {
    phase_ = 0.f;
    randomMax_ = Rng::reciprocal(lfo_->min(), lfo_->max());
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

  float next() {
    float skewPhase;
    float skewAmount = EngineUtils::spread(lfo_->skew(), lfo_->skewSpread(), playOrder_);
    float phase = nextPhase();

    if (phase < skewAmount) {
      stage_ = RISING;
      skewPhase = phase * (1.0f / skewAmount);
    } else {
      stage_ = FALLING;
      skewPhase = 1.f - (phase - skewAmount) * (1.0f / (1.0f - skewAmount));
    }

    float x = 0.f;

    switch (lfo_->type()) {
      case Lfo::LINEAR:
        x = skewPhase;
        break;
      case Lfo::EXP_LOG:
        if (stage_ == RISING) {
          x = LookupTablesUtils::readInterpolated(lut_inv_exp, skewPhase);
        } else {
          x = LookupTablesUtils::readInterpolated(lut_exp, skewPhase);
        }
        break;
      case Lfo::LOG_EXP:
        if (stage_ == RISING) {
          x = LookupTablesUtils::readInterpolated(lut_exp, skewPhase);
        } else {
          x = LookupTablesUtils::readInterpolated(lut_inv_exp, skewPhase);
        }
        break;
      case Lfo::SQUARE:
        x = phase_ < skewAmount ? 1.f : 0.f;
        break;
      default:
        break;
    }

    float min = lfo_->min();
    float max = lfo_->randomise() ? randomMax_ : lfo_->max();
    return min + (max - min) * x;
  }

 private:
  Lfo* lfo_;
  float phase_ = 0.f;
  float randomMax_ = 1.f;
  int playOrder_ = 0;
  Stage stage_;

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
      if (lfo_->oneShot()) {
        phase_ = 1.f;
      } else {
        reset();
      }
    }

    return offsetPhase;
  }
};

#endif
