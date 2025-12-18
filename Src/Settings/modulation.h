#ifndef Modulation_h
#define Modulation_h

#include "settingsText.h"
#include "settingsUtils.h"
#include "fileWriter.h"
#include "fileReader.h"
#include "midiEngine.h"

class Modulation {

public:

	static const size_t kNumUserCc = 4;
	static const size_t kNumGatesToNote = 4;

	void init() {
		for (size_t i = 0; i < kNumGatesToNote; ++i) {
			set_gate_to_midi_port(i, 0);
			set_gate_to_midi_channel(i, i);
			set_gate_to_midi_note(i, 60 + i);
			set_gate_to_midi_velocity(i, 100);
		}

		for (size_t i = 0; i < kNumUserCc; ++i) {
			set_midi_cc_number(i, i);
		}
	}

	// Midi CC
	int midi_cc_number(int index) {
		return midi_cc_number_[index];
	}

	void set_midi_cc_number(int index, int cc_number) {
		midi_cc_number_[index] = SettingsUtils::clip(0, 127, cc_number);
	}

	const char* midi_cc_number_text(int index) {
		return SettingsText::str.write("CC ", midi_cc_number(index));
	}

	// Gate to midi
	MidiEngine::Event &gate_to_midi(uint8_t gate) {
		return gate_to_midi_[gate];
	}

	// port
	uint8_t gate_to_midi_port(uint8_t gate) {
		return gate_to_midi_[gate].port;
	}

	void set_gate_to_midi_port(uint8_t gate, uint8_t port) {
		gate_to_midi_[gate].port = SettingsUtils::clip(0, Midi::NUM_PORTS - 1, port);
	}

	const char *gate_to_midi_port_text(uint8_t gate) {
		return Midi::port_text(gate_to_midi_port(gate));
	}


	// channel
	uint8_t gate_to_midi_channel(uint8_t gate) {
		return gate_to_midi_[gate].message;
	}

	void set_gate_to_midi_channel(uint8_t gate, uint8_t chn) {
		gate_to_midi_[gate].message = SettingsUtils::clip(0, 15, chn);
	}

	const char *gate_to_midi_channel_text(uint8_t gate) {
		return SettingsText::str.write("CHN ", gate_to_midi_channel(gate) + 1);
	}

	// note
	uint8_t gate_to_midi_note(uint8_t gate) {
		return gate_to_midi_[gate].data[0];
	}

	void set_gate_to_midi_note(uint8_t gate, uint8_t note) {
		gate_to_midi_[gate].data[0] = SettingsUtils::clip(0, 127, note);
	}

	const char *gate_to_midi_note_text(uint8_t gate) {
		return SettingsText::note_to_text(gate_to_midi_note(gate));
	}

	// velocity
	uint8_t gate_to_midi_velocity(uint8_t gate) {
		return gate_to_midi_[gate].data[1];
	}

	void set_gate_to_midi_velocity(uint8_t gate, uint8_t vel) {
		gate_to_midi_[gate].data[1] = SettingsUtils::clip(0, 127, vel);
	}

	const char *gate_to_midi_velocity_text(uint8_t gate) {
		return SettingsText::str.write("VEL ", gate_to_midi_velocity(gate));
	}


	// Storage
	void save(FileWriter &fileWriter) {
		for (size_t i = 0; i < kNumUserCc; ++i) {
			fileWriter.write(midi_cc_number_[i]);
		}

		for (size_t i = 0; i < kNumGatesToNote; ++i) {
			fileWriter.write(gate_to_midi_[i]);
		}
	}

	void load(FileReader &fileReader) {
		for (size_t i = 0; i < kNumUserCc; ++i) {
			fileReader.read(midi_cc_number_[i]);
		}

		for (size_t i = 0; i < kNumGatesToNote; ++i) {
			fileReader.read(gate_to_midi_[i]);
		}
	}

	void paste(Modulation *modulation) {
		for (size_t i = 0; i < kNumUserCc; ++i) {
			midi_cc_number_[i] = modulation->midi_cc_number(i);
		}

		for (size_t i = 0; i < kNumGatesToNote; ++i) {
			gate_to_midi_[i] = modulation->gate_to_midi(i);
		}
	}

private:
	uint8_t midi_cc_number_[kNumUserCc];
	MidiEngine::Event gate_to_midi_[kNumGatesToNote];
};

#endif
