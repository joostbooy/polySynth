#ifndef LfoList_h
#define LfoList_h

#include "settings.h"
#include "settingsText.h"

class LfoList : public SettingsList {

public:

	enum Item {
		Lfo,
		Speed,
		ClockSync,
		Shape,
		Skew,
		Min,
		Max,
		SyncPhase,
		Randomise,
		Retrigger,

		NumItems
	};

	const int numItems() override {
		return NumItems;
	}

	const char* itemText(int item) override {
		switch (item)
		{
		case Lfo:		return "LFO";
		case Speed:		return "SPEED";
		case ClockSync:	return "CLOCK SYNC";
		case Shape:		return "SHAPE";
		case Skew:		return "SKEW";
		case Min:		return "MIN";
		case Max:		return "MAX";
		case SyncPhase:	return "SYNC PHASE";
		case Randomise:	return "RANDOMISE";
		case Retrigger:	return "RETRIGGER";
		default:
			break;
		}
		return nullptr;
	}

	const char* valueText(int item) override {
		Lfo &lfo = settings_->selected_lfo();

		switch (item)
		{
		case Lfo:		return SettingsText::int_to_text(settings_->selected_lfo_index() + 1);
		case Speed:		return lfo.speedText();
		case ClockSync:	return lfo.clockSyncText();
		case Shape:		return lfo.shapeText();
		case Skew:		return lfo.skewText();
		case Min:		return lfo.minText();
		case Max:		return lfo.maxText();
		case SyncPhase:	return lfo.syncPhaseText();
		case Randomise:	return lfo.randomiseText();
		case Retrigger:	return lfo.retriggerText();
		default:
			break;
		}
		return nullptr;
	}

	void edit(int item, int inc, bool shifted) override {
		Lfo &lfo = settings_->selected_lfo();

		switch (item)
		{
		case Lfo:
			settings_->selectLfoIndex(settings_->selectedLfoIndex() + inc);
			break;
		case Speed:
			lfo.setSpeed(lfo.speed() + SettingsUtils::fInc(inc, shifted));
			break;
		case ClockSync:
			lfo.setClockSync(inc > 0);
			break;
		case Shape:
			lfo.setShape(lfo.shape() + SettingsUtils::fInc(inc, shifted));
			break;
		case Skew:
			lfo.setSkew(lfo.skew() + SettingsUtils::fInc(inc, shifted));
			break;
		case Min:
			lfo.setMin(lfo.min() + SettingsUtils::fInc(inc, shifted));
			break;
		case Max:
			lfo.setMax(lfo.max() + SettingsUtils::fInc(inc, shifted));
			break;
		case SyncPhase:
			lfo.setSyncPhase(lfo.syncPhase() + SettingsUtils::fInc(inc, shifted));
			break;
		case Randomise:
			lfo.setRandomise(inc > 0);
			break;
		case RETRIGGER:
			lfo.setRetrigger(inc > 0);
			break;
		default:
			break;
		}
	}

private:

};

#endif
