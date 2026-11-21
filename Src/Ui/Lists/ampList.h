#ifndef AmpList_h
#define AmpList_h

#include "settings.h"
#include "settingsText.h"

class AmpList : public SettingsList {
 public:
  enum Item {
    PAN,
    PAN_SPREAD,
    VCO_MIX,
    AM,

    NUM_ITEMS,
  };

  const int numItems() override {
    return NUM_ITEMS;
  }

  const char* itemText(int item) override {
    switch (item) {
      case PAN:          return "PAN";
      case PAN_SPREAD:   return "PAN SPREAD";
      case VCO_MIX:      return "VCO MIX";
      case AM:           return "AM";
      default:
        break;
    }
    return nullptr;
  }

  const char* valueText(int item) override {
    Amp& amp = settings_->amp();

    switch (item) {
      case PAN:          return amp.panText();
      case PAN_SPREAD:   return amp.panSpreadText();
      case VCO_MIX:      return amp.vcoMixText();
      case AM:           return amp.enableAmText();
      default:
        break;
    }
    return nullptr;
  }

  void edit(int item, int inc, bool shifted) override {
    Amp& amp = settings_->amp();

    switch (item) {
      case PAN:
        amp.setPan(amp.pan() + SettingsUtils::fInc(inc, shifted));
        break;
      case PAN_SPREAD:
        amp.setPanSpread(amp.panSpread() + SettingsUtils::fInc(inc, shifted));
        break;
      case VCO_MIX:
        amp.setVcoMix(amp.vcoMix() + SettingsUtils::fInc(inc, shifted));
        break;
      case AM:
        amp.setAmEnable(inc > 0);
        break;
      default:
        break;
    }

    resetPot(item);
  }

 private:
  void resetPot(int item) {
    switch (item) {
      case PAN:
        ui_->resetPot(Pots::PAN);
        break;
      case VCO_MIX:
        ui_->resetPot(Pots::VCO_MIX);
      default:
        break;
    }
  }
};

#endif
