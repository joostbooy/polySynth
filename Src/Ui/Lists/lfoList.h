#ifndef LfoList_h
#define LfoList_h

#include "settings.h"
#include "settingsText.h"

class LfoList : public SettingsList {

public:

	enum Item {
		LFO,
		SPEED,
		CLOCK_SYNC,
		SHAPE,
		SKEW,
		MIN,
		MAX,
		SYNC_PHASE,
		RANDOMISE,
		RETRIGGER_PORT,
		RETRIGGER_CHANNEL,

		NUM_ITEMS
	};


	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case LFO:				return "LFO";
		case SPEED:				return "SPEED";
		case CLOCK_SYNC:		return "CLOCK SYNC";
		case SHAPE:				return "SHAPE";
		case SKEW:				return "SKEW";
		case MIN:				return "MIN";
		case MAX:				return "MAX";
		case SYNC_PHASE:		return "SYNC PHASE";
		case RANDOMISE:			return "RANDOMISE";
		case RETRIGGER_PORT:	return "RETRIGGER PORT";
		case RETRIGGER_CHANNEL:	return "RETRIGGER CHANNEL";
		default:
			break;
		}
		return nullptr;
	}

	const char* value_text(int item) override {
		Lfo &lfo = settings_->selected_lfo();

		switch (item)
		{
		case LFO:				return SettingsText::int_to_text(settings_->selected_lfo_index() + 1);
		case SPEED:				return lfo.speed_text();
		case CLOCK_SYNC:		return lfo.clock_sync_text();
		case SHAPE:				return lfo.shape_text();
		case SKEW:				return lfo.skew_text();
		case MIN:				return lfo.min_text();
		case MAX:				return lfo.max_text();
		case SYNC_PHASE:		return lfo.sync_phase_text();
		case RANDOMISE:			return lfo.randomise_text();
		case RETRIGGER_PORT:	return lfo.retrigger_port_text();
		case RETRIGGER_CHANNEL:	return lfo.retrigger_channel_text();
		default:
			break;
		}
		return nullptr;
	}

	void edit(int item, int inc, bool shifted) override {
		Lfo &lfo = settings_->selected_lfo();

		switch (item)
		{
		case LFO:
			settings_->select_lfo_index(settings_->selected_lfo_index() + inc);
			break;
		case SPEED:
			lfo.set_speed(lfo.speed() + SettingsUtils::f_inc(inc, shifted));
			break;
		case CLOCK_SYNC:
			lfo.set_clock_sync(inc > 0);
			break;
		case SHAPE:
			lfo.set_shape(lfo.shape() + SettingsUtils::f_inc(inc, shifted));
			break;
		case SKEW:
			lfo.set_skew(lfo.skew() + SettingsUtils::f_inc(inc, shifted));
			break;
		case MIN:
			lfo.set_min(lfo.min() + SettingsUtils::f_inc(inc, shifted));
			break;
		case MAX:
			lfo.set_max(lfo.max() + SettingsUtils::f_inc(inc, shifted));
			break;
		case SYNC_PHASE:
			lfo.set_sync_phase(lfo.sync_phase() + SettingsUtils::f_inc(inc, shifted));
			break;
		case RANDOMISE:
			lfo.set_randomise(inc > 0);
			break;
		case RETRIGGER_PORT:
			lfo.set_retrigger_port(lfo.retrigger_port() + inc);
			break;
		case RETRIGGER_CHANNEL:
			lfo.set_retrigger_channel(lfo.retrigger_channel() + inc);
			break;
		default:
			break;
		}
	}

private:

};

#endif
