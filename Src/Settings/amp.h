#if !defined(Amp_h)
#define Amp_h

#include "settingsText.h"
#include "settingsUtils.h"

class Amp {
 public:
  void init() {
    setPan(0.5f);
    setDrive(0.f);
  }

  // Pan
  float pan() {
    return pan_;
  }

  void setPan(float value) {
    pan_ = SettingsUtils::clipFloat(value);
  }

  const char* panText() {
    return SettingsText::floatToText(pan(), -100, 100);
  }

    // Drive
  float drive() {
    return drive_;
  }

  void setDrive(float value) {
    drive_ = SettingsUtils::clipFloat(value);
  }

  const char* driveText() {
    return SettingsText::floatToText(pan(), 0, 100);
  }

   // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(pan_);
    fileWriter.write(drive_);
  }

  void load(FileReader& fileReader) {
    fileReader.read(pan_);
    fileReader.read(drive_);
  }

  void paste(Amp* amp) {
    pan_ = amp->pan();
    drive_ = amp->drive();
  }

 private:
  float pan_;
  float drive_;
};

#endif  // Amp_h
