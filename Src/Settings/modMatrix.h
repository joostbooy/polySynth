#ifndef ModMatrix_h
#define ModMatrix_h

//#include "settings.h"

class ModMatrix {
 public:
  enum Destination {
    PITCH,
    GAIN,

    NUM_DESTINATIONS
  };

  static_assert(NUM_DESTINATIONS < 32, "Too many destinations !");

  static const char* destination_text(int value) {
    switch (value) {
      case PITCH:
        return "PITCH";
      case GAIN:
        return "GAIN";
      default:
        break;
    }
    return nullptr;
  }

  enum Source {
    LFO_1,
    LFO_2,
    ENVELOPE_1,
    ENVELOPE_2,
    CV_1,
    CV_2,
    MIDI_BEND,
    MIDI_VELOCITY,
    MIDI_CC_A,
    MIDI_CC_B,
    MIDI_CC_C,
    MIDI_CC_D,
    // 13
    // 14
    // 15
    // 16

    NUM_SOURCES
  };

  static const char* source_text(int value) {
    switch (value) {
      case LFO_1:
        return "LFO 1";
      case LFO_2:
        return "LFO 2";
      case ENVELOPE_1:
        return "ENVELOPE 1";
      case ENVELOPE_2:
        return "ENVELOPE 2";
      case CV_1:
        return "CV 1";
      case CV_2:
        return "CV 2";
      case MIDI_BEND:
        return "MIDI BEND";
      case MIDI_VELOCITY:
        return "MIDI VELOCITY";
      case MIDI_CC_A:
        return nullptr;//midi_cc_number_text(0);
      case MIDI_CC_B:
        return nullptr;//midi_cc_number_text(1);
      case MIDI_CC_C:
        return nullptr;//midi_cc_number_text(2);
      case MIDI_CC_D:
        return nullptr;//midi_cc_number_text(3);
      default:
        break;
    }
    return nullptr;
  }


  void init() {
    clear();
    toggle(MIDI_VELOCITY, GAIN);
    toggle(MIDI_BEND, PITCH);
    toggle(ENVELOPE_1, GAIN);

  //  for (size_t i = 0; i < Settings::kNumUserCc; ++i) {
    //  set_midi_cc_number(i, i);
   // }
  }

  bool read(size_t src, size_t dest) {
    return matrix_[src] & (1 << dest);
  }

  void clear() {
    for (size_t i = 0; i < NUM_SOURCES; ++i) {
      matrix_[i] = 0;
    }
  }

  void toggle(size_t src, size_t dest) {
    uint32_t data = matrix_[src];
    matrix_[src] = data ^ (1 << dest);
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
    for (size_t i = 0; i < NUM_SOURCES; ++i) {
      fileWriter.write(matrix_[i]);
    }

  //  for (size_t i = 0; i < Settings::kNumUserCc; ++i) {
    //  fileWriter.write(midi_cc_number_[i]);
  //  }
  }

  void load(FileReader& fileReader) {
    for (size_t i = 0; i < NUM_SOURCES; ++i) {
      fileReader.read(matrix_[i]);
    }

 //   for (size_t i = 0; i < Settings::kNumUserCc; ++i) {
  //    fileReader.read(midi_cc_number_[i]);
  //  }
  }

  void paste(ModMatrix* modMatrix) {
    clear();
    for (size_t y = 0; y < NUM_SOURCES; ++y) {
      for (size_t x = 0; x < NUM_DESTINATIONS; ++x) {
        if (modMatrix->read(x, y)) {
          toggle(x, y);
        }
      }
    }

  //  for (size_t i = 0; i < Settings::kNumUserCc; ++i) {
    //  midi_cc_number_[i] = midi_cc_number_[i];
   // }
  }

 private:
  uint32_t matrix_[NUM_SOURCES];
  uint8_t midi_cc_number_[4];//[Settings::kNumUserCc];
};

#endif
