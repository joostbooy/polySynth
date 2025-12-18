#ifndef Sample_h
#define Sample_h

#include "settingsText.h"
#include "settingsUtils.h"
#include "stringUtils.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "hash.h"

class Sample {

public:

	enum PlayMode {
		FORWARD,
		BACKWARD,

		NUM_PLAY_MODES
	};

	static const char* play_mode_text(int mode) {
		switch (mode)
		{
		case FORWARD:	return "FORWARD";
		case BACKWARD:	return "BACKWARD";
		default:
			break;
		}
		return nullptr;
	}

	void init() {
		set_path("");
		set_size(100);
		set_num_channels(0);
		set_data(nullptr);
		set_loop(false);
		set_u_turn(false);
		set_play_mode(FORWARD);
		set_cents(0);
		set_root_note(60);
		set_gain(1.0);
		set_pan(0.5f);

		// these need to be initialised without setters,
		// because they depend on eachother
		start_ = 0;
		end_ = 100;
		loop_start_ = 0;
		loop_end_ = 100;
		key_range_low_ = 0;
		key_range_high_ = 127;
	}

	// data
	int16_t *data() {
		return data_;
	}

	void set_data(int16_t *data) {
		data_ = data;
	}

	void read(size_t index, int16_t *left, int16_t *right) {
		if (is_stereo()) {
			size_t i = index * 2;
			*left = data_[i];
			*right = data_[i + 1];
		} else {
			*left = data_[index];
			*right = *left;
		}
	}

	bool has_data() {
		return data_ != nullptr;
	}

	// path
	const char *path() {
		return path_;
	}

	void set_path(const char *new_path) {
		StringUtils::copy(path_, const_cast<char*>(new_path), kMaxPathLength);
		hash_.init();
		hash_.write(path_, kMaxPathLength);
	}

	uint32_t hash() {
		return hash_.read();
	}

	const char *name() {
		StringUtils::get_file_name_from_path(path_, name_);
		return name_;
	}

	// size
	size_t size() {
		return size_;
	}

	void set_size(size_t size) {
		size_ = size;
	}

	const char *size_text() {
		return SettingsText::samples_to_time(size_samples());
	}

	size_t size_samples() {
		return is_stereo() ? size_ / 2 : size_;
	}

	void refresh_start_end_points() {
		if (end_ >= size_samples()) {
			end_ = size_samples();
		}

		if (loop_end_ >= size_samples()) {
			loop_end_ = size_samples();
		}

		if (start_ >= end_) {
			start_ = end_ - 1;
		}

		if (loop_start_ >= loop_end_) {
			loop_start_ = loop_end_ - 1;
		}
	}

	// channels
	int num_channels() {
		return num_channels_;
	}

	void set_num_channels(int num_channels) {
		num_channels_ = num_channels;
	}

	bool is_stereo() {
		return num_channels_ == 2;
	}


	// start
	void set_start(int value) {
		start_ = SettingsUtils::clip(0, end() - 1, value);
	}

	size_t start() {
		return start_;
	}

	const char *start_text() {
		return SettingsText::samples_to_time(start());
	}


	// end
	void set_end(int value) {
		end_ = SettingsUtils::clip(start() + 1, size_samples(), value);
	}

	size_t end() {
		return end_;
	}

	const char *end_text() {
		return SettingsText::samples_to_time(end());
	}

	// loop start
	void set_loop_start(int value) {
		loop_start_ = SettingsUtils::clip(1, loop_end(), value);
	}

	size_t loop_start() {
		return loop_start_;
	}

	const char *loop_start_text() {
		return SettingsText::samples_to_time(loop_start());
	}


	// loop end
	void set_loop_end(int value) {
		loop_end_ = SettingsUtils::clip(loop_start(), size_samples() - 1, value);
	}

	size_t loop_end() {
		return loop_end_;
	}

	const char *loop_end_text() {
		return SettingsText::samples_to_time(loop_end());
	}


	// loop
	void set_loop(bool value) {
		loop_ = value;
	}

	bool loop() {
		return loop_;
	}

	const char *loop_text() {
		return SettingsText::bool_to_on_off(loop());
	}


	// Cents
	void set_cents(int value) {
		cents_ = SettingsUtils::clip(-99, 99, value);
	}

	int cents() {
		return cents_;
	}

	const char *cents_text() {
		return SettingsText::str.write(cents());
	}

	// Root note
	void set_root_note(int value) {
		root_note_ = SettingsUtils::clip(0, 127, value);
	}

	uint8_t root_note() {
		return root_note_;
	}

	const char *root_note_text() {
		return SettingsText::note_to_text(root_note());
	}

	// Key range low
	void set_key_range_low(int value) {
		key_range_low_ = SettingsUtils::clip(0, key_range_high(), value);
	}

	uint8_t key_range_low() {
		return key_range_low_;
	}

	const char *key_range_low_text() {
		return SettingsText::note_to_text(key_range_low());
	}

	// Key range high
	void set_key_range_high(int value) {
		key_range_high_ = SettingsUtils::clip(key_range_low(), 127, value);
	}

	uint8_t key_range_high() {
		return key_range_high_;
	}

	const char *key_range_high_text() {
		return SettingsText::note_to_text(key_range_high());
	}

	bool within_key_range(uint8_t key) {
		return key >= key_range_low() && key <= key_range_high();
	}

	// Play mode
	void set_play_mode(int value) {
		play_mode_ = SettingsUtils::clip(0, NUM_PLAY_MODES - 1, value);
	}

	uint8_t play_mode() {
		return play_mode_;
	}

	const char *play_mode_text() {
		return play_mode_text(play_mode());
	}

	// U turn
	void set_u_turn(bool value) {
		u_turn_ = value;
	}

	bool u_turn() {
		return u_turn_;
	}

	const char *u_turn_text() {
		return SettingsText::bool_to_on_off(u_turn());
	}

	// Gain
	void set_gain(float value) {
		gain_ = SettingsUtils::clip_float(value);
	}

	float gain() {
		return gain_;
	}

	const char *gain_text() {
		return SettingsText::float_to_text(gain(), 0, 100);
	}

	// pan
	float pan() {
		return pan_;
	}

	void set_pan(float value) {
		pan_ = SettingsUtils::clip_float(value);
	}

	const char* pan_text() {
		return SettingsText::float_to_text(pan(), -100, 100);
	}


	// name to note
	bool map_name_to_root_note() {
		uint8_t note;
		if (name_to_midi_note(name(), &note)) {
			root_note_ = note;
			return true;
		}
		return false;
	}

	// name to key range
	bool map_name_to_key_range() {
		uint8_t note;
		if (name_to_midi_note(name(), &note)) {
			key_range_low_ = note;
			key_range_high_ = note;
			return true;
		}
		return false;
	}

	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(start_);
		fileWriter.write(end_);
		fileWriter.write(loop_);
		fileWriter.write(u_turn_);
		fileWriter.write(play_mode_);
		fileWriter.write(loop_start_);
		fileWriter.write(loop_end_);
		fileWriter.write(cents_);
		fileWriter.write(root_note_);
		fileWriter.write(key_range_low_);
		fileWriter.write(key_range_high_);
		fileWriter.write(gain_);
		fileWriter.write(pan_);
		fileWriter.write(num_channels_);

		for (size_t i = 0; i < kMaxPathLength; ++i) {
			fileWriter.write(path_[i]);
		}

		// dont save the size or data
		// thats handled by the sampleAllocator
	}

	void load(FileReader &fileReader) {
		fileReader.read(start_);
		fileReader.read(end_);
		fileReader.read(loop_);
		fileReader.read(u_turn_);
		fileReader.read(play_mode_);
		fileReader.read(loop_start_);
		fileReader.read(loop_end_);
		fileReader.read(cents_);
		fileReader.read(root_note_);
		fileReader.read(key_range_low_);
		fileReader.read(key_range_high_);
		fileReader.read(gain_);
		fileReader.read(pan_);
		fileReader.read(num_channels_);

		for (size_t i = 0; i < kMaxPathLength; ++i) {
			fileReader.read(path_[i]);
		}

		// dont load the size or data
		// thats handled by the sampleAllocator
	}

	void paste(Sample *sample) {
		start_ = sample->start();
		end_ = sample->end();
		loop_ = sample->loop();
		u_turn_ = sample->u_turn();
		play_mode_ = sample->play_mode();
		loop_start_ = sample->loop_start();
		loop_end_ = sample->loop_end();
		cents_ = sample->cents();
		root_note_ = sample->root_note();
		key_range_low_ = sample->key_range_low();
		key_range_high_ = sample->key_range_high();
		gain_ = sample->gain();
		pan_ = sample->gain();

		refresh_start_end_points();

		// dont paste data, path, size or num_channels
		// thats handled by the sampleAllocator
	}

private:
	static const size_t kMaxPathLength = 64;

	int16_t *data_;
	size_t size_;

	Hash hash_;

	char path_[kMaxPathLength];
	int num_channels_;
	size_t start_;
	size_t end_;
	bool loop_;
	bool u_turn_;
	uint8_t play_mode_;
	size_t loop_start_;
	size_t loop_end_;
	int cents_;
	uint8_t root_note_;
	uint8_t key_range_low_;
	uint8_t key_range_high_;
	float gain_;
	float pan_;


	static bool name_to_midi_note(const char *name, uint8_t *note);
	static constexpr const uint8_t char_to_note_[7] = { 9, 11, 0, 2, 4, 5, 7 };
	static constexpr const bool has_seminote_[12] = { 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0 };
	static char name_[kMaxPathLength];
};

#endif
