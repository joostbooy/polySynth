#ifndef MidiClock_h
#define MidiClock_h

#include "midi.h"
#include "settingsText.h"

class MidiClock {
 public:
  enum Tempo {
    _1_1_DOTTED,
    _1_1_WHOLE,
    _1_1_TRIPLET,
    _1_2_DOTTED,
    _1_2_WHOLE,
    _1_2_TRIPLET,
    _1_4_DOTTED,
    _1_4_WHOLE,
    _1_4_TRIPLET,
    _1_8_DOTTED,
    _1_8_WHOLE,
    _1_8_TRIPLET,
    _1_16_DOTTED,
    _1_16_WHOLE,
    _1_16_TRIPLET,
    _1_32_DOTTED,
    _1_32_WHOLE,
    _1_32_TRIPLET,
    _1_64_DOTTED,
    _1_64_WHOLE,
    _1_64_TRIPLET,

    NUM_TEMPOS
  };

  static const char* tempoText(int value) {
    switch (value) {
      case _1_1_DOTTED:		return "1/1 D";
      case _1_1_WHOLE:		return "1/1";
      case _1_1_TRIPLET:	return "1/1 T";
      case _1_2_DOTTED:		return "1/2 D";
      case _1_2_WHOLE:		return "1/2";
      case _1_2_TRIPLET:	return "1/2 T";
      case _1_4_DOTTED:		return "1/4 D";
      case _1_4_WHOLE:		return "1/4";
      case _1_4_TRIPLET:	return "1/4 T";
      case _1_8_DOTTED:		return "1/8 D";
      case _1_8_WHOLE:		return "1/8";
      case _1_8_TRIPLET:	return "1/8 T";
      case _1_16_DOTTED:	return "1/16 D";
      case _1_16_WHOLE:		return "1/16";
      case _1_16_TRIPLET:	return "1/16 T";
      case _1_32_DOTTED:	return "1/32 D";
      case _1_32_WHOLE:		return "1/32";
      case _1_32_TRIPLET:	return "1/32 T";
      case _1_64_DOTTED:	return "1/64 D";
      case _1_64_WHOLE:		return "1/64";
      case _1_64_TRIPLET:	return "1/64 T";
      default:
        break;
    }
    return nullptr;
  }

  static const char* tempoText(float value) {
    return tempoText(int(value * (NUM_TEMPOS - 1)));
  }

  enum ClockSource {
    EXTERNAL = Midi::NUM_PORTS - 1,
    INTERNAL,

    NUM_CLOCK_SOURCES
  };

  static const char* sourceText(int value) {
    if (value <= EXTERNAL) {
      return Midi::portText(value);
    } else {
      return "INTERNAL";
    }
  }

  void init() {
    setBpm(120);
    setBpmFractional(0);
    setSource(INTERNAL);

    for (int i = 0; i < Midi::Midi::NUM_PORTS; ++i) {
      setSend(i, true);
    }
  }

  // bpm
  uint16_t bpm() {
    return bpm_;
  }

  void setBpm(int value) {
    bpm_ = SettingsUtils::clip(MIN_BPM, MAX_BPM, value);
  }

  // bpm fractional
  uint16_t bpmFractional() {
    return bpmFractional_;
  }

  void setBpmFractional(int value) {
    bpmFractional_ = SettingsUtils::clip(0, MAX_BPM_FRACTIONAL, value);
  }

  const char* bpmText() {
    return SettingsText::str.write(bpm(), ".", bpmFractional());
  }

  // clock source
  uint8_t source() {
    return clockSource_;
  }

  void setSource(int value) {
    clockSource_ = SettingsUtils::clip(0, NUM_CLOCK_SOURCES - 1, value);
  }

  const char* sourceText() {
    return sourceText(source());
  }

  // clock source
  bool send(int port) {
    return send_[port];
  }

  void setSend(int port, bool value) {
    send_[port] = value;
  }

  const char* sendText(int port) {
    return SettingsText::boolToOnOff(send(port));
  }

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(bpm_);
    fileWriter.write(bpmFractional_);
    fileWriter.write(clockSource_);

    for (int i = 0; i < Midi::NUM_PORTS; ++i) {
      fileWriter.write(send_[i]);
    }
  }

  void load(FileReader& fileReader) {
    fileReader.read(bpm_);
    fileReader.read(bpmFractional_);
    fileReader.read(clockSource_);

    for (int i = 0; i < Midi::NUM_PORTS; ++i) {
      fileReader.read(send_[i]);
    }
  }

  void paste(MidiClock* midiClock) {
    bpm_ = midiClock->bpm();
    bpmFractional_ = midiClock->bpmFractional();
    clockSource_ = midiClock->source();

    for (int i = 0; i < Midi::NUM_PORTS; ++i) {
      send_[i] = midiClock->send(i);
    }
  }

  void writeHash(Hash& hash) {
    hash.write(bpm_);
    hash.write(bpmFractional_);
    hash.write(clockSource_);

    for (int i = 0; i < Midi::NUM_PORTS; ++i) {
      hash.write(send_[i]);
    }
  }

 private:
  uint16_t bpm_;
  uint16_t bpmFractional_;
  uint8_t clockSource_;
  bool send_[Midi::NUM_PORTS];
};

#endif
