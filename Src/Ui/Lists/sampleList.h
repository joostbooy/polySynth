#ifndef SampleList_h
#define SampleList_h

#include "settings.h"
#include "settingsText.h"

class SampleList : public SettingsList {

public:

	enum Item {
		SAMPLE,
		GAIN,
		PAN,
		START,
		END,
		LOOP,
		LOOP_START,
		LOOP_END,
		U_TURN,
		PLAY_MODE,
		CENTS,
		ROOT_NOTE,
		KEY_RANGE_LOW,
		KEY_RANGE_HIGH,

		NUM_ITEMS
	};


	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case SAMPLE:			return "SAMPLE";
		case GAIN:				return "GAIN";
		case PAN:				return "PAN";
		case START:				return "START";
		case END:				return "END";
		case LOOP:				return "LOOP";
		case LOOP_END:			return "LOOP END";
		case LOOP_START:		return "LOOP START";
		case U_TURN:			return "U TURN";
		case PLAY_MODE:			return "PLAY MODE";
		case CENTS:				return "CENTS";
		case ROOT_NOTE:			return "ROOT NOTE";
		case KEY_RANGE_LOW:		return "KEY RANGE LOW";
		case KEY_RANGE_HIGH:	return "KEY RANGE HIGH";
		default:
			break;
		}
		return nullptr;
	}

	const char* value_text(int item) override {
		Sample *sample = settings_->selected_sample();

		switch (item)
		{
		case SAMPLE:			return sample->name();
		case GAIN:				return sample->gain_text();
		case PAN:				return sample->pan_text();
		case START:				return sample->start_text();
		case END:				return sample->end_text();
		case LOOP:				return sample->loop_text();
		case LOOP_END:			return sample->loop_end_text();
		case LOOP_START:		return sample->loop_start_text();
		case U_TURN:			return sample->u_turn_text();
		case PLAY_MODE:			return sample->play_mode_text();
		case CENTS:				return sample->cents_text();
		case ROOT_NOTE:			return sample->root_note_text();
		case KEY_RANGE_LOW:		return sample->key_range_low_text();
		case KEY_RANGE_HIGH:	return sample->key_range_high_text();
		default:
			break;
		}
		return nullptr;
	}

	void edit(int item, int inc, bool shifted) override {
		Sample *sample = settings_->selected_sample();

		switch (item)
		{
		case SAMPLE:
			settings_->select_sample_index(settings_->selected_sample_index() + inc);
			break;
		case GAIN:
			sample->set_gain(sample->gain() + SettingsUtils::f_inc(inc, shifted));
			break;
		case PAN:
			sample->set_pan(sample->pan() + SettingsUtils::f_inc(inc, shifted));
			break;
		case START:
			sample->set_start(sample->start() + (inc * (shifted ? kQuarterSecond : kMillisecond)));
			break;
		case END:
			sample->set_end(sample->end() + (inc * (shifted ? kQuarterSecond : kMillisecond)));
			break;
		case LOOP:
			sample->set_loop(inc > 0);
			break;
		case LOOP_START:
			sample->set_loop_start(sample->loop_start() + (inc * (shifted ? kQuarterSecond : kMillisecond)));
			break;
		case LOOP_END:
			sample->set_loop_end(sample->loop_end() + (inc * (shifted ? kQuarterSecond : kMillisecond)));
			break;
		case U_TURN:
			sample->set_u_turn(inc > 0);
			break;
		case PLAY_MODE:
			sample->set_play_mode(sample->play_mode() + inc);
			break;
		case CENTS:
			sample->set_cents(sample->cents() + (inc * (shifted ? 10 : 1)));
			break;
		case ROOT_NOTE:
			sample->set_root_note(sample->root_note() + (inc * (shifted ? 12 : 1)));
			break;
		case KEY_RANGE_LOW:
			sample->set_key_range_low(sample->key_range_low() + (inc * (shifted ? 12 : 1)));
			break;
		case KEY_RANGE_HIGH:
			sample->set_key_range_high(sample->key_range_high() + (inc * (shifted ? 12 : 1)));
			break;
		default:
			break;
		}
	}

private:
	static const uint32_t kMillisecond = SAMPLE_RATE / 1000;
	static const uint32_t kQuarterSecond = SAMPLE_RATE / 4;
};

#endif
