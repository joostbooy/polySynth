#ifndef LfoEngine_h
#define LfoEngine_h

#include "lfo.h"
#include "curve.h"
#include "rng.h"

class LfoEngine {

public:

	enum Stage {
		FALLING = 0,
		RISING = 1,
	};

	void init(Lfo *lfo) {
		lfo_ = lfo;
		reset();

		if (stage_ == RISING) {
			last_value_ = lfo_->min();
		} else {
			last_value_ = lfo_->max();
		}
	}

	void reset() {
		phase_ = lfo_->sync_phase();
		set_stage(phase_ < lfo_->skew() ? RISING : FALLING, true);
	}

	void retrigger(MidiEngine::Event &e) {
		if (lfo_->retrigger_accepted(e)) {
			reset();
		}
	}

	float phase() { return phase_; }
	float value() { return value_; }

	float next() {
		float skew_phase;
		float skew_amount = lfo_->skew();

		if (phase_ < skew_amount) {
			set_stage(RISING);
			skew_phase = phase_ * (1.0f / skew_amount);
		} else {
			set_stage(FALLING);
			skew_phase = (phase_ - skew_amount) * (1.0f / (1.0f - skew_amount));
		}

		value_ = Dsp::cross_fade(last_value_, target_value_, Curve::read(skew_phase, lfo_->shape()));

		phase_ += lfo_->inc();
		if (phase_ >= 1.f) {
			phase_ = 0.f;
		}

		return value_;
	}

private:
	Lfo *lfo_;
	Stage stage_ = RISING;
	float phase_ = 0.f;
	float value_ = 0.f;
	float last_value_ = 0.f;
	float target_value_ = 1.f;

	inline void set_stage(Stage stage, bool force = false) {
		if (stage_ != stage || force == true) {
			stage_ = stage;

			last_value_ = value_;

			if (lfo_->randomise()) {
				target_value_ = Rng::reciprocal(lfo_->min(), lfo_->max());
			} else {
				target_value_ = (stage_ == RISING) ? lfo_->max() : lfo_->min();
			}
		}
	}

};

#endif
