#include "settings.h"

void Settings::save() {
  eepromBusy_ = true;

  fileWriter_.start(0, current_version());
  fileWriter_.write(patchIndex_);

  for (size_t i = 0; i < kNumPatches; i++) {
    patch(i).save(fileWriter_);
  }

  fileWriter_.stop();

  eepromBusy_ = false;
};

bool Settings::load() {
  eepromBusy_ = true;

  init();
  fileReader_.start(0);
  fileReader_.read(patchIndex_);

  for (size_t i = 0; i < kNumPatches; i++) {
    patch(i).load(fileReader_);
  }

  fileReader_.stop();

  eepromBusy_ = false;

  if (!fileReader_.readOk()) {
    init();
    return false;
  }

  loadPatch(patchIndex_);

  return true;
};

void Settings::saveCalibration() {
  eepromBusy_ = true;

  fileWriter_.start(kPatchStorageBlock, current_version());
  calibration_.save(fileWriter_);
  fileWriter_.stop();

  eepromBusy_ = false;
}

bool Settings::loadCalibration() {
  eepromBusy_ = true;

  fileReader_.start(kPatchStorageBlock);
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