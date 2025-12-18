#ifndef EnvelopeEngine_h
#define EnvelopeEngine_h

#include "envelope.h"
#include "curve.h"

class EnvelopeEngine {

public:

	enum Stage {
		ATTACK,
		DECAY,
		SUSTAIN,
		HOLD,
		RELEASE,
		IDLE
	};

	void init(Envelope *envelope) {
		envelope_ = envelope;
		stage_ = IDLE;
		phase_ = 0.0f;
		sample_ = 0.0f;
		mode_ = 0;
	}

	void attack() {
		phase_ = 0.0f;
		sample_ = 0.0f;
		stage_ = ATTACK;
		mode_ = envelope_->mode();
	}

	void release() {
		if (mode_ == Envelope::GATE) {
			release_level_ = sample_;
			phase_ = 0.0f;
			stage_ = RELEASE;
		}
	}

	float phase() {
		return phase_;
	}

	float sample() {
		return sample_;
	}

	float next() {
		switch (stage_)
		{
		case IDLE:
			sample_ = 0.f;
			break;
		case ATTACK:
			phase_ += envelope_->attack_inc();
			if (phase_ < 1.f) {
				sample_ = Curve::read(phase_, envelope_->attack_shape());
			} else {
				phase_ = 0.f;
				stage_ = DECAY;
			}
			break;
		case DECAY:
			phase_ += envelope_->decay_inc();
			if (phase_ < 1.f) {
				sample_ = Dsp::cross_fade(1.f, envelope_->sustain_level(), Curve::read(phase_, envelope_->decay_shape()));
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
			sample_ = envelope_->sustain_level();
			break;
		case HOLD:
			phase_ += envelope_->hold_inc();
			if (phase_ < 1.f) {
				sample_ = envelope_->sustain_level();
			} else {
				phase_ = 0.f;
				release_level_ = sample_;
				stage_ = RELEASE;
			}
			break;
		case RELEASE:
			phase_ += envelope_->release_inc();
			if (phase_ < 1.f) {
				sample_ = Dsp::cross_fade(release_level_, 0.f, Curve::read(phase_, envelope_->release_shape()));
			} else {
				phase_ = 0.f;
				stage_ = IDLE;
			}
			break;
		default:
			break;
		}

		return sample_;
	}

private:
	Stage stage_;
	float phase_;
	float sample_;
	float release_level_;

	uint8_t mode_;
	Envelope *envelope_;
};

#endif
