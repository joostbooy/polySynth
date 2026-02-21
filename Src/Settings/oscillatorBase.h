#if !defined(OscillatorBase_h)
#define OscillatorBase_h

#include "settingsText.h"

class OscillatorBase {
 public:

  void init() {
    setMuteOsc(false);
    setShape(0.f);
    setTrackNote(true);
    setSlideAmmount(0.f);
    setSlideEnable(false);
    setOctaveOffset(0);
    setTune(0.5f);
    setTuneSemiToneRange(2);
  }

  // Mute Osc
  bool muteOsc() {
    return muteOsc_;
  }

  void setMuteOsc(bool value) {
    muteOsc_ = value;
  }

  const char* muteOscText() {
    return SettingsText::boolToOnOff(muteOsc());
  }

  // Shape 
  float shape() {
    return shape_;
  }

  void setShape(float value) {
    shape_ = SettingsUtils::clipFloat(value);
  }

  const char* shapeText() {
    return SettingsText::floatToText(shape());
  }

  // Track note Osc
  bool trackNote() {
    return trackNote_;
  }

  void setTrackNote(bool value) {
    trackNote_ = value;
  }

  const char* trackNoteText() {
    return SettingsText::boolToOnOff(trackNote());
  }

  // Slide enablbe
  bool slideEnable() {
    return slideEnable_;
  }

  void setSlideEnable(bool value) {
    slideEnable_ = value;
  }

  const char* slideEnableText() {
    return SettingsText::boolToOnOff(slideEnable());
  }

  // Slide ammount
  float slideAmmount() {
    return slideAmmount_;
  }

  void setSlideAmmount(float value) {
    slideAmmount_ = SettingsUtils::clipFloat(value);
  }

  const char* slideAmmountText() {
    return SettingsText::floatToText(slideAmmount());
  }

  float slideInc() {
    return lut_phase_inc[int((1.f - slideAmmount()) * (PHASE_TABLE_SIZE - 1))];
  }

  // Octave offset
  int octaveOffset() {
    return octaveOffset_;
  }

  void setOctaveOffset(int value) {
    octaveOffset_ = SettingsUtils::clip(-24, 24, value);
  }

  const char* octaveOffsetText() {
    return SettingsText::intToText(octaveOffset());
  }

    // Tune
  float tune1() {
    return tune_;
  }

  void setTune(float value) {
    tune_ = SettingsUtils::clipFloat(value);
  }

  const char* tuneText() {
    return SettingsText::floatToText(tune());
  }

  // Tune range
  int tuneSemiToneRange() {
    return tuneSemiToneRange_;
  }

  void setTuneSemiToneRange(int value) {
    tuneSemiToneRange_ = SettingsUtils::clip(1, 11, value);
  }

  const char* tuneSemiToneRangeText() {
    return SettingsText::intToText(tuneSemiToneRange());
  }

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(muteOsc_);
    fileWriter.write(shape_);
    fileWriter.write(trackNote_);
    fileWriter.write(slideAmmount_);
    fileWriter.write(slideEnable_);
    fileWriter.write(octaveOffset_);
    fileWriter.write(tune_);
    fileWriter.write(tuneSemiToneRange_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(muteOsc_);
    fileReader.read(shape_);
    fileReader.read(type1_);
    fileReader.read(trackNote_);
    fileReader.read(slideAmmount_);
    fileReader.read(slideEnable_);
    fileReader.read(octaveOffset_);
    fileReader.read(tune_);
    fileReader.read(tuneSemiToneRange_);
  }

  void paste(Oscillator* oscillator) {
    muteOsc_ = oscillator->muteOsc1();
    shape_ = oscillator->shape1();
    trackNote_ = oscillator->trackNote1();
    slideAmmount_ = oscillator->slideAmmount1();
    slideEnable_ = oscillator->slideEnable1();
    octaveOffset_ = oscillator->octaveOffset1();
    tune_ = oscillator->tune1();
    tuneSemiToneRange_ = oscillator->tuneSemiToneRange1();
  }

    void writeHash(Hash& hash) {
    hash.write(muteOsc_);
    hash.write(shape_);
    hash.write(trackNote_);
    hash.write(slideAmmount_);
    hash.write(slideEnable_);
    hash.write(octaveOffset_);
    hash.write(tune_);
    hash.write(tuneSemiToneRange_);
  }

 private:
  bool trackNote_;
  bool muteOsc_;
  float shape_;
  float modDepth_;
  bool modSource_;
  bool slideEnable_;
  bool slideEnable2_;
  bool linkSlideAmmount_;
  float slideAmmount_;
  float slideAmmount2_;
  int octaveOffset_;
  int octaveOffset2_;
  float tune_;
  float tune2_;
  int tuneSemiToneRange_;
  int tuneSemiToneRange2_;
  Type1 type1_;
  Type2 type2_;
  VoiceMode voiceMode_;
};

#endif  // Oscilator_h
