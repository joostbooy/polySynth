#ifndef MidiClockEngine_h
#define MidiClockEngine_h

#include "midi.h"
#include "lookupTables.h"

class MidiClockEngine {

public:

	void init(Midi *midi) {
		midi_ = midi;
		bpm_ = 120;
		extBpm_ = 120;
		reset();
	}

	void reset() {
		ticks = 0;
		isrTicks_ = 0;
		isrAverage_ = 0;
		tempoPhase_ = 0;
	}

	static uint16_t bpm() {
		return bpm_;
	}

	bool tick() {
		if (midi_->clockSource() == Midi::INTERNAL) {
			bpm_ = midi_->bpm();
		} else {
			bpm_ = extBpm_;
		}

		++isrTicks_;
		tempoPhase_ += lut_bpm_inc[bpm_ - MIN_BPM];
		return tempoPhase_ < tempoInc_;
	}

	void syncBpm() {
		isrAverage_ += isrTicks_;

		if (((++numReadings_ % 4) == 0) && isrAverage_ > 0) {
			extBpm_ = uint32_t(45000000UL * 60UL / 24UL / (kUpdatePeriod * (isrAverage_ / 4)));
			extBpm_ = SettingsUtils::clip(MIN_BPM, MAX_BPM, extBpm_);
			isrAverage_ = 0;
		}

		isrTicks_ = 0;
	}

private:
	Midi *midi_;
	uint8_t syncPort_;
	static inline uint16_t bpm_;
	static inline uint16_t bpmFractional_;

	volatile uint16_t extBpm_;
	volatile uint32_t tempoInc_, tempoPhase_, ticks, isrTicks_, isrAverage_;
	volatile uint8_t numReadings_ = 0;
	static const uint32_t kUpdatePeriod = 45000000UL / UPDATE_FREQ;
};

#endif
