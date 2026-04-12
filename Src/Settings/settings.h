#ifndef Settings_h
#define Settings_h

#include "calibration.h"
#include "eeprom.h"
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

  void init(Eeprom* eeprom) {
    eeprom_ = eeprom;

    fileReader_.init(eeprom);
    fileWriter_.init(eeprom);

    lfoIndex_ = 0;
    patchIndex_ = 0;
    envelopeIndex_ = 0;

    selectedPatch_.init();
    for (size_t i = 0; i < kNumPatches; i++) {
      patch(i).init();
    }
    loadPatch(patchIndex_);

    if (!calibrationLoaded_) {
      calibrationLoaded_ = loadCalibration();
    }
  }

  void init() {
    if (eeprom_ != nullptr) {
      init(eeprom_);
    }
  }

  // save & load
  void save(int index);
  bool load();
  bool eepromBusy() {
    return eepromBusy_;
  }

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

  void loadPatch(int index) {
    patchIndex_ = SettingsUtils::clip(0, kNumPatches - 1, index);
    selectedPatch_.paste(&patch_[patchIndex_]);
  }

  void savePatch() {
    patch_[patchIndex_].paste(&selectedPatch_);
    save(patchIndex_);
  }

  bool patchHasUnsavedChanges() {
    return selectedPatch().readHash() != selectedPatchOrignalState().readHash();
  }

  // Calibration
  Calibration& calibration() {
    return calibration_;
  }

  void saveCalibration();
  bool loadCalibration();

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

  // midi clock
  MidiClock& midiClock() {
    return selectedPatch().midiClock();
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

  // Amp
  Amp& amp() {
    return selectedPatch().amp();
  }

 private:
  StringBuilderBase<63> path;

  FileWriter fileWriter_;
  FileReader fileReader_;

  Eeprom* eeprom_ = nullptr;

  int lfoIndex_;
  int patchIndex_;
  int envelopeIndex_;
  volatile bool eepromBusy_ = false;

  bool calibrationLoaded_ = false;
  Calibration calibration_;

  Patch patch_[kNumPatches];
  Patch selectedPatch_;

  static constexpr size_t kPatchStorageBlockSize = 64000 - 512;
  static constexpr size_t kPatchStorageSize = kPatchStorageBlockSize / kNumPatches;
  static_assert((sizeof(patch_[0])) < kPatchStorageSize, "Patch storage size exceeded!"); 
  static_assert(kPatchStorageBlockSize + (sizeof(calibration_)) < 64000, "EEPROM exceeded!");
};

#endif
