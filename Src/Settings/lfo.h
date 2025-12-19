#ifndef Lfo_h
#define Lfo_h

#include "settingsText.h"
#include "settingsUtils.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "midi.h"
#include "midiSync.h"
#include "midiEngine.h"

class Lfo {

public:

	void init() {
		set_speed(0);
		set_shape(0.5f);
		set_skew(0.5f);
		set_min(0.0f);
		set_max(1.0f);
		set_sync_phase(0.0f);
		set_clock_sync(false);
		set_randomise(false);
		set_retrigger_port(Midi::UART);
		set_retrigger_channel(-1);
	}

	// speed
	float speed() {
		return speed_;
	}

	void set_speed(float value) {
		speed_ = SettingsUtils::clip_float(value);
	}

	const char *speed_text() {
		if (clock_sync()) {
			return MidiSync::tempo_text(speed() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			size_t index = speed() * PHASE_TABLE_SIZE;
			return SettingsText::floatToText(lut_phase_length[index], "HZ");
		}
	}

	float inc() {
		if (clock_sync()) {
			return MidiSync::read_inc(speed() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return lut_phase_inc[int(speed() * (PHASE_TABLE_SIZE - 1))];
		}
	}

	// shape
	float shape() {
		return shape_;
	}

	void set_shape(float value) {
		shape_ = SettingsUtils::clip_float(value);
	}

	const char *shape_text() {
		return SettingsText::floatToText(shape(), -100, 100);
	}

	// Skew
	float skew() {
		return skew_;
	}

	void set_skew(float value) {
		skew_ = SettingsUtils::clip_float(value);
	}

	const char *skew_text() {
		return SettingsText::floatToText(skew(), -100, 100);
	}

	// Min
	float min() {
		return min_;
	}

	void set_min(float value) {
		min_ = SettingsUtils::clip_float(value);
	}

	const char *min_text() {
		return SettingsText::floatToText(min(), 0, 100);
	}

	// Max
	float max() {
		return max_;
	}

	void set_max(float value) {
		max_ = SettingsUtils::clip_float(value);
	}

	const char *max_text() {
		return SettingsText::floatToText(max(), 0, 100);
	}

	// Sync phase
	float sync_phase() {
		return sync_phase_;
	}

	void set_sync_phase(float value) {
		sync_phase_ = SettingsUtils::clip_float(value);
	}

	const char *sync_phase_text() {
		return SettingsText::percentageToText(sync_phase() * 100, 100);
	}

	// clock sync
	bool clock_sync() {
		return clock_sync_;
	}

	void set_clock_sync(bool value) {
		clock_sync_ = value;
	}

	const char *clock_sync_text() {
		return SettingsText::boolToOnOff(clock_sync());
	}

	// Randomise
	bool randomise() {
		return randomise_;
	}

	void set_randomise(bool value) {
		randomise_ = value;
	}

	const char *randomise_text() {
		return SettingsText::boolToOnOff(randomise());
	}

	// Retrigger port
	uint8_t retrigger_port() {
		return retrigger_port_;
	}

	void set_retrigger_port(int value) {
		retrigger_port_ = SettingsUtils::clip(0, Midi::NUM_PORTS - 1, value);
	}

	const char *retrigger_port_text() {
		return Midi::portText(retrigger_port());
	}

	bool retrigger_port_accepted(int port) {
		return port == retrigger_port();
	}


	// Retrigger channel
	int retrigger_channel() {
		return retrigger_channel_;
	}

	void set_retrigger_channel(int value) {
		retrigger_channel_ = SettingsUtils::clip(-1, 16, value);
	}

	const char *retrigger_channel_text() {
		return SettingsText::midiChannelText(retrigger_channel());
	}

	bool retrigger_channel_accepted(int channel) {
		return (retrigger_channel() >= 16) || (retrigger_channel() == channel);
	}


	// Retrigger
	bool retrigger_accepted(MidiEngine::Event &e) {
		return retrigger_port_accepted(e.port) && retrigger_channel_accepted(e.message & 0x0F);
	}

	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(skew_);
		fileWriter.write(shape_);
		fileWriter.write(speed_);
		fileWriter.write(min_);
		fileWriter.write(max_);
		fileWriter.write(sync_phase_);
		fileWriter.write(randomise_);
		fileWriter.write(clock_sync_);
		fileWriter.write(retrigger_port_);
		fileWriter.write(retrigger_channel_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(skew_);
		fileReader.read(shape_);
		fileReader.read(speed_);
		fileReader.read(min_);
		fileReader.read(max_);
		fileReader.read(sync_phase_);
		fileReader.read(randomise_);
		fileReader.read(clock_sync_);
		fileReader.read(retrigger_port_);
		fileReader.read(retrigger_channel_);
	}

	void paste(Lfo *lfo) {
		skew_ = lfo->skew();
		shape_ = lfo->shape();
		speed_ = lfo->speed();
		min_ = lfo->min();
		max_ = lfo->max();
		sync_phase_ = lfo->sync_phase();
		randomise_ = lfo->randomise();
		clock_sync_ = lfo->clock_sync();
		retrigger_port_ = lfo->retrigger_port();
		retrigger_channel_ = lfo->retrigger_channel();
	}

private:
	float skew_;
	float shape_;
	float speed_;
	float min_;
	float max_;
	float sync_phase_;
	bool randomise_;
	bool clock_sync_;
	int retrigger_port_;
	int retrigger_channel_;
};

#endif
