#ifndef DelayEngine_h
#define DelayEngine_h

#include "settings.h"
#include <algorithm>

class DelayEngine {

public:

	void init(Settings *settings) {
		pos_ = 0;
		delay_ = &settings->delay();
		std::fill(&delay_l_[0], &delay_l_[Delay::kMaxDelay], 0);
		std::fill(&delay_r_[0], &delay_r_[Delay::kMaxDelay], 0);
	}

	void fill(Dac::Channel *channel, size_t size) {
		int16_t *left = &channel[delay_->audio_channel()].left[0];
		int16_t *right = &channel[delay_->audio_channel()].right[0];

		float mix = delay_->mix();
		float feedback = delay_->feedback();
		size_t num_samples = delay_->num_samples();

		while (size--) {
			int16_t l_in = *left;
			int16_t r_in = *right;

			delay_l_[pos_] = l_in + (delay_l_[pos_] * feedback);
			delay_r_[pos_] = r_in + (delay_r_[pos_] * feedback);

			*left++ = Dsp::cross_fade(l_in, delay_l_[pos_], mix);
			*right++ = Dsp::cross_fade(r_in, delay_r_[pos_], mix);

			if (++pos_ >= num_samples) {
				pos_ = 0;
			}
		}
	}

private:
	Delay *delay_;
	size_t pos_;
	int16_t delay_l_[Delay::kMaxDelay];
	int16_t delay_r_[Delay::kMaxDelay];
};

#endif
