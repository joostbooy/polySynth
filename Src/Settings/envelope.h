#ifndef Envelope_h
#define Envelope_h

#include "settingsText.h"
#include "settingsUtils.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "midiSync.h"

class Envelope {

public:

	enum Mode {
		GATE,
		TRIGGER,

		NUM_MODES
	};

	static const char* mode_text(uint8_t value) {
		switch (value)
		{
		case GATE:		return "GATE";
		case TRIGGER:	return "TRIGGER";
		default:
			break;
		}
		return nullptr;
	}

	void init() {
		set_clock_sync(false);
		set_mode(GATE);
		set_attack_time(0);
		set_attack_shape(0.5f);
		set_decay_time(0);
		set_decay_shape(0.5f);
		set_hold_time(0);
		set_sustain_level(0.5f);
		set_release_time(0);
		set_release_shape(0.5f);
	}

	// Clock sync
	bool clock_sync() {
		return clock_sync_;
	}

	void set_clock_sync(bool value) {
		clock_sync_ = value;
	}

	const char *clock_sync_text() {
		return SettingsText::boolToOnOff(clock_sync());
	}

	// Mode
	int mode() {
		return mode_;
	}

	void set_mode(int value) {
		mode_ = SettingsUtils::clip(0, NUM_MODES - 1, value);
	}

	const char *mode_text() {
		return mode_text(mode());
	}

	// Attack time
	float attack_time() {
		return attack_time_;
	}

	void set_attack_time(float value) {
		attack_time_ = SettingsUtils::clip_float(value);
	}

	const char *attack_time_text() {
		if (clock_sync()) {
			return MidiSync::tempo_text(attack_time() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return SettingsText::floatToText(attack_time(), 0, 100);
		}
	}

	float attack_inc() {
		if (clock_sync()) {
			return MidiSync::read_inc(attack_time() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return lut_phase_inc[int(attack_time() * (PHASE_TABLE_SIZE - 1))];
		}
	}

	// Attack shape
	float attack_shape() {
		return attack_shape_;
	}

	void set_attack_shape(float value) {
		attack_shape_ = SettingsUtils::clip_float(value);
	}

	const char *attack_shape_text() {
		return SettingsText::floatToText(attack_shape(), -100, 100);
	}

	// Decay time
	float decay_time() {
		return attack_time_;
	}

	void set_decay_time(float value) {
		attack_time_ = SettingsUtils::clip_float(value);
	}

	const char *decay_time_text() {
		if (clock_sync()) {
			return MidiSync::tempo_text(decay_time() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return SettingsText::floatToText(decay_time(), 0, 100);
		}
	}

	float decay_inc() {
		if (clock_sync()) {
			return MidiSync::read_inc(decay_time() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return lut_phase_inc[int(decay_time() * (PHASE_TABLE_SIZE - 1))];
		}
	}

	// Decay shape
	float decay_shape() {
		return decay_shape_;
	}

	void set_decay_shape(float value) {
		decay_shape_ = SettingsUtils::clip_float(value);
	}

	const char *decay_shape_text() {
		return SettingsText::floatToText(decay_shape(), -100, 100);
	}

	// Hold time
	float hold_time() {
		return hold_time_;
	}

	void set_hold_time(float value) {
		if (mode() == TRIGGER) {
			hold_time_ = SettingsUtils::clip_float(value);
		}
	}

	const char *hold_time_text() {
		if (mode() == GATE) {
			return "-";
		} else if (clock_sync()) {
			return MidiSync::tempo_text(hold_time() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			size_t index = hold_time() * PHASE_TABLE_SIZE;
			return SettingsText::floatToText(lut_phase_length[index], "HZ");
		}
	}

	float hold_inc() {
		if (clock_sync()) {
			return MidiSync::read_inc(hold_time() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return lut_phase_inc[int(hold_time() * (PHASE_TABLE_SIZE - 1))];
		}
	}


	// Sustain level
	float sustain_level() {
		return sustain_level_;
	}

	void set_sustain_level(float value) {
		sustain_level_ = SettingsUtils::clip_float(value);
	}

	const char *sustain_level_text() {
		return SettingsText::floatToText(sustain_level(), 0, 100);
	}

	// Release time
	float release_time() {
		return release_time_;
	}

	void set_release_time(float value) {
		release_time_ = SettingsUtils::clip_float(value);
	}

	const char *release_time_text() {
		if (clock_sync()) {
			return MidiSync::tempo_text(release_time() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return SettingsText::floatToText(release_time(), 0, 100);
		}
	}

	float release_inc() {
		if (clock_sync()) {
			return MidiSync::read_inc(release_time() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return lut_phase_inc[int(release_time() * (PHASE_TABLE_SIZE - 1))];
		}
	}

	// Release shape
	float release_shape() {
		return release_shape_;
	}

	void set_release_shape(float value) {
		release_shape_ = SettingsUtils::clip_float(value);
	}

	const char *release_shape_text() {
		return SettingsText::floatToText(release_shape(), -100, 100);
	}

	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(clock_sync_);
		fileWriter.write(mode_);
		fileWriter.write(attack_time_);
		fileWriter.write(attack_shape_);
		fileWriter.write(decay_time_);
		fileWriter.write(decay_shape_);
		fileWriter.write(hold_time_);
		fileWriter.write(sustain_level_);
		fileWriter.write(release_time_);
		fileWriter.write(release_shape_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(clock_sync_);
		fileReader.read(mode_);
		fileReader.read(attack_time_);
		fileReader.read(attack_shape_);
		fileReader.read(decay_time_);
		fileReader.read(decay_shape_);
		fileReader.read(hold_time_);
		fileReader.read(sustain_level_);
		fileReader.read(release_time_);
		fileReader.read(release_shape_);
	}

	void paste(Envelope *envelope) {
		clock_sync_ = envelope->clock_sync();
		mode_ = envelope->mode();
		attack_time_ = envelope->attack_time();
		attack_shape_ = envelope->attack_shape();
		decay_time_ = envelope->decay_time();
		decay_shape_ = envelope->decay_shape();
		hold_time_ = envelope->hold_time();
		sustain_level_ = envelope->sustain_level();
		release_time_ = envelope->release_time();
		release_shape_ = envelope->release_shape();
	}

private:
	bool clock_sync_;
	int mode_;
	float attack_time_;
	float attack_shape_;
	float decay_time_;
	float decay_shape_;
	float hold_time_;
	float sustain_level_;
	float release_time_;
	float release_shape_;
};

#endif
