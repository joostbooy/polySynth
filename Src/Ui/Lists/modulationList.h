#ifndef ModulationList_h
#define ModulationList_h

#include "settings.h"
#include "settingsText.h"

class ModulationList : public SettingsList {

public:

	enum Item {
		MIDI_CC_A,
		MIDI_CC_B,
		MIDI_CC_C,
		MIDI_CC_D,
		GATE,
		GATE_PORT,
		GATE_CHANNEL,
		GATE_NOTE,
		GATE_VELOCITY,

		NUM_ITEMS
	};

	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case MIDI_CC_A:		return "";
		case MIDI_CC_B:		return "";
		case MIDI_CC_C:		return "";
		case MIDI_CC_D:		return "";
		case GATE:			return "GATE";
		case GATE_PORT:		return "GATE PORT";
		case GATE_CHANNEL:	return "GATE CHANNEL";
		case GATE_NOTE:		return "GATE NOTE";
		case GATE_VELOCITY:	return "GATE VELOCITY";
		default:
			break;
		}
		return nullptr;
	}

	const char* value_text(int item) override {
		Modulation &modulation = settings_->modulation();

		switch (item)
		{
		case MIDI_CC_A:		return modulation.midi_cc_number_text(0);
		case MIDI_CC_B:		return modulation.midi_cc_number_text(1);
		case MIDI_CC_C:		return modulation.midi_cc_number_text(2);
		case MIDI_CC_D:		return modulation.midi_cc_number_text(3);
		case GATE:			return SettingsText::int_to_text(selected_gate_ + 1);
		case GATE_PORT:		return modulation.gate_to_midi_port_text(selected_gate_);
		case GATE_CHANNEL:	return modulation.gate_to_midi_channel_text(selected_gate_);
		case GATE_NOTE:		return modulation.gate_to_midi_note_text(selected_gate_);
		case GATE_VELOCITY:	return modulation.gate_to_midi_velocity_text(selected_gate_);
			break;
		}
		return nullptr;
	}

	void edit(int item, int inc, bool shifted) override {
		Modulation &modulation = settings_->modulation();

		switch (item)
		{
		case MIDI_CC_A:
			modulation.set_midi_cc_number(0, modulation.midi_cc_number(0) + (inc * (shifted ? 10 : 1)));
			break;
		case MIDI_CC_B:
			modulation.set_midi_cc_number(1, modulation.midi_cc_number(1) + (inc * (shifted ? 10 : 1)));
			break;
		case MIDI_CC_C:
			modulation.set_midi_cc_number(2, modulation.midi_cc_number(2) + (inc * (shifted ? 10 : 1)));
			break;
		case MIDI_CC_D:
			modulation.set_midi_cc_number(3, modulation.midi_cc_number(3) + (inc * (shifted ? 10 : 1)));
			break;
		case GATE:
			selected_gate_ = SettingsUtils::clip(0, Modulation::kNumGatesToNote - 1, selected_gate_ + inc);
			break;
		case GATE_PORT:
			kill_midi(modulation, selected_gate_);
			modulation.set_gate_to_midi_port(selected_gate_, modulation.gate_to_midi_port(selected_gate_) + inc);
			break;
		case GATE_CHANNEL:
			kill_midi(modulation, selected_gate_);
			modulation.set_gate_to_midi_channel(selected_gate_, modulation.gate_to_midi_channel(selected_gate_) + inc);
			break;
		case GATE_NOTE:
			kill_midi(modulation, selected_gate_);
			modulation.set_gate_to_midi_note(selected_gate_, modulation.gate_to_midi_note(selected_gate_) + inc);
			break;
		case GATE_VELOCITY:
			modulation.set_gate_to_midi_velocity(selected_gate_, modulation.gate_to_midi_velocity(selected_gate_) + inc);
			break;
		default:
			break;
		}
	}

private:

	void kill_midi(Modulation &modulation, int gate) {
		if (engine_->last_gate(gate) > 0) {
			engine_->set_midi_channel_to_kill(modulation.gate_to_midi_port(gate), modulation.gate_to_midi_channel(gate));
			engine_->add_request_blocking(Engine::KILL_MIDI_CHANNEL);
		}
	}

	int selected_gate_ = 0;
};

#endif
