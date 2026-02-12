#if !defined(Oscillator_h)
#define Oscillator_h

#include "settingsText.h"

class Oscillator {
 public:
  enum Type1 { SAW1, TRIANGLE1, SINE1, SQUARE1, NUM_TYPES1 };
  enum Type2 { SAW2, TRIANGLE2, NOISE2, SQUARE2, NUM_TYPES2 };

  void init() {
    setFmEnable(false);
    setSyncEnable(false);
    setType1(SAW1);
    setType2(SAW2);
    setMuteOsc1(false);
    setMuteOsc2(false);
    setShape1(0.f);
    setShape2(0.f);
    setTrackNote1(true);
    setTrackNote2(true);
    setModDepth(0.f);
    setModSource(0);
    setSlideAmmount1(0.f);
    setSlideAmmount2(0.f);
    setLinkSlideAmmount(true);
    setSlideEnable1(false);
    setSlideEnable2(false);
    setOctaveOffset1(0);
    setOctaveOffset2(0);
    setTune1(0.5f);
    setTune2(0.5f);
  }

  static const char* type1Text(Type1 type) {
    switch (type) {
      case SAW1:
        return "SAW";
      case TRIANGLE1:
        return "TRIANGLE";
      case SINE1:
        return "SINE";
      case SQUARE1:
        return "SQUARE";
      default:
        break;
    }
    return nullptr;
  }

  static const char* type2Text(Type2 type) {
    switch (type) {
      case SAW2:
        return "SAW";
      case TRIANGLE2:
        return "TRIANGLE";
      case NOISE2:
        return "NOISE";
      case SQUARE2:
        return "SQUARE";
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
    type1_ = Type1(value % NUM_TYPES1);
  }

  const char* type1Text() {
    return type1Text(type1());
  }

  // Type 2
  Type2 type2() {
    return type2_;
  }

  void setType2(int value) {
    type2_ = Type2(value % NUM_TYPES2);
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

  void setSyncEnable(bool value) {
    syncEnable_ = value;
  }

  const char* syncEnableText() {
    return SettingsText::boolToOnOff(syncEnable());
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

  // Track note Osc 1
  bool trackNote1() {
    return trackNote1_;
  }

  void setTrackNote1(bool value) {
    trackNote1_ = value;
  }

  const char* trackNote1Text() {
    return SettingsText::boolToOnOff(trackNote1());
  }

  // Track note Osc 2
  bool trackNote2() {
    return trackNote2_;
  }

  void setTrackNote2(bool value) {
    trackNote2_ = value;
  }

  const char* trackNote2Text() {
    return SettingsText::boolToOnOff(trackNote2());
  }

  // Mod depth
  float modDepth() {
    return modDepth_;
  }

  void setModDepth(float value) {
    modDepth_ = SettingsUtils::clipFloat(value);
  }

  const char* modDepthText() {
    return SettingsText::floatToText(modDepth());
  }

  // Mod source
  int modSource() {
    return modSource_;
  }

  void setModSource(int value) {
    modDepth_ = value % 2;
  }

  const char* modSourceText() {
    return SettingsText::str.write(modSource() == 0 ? "VCO 1" : "VCO 2");
  }

  // Slide enablbe 1
  bool slideEnable1() {
    return slideEnable1_;
  }

  void setSlideEnable1(bool value) {
    slideEnable1_ = value;
  }

  const char* slideEnable1Text() {
    return SettingsText::boolToOnOff(slideEnable1());
  }

  // Slide enablbe 2
  bool slideEnable2() {
    return slideEnable2_;
  }

  void setSlideEnable2(bool value) {
    slideEnable2_ = value;
  }

  const char* slideEnable2Text() {
    return SettingsText::boolToOnOff(slideEnable2());
  }

  // Link slide ammount
  bool linkSlideAmmount() {
    return linkSlideAmmount_;
  }

  void setLinkSlideAmmount(bool value) {
    linkSlideAmmount_ = value;
  }

  const char* linkSlideAmmountText() {
    return SettingsText::boolToOnOff(linkSlideAmmount());
  }

  // Slide ammount 1
  float slideAmmount1() {
    return slideAmmount1_;
  }

  void setSlideAmmount1(float value) {
    slideAmmount1_ = SettingsUtils::clipFloat(value);
  }

  const char* slideAmmount1Text() {
    return SettingsText::floatToText(slideAmmount1());
  }

  // Slide ammount 2
  float slideAmmount2() {
    return linkSlideAmmount() ? slideAmmount1_ : slideAmmount2_;
  }

  void setSlideAmmount2(float value) {
    slideAmmount2_ = SettingsUtils::clipFloat(value);
  }

  const char* slideAmmount2Text() {
    return SettingsText::floatToText(slideAmmount2());
  }

  // Octave offset 1
  int octaveOffset1() {
    return octaveOffset1_;
  }

  void setOctaveOffset1(int value) {
    octaveOffset1_ = SettingsUtils::clip(-24, 24, value);
  }

  const char* octaveOffset1Text() {
    return SettingsText::intToText(octaveOffset1());
  }

  // Octave offset 2
  int octaveOffset2() {
    return octaveOffset2_;
  }

  void setOctaveOffset2(int value) {
    octaveOffset2_ = SettingsUtils::clip(-24, 24, value);
  }

  const char* octaveOffset2Text() {
    return SettingsText::intToText(octaveOffset2());
  }

    // Tune 1
  float tune1() {
    return tune1_;
  }

  void setTune1(float value) {
    tune1_ = SettingsUtils::clipFloat(value);
  }

  const char* tune1Text() {
    return SettingsText::floatToText(tune1());
  }

    // Tune 2
  float tune2() {
    return tune2_;
  }

  void setTune2(float value) {
    tune2_ = SettingsUtils::clipFloat(value);
  }

  const char* tune2Text() {
    return SettingsText::floatToText(tune2());
  }


  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(fmEnable_);
    fileWriter.write(muteOsc1_);
    fileWriter.write(muteOsc2_);
    fileWriter.write(shape1_);
    fileWriter.write(shape2_);
    fileWriter.write(type1_);
    fileWriter.write(type2_);
    fileWriter.write(syncEnable_);
    fileWriter.write(trackNote1_);
    fileWriter.write(trackNote2_);
    fileWriter.write(modDepth_);
    fileWriter.write(modSource_);
    fileWriter.write(slideAmmount1_);
    fileWriter.write(slideAmmount2_);
    fileWriter.write(linkSlideAmmount_);
    fileWriter.write(slideEnable1_);
    fileWriter.write(slideEnable2_);
    fileWriter.write(octaveOffset1_);
    fileWriter.write(octaveOffset2_);
    fileWriter.write(tune1_);
    fileWriter.write(tune2_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(fmEnable_);
    fileReader.read(muteOsc1_);
    fileReader.read(muteOsc2_);
    fileReader.read(shape1_);
    fileReader.read(shape2_);
    fileReader.read(type1_);
    fileReader.read(type2_);
    fileReader.read(syncEnable_);
    fileReader.read(trackNote1_);
    fileReader.read(trackNote2_);
    fileReader.read(modDepth_);
    fileReader.read(modSource_);
    fileReader.read(slideAmmount1_);
    fileReader.read(slideAmmount2_);
    fileReader.read(linkSlideAmmount_);
    fileReader.read(slideEnable1_);
    fileReader.read(slideEnable2_);
    fileReader.read(octaveOffset1_);
    fileReader.read(octaveOffset2_);
    fileReader.read(tune1_);
    fileReader.read(tune2_);
  }

  void paste(Oscillator* oscillator) {
    fmEnable_ = oscillator->fmEnable();
    muteOsc1_ = oscillator->muteOsc1();
    muteOsc2_ = oscillator->muteOsc2();
    shape1_ = oscillator->shape1();
    shape2_ = oscillator->shape2();
    type1_ = oscillator->type1();
    type2_ = oscillator->type2();
    syncEnable_ = oscillator->syncEnable();
    trackNote1_ = oscillator->trackNote1();
    trackNote2_ = oscillator->trackNote2();
    modDepth_ = oscillator->modDepth();
    modSource_ = oscillator->modSource();
    slideAmmount1_ = oscillator->slideAmmount1();
    slideAmmount2_ = oscillator->slideAmmount2();
    linkSlideAmmount_ = oscillator->linkSlideAmmount();
    slideEnable1_ = oscillator->slideEnable1();
    slideEnable2_ = oscillator->slideEnable2();
    octaveOffset1_ = oscillator->octaveOffset1();
    octaveOffset2_ = oscillator->octaveOffset2();
    tune1_ = oscillator->tune1();
    tune2_ = oscillator->tune2();
  }

 private:
  bool trackNote1_;
  bool trackNote2_;
  bool syncEnable_;
  bool fmEnable_;
  bool muteOsc1_;
  bool muteOsc2_;
  float shape1_;
  float shape2_;
  float modDepth_;
  bool modSource_;
  bool slideEnable1_;
  bool slideEnable2_;
  bool linkSlideAmmount_;
  float slideAmmount1_;
  float slideAmmount2_;
  int octaveOffset1_;
  int octaveOffset2_;
  float tune1_;
  float tune2_;
  Type1 type1_;
  Type2 type2_;
};

#endif  // Oscilator_h
