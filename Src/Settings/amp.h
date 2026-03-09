#if !defined(Amp_h)
#define Amp_h

#include "settingsText.h"
#include "settingsUtils.h"

class Amp {
 public:
  void init() {
    setPan(0.5f);
    setPanSpread(0.f);
    setDrive(0.f);
    setAmEnable(false);
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
  float drive() {
    return drive_;
  }

  void setDrive(float value) {
    drive_ = SettingsUtils::clipFloat(value);
  }

  const char* driveText() {
    return SettingsText::floatToText(pan());
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

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(pan_);
    fileWriter.write(panSpread_);
    fileWriter.write(drive_);
    fileWriter.write(amEnable_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(pan_);
    fileReader.read(panSpread_);
    fileReader.read(drive_);
    fileReader.read(amEnable_);
  }

  void paste(Amp* amp) {
    pan_ = amp->pan();
    panSpread_ = amp->panSpread();
    drive_ = amp->drive();
    amEnable_ = amp->amEnable();
  }

  void writeHash(Hash& hash) {
    hash.write(pan_);
    hash.write(panSpread_);
    hash.write(drive_);
    hash.write(amEnable_);
  }

 private:
  float pan_;
  float panSpread_;
  float drive_;
  bool amEnable_;
};

#endif  // Amp_h
