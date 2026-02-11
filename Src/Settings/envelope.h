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

	static const char* modeText(uint8_t value) {
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
		setClockSync(false);
		setMode(GATE);
		setAttackTime(0);
		setAttackShape(0.5f);
		setDecayTime(0);
		setDecayShape(0.5f);
		setHoldTime(0);
		setSustainLevel(0.5f);
		setReleaseTime(0);
		setReleaseShape(0.5f);
		setLoop(false);
		setInvert(false);
	}

	// Clock sync
	bool clockSync() {
		return clockSync_;
	}

	void setClockSync(bool value) {
		clockSync_ = value;
	}

	const char *clockSyncText() {
		return SettingsText::boolToOnOff(clockSync());
	}

	// Mode
	int mode() {
		return mode_;
	}

	void setMode(int value) {
		mode_ = SettingsUtils::clip(0, NUM_MODES - 1, value);
	}

	const char *modeText() {
		return modeText(mode());
	}

	// Attack time
	float attackTime() {
		return decayTime_;
	}

	void setAttackTime(float value) {
		decayTime_ = SettingsUtils::clipFloat(value);
	}

	const char *attackTimeText() {
		if (clockSync()) {
			return MidiSync::tempoText(attackTime() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return SettingsText::floatToText(attackTime(), 0, 100);
		}
	}

	float attackInc() {
		if (clockSync()) {
			return MidiSync::readInc(attackTime() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return lut_phase_inc[int(attackTime() * (PHASE_TABLE_SIZE - 1))];
		}
	}

	// Attack shape
	float attackShape() {
		return attackShape_;
	}

	void setAttackShape(float value) {
		attackShape_ = SettingsUtils::clipFloat(value);
	}

	const char *attackShapeText() {
		return SettingsText::floatToText(attackShape(), -100, 100);
	}

	// Decay time
	float decayTime() {
		return decayTime_;
	}

	void setDecayTime(float value) {
		decayTime_ = SettingsUtils::clipFloat(value);
	}

	const char *decayTimeText() {
		if (clockSync()) {
			return MidiSync::tempoText(decayTime() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return SettingsText::floatToText(decayTime(), 0, 100);
		}
	}

	float decayInc() {
		if (clockSync()) {
			return MidiSync::readInc(decayTime() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return lut_phase_inc[int(decayTime() * (PHASE_TABLE_SIZE - 1))];
		}
	}

	// Decay shape
	float decayShape() {
		return decayShape_;
	}

	void setDecayShape(float value) {
		decayShape_ = SettingsUtils::clipFloat(value);
	}

	const char *decayShapeText() {
		return SettingsText::floatToText(decayShape(), -100, 100);
	}

	// Hold time
	float holdTime() {
		return holdTime_;
	}

	void setHoldTime(float value) {
		if (mode() == TRIGGER) {
			holdTime_ = SettingsUtils::clipFloat(value);
		}
	}

	const char *holdTimeText() {
		if (mode() == GATE) {
			return "-";
		} else if (clockSync()) {
			return MidiSync::tempoText(holdTime() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			size_t index = holdTime() * PHASE_TABLE_SIZE;
			return SettingsText::floatToText(lut_phase_length[index], "HZ");
		}
	}

	float holdInc() {
		if (clockSync()) {
			return MidiSync::readInc(holdTime() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return lut_phase_inc[int(holdTime() * (PHASE_TABLE_SIZE - 1))];
		}
	}

	// Sustain level
	float sustainLevel() {
		return sustainLevel_;
	}

	void setSustainLevel(float value) {
		sustainLevel_ = SettingsUtils::clipFloat(value);
	}

	const char *sustainLevelText() {
		return SettingsText::floatToText(sustainLevel(), 0, 100);
	}

	// Release time
	float releaseTime() {
		return releaseTime_;
	}

	void setReleaseTime(float value) {
		releaseTime_ = SettingsUtils::clipFloat(value);
	}

	const char *releaseTimeText() {
		if (clockSync()) {
			return MidiSync::tempoText(releaseTime() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return SettingsText::floatToText(releaseTime(), 0, 100);
		}
	}

	float releaseInc() {
		if (clockSync()) {
			return MidiSync::readInc(releaseTime() * (MidiSync::NUM_TEMPOS - 1));
		} else {
			return lut_phase_inc[int(releaseTime() * (PHASE_TABLE_SIZE - 1))];
		}
	}

	// Release shape
	float releaseShape() {
		return releaseShape_;
	}

	void setReleaseShape(float value) {
		releaseShape_ = SettingsUtils::clipFloat(value);
	}

	const char *releaseShapeText() {
		return SettingsText::floatToText(releaseShape(), -100, 100);
	}

	// Loop 
	bool loop() {
		return loop_;
	}

	void setLoop(bool value) {
		loop_ = value;
	}

	const char *loopText() {
		return SettingsText::boolToOnOff(loop());
	}

	// Invert 
	bool invert() {
		return invert_;
	}

	void setInvert(bool value) {
		invert_ = value;
	}

	const char *invertText() {
		return SettingsText::boolToOnOff(invert());
	}

	// Storage
	void save(FileWriter &fileWriter) {
		fileWriter.write(clockSync_);
		fileWriter.write(mode_);
		fileWriter.write(decayTime_);
		fileWriter.write(attackShape_);
		fileWriter.write(decay_time_);
		fileWriter.write(decayShape_);
		fileWriter.write(holdTime_);
		fileWriter.write(sustainLevel_);
		fileWriter.write(releaseTime_);
		fileWriter.write(releaseShape_);
		fileWriter.write(loop_);
		fileWriter.write(invert_);
	}

	void load(FileReader &fileReader) {
		fileReader.read(clockSync_);
		fileReader.read(mode_);
		fileReader.read(decayTime_);
		fileReader.read(attackShape_);
		fileReader.read(decay_time_);
		fileReader.read(decayShape_);
		fileReader.read(holdTime_);
		fileReader.read(sustainLevel_);
		fileReader.read(releaseTime_);
		fileReader.read(releaseShape_);
		fileReader.read(loop_);
		fileReader.read(invert_);
	}

	void paste(Envelope *envelope) {
		clockSync_ = envelope->clockSync();
		mode_ = envelope->mode();
		decayTime_ = envelope->attackTime();
		attackShape_ = envelope->attackShape();
		decay_time_ = envelope->decayTime();
		decayShape_ = envelope->decayShape();
		holdTime_ = envelope->holdTime();
		sustainLevel_ = envelope->sustainLevel();
		releaseTime_ = envelope->releaseTime();
		releaseShape_ = envelope->releaseShape();
		loop_ = envelope->loop();
		invert_ = envelope->invert();
	}

private:
	int mode_;
	bool loop_;
	bool invert_;
	bool clockSync_;
	float decayTime_;
	float attackShape_;
	float decay_time_;
	float decayShape_;
	float holdTime_;
	float sustainLevel_;
	float releaseTime_;
	float releaseShape_;
};

#endif
