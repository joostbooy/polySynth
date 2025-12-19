#if !defined(Filter_h)
#define Filter_h

#include "gpio.h"

class Filter {
 public:
  void init() {
    //
  }

  Gpio::FilterType selectedType() {
    return selectedType_;
  }

  void setSelectedType(int value) {
    selectedType_ = Gpio::FilterType(SettingsUtils::clip(0, Gpio::NUM_FILTER_TYPES - 1, value));
  }

  Gpio::FilterRouting selectedRouting() {
    return selectedRouting_;
  }

  void setRouting(int value) {
    selectedRouting_ = Gpio::FilterRouting(SettingsUtils::clip(0, Gpio::NUM_FILTER_ROUTINGS - 1, value));
  }

  // Cutoff 1
  float cutoff1() {
    return cutoff1_;
  }

  void setCutoff1(float value) {
    cutoff1_ = value;
  }

  // Cutoff 2
  float cutoff2() {
    return cutoff2_;
  }

  void setCutoff2(float value) {
    cutoff2_ = value;
  }

  // Resonance 1
  float resonance1() {
    return resonace1_;
  }

  void setResonace1(float value) {
    resonace1_ = value;
  }

  // Resonance 2
  float resonance2() {
    return resonace2_;
  }
  void setResonace2(float value) {
    resonace2_ = value;
  }

 private:
  float cutoff1_;
  float cutoff2_;
  float resonace1_;
  float resonace2_;
  Gpio::FilterType selectedType_;
  Gpio::FilterRouting selectedRouting_;
};

#endif  // Filter_h
