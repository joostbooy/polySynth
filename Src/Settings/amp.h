#if !defined(Amp_h)
#define Amp_h

#include "settingsText.h"
#include "settingsUtils.h"
#include "rng.h"

class Amp {
 public:
  void init() {
    setPan(0.5f);
    setPanSpread(0.f);
    setVcoMix(0.5f);
    setAmEnable(false);
    setRingModEnable(false);
    setCarrierSelect(0);
  }

  // Pan
  float pan() {
    return pan_;
  }

  void setPan(float value) {
    pan_ = SettingsUtils::clipFloat(value);
  }

  const char* panText() {
    return SettingsText::floatToText((pan() * 2.f) - 1.f);
  }

  // Pan spread
  float panSpread() {
    return panSpread_;
  }

  void setPanSpread(float value) {
    panSpread_ = SettingsUtils::clipFloat(value);
  }

  const char* panSpreadText() {
    return SettingsText::floatToText(panSpread());
  }

  // Drive
  float vcoMix() {
    return vcoMix_;
  }

  void setVcoMix(float value) {
    vcoMix_ = SettingsUtils::clipFloat(value);
  }

  const char* vcoMixText() {
    return SettingsText::floatToText(vcoMix());
  }

  // Enable AM
  bool amEnable() {
    return amEnable_;
  }

  void setAmEnable(bool value) {
    amEnable_ = value;
  }

  const char* enableAmText() {
    return SettingsText::boolToOnOff(amEnable());
  }

  // Ring mod enable
  bool ringModEnable() {
    return ringModEnable_;
  }

  void setRingModEnable(bool value) {
    ringModEnable_ = value;
  }

  const char* ringModEnableText() {
    return SettingsText::boolToOnOff(ringModEnable());
  }

  // Carrier select
  bool carrierSelect() {
    return carrierSelect_;
  }

  void setCarrierSelect(bool value) {
    carrierSelect_ = value;
  }

  const char* carrierSelectText() {
    return carrierSelect_ == 0 ? "VCO 1" : "VCO 2";
  }

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(pan_);
    fileWriter.write(panSpread_);
    fileWriter.write(vcoMix_);
    fileWriter.write(amEnable_);
    fileWriter.write(ringModEnable_);
    fileWriter.write(carrierSelect_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(pan_);
    fileReader.read(panSpread_);

    if (fileReader.version() >= 3) {
      fileReader.read(vcoMix_);
    } else {
      fileReader.skip(sizeof(float));
    }

    fileReader.read(amEnable_);

    if (fileReader.version() >= 3) {
      fileReader.read(ringModEnable_);
      fileReader.read(carrierSelect_);
    }
  }

  void paste(Amp* amp) {
    pan_ = amp->pan();
    panSpread_ = amp->panSpread();
    vcoMix_ = amp->vcoMix();
    amEnable_ = amp->amEnable();
    ringModEnable_ = amp->ringModEnable();
    carrierSelect_ = amp->carrierSelect();
  }

  void writeHash(Hash& hash) {
    hash.write(pan_);
    hash.write(panSpread_);
    hash.write(vcoMix_);
    hash.write(amEnable_);
    hash.write(ringModEnable_);
    hash.write(carrierSelect_);
  }

  void randomise() {
    pan_ = Rng::reciprocal();
    panSpread_ = Rng::reciprocal();
    vcoMix_ = Rng::reciprocal();
    amEnable_ = Rng::u16(0, 1);
    ringModEnable_ = Rng::u16(0, 1);
    carrierSelect_ = Rng::u16(0, 1);
  }

 private:
  float pan_;
  float panSpread_;
  float vcoMix_;
  bool amEnable_;
  bool ringModEnable_;
  bool carrierSelect_;
};

#endif  // Amp_h
