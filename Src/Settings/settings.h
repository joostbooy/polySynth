#ifndef Settings_h
#define Settings_h

#include "calibration.h"
#include "disk.h"
#include "fileReader.h"
#include "fileWriter.h"
#include "patch.h"
#include "settingsUtils.h"
#include "stringBuilder.h"

class Settings {
 public:
  static const size_t kNumVoices = 8;
  static const size_t kNumUserCc = 4;
  static const size_t kNumPatches = 128;
  static const size_t kNumLfos = 2;
  static const size_t kNumEnvelopes = 2;

  void init(Disk* disk) {
    disk_ = disk;

    lfoIndex_ = 0;
    patchIndex_ = 0;
    envelopeIndex_ = 0;

    path.clear();

    selectedPatch_.init();
    for (size_t i = 0; i < kNumPatches; i++) {
      patch(i).init();
    }
    loadPatch();

    if (!calibrationLoaded_) {
      calibrationLoaded_ = loadCalibration();
    }
  }

  void init() {
    if (disk_ != nullptr) {
      init(disk_);
    }
  }

  // save & load
  bool save();
  bool save(const char* new_path);
  bool load(const char* new_path);

  uint32_t current_version() {
    return 0;
  }

  // Patch
  Patch& patch(int index) {
    return patch_[index];
  }

  Patch& selectedPatch() {
    return selectedPatch_;
  }

  Patch& selectedPatchOrignalState() {
    return patch_[patchIndex_];
  }

  int patchIndex() {
    return patchIndex_;
  }

  void selectPatchIndex(int value) {
    patchIndex_ = SettingsUtils::clip(0, kNumPatches - 1, value);
  }

  void loadPatch() {
      selectedPatch_.paste(&patch_[patchIndex_]);
  }

  void savePatch() {
    patch_[patchIndex_].paste(&selectedPatch_);
   //save();
  }

  Disk* disk() {
    return disk_;
  }

  // Calibration
  Calibration& calibration() {
    return calibration_;
  }

  bool saveCalibration() {
    calibration_.save(fileWriter);
    return false;
  }

  bool loadCalibration() {
    calibration_.init();
    calibration_.load(fileReader);
    return false;
  }

  // oscilator
  Oscillator& oscillator() {
    return selectedPatch().oscillator();
  }

  // filter
  Filter& filter() {
    return selectedPatch().filter();
  }

  // midi
  Midi& midi() {
    return selectedPatch().midi();
  }

  // Envelope
  Envelope& envelope(int index) {
    return selectedPatch().envelope(index);
  }

  Envelope& selectedEnvelope() {
    return envelope(envelopeIndex_);
  }

  int envelopeIndex() {
    return envelopeIndex_;
  }

  void setEnvelopeIndex(int value) {
    envelopeIndex_ = SettingsUtils::clip(0, kNumEnvelopes - 1, value);
  }

  // Lfo
  Lfo& lfo(int index) {
    return selectedPatch().lfo(index);
  }

  int lfoIndex() {
    return lfoIndex_;
  }

  void selectLfoIndex(int value) {
    lfoIndex_ = SettingsUtils::clip(0, kNumLfos - 1, value);
  }

  Lfo& selectedLfo() {
    return lfo(lfoIndex_);
  }

  // mod matrix
  ModMatrix& modMatrix() {
    return selectedPatch().modMatrix();
  }

 private:
  StringBuilderBase<63> path;

  FileWriter fileWriter;
  FileReader fileReader;

  Disk* disk_ = nullptr;

  int lfoIndex_;
  int patchIndex_;
  int envelopeIndex_;

  bool calibrationLoaded_ = false;
  Calibration calibration_;

  Patch patch_[kNumPatches];
  Patch selectedPatch_;
};

#endif
