#include "settings.h"

void Settings::save(int index) {
  eepromBusy_ = true;

  fileWriter_.start(index * kPatchStorageSize, current_version());
  patch_[index].save(fileWriter_);
  fileWriter_.stop();

  eepromBusy_ = false;
};

bool Settings::load() {
  bool error = false;
  eepromBusy_ = true;

  init();

  for (size_t i = 0; i < kNumPatches; i++) {
    fileReader_.start(i * kPatchStorageSize);
    patch_[i].load(fileReader_);
    fileReader_.stop();
    
    if (!fileReader_.readOk()) {
      patch_[i].init();
      error = true;
    }
  }

  eepromBusy_ = false;

  loadPatch(0);

  return error;
};

void Settings::saveCalibration() {
  eepromBusy_ = true;

  fileWriter_.start(kPatchStorageBlockSize, current_version());
  calibration_.save(fileWriter_);
  fileWriter_.stop();

  eepromBusy_ = false;
}

bool Settings::loadCalibration() {
  eepromBusy_ = true;

  fileReader_.start(kPatchStorageBlockSize);
  calibration_.load(fileReader_);
  fileReader_.stop();

  eepromBusy_ = false;

  if (fileReader_.readOk()) {
    return true;
  } else {
    calibration_.init();
    return false;
  }
}