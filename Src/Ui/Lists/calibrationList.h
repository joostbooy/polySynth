#ifndef CalibrationList_h
#define CalibrationList_h

#include "settings.h"
#include "settingsText.h"

class CalibrationList : public SettingsList {
 public:
  enum Item {
    MIN,
    MAX,

    NUM_ITEMS,
  };

  const int numItems() override {
    return NUM_ITEMS;
  }

  const char* itemText(int item) override {
    switch (item) {
      case MIN:
        return "MIN";
      case MAX:
        return "MAX";
      default:
        break;
    }
    return nullptr;
  }

  const char* valueText(int item) override {
    switch (item) {
      case MIN:
        return settings_->calibration().minText();
      case MAX:
        return settings_->calibration().maxText();
      default:
        break;
    }
    return nullptr;
  }

  void edit(int item, int inc, bool shifted) override {
    switch (item) {
      case MIN:
       settings_->calibration().setMin(settings_->calibration().min() + (inc * 10));
        break;
      case MAX:
       settings_->calibration().setMax(settings_->calibration().max() + (inc * 10));
        break;
      default:
        break;
    }
  }

 private:
};

#endif
