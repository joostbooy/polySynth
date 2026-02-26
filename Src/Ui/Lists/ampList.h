#ifndef AmpList_h
#define AmpList_h

#include "settings.h"
#include "settingsText.h"

class AmpList : public SettingsList {
 public:
  enum Item {
    PAN,
    DRIVE,
    AM,

    NUM_ITEMS,
  };

  const int numItems() override {
    return NUM_ITEMS;
  }

  const char* itemText(int item) override {
    switch (item) {
      case PAN:   return "PAN";
      case DRIVE: return "DRIVE";
      case AM:    return "AM";
      default:
        break;
    }
    return nullptr;
  }

  const char* valueText(int item) override {
    Amp& amp = settings_->amp();

    switch (item) {
      case PAN:   return amp.panText();
      case DRIVE: return amp.driveText();
      case AM:    return amp.enableAmText();
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
      case DRIVE:
        amp.setDrive(amp.drive() + SettingsUtils::fInc(inc, shifted));
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
      case DRIVE:
        ui_->resetPot(Pots::DRIVE);
      default:
        break;
    }
  }
};

#endif
