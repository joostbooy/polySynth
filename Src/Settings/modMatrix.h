#ifndef ModMatrix_h
#define ModMatrix_h

class ModMatrix {
 public:
  static const size_t kNumUserCc = 4;

  enum Destination {
    TUNE_1,
    TUNE_2,
    GAIN,
    CUTOFF_1,
    CUTOFF_2,
    RESONANCE_1,
    RESONANCE_2,
    SHAPE_1,
    SHAPE_2,
    VCO_MOD_DEPTH,
    PAN,
    DRIVE,

    NUM_DESTINATIONS
  };

  static_assert(NUM_DESTINATIONS < 32, "Too many destinations !");

  static const char* destination_text(int value) {
    switch (value) {
      case TUNE_1:        return "TUNE 1";
      case TUNE_2:        return "TUNE 2";
      case GAIN:          return "GAIN";
      case CUTOFF_1:      return "CUTOFF 1";
      case CUTOFF_2:      return "CUTOFF 2";
      case RESONANCE_1:   return "RESONANCE 1";
      case RESONANCE_2:   return "RESONANCE 2";
      case SHAPE_1:       return "SHAPE 1";
      case SHAPE_2:       return "SHAPE 2";
      case VCO_MOD_DEPTH: return "VCO MOD DEPTH";
      case PAN:           return "PAN";
      case DRIVE:         return "DRIVE";
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
    MIDI_NOTE,

    NUM_SOURCES
  };

  const char* source_text(int value) {
    switch (value) {
      case LFO_1:         return "LFO 1";
      case LFO_2:         return "LFO 2";
      case ENVELOPE_1:    return "ENVELOPE 1";
      case ENVELOPE_2:    return "ENVELOPE 2";
      case CV_1:          return "CV 1";
      case CV_2:          return "CV 2";
      case MIDI_BEND:     return "MIDI BEND";
      case MIDI_VELOCITY: return "MIDI VELOCITY";
      case MIDI_CC_A:     return midiCcNumberText(0);
      case MIDI_CC_B:     return midiCcNumberText(1);
      case MIDI_CC_C:     return midiCcNumberText(2);
      case MIDI_CC_D:     return midiCcNumberText(3);
      case MIDI_NOTE:     return "MIDI NOTE";
      default:
        break;
    }
    return nullptr;
  }

  void init() {
    clear();
    matrix_[MIDI_BEND] |= (1 << TUNE_1);
    matrix_[MIDI_BEND] |= (1 << TUNE_2);
    matrix_[MIDI_VELOCITY] |= (1 << GAIN);
    matrix_[ENVELOPE_1] |= (1 << GAIN);
    matrix_[ENVELOPE_2] |= (1 << CUTOFF_1);
    matrix_[ENVELOPE_2] |= (1 << CUTOFF_2);

    for (size_t i = 0; i < kNumUserCc; ++i) {
      setMidiCcNumber(i, i);
    }
  }

  bool read(size_t src, size_t dest) {
    return matrix_[src] & (1 << dest);
  }
  
  void clear() {
    std::fill(&destinationDepth_[0], &destinationDepth_[NUM_DESTINATIONS], 1.f);

    for (size_t i = 0; i < NUM_SOURCES; ++i) {
      matrix_[i] = 0;
      invert_[i] = 0;
    }
    matrix_[ENVELOPE_1] |= (1 << GAIN); // envelope 1 is always tied to gain !
  }

  void toggle(size_t src, size_t dest) {
    set(src, dest, !read(src, dest));
  }

  void set(size_t src, size_t dest, bool state) {
    uint32_t data = matrix_[src];
    if (state) {
      matrix_[src] = data | (1 << dest);
    } else {
      matrix_[src] = data & ~(1 << dest);
    }
    matrix_[ENVELOPE_1] |= (1 << GAIN);
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

  // Destination depth
  void setDestinationDepth(size_t dest, float value) {
    destinationDepth_[dest] = value;
  }

  float destinationDepth(size_t dest) {
    return destinationDepth_[dest];
  }
 
  const char* destinationDepthText(size_t dest) {
    return SettingsText::floatToText(destinationDepth(dest));
  }

  // Invert
  void setInvert(size_t src, size_t dest, bool state) {
    uint32_t data = invert_[src];
    if (state) {
      invert_[src] = data | (1 << dest);
    } else {
      invert_[src] = data & ~(1 << dest);
    }
  }

  bool invert(size_t src, size_t dest) {
    return invert_[src] & (1 << dest);
  }

  const char* invertText(size_t src, size_t dest) {
    return SettingsText::boolToOnOff(invert(src, dest));
  }

  // Storage
  void save(FileWriter& fileWriter) {
    for (size_t i = 0; i < NUM_SOURCES; ++i) {
      fileWriter.write(matrix_[i]);
      fileWriter.write(invert_[i]);
    }

    for (size_t i = 0; i < kNumUserCc; ++i) {
      fileWriter.write(midiCcNumber_[i]);
    }
  }

  void load(FileReader& fileReader) {
    for (size_t i = 0; i < NUM_SOURCES; ++i) {
      fileReader.read(matrix_[i]);
      fileReader.read(invert_[i]);
    }

    for (size_t i = 0; i < kNumUserCc; ++i) {
      fileReader.read(midiCcNumber_[i]);
    }
  }

  void paste(ModMatrix* modMatrix) {
    clear();
    for (size_t x = 0; x < NUM_SOURCES; ++x) {
      for (size_t y = 0; y < NUM_DESTINATIONS; ++y) {
        if (modMatrix->read(x, y)) {
          matrix_[x] |= (1 << y);
        }
        if (modMatrix->invert(x, y)) {
          invert_[x] |= (1 << y);
        }
      }
    }

    for (size_t i = 0; i < kNumUserCc; ++i) {
      midiCcNumber_[i] = midiCcNumber_[i];
    }
  }

  void writeHash(Hash& hash) {
    for (size_t i = 0; i < NUM_SOURCES; ++i) {
      hash.write(matrix_[i]);
      hash.write(invert_[i]);
    }

    for (size_t i = 0; i < kNumUserCc; ++i) {
      hash.write(midiCcNumber_[i]);
    }
  }

 private:
  uint8_t midiCcNumber_[4];
  uint32_t matrix_[NUM_SOURCES];
  uint32_t invert_[NUM_SOURCES];
  float destinationDepth_[NUM_DESTINATIONS];
};

#endif
