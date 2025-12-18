#ifndef Delay_h
#define Delay_h

class Delay {

public:

	static const size_t kMaxDelay = SAMPLE_RATE * 2;

	void init() {
		set_amount(0.5f);
		set_feedback(0.5f);
		set_mix(0.0f);
		set_audio_channel(0);
		set_sync(false);
	}

	// Amount
	float amount() {
		return amount_;
	}

	void set_amount(float value) {
		amount_ = SettingsUtils::clip_float(value);
	}

	const char *amount_text() {
		if (sync()) {
			return MidiSync::tempo_text(amount() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return SettingsText::samples_to_time(amount() * kMaxDelay);
		}
	}

	size_t num_samples() {
		if (sync()) {
			size_t samples = MidiSync::read_beat_length(amount() * (MidiSync::NUM_TEMPOS - 1));
			return SettingsUtils::clip_max(kMaxDelay, samples);
		} else {
			return amount() * kMaxDelay;
		}
	}

	// Feedback
	float feedback() {
		return feedback_;
	}

	void set_feedback(float value) {
		feedback_ = SettingsUtils::clip_float(value);
	}

	const char *feedback_text() {
		return SettingsText::float_to_text(feedback(), 0, 100);
	}

	// Mix
	float mix() {
		return mix_;
	}

	void set_mix(float value) {
		mix_ = SettingsUtils::clip_float(value);
	}

	const char *mix_text() {
		return SettingsText::float_to_text(mix(), 0, 100);
	}

	// Audio channel
	int audio_channel() {
		return audio_channel_;
	}

	void set_audio_channel(int value) {
		audio_channel_ = SettingsUtils::clip(0, Dac::kNumStereoChannels - 1, value);
	}

	const char *audio_channel_text() {
		return SettingsText::audio_channel_to_text(audio_channel());
	}

	// Sync
	bool sync() {
		return sync_;
	}

	void set_sync(bool value) {
		sync_ = value;
	}

	const char *sync_text() {
		return SettingsText::bool_to_on_off(sync());
	}

	// storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(amount_);
		fileWriter.write(feedback_);
		fileWriter.write(mix_);
		fileWriter.write(audio_channel_);
		fileWriter.write(sync_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(amount_);
		fileReader.read(feedback_);
		fileReader.read(mix_);
		fileReader.read(audio_channel_);
		fileReader.read(sync_);
	}

	void paste(Delay *delay) {
		mix_ = delay->mix();
		amount_ = delay->amount();
		feedback_ = delay->feedback();
		audio_channel_ = delay->audio_channel();
		sync_ = delay->sync();
	}

private:
	float amount_;
	float feedback_;
	float mix_;
	int audio_channel_;
	bool sync_;
};

#endif
