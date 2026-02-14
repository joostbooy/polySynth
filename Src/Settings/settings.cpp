#include "settings.h"

#include "diskUtils.h"

bool Settings::save() {
  fileWriter.start(&disk_->file(), "0:/PATCHES.SET", current_version());

  fileWriter.write(patchIndex_);

  for (size_t i = 0; i < kNumPatches; i++) {
    patch(i).save(fileWriter);
  }

  fileWriter.stop();

  return fileWriter.writeOk();
};

bool Settings::load() {
  init();

  fileReader.start(&disk_->file(), "0:/PATCHES.SET");

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
  return true;
};

bool Settings::saveCalibration() {
  fileWriter.start(&disk_->file(), "0:/CALIBRATION.SET", current_version());
  calibration_.save(fileWriter);
  fileWriter.stop();

  return fileWriter.writeOk();
}

bool Settings::loadCalibration() {
  fileReader.start(&disk_->file(), "0:/CALIBRATION.SET");
  calibration_.load(fileReader);
  fileReader.stop();

  if (fileReader.readOk()) {
    calibrationLoaded_ = true;
  } else {
    calibration_.init();
    calibrationLoaded_ = false;
  }

  return calibrationLoaded_;
}