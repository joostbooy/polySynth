#ifndef WavImporter_h
#define WavImporter_h

#include "wavFile.h"
#include "sampleAllocator.h"

class WavImporter {

public:

	void init(SampleAllocator *sampleAllocator) {
		sampleAllocator_ = sampleAllocator;
		wavFile_.init();
	}

	bool import(const char* path, bool as_mono) {
		// open the wav
		if (!wavFile_.open(path)) {
			wavFile_.close();
			return false;
		}

		as_mono_ = as_mono;

		// reset some variables
		curr_chn_ = 0;
		sample_raw_ = 0;
		sample_bytes_read_ = 0;
		bytes_received_ = 0;
		samples_received_ = 0;

		// set expected sizes
		size_t block_size = wavFile_.format.block_allign + (wavFile_.format.block_allign & 1);
		bytes_per_sample_ = block_size / wavFile_.format.num_of_channels;
		shifts_ = 8 * (bytes_per_sample_ - 1);

		sample_rate_prescaler_ = wavFile_.format.sample_rate / SAMPLE_RATE;
		size_t requested_size = (wavFile_.data.size / bytes_per_sample_) / sample_rate_prescaler_;

		int num_channels;

		if (as_mono_) {
			num_channels = 1;
			requested_size /= wavFile_.format.num_of_channels;
		} else {
			num_channels = wavFile_.format.num_of_channels;
		}

		// allocate ram
		sdram_ptr_ = sampleAllocator_->allocate(path, num_channels, requested_size);

		if (sdram_ptr_ == nullptr) {
			wavFile_.close();
			return false;
		}

		// get the allocated slot
		int slot = sampleAllocator_->num_samples() - 1;

		// read from file & write to ram
		uint8_t *data;
		uint32_t size;

		while (wavFile_.read(&data, &size)) {
			if (!write(data, size)) {
				sampleAllocator_->remove(slot);
				wavFile_.close();
				return false;
			}
		}

		wavFile_.close();

		if (bytes_received_ != wavFile_.data.size) {
			sampleAllocator_->remove(slot);
			return false;
		}

		return true;
	}

private:
	WavFile wavFile_;
	SampleAllocator *sampleAllocator_;

	bool as_mono_;

	int16_t* sdram_ptr_ = nullptr;
	int16_t mono_sample_;
	int16_t stereo_sample_left_;
	int16_t stereo_sample_right_;

	int shifts_;
	int curr_chn_;
	int bytes_per_sample_;
	int sample_bytes_read_;

	size_t sample_raw_;
	size_t bytes_received_;
	size_t samples_received_;
	size_t sample_rate_prescaler_;

	bool mono_sample_received(uint8_t data) {
		sample_raw_ >>= 8;
		sample_raw_ |= data << shifts_;

		if (++sample_bytes_read_ >= bytes_per_sample_) {
			mono_sample_ = static_cast<int16_t>(sample_raw_);
			sample_raw_ = 0;
			sample_bytes_read_ = 0;
			return true;
		}

		return false;
	}

	bool stereo_sample_received(uint8_t data) {
		if (curr_chn_ == 0) {
			if (mono_sample_received(data)) {
				stereo_sample_left_ = mono_sample_;
				curr_chn_ = 1;
			}
		} else {
			if (mono_sample_received(data)) {
				stereo_sample_right_ = mono_sample_;
				curr_chn_ = 0;
				return true;
			}
		}
		return false;
	}

	bool write(uint8_t *data, size_t size) {
		if ((bytes_received_ += size) > wavFile_.data.size) {
			return false;
		}

		if (wavFile_.format.num_of_channels == 1) {
			// mono
			while (size--) {
				if (mono_sample_received(*data++)) {
					if (++samples_received_ >= sample_rate_prescaler_) {
						samples_received_ = 0;
						*sdram_ptr_++ = mono_sample_;
					}
				}
			}
		} else if (wavFile_.format.num_of_channels == 2) {
			// stereo to mono
			if (as_mono_) {
				while (size--) {
					if (stereo_sample_received(*data++)) {
						if (++samples_received_ >= sample_rate_prescaler_) {
							samples_received_ = 0;
							*sdram_ptr_++ = (stereo_sample_left_ + stereo_sample_right_) * 0.5f;
						}
					}
				}
			} else {
				// stereo
				while (size--) {
					if (stereo_sample_received(*data++)) {
						if (++samples_received_ >= sample_rate_prescaler_) {
							samples_received_ = 0;
							*sdram_ptr_++ = stereo_sample_left_;
							*sdram_ptr_++ = stereo_sample_right_;
						}
					}
				}
			}
		}

		return true;
	}

};

#endif
