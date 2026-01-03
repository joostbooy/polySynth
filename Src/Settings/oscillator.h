#if !defined(Oscillator_h)
#define Oscillator_h

#include "settingsText.h"

class Oscillator {
 public:
  enum Type1 { SAW1, TRIANGLE1, SINE1, SQUARE1, NUM_TYPES1 };
  enum Type2 { SAW2, TRIANGLE2, NOISE2, SQUARE2, NUM_TYPES2 };

  void init() {
    setAmEnable(false);
    setFmEnable(false);
    setSyncEnable_(false);
    setType1(SAW1);
    setType2(SAW2);
    setMuteOsc1(false);
    setMuteOsc2(false);
    setShape1(0.f);
    setShape2(0.f);
  }

  static const char* type1Text(Type1 type) {
    switch (type) {
      case SAW1:      return "SAW";
      case TRIANGLE1: return "TRIANGLE";
      case SINE1:     return "SINE";
      case SQUARE1:   return "SQUARE";
      default:
        break;
    }
    return nullptr;
  }

  static const char* type2Text(Type2 type) {
    switch (type) {
      case SAW2:      return "SAW";
      case TRIANGLE2: return "TRIANGLE";
      case NOISE2:    return "NOISE";
      case SQUARE2:   return "SQUARE";
      default:
        break;
    }
    return nullptr;
  }

  // Type 1
  Type1 type1() {
    return type1_;
  }

  void setType1(int value) {
    type1_ = Type1(SettingsUtils::clip(0, NUM_TYPES1 - 1, value));
  }

  const char* type1Text() {
    return type1Text(type1());
  }

  // Type 2
  Type2 type2() {
    return type2_;
  }

  void setType2(int value) {
    type2_ = Type2(SettingsUtils::clip(0, NUM_TYPES2 - 1, value));
  }

  const char* type2Text() {
    return type2Text(type2());
  }

  // FM enable
  bool fmEnable() {
    return fmEnable_;
  }

  void setFmEnable(bool value) {
    fmEnable_ = value;
  }

  const char* fmEnableText() {
    return SettingsText::boolToOnOff(fmEnable());
  }

  // sync enable
  bool syncEnable() {
    return syncEnable_;
  }

  void setSyncEnable_(bool value) {
    syncEnable_ = value;
  }

  const char* syncEnableText() {
    return SettingsText::boolToOnOff(syncEnable());
  }

  // AM enable
  bool amEnable() {
    return amEnable_;
  }

  void setAmEnable(bool value) {
    amEnable_ = value;
  }

  const char* amEnableText() {
    return SettingsText::boolToOnOff(amEnable());
  }

  // Mute Osc 1
  bool muteOsc1() {
    return muteOsc1_;
  }

  void setMuteOsc1(bool value) {
    muteOsc1_ = value;
  }

  const char* muteOsc1Text() {
    return SettingsText::boolToOnOff(muteOsc1());
  }

  // Mute Osc 2
  bool muteOsc2() {
    return muteOsc2_;
  }

  void setMuteOsc2(bool value) {
    muteOsc2_ = value;
  }

  const char* muteOsc2Text() {
    return SettingsText::boolToOnOff(muteOsc2());
  }

  // Shape 1
  float shape1() {
    return shape1_;
  }

  void setShape1(float value) {
    shape1_ = SettingsUtils::clipFloat(value);
  }

  const char* shape1Text() {
    return SettingsText::floatToText(shape1());
  }

  // Shape 2
  float shape2() {
    return shape2_;
  }

  void setShape2(float value) {
    shape2_ = SettingsUtils::clipFloat(value);
  }

  const char* shape2Text() {
    return SettingsText::floatToText(shape2());
  }

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(fmEnable_);
    fileWriter.write(amEnable_);
    fileWriter.write(muteOsc1_);
    fileWriter.write(muteOsc2_);
    fileWriter.write(shape1_);
    fileWriter.write(shape2_);
    fileWriter.write(type1_);
    fileWriter.write(type2_);
    fileWriter.write(syncEnable_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(fmEnable_);
    fileReader.read(amEnable_);
    fileReader.read(muteOsc1_);
    fileReader.read(muteOsc2_);
    fileReader.read(shape1_);
    fileReader.read(shape2_);
    fileReader.read(type1_);
    fileReader.read(type2_);
    fileReader.read(syncEnable_);
  }

  void paste(Oscillator* oscillator) {
    fmEnable_ = oscillator->fmEnable();
    amEnable_ = oscillator->amEnable();
    muteOsc1_ = oscillator->muteOsc1();
    muteOsc2_ = oscillator->muteOsc2();
    shape1_ = oscillator->shape1();
    shape2_ = oscillator->shape2();
    type1_ = oscillator->type1();
    type2_ = oscillator->type2();
    syncEnable_ = oscillator->syncEnable();
  }

 private:
  bool syncEnable_;
  bool fmEnable_;
  bool amEnable_;
  bool muteOsc1_;
  bool muteOsc2_;
  float shape1_;
  float shape2_;
  Type1 type1_;
  Type2 type2_;
};

#endif  // Oscilator_h
