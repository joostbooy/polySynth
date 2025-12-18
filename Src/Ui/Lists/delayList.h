#ifndef DelayList_h
#define DelayList_h

#include "settings.h"
#include "settingsText.h"

class DelayList : public SettingsList {

public:

	enum Item {
		AMOUNT,
		FEEDBACK,
		MIX,
		AUDIO_CHANNEL,
		SYNC,

		NUM_ITEMS
	};


	const int num_items() override {
		return NUM_ITEMS;
	}

	const char* item_text(int item) override {
		switch (item)
		{
		case AMOUNT:		return "AMOUNT";
		case FEEDBACK:		return "FEEDBACK";
		case MIX:			return "MIX";
		case AUDIO_CHANNEL:	return "AUDIO CHANNEL";
		case SYNC:			return "SYNC";
		default:
			break;
		}
		return nullptr;
	}

	const char* value_text(int item) override {
		Delay &delay = settings_->delay();

		switch (item)
		{
		case AMOUNT:		return delay.amount_text();
		case FEEDBACK:		return delay.feedback_text();
		case MIX:			return delay.mix_text();
		case AUDIO_CHANNEL:	return delay.audio_channel_text();
		case SYNC:			return delay.sync_text();
		default:
			break;
		}
		return nullptr;
	}

	void edit(int item, int inc, bool shifted) override {
		Delay &delay = settings_->delay();

		switch (item)
		{
		case AMOUNT:
			delay.set_amount(delay.amount() + SettingsUtils::f_inc(inc, shifted));
			break;
		case FEEDBACK:
			delay.set_feedback(delay.feedback() + SettingsUtils::f_inc(inc, shifted));
			break;
		case MIX:
			delay.set_mix(delay.mix() + SettingsUtils::f_inc(inc, shifted));
			break;
		case AUDIO_CHANNEL:
			delay.set_audio_channel(delay.audio_channel() +inc);
			break;
		case SYNC:
			delay.set_sync(inc > 0);
			break;
		default:
			break;
		}
	}

private:

};

#endif
