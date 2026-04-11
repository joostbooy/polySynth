#include "settings.h"

bool Settings::save() {
  eepromBusy_ = true;

  fileWriter_.start(0, current_version());

  fileWriter_.write(patchIndex_);

  for (size_t i = 0; i < kNumPatches; i++) {
    patch(i).save(fileWriter_);
  }

  fileWriter_.stop();

  eepromBusy_ = false;

  return fileWriter_.writeOk();
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

  if (!fileReader_.readOk()) {
    init();
    return false;
  }

  loadPatch(patchIndex_);

  eepromBusy_ = false;

  return true;
};

bool Settings::saveCalibration() {
  eepromBusy_ = true;

  fileWriter_.start(64000 - sizeof(calibration_), current_version());
  calibration_.save(fileWriter_);
  fileWriter_.stop();

  eepromBusy_ = false;

  return fileWriter_.writeOk();
}

bool Settings::loadCalibration() {
  eepromBusy_ = true;

  fileReader_.start(64000 - sizeof(calibration_));
  calibration_.load(fileReader_);
  fileReader_.stop();

  if (fileReader_.readOk()) {
    return true;
  } else {
    calibration_.init();
    return false;
  }

  eepromBusy_ = false;
}