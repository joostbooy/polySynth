#include "settings.h"

bool Settings::save() {
  eepromBusy_ = true;

  fileWriter.start(0, current_version());

  fileWriter.write(patchIndex_);

  for (size_t i = 0; i < kNumPatches; i++) {
    patch(i).save(fileWriter);
  }

  fileWriter.stop();

  eepromBusy_ = false;

  return fileWriter.writeOk();
};

bool Settings::load() {
  eepromBusy_ = true;

  init();

  fileReader.start(0);

  fileReader.read(patchIndex_);

  for (size_t i = 0; i < kNumPatches; i++) {
    patch(i).load(fileReader);
  }

  fileReader.stop();

  if (!fileReader.readOk()) {
    init();
    return false;
  }

  loadPatch(patchIndex_);

  eepromBusy_ = false;

  return true;
};

bool Settings::saveCalibration() {
  eepromBusy_ = true;

  fileWriter.start(64000 - sizeof(calibration_), current_version());
  calibration_.save(fileWriter);
  fileWriter.stop();

  eepromBusy_ = false;

  return fileWriter.writeOk();
}

bool Settings::loadCalibration() {
  eepromBusy_ = true;

  fileReader.start(64000 - sizeof(calibration_));
  calibration_.load(fileReader);
  fileReader.stop();

  if (fileReader.readOk()) {
    return true;
  } else {
    calibration_.init();
    return false;
  }

  eepromBusy_ = false;
}