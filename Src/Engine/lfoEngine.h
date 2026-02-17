#ifndef LfoEngine_h
#define LfoEngine_h

#include "curve.h"
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
    phase_ = lfo_->syncPhase();
    setStage(phase_ < lfo_->skew() ? RISING : FALLING, true);
  }

  void retrigger() {
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
    float skewAmount = lfo_->skew();

    if (phase_ < skewAmount) {
      setStage(RISING);
      skewPhase = phase_ * (1.0f / skewAmount);
    } else {
      setStage(FALLING);
      skewPhase = (phase_ - skewAmount) * (1.0f / (1.0f - skewAmount));
    }

    float x = 0.f;

    switch (lfo_->type()) {
      case Lfo::LINEAR:
        x = skewPhase;
        break;
      case Lfo::EXP_LOG:
        x = Curve::read(skewPhase, 1.f);
        break;
      case Lfo::LOG_EXP:
        x = Curve::read(skewPhase, 0.f);
        break;
      case Lfo::SQUARE:
        x = phase_ < skewAmount ? 1.f : 0.f;
        break;
      default:
        break;
    }

    value_ = Dsp::cross_fade(lastValue_, targetValue_, x);

    phase_ += lfo_->inc();
    if (phase_ >= 1.f) {
      phase_ = lfo_->oneShot() ? 1.f : 0.f;
    }

    return value_;
  }

 private:
  Lfo* lfo_;
  Stage stage_ = RISING;
  float phase_ = 0.f;
  float value_ = 0.f;
  float lastValue_ = 0.f;
  float targetValue_ = 1.f;

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
};

#endif
