#ifndef MidiClockEngine_h
#define MidiClockEngine_h

#include "midi.h"
#include "lookupTables.h"

class MidiClockEngine {

public:

	void init(Midi *midi) {
		midi_ = midi;
		bpm_ = 120;
		ext_bpm_ = 120;
		reset();
	}

	void reset() {
		ticks = 0;
		isr_ticks = 0;
		isr_average = 0;
		tempo_phase = 0;
	}

	static uint16_t bpm() {
		return bpm_;
	}

	bool tick() {
		if (midi_->clock_source() == Midi::INTERNAL) {
			bpm_ = midi_->bpm();
		} else {
			bpm_ = ext_bpm_;
		}

		++isr_ticks;
		tempo_phase += lut_bpm_inc[bpm_ - MIN_BPM];
		return tempo_phase < tempo_inc;
	}

	void sync_bpm() {
		isr_average += isr_ticks;

		if (((++num_readings % 4) == 0) && isr_average > 0) {
			ext_bpm_ = uint32_t(45000000UL * 60UL / 24UL / (kUpdatePeriod * (isr_average / 4)));
			ext_bpm_ = SettingsUtils::clip(MIN_BPM, MAX_BPM, ext_bpm_);
			isr_average = 0;
		}

		isr_ticks = 0;
	}

private:
	Midi *midi_;
	uint8_t sync_port_;
	static uint16_t bpm_;
	static uint16_t bpm_fractional_;

	volatile uint16_t ext_bpm_;
	volatile uint32_t tempo_inc, tempo_phase, ticks, isr_ticks, isr_average;
	volatile uint8_t num_readings = 0;
	static const uint32_t kUpdatePeriod = 45000000UL / CLOCK_ISR_FREQ;
};

#endif
