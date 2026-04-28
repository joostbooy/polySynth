#ifndef Calibration_h
#define Calibration_h

#include "SettingsText.h"
#include "SettingsUtils.h"
#include "dsp.h"
#include "fileReader.h"
#include "fileWriter.h"

class Calibration {
 public:
  void init() {
    selectedVoice_ = 0;
    selectedNote_ = 0;
    enabled_ = false;

    for (size_t i = 0; i < kMaxNotes; i++) {
      noteValue_[i] = 65535 - (i * (65535 / kMaxNotes));
    }
  }

  void start() {
    enabled_ = true;
  }

  void stop() {
    enabled_ = false;
  }

  bool enabled() {
    return enabled_;
  }

  // Selected voice
  void selectNextVoice() {
    if (++selectedVoice_ >= 8) {
      selectedVoice_ = 0;
    }
  }

  int selectedVoice() {
    return selectedVoice_;
  }

  const char* selectedVoiceText() {
    return SettingsText::intToText(selectedVoice() + 1);
  }

  // selected note
  void setSelectedNote(int note) {
    selectedNote_ = SettingsUtils::clip(0, kMaxNotes - 1, note);
  }

  int selectedNote() {
    return selectedNote_;
  }

  const char* selectedNoteText() {
    return SettingsText::noteToText(selectedNote_);
  }

  // note value
  void setNoteValue(uint16_t value) {
    noteValue_[selectedNote_] = value;
  }

  uint16_t noteValue(int note) {
    return noteValue_[SettingsUtils::clip(0, kMaxNotes - 1, note)];
  }

  uint16_t noteValue() {
    return noteValue_[selectedNote_];
  }

  uint16_t semiNoteValue() {
    return semiNoteValue_;
  }

  const char* noteValueText() {
    return SettingsText::intToText(noteValue());
  }

  // storage
  void save(FileWriter& fileWriter) {
    for (size_t i = 0; i < kMaxNotes; i++) {
      fileWriter.write(noteValue_[i]);
    }
    updateSemiNoteValue();
  }

  void load(FileReader& fileReader) {
    for (size_t i = 0; i < kMaxNotes; i++) {
      fileReader.read(noteValue_[i]);
    }
    updateSemiNoteValue();
  }

 private:
  bool enabled_;
  static const int kMaxVolts = 10;  // -3V / +7V
  static const int kMaxNotes = kMaxVolts * 12;
  int selectedVoice_;
  int selectedNote_;
  uint16_t noteValue_[kMaxNotes];
  uint16_t semiNoteValue_;

  void updateSemiNoteValue() {
    semiNoteValue_ = noteValue(0) - noteValue(1);
  }
};

#endif
