#ifndef FilterList_h
#define FilterList_h

#include "settings.h"
#include "settingsText.h"

class FilterList : public SettingsList {
 public:
  enum Item {
    TYPE,
    CUTOFF_1,
    RESONANCE_1,
    FM_ENABLE_1,
    CUTOFF_2,
    RESONANCE_2,
    FM_ENABLE_2,
    ROUTING,

    NUM_ITEMS,
  };

  const int numItems() override {
    return NUM_ITEMS;
  }

  const char* itemText(int item) override {
    switch (item) {
      case TYPE:        return "TYPE";
      case CUTOFF_1:    return "CUTOFF 1";
      case RESONANCE_1: return "RESONANCE 1";
      case FM_ENABLE_1: return "FM 1";
      case CUTOFF_2:    return "CUTOFF 2";
      case RESONANCE_2: return "RESONANCE 2";
      case FM_ENABLE_2: return "FM 2";
      case ROUTING:     return "ROUTING";
      default:
        break;
    }
    return nullptr;
  }

  const char* valueText(int item) override {
    Filter& filter = settings_->filter();

    switch (item) {
      case TYPE:        return filter.typeText();
      case CUTOFF_1:    return filter.cutoff1Text();
      case RESONANCE_1: return filter.resonance1Text();
      case FM_ENABLE_1: return filter.fmEnable1Text();
      case CUTOFF_2:    return filter.cutoff2Text();
      case RESONANCE_2: return filter.resonance2Text();
      case FM_ENABLE_2: return filter.fmEnable2Text();
      case ROUTING:     return filter.routingText();
      default:
        break;
    }
    return nullptr;
  }

  void edit(int item, int inc, bool shifted) override {
    Filter& filter = settings_->filter();

    switch (item) {
      case TYPE:
        filter.setType(filter.type() + inc);
        break;
      case CUTOFF_1:
        filter.setCutoff1(filter.cutoff1() + SettingsUtils::fInc(inc, shifted));
        break;
      case RESONANCE_1:
        filter.setResonace1(filter.resonance1() + SettingsUtils::fInc(inc, shifted));
        break;
      case FM_ENABLE_1:
        filter.setFmEnable1(inc > 0);
        break;
      case CUTOFF_2:
        filter.setCutoff2(filter.cutoff2() + SettingsUtils::fInc(inc, shifted));
        break;
      case RESONANCE_2:
        filter.setResonace2(filter.resonance2() + SettingsUtils::fInc(inc, shifted));
        break;
      case FM_ENABLE_2:
        filter.setFmEnable2(inc > 0);
        break;
      case ROUTING:
        filter.setRouting(filter.routing() + inc);
        break;
      default:
        break;
    }

    resetPot(item);
  }

 private:
  void resetPot(int item) {
    switch (item) {
      case CUTOFF_1:
        ui_->resetPot(Pots::CUTOFF_A);
        break;
      case RESONANCE_1:
        ui_->resetPot(Pots::RESONANCE_A);
        break;
      case CUTOFF_2:
        ui_->resetPot(Pots::CUTOFF_B);
        break;
      case RESONANCE_2:
        ui_->resetPot(Pots::RESONANCE_B);
        break;
      default:
        break;
    }
  }
};

#endif
