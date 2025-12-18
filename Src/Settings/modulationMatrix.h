#ifndef ModulationMatrix_h
#define ModulationMatrix_h

#include "modulation.h"

class ModulationMatrix {

public:

	enum Destination {
		PAN,
		BEND,
		GAIN,
		START,
		END,
		LOOP_START,
		LOOP_END,
		BIT_DEPTH,
		SAMPLE_RATE_DIVIDER,

		NUM_DESTINATIONS
	};

	static_assert(NUM_DESTINATIONS < 32, "Too many destinations !");

	static const char *destination_text(int value) {
		switch (value)
		{
		case PAN:					return "PAN";
		case BEND:					return "BEND";
		case GAIN:					return "GAIN";
		case START:					return "START";
		case END:					return "END";
		case LOOP_START:			return "LOOP START";
		case LOOP_END:				return "LOOP END";
		case BIT_DEPTH:				return "BIT DEPTH";
		case SAMPLE_RATE_DIVIDER:	return "SAMPLE RATE";
		default:
			break;
		}
		return nullptr;
	}

	enum Source {
		LFO_1,
		LFO_2,
		LFO_3,
		LFO_4,
		CV_1,
		CV_2,
		CV_3,
		CV_4,
		MIDI_BEND,
		MIDI_VELOCITY,
		MIDI_CC_A,
		MIDI_CC_B,
		MIDI_CC_C,
		MIDI_CC_D,
		ENVELOPE_1,
		ENVELOPE_2,

		NUM_SOURCES
	};

	static const char *source_text(int value) {
		switch (value)
		{
		case LFO_1:			return "LFO 1";
		case LFO_2:			return "LFO 2";
		case LFO_3:			return "LFO 3";
		case LFO_4:			return "LFO 4";
		case CV_1:			return "CV 1";
		case CV_2:			return "CV 2";
		case CV_3:			return "CV 3";
		case CV_4:			return "CV 4";
		case MIDI_BEND:		return "MIDI BEND";
		case MIDI_VELOCITY:	return "MIDI VELOCITY";
		case MIDI_CC_A:		return modulation_->midi_cc_number_text(0);
		case MIDI_CC_B:		return modulation_->midi_cc_number_text(1);
		case MIDI_CC_C:		return modulation_->midi_cc_number_text(2);
		case MIDI_CC_D:		return modulation_->midi_cc_number_text(3);
		case ENVELOPE_1:	return "ENVELOPE 1";
		case ENVELOPE_2:	return "ENVELOPE 2";
		default:
			break;
		}
		return nullptr;
	}

	static void init(Modulation *modulation) {
		modulation_ = modulation;
	}

	void init() {
		clear();
		toggle(MIDI_VELOCITY, GAIN);
		toggle(MIDI_BEND, BEND);
		toggle(ENVELOPE_1, GAIN);
	}

	bool read(size_t src, size_t dest) {
		return matrix_[src] & (1 << dest);
	}

	void clear() {
		for (size_t i = 0; i < NUM_SOURCES; ++i) {
			matrix_[i] = 0;
		}
	}

	void toggle(size_t src, size_t dest) {
		uint32_t data = matrix_[src];
		matrix_[src] = data ^ (1 << dest);
	}

	// Storage
	void save(FileWriter &fileWriter) {
		for (size_t i = 0; i < NUM_SOURCES; ++i) {
			fileWriter.write(matrix_[i]);
		}
	}

	void load(FileReader &fileReader) {
		for (size_t i = 0; i < NUM_SOURCES; ++i) {
			fileReader.read(matrix_[i]);
		}
	}

	void paste(ModulationMatrix *modulationMatrix) {
		clear();
		for (size_t y = 0; y < NUM_SOURCES; ++y) {
			for (size_t x = 0; x < NUM_DESTINATIONS; ++x) {
				if (modulationMatrix->read(x, y)) {
					toggle(x, y);
				}
			}
		}
	}

private:
	static Modulation *modulation_;
	uint32_t matrix_[NUM_SOURCES];
};

#endif
