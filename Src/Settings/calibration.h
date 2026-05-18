#ifndef Calibration_h
#define Calibration_h

#include "SettingsText.h"
#include "SettingsUtils.h"
#include "fileReader.h"
#include "fileWriter.h"

class Calibration {
 public:
  void init() {
    selectedVoice_ = 0;
    selectedNote_ = 0;
    selectedVco_ = 0;
    enabled_ = false;
    tuneError1_ = 0.f;
    tuneError2_ = 0.f;

    for (size_t i = 0; i < 8; i++) {
      for (size_t j = 0; j < kMaxNotes; j++) {
        uint16_t value = 65535 - (j * (65535 / kMaxNotes));
        voice_[i].noteValueOsc1[j] = value;
        voice_[i].noteValueOsc2[j] = value;
      }
    }
    updateSemiNoteValue();
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

  // Selected Vco
  void selectNextVco() {
    if (++selectedVco_ >= 2) {
      selectedVco_ = 0;
    }
  }

  int selectedVco() {
    return selectedVco_;
  }

  const char* selectedVcoText() {
    return SettingsText::intToText(selectedVco() + 1);
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

  // Seminote 
  uint16_t semiNoteValue() {
    return semiNoteValue_;
  }

  // note value
  void setNoteValue(uint16_t value) {
    if (selectedVco() == 0) {
      voice_[selectedVoice_].noteValueOsc1[selectedNote_] = value;
    } else {
      voice_[selectedVoice_].noteValueOsc2[selectedNote_] = value;
    }
  }

  const char* noteValueText() {
    return SettingsText::intToText(noteValue());
  }

  uint16_t noteValue() {
    return noteValue(selectedVoice_, selectedVco_, selectedNote_);
  }

  uint16_t noteValue(int voice, int osc, int note) {
    int note_ = SettingsUtils::clip(0, kMaxNotes - 1, note);
    if (osc == 0) {
      return voice_[voice].noteValueOsc1[note_];
    } else {
      return voice_[voice].noteValueOsc2[note_];
    }
  }

  // Tune error
  void setTuneError1(float value) {
    tuneError1_ = value;
  }

  void setTuneError2(float value) {
    tuneError2_ = value;
  }

  float tuneError1() {
    return tuneError1_;
  }

  float tuneError2() {
    return tuneError2_;
  }

  // storage
  void save(FileWriter& fileWriter) {
    for (size_t i = 0; i < 8; i++) {
      for (size_t j = 0; j < kMaxNotes; j++) {
        fileWriter.write(voice_[i].noteValueOsc1[j]);
        fileWriter.write(voice_[i].noteValueOsc2[j]);
      }
    }
    fileWriter.write(tuneError1_);
    fileWriter.write(tuneError2_);
    updateSemiNoteValue();
  }

  void load(FileReader& fileReader) {
    for (size_t i = 0; i < 8; i++) {
      for (size_t j = 0; j < kMaxNotes; j++) {
        fileReader.read(voice_[i].noteValueOsc1[j]);
        fileReader.read(voice_[i].noteValueOsc2[j]);
      }
    }
    if (fileReader.version() >= 2) {
      fileReader.read(tuneError1_);
      fileReader.read(tuneError2_);
    }
    updateSemiNoteValue();
  }

 private:
  bool enabled_;
  static const int kMaxVolts = 10;  // -3V / +7V
  static const int kMaxNotes = kMaxVolts * 12;
  int selectedVoice_;
  int selectedNote_;
  int selectedVco_;
  uint16_t semiNoteValue_;
  float tuneError1_;
  float tuneError2_;

  struct Voice {
    uint16_t noteValueOsc1[kMaxNotes];
    uint16_t noteValueOsc2[kMaxNotes];
  } voice_[8];

  void updateSemiNoteValue() {
    semiNoteValue_ = noteValue(0, 0, 60) - noteValue(0, 0, 61);
  }
};

#endif
