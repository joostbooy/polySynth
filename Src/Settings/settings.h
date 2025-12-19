#ifndef Settings_h
#define Settings_h

#include "disk.h"
#include "envelope.h"
#include "fileReader.h"
#include "fileWriter.h"
#include "lfo.h"
#include "midi.h"
#include "modMatrix.h"
#include "pots.h"
#include "settingsUtils.h"
#include "stringBuilder.h"

class Settings {
  class Patch;

 public:
  static const size_t kNumVoices = 8;
  static const size_t kNumLfos = 2;
  static const size_t kNumUserCc = 4;
  static const size_t kNumPatches = 255;

  void init(Disk* disk) {
    disk_ = disk;

    selected_lfo_index_ = 0;
    selectedPatchIndex_ = 0;

    path.clear();

    midi().init();
    modMatrix().init();
    ampEnvelope().init();
    modEnvelope().init();

    for (size_t i = 0; i < kNumLfos; i++) {
      lfo(i).init();
    }

    set_project_name("NEW.PRJ");
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

  bool has_valid_path() {
    char file_name[max_name_length()];

    if (path.length() > 0) {
      StringUtils::get_file_name_from_path(const_cast<char*>(path.read()), file_name);
      return StringUtils::text_is_equal(file_name, project_name_);
    }
    return false;
  }

  uint32_t current_version() {
    return 0;
  }

  Patch& selectedePatch() {
    return patch_[selectedPatchIndex_];
  }

  Disk* disk() {
    return disk_;
  }

  Pots& pots() {
    return pots_;
  }

  Midi& midi() {
    return selectedePatch().midi_;
  }

  Envelope& ampEnvelope() {
    return selectedePatch().ampEnvelope_;
  }

  Envelope& modEnvelope() {
    return selectedePatch().modEnvelope_;
  }

  Lfo& lfo(int index) {
    return selectedePatch().lfo_[index];
  }

  ModMatrix& modMatrix() {
    return selectedePatch().modMatrix_;
  }

  // name
  const char* project_name() {
    return project_name_;
  }

  void set_project_name(const char* value) {
    return StringUtils::copy(project_name_, const_cast<char*>(value), max_name_length());
  }

  const size_t max_name_length() {
    return 13;
  }

 private:
  StringBuilderBase<63> path;

  FileWriter fileWriter;
  FileReader fileReader;

  Disk* disk_ = nullptr;

  int selected_lfo_index_;
  int selectedPatchIndex_;

  char project_name_[8];

  Pots pots_;

  struct Patch {
    Midi midi_;
    Envelope ampEnvelope_;
    Envelope modEnvelope_;
    ModMatrix modMatrix_;
    Lfo lfo_[kNumLfos];
  };
  Patch patch_[kNumPatches];
};

#endif
