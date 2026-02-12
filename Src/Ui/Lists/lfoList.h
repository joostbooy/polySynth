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
		TYPE,
		SKEW,
		MIN,
		MAX,
		SYNC_PHASE,
		RANDOMISE,
		RETRIGGER,
		ONE_SHOT,

		NumItems
	};

	const int numItems() override {
		return NumItems;
	}

	const char* itemText(int item) override {
		switch (item)
		{
		case LFO:			return "LFO";
		case SPEED:			return "SPEED";
		case CLOCK_SYNC:	return "CLOCK SYNC";
		case TYPE:			return "SHAPE";
		case SKEW:			return "SKEW";
		case MIN:			return "MIN";
		case MAX:			return "MAX";
		case SYNC_PHASE:	return "SYNC PHASE";
		case RANDOMISE:		return "RANDOMISE";
		case RETRIGGER:		return "RETRIGGER";
		case ONE_SHOT:		return "ONE SHOT";
		default:
			break;
		}
		return nullptr;
	}

	const char* valueText(int item) override {
		Lfo &lfo = settings_->selectedLfo();

		switch (item)
		{
		case LFO:			return SettingsText::intToText(settings_->lfoIndex() + 1);
		case SPEED:			return lfo.speedText();
		case CLOCK_SYNC:	return lfo.clockSyncText();
		case TYPE:			return lfo.typeText();
		case SKEW:			return lfo.skewText();
		case MIN:			return lfo.minText();
		case MAX:			return lfo.maxText();
		case SYNC_PHASE:	return lfo.syncPhaseText();
		case RANDOMISE:		return lfo.randomiseText();
		case RETRIGGER:		return lfo.retriggerText();
		case ONE_SHOT:		return lfo.oneShotText();
		default:
			break;
		}
		return nullptr;
	}

	void edit(int item, int inc, bool shifted) override {
		Lfo &lfo = settings_->selectedLfo();

		switch (item)
		{
		case LFO:
			settings_->selectLfoIndex(settings_->lfoIndex() + inc);
			break;
		case SPEED:
			lfo.setSpeed(lfo.speed() + SettingsUtils::fInc(inc, shifted));
			break;
		case CLOCK_SYNC:
			lfo.setClockSync(inc > 0);
			break;
		case TYPE:
			lfo.setType(lfo.type() + inc);
			break;
		case SKEW:
			lfo.setSkew(lfo.skew() + SettingsUtils::fInc(inc, shifted));
			break;
		case MIN:
			lfo.setMin(lfo.min() + SettingsUtils::fInc(inc, shifted));
			break;
		case MAX:
			lfo.setMax(lfo.max() + SettingsUtils::fInc(inc, shifted));
			break;
		case SYNC_PHASE:
			lfo.setSyncPhase(lfo.syncPhase() + SettingsUtils::fInc(inc, shifted));
			break;
		case RANDOMISE:
			lfo.setRandomise(inc > 0);
			break;
		case RETRIGGER:
			lfo.setRetrigger(inc > 0);
			break;
		case ONE_SHOT:
			lfo.setOneShot(inc > 0);
			break;
		default:
			break;
		}
	}

private:

};

#endif
