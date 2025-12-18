#ifndef EnvelopeList_h
#define EnvelopeList_h

#include "settings.h"
#include "settingsList.h"

class EnvelopeList : public SettingsList {

public:

	enum Item {
		ENVELOPE,
		MODE,
		CLOCK_SYNC,
		ATTACK_TIME,
		ATTACK_SHAPE,
		DECAY_TIME,
		DECAY_SHAPE,
		SUSTAIN_LEVEL,
		HOLD_TIME,
		RELEASE_TIME,
		RELEASE_SHAPE,

		NUM_ITEMS
	};


	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case ENVELOPE:		return "ENVELOPE";
		case MODE:			return "MODE";
		case CLOCK_SYNC:	return "CLOCK SYNC";
		case ATTACK_TIME:	return "ATTACK TIME";
		case ATTACK_SHAPE:	return "ATTACK SHAPE";
		case DECAY_TIME:	return "DECAY TIME";
		case DECAY_SHAPE:	return "DECAY SHAPE";
		case SUSTAIN_LEVEL:	return "SUSTAIN LEVEL";
		case HOLD_TIME:		return "HOLD TIME";
		case RELEASE_TIME:	return "RELEASE TIME";
		case RELEASE_SHAPE:	return "RELEASE SHAPE";
		default:
			break;
		}
		return nullptr;
	}

	const char* value_text(int item) override {
		Envelope &envelope = settings_->selected_envelope();

		switch (item)
		{
		case ENVELOPE:		return SettingsText::int_to_text(settings_->selected_envelope_index() + 1);
		case MODE:			return envelope.mode_text();
		case CLOCK_SYNC:	return envelope.clock_sync_text();
		case ATTACK_TIME:	return envelope.attack_time_text();
		case ATTACK_SHAPE:	return envelope.attack_shape_text();
		case DECAY_TIME:	return envelope.decay_time_text();
		case DECAY_SHAPE:	return envelope.decay_shape_text();
		case SUSTAIN_LEVEL:	return envelope.sustain_level_text();
		case HOLD_TIME:		return envelope.hold_time_text();
		case RELEASE_TIME:	return envelope.release_time_text();
		case RELEASE_SHAPE:	return envelope.release_shape_text();
		default:
			break;
		}
		return nullptr;
	}

	void edit(int item, int inc, bool shifted) override {
		Envelope &envelope = settings_->selected_envelope();

		switch (item)
		{
		case ENVELOPE:
			settings_->select_envelope_index(settings_->selected_envelope_index() + inc);
			break;
		case MODE:
			envelope.set_mode(envelope.mode() + inc);
			break;
		case CLOCK_SYNC:
			envelope.set_clock_sync(inc > 0);
			break;
		case ATTACK_TIME:
			envelope.set_attack_time(envelope.attack_time() + SettingsUtils::f_inc(inc, shifted));
			break;
		case ATTACK_SHAPE:
			envelope.set_attack_shape(envelope.attack_shape() + SettingsUtils::f_inc(inc, shifted));
			break;
		case DECAY_TIME:
			envelope.set_decay_time(envelope.decay_time() + SettingsUtils::f_inc(inc, shifted));
			break;
		case DECAY_SHAPE:
			envelope.set_decay_shape(envelope.decay_shape() + SettingsUtils::f_inc(inc, shifted));
			break;
		case SUSTAIN_LEVEL:
			envelope.set_sustain_level(envelope.sustain_level() + SettingsUtils::f_inc(inc, shifted));
			break;
		case HOLD_TIME:
			envelope.set_hold_time(envelope.hold_time() + SettingsUtils::f_inc(inc, shifted));
			break;
		case RELEASE_TIME:
			envelope.set_release_time(envelope.release_time() + SettingsUtils::f_inc(inc, shifted));
			break;
		case RELEASE_SHAPE:
			envelope.set_release_shape(envelope.release_shape() + SettingsUtils::f_inc(inc, shifted));
			break;
		default:
			break;
		}
	}

private:

};

#endif
