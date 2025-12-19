#ifndef Settings_h
#define Settings_h

#include "disk.h"
#include "envelope.h"
#include "fileReader.h"
#include "fileWriter.h"
#include "lfo.h"
#include "midi.h"
#include "settingsUtils.h"
#include "stringBuilder.h"

class Settings {
 public:
  static const size_t kNumVoices = 8;
  static const size_t kNumLfos = 2;
  static const size_t kNumEnvelopes = 2;

  void init(Disk* disk) {
    disk_ = disk;

    selected_lfo_index_ = 0;
    selected_envelope_index_ = 0;

    path.clear();

    for (size_t i = 0; i < kNumEnvelopes; i++) {
      envelope(i).init();
    }

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

  Disk* disk() {
    return disk_;
  }

  Midi& midi() {
    return midi_;
  }

  Envelope& envelope(int index) {
    return envelope_[index];
  }

  Lfo& lfo(int index) {
    return lfo_[index];
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
  int selected_envelope_index_;

  char project_name_[8];

  Midi midi_;
  Lfo lfo_[kNumLfos];
  Envelope envelope_[kNumEnvelopes];
};

#endif
