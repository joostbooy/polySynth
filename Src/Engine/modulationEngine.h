#ifndef ModulationEngine_h
#define ModulationEngine_h

#include "dac.h"
#include "lfoEngine.h"
#include "modulation.h"
#include "modulationMatrix.h"
#include "settings.h"

class ModulationEngine {

public:

	struct Frame {
		float data[ModulationMatrix::NUM_DESTINATIONS];
	};

	LfoEngine &lfoEngine(int index) {
		return lfoEngine_[index];
	}

	void init(Settings *settings) {
		modulation_ = &settings->modulation();

		for (size_t i = 0; i < Settings::kNumLfos; ++i) {
			lfoEngine_[i].init(&settings->lfo(i));
		}
	}

	void set_envelope(int index, float value) {
		source_[ModulationMatrix::ENVELOPE_1 + index] = value;
	}

	void set_cv(int channel, float value) {
		source_[ModulationMatrix::CV_1 + channel] = value;
	}

	void set_midi_velocity(float value) {
		source_[ModulationMatrix::MIDI_VELOCITY] = value;
	}

	void set_midi_bend(float value) {
		source_[ModulationMatrix::MIDI_BEND] = value;
	}

	void reset_lfos() {
		for (size_t i = 0; i < Settings::kNumLfos; ++i) {
			lfoEngine_[i].reset();
		}
	}

	void retrigger_lfos(MidiEngine::Event &e) {
		for (size_t i = 0; i < Settings::kNumLfos; ++i) {
			lfoEngine_[i].retrigger(e);
		}
	}

	void tick_lfos() {
		for (size_t i = 0; i < Settings::kNumLfos; ++i) {
			source_[i + ModulationMatrix::LFO_1] = lfoEngine_[i].next();
		}
	}

	void set_midi_cc(uint8_t number, float value) {
		for (size_t i = 0; i < Modulation::kNumUserCc; ++i) {
			if (number == modulation_->midi_cc_number(i)) {
				source_[i + ModulationMatrix::MIDI_CC_A] = value;
			}
		}
	}

	Frame* process(ModulationMatrix *matrix) {
		for (int y = 0; y < ModulationMatrix::NUM_DESTINATIONS; ++y) {
			frame_.data[y] = 1.f;
			for (int x = 0; x < ModulationMatrix::NUM_SOURCES; ++x) {
				if (matrix->read(x, y)) {
					frame_.data[y] *= source_[x];
				}
			}
		}

		return &frame_;
	}

	//	Frame* process(ModulationMatrix *matrix) {
	//		std::fill(&frame_.data[0], &frame_.data[ModulationMatrix::NUM_DESTINATIONS], 1.0f);

	//		for (int x = 0; x < ModulationMatrix::NUM_SOURCES; ++x) {
	//			for (int y = 0; y < ModulationMatrix::NUM_DESTINATIONS; ++y) {
	//				if (matrix->read(x, y)) {
	//					frame_.data[y] *= source_[x];
	//				}
	//			}
	//		}

	//		return &frame_;
	//	}

private:
	Frame frame_;
	Modulation *modulation_;
	float source_[ModulationMatrix::NUM_SOURCES];
	LfoEngine lfoEngine_[Settings::kNumLfos];
};

#endif
