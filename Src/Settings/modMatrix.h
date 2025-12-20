#ifndef ModMatrix_h
#define ModMatrix_h

class ModMatrix {
 public:
  static const size_t kNumUserCc = 4;

  enum Destination {
    PITCH,
    GAIN,
    CUTOFF_1,
    CUTOFF_2,
    RESONANCE_1,
    RESONANCE_2,
    SHAPE_1,
    SHAPE_2,

    NUM_DESTINATIONS
  };

  static_assert(NUM_DESTINATIONS < 32, "Too many destinations !");

  static const char* destination_text(int value) {
    switch (value) {
      case PITCH:
        return "PITCH";
      case GAIN:
        return "GAIN";
      case CUTOFF_1:
        return "CUTOFF 1";
      case CUTOFF_2:
        return "CUTOFF 2";
      case RESONANCE_1:
        return "RESONANCE 1";
      case RESONANCE_2:
        return "RESONANCE 2";
      case SHAPE_1:
        return "SHAPE 1";
      case SHAPE_2:
        return "SHAPE 2";
      default:
        break;
    }
    return nullptr;
  }

  enum Source {
    LFO_1,
    LFO_2,
    AMP_ENVELOPE,
    MOD_ENVELOPE,
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

  const char* source_text(int value) {
    switch (value) {
      case LFO_1:
        return "LFO 1";
      case LFO_2:
        return "LFO 2";
      case AMP_ENVELOPE:
        return "AMP ENVELOPE";
      case MOD_ENVELOPE:
        return "MOD ENVELOPE";
      case CV_1:
        return "CV 1";
      case CV_2:
        return "CV 2";
      case MIDI_BEND:
        return "MIDI BEND";
      case MIDI_VELOCITY:
        return "MIDI VELOCITY";
      case MIDI_CC_A:
        return midiCcNumberText(0);
      case MIDI_CC_B:
        return midiCcNumberText(1);
      case MIDI_CC_C:
        return midiCcNumberText(2);
      case MIDI_CC_D:
        return midiCcNumberText(3);
      default:
        break;
    }
    return nullptr;
  }

  void init() {
    clear();
    matrix_[MIDI_BEND] |= (1 << PITCH);
    matrix_[MIDI_VELOCITY] |= (1 << GAIN);
    matrix_[AMP_ENVELOPE] |= (1 << GAIN);
    matrix_[MOD_ENVELOPE] |= (1 << CUTOFF_2);

    for (size_t i = 0; i < kNumUserCc; ++i) {
      setMidiCcNumber(i, i);
    }
  }

  bool read(size_t src, size_t dest) {
    return matrix_[src] & (1 << dest);
  }

  void clear() {
    for (size_t i = 0; i < NUM_SOURCES; ++i) {
      matrix_[i] = 0;
    }
  }

  // Amp envelope & gain are always tied togheter!
  void toggle(size_t src, size_t dest) {
    uint32_t data = matrix_[src];
    matrix_[src] = data ^ (1 << dest);
    matrix_[AMP_ENVELOPE] |= (1 << GAIN);
  }

  // Midi CC
  int midiCcNumber(int index) {
    return midiCcNumber_[index];
  }

  void setMidiCcNumber(int index, int cc_number) {
    midiCcNumber_[index] = SettingsUtils::clip(0, 127, cc_number);
  }

  const char* midiCcNumberText(int index) {
    return SettingsText::str.write("CC ", midiCcNumber(index));
  }

  // Storage
  void save(FileWriter& fileWriter) {
    for (size_t i = 0; i < NUM_SOURCES; ++i) {
      fileWriter.write(matrix_[i]);
    }

    for (size_t i = 0; i < kNumUserCc; ++i) {
      fileWriter.write(midiCcNumber_[i]);
    }
  }

  void load(FileReader& fileReader) {
    for (size_t i = 0; i < NUM_SOURCES; ++i) {
      fileReader.read(matrix_[i]);
    }

    for (size_t i = 0; i < kNumUserCc; ++i) {
      fileReader.read(midiCcNumber_[i]);
    }
  }

  void paste(ModMatrix* modMatrix) {
    clear();
    for (size_t y = 0; y < NUM_SOURCES; ++y) {
      for (size_t x = 0; x < NUM_DESTINATIONS; ++x) {
        if (modMatrix->read(x, y)) {
          matrix_[y] |= (1 << x);
        }
      }
    }

    for (size_t i = 0; i < kNumUserCc; ++i) {
      midiCcNumber_[i] = midiCcNumber_[i];
    }
  }

 private:
  uint32_t matrix_[NUM_SOURCES];
  uint8_t midiCcNumber_[4];
};

#endif
