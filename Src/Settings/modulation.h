#ifndef Modulation_h
#define Modulation_h

#include "fileReader.h"
#include "fileWriter.h"
#include "midiEngine.h"
#include "settingsText.h"
#include "settingsUtils.h"

class Modulation {
 public:
  static const size_t kNumUserCc = 4;

  void init() {
    for (size_t i = 0; i < kNumUserCc; ++i) {
      set_midi_cc_number(i, i);
    }
  }

  // Midi CC
  int midi_cc_number(int index) {
    return midi_cc_number_[index];
  }

  void set_midi_cc_number(int index, int cc_number) {
    midi_cc_number_[index] = SettingsUtils::clip(0, 127, cc_number);
  }

  const char* midi_cc_number_text(int index) {
    return SettingsText::str.write("CC ", midi_cc_number(index));
  }

  // Storage
  void save(FileWriter& fileWriter) {
    for (size_t i = 0; i < kNumUserCc; ++i) {
      fileWriter.write(midi_cc_number_[i]);
    }
  }

  void load(FileReader& fileReader) {
    for (size_t i = 0; i < kNumUserCc; ++i) {
      fileReader.read(midi_cc_number_[i]);
    }
  }

  void paste(Modulation* modulation) {
    for (size_t i = 0; i < kNumUserCc; ++i) {
      midi_cc_number_[i] = modulation->midi_cc_number(i);
    }
  }

 private:
  uint8_t midi_cc_number_[kNumUserCc];
};

#endif
