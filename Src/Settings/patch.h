#if !defined(Patch_h)
#define Patch_h

#include "envelope.h"
#include "filter.h"
#include "lfo.h"
#include "midi.h"
#include "modMatrix.h"
#include "oscillator.h"

class Patch {
 public:
  static const size_t kMaxNameLength = 16;

  enum VoiceMode { MONO, POLY, UNISON, NUM_VOICE_MODES };

  const char* voiceModeText(VoiceMode value) {
    switch (value) {
      case MONO:    return "MONO";
      case POLY:    return "POLY";
      case UNISON:  return "UNISON";
      default:
        break;
    }
    return nullptr;
  }

  void init() {
    lfo(0).init();
    lfo(1).init();
    midi().init();
    oscillator().init();
    filter().init();
    ampEnvelope().init();
    modEnvelope().init();
    modMatrix().init();
    setName("EMPTY PATCH");
  }

  const char* name() {
    return name_;
  }

  void setName(const char* text) {
    StringUtils::copy(name_, const_cast<char*>(text), kMaxNameLength);
  }

  VoiceMode voiceMode() {
    return voiceMode_;
  }

  void setVoiceMode(int value) {
    voiceMode_ = VoiceMode(SettingsUtils::clip(0, NUM_VOICE_MODES - 1, value));
  }

  const char *voiceModeText() {
    return voiceModeText(voiceMode());
  }

  Midi& midi() {
    return midi_;
  }

  Oscillator& oscillator() {
    return oscillator_;
  }

  Filter& filter() {
    return filter_;
  }

  Envelope& ampEnvelope() {
    return ampEnvelope_;
  }

  Envelope& modEnvelope() {
    return modEnvelope_;
  }

  ModMatrix& modMatrix() {
    return modMatrix_;
  }

  Lfo& lfo(int index) {
    return lfo_[index];
  }

  // Storage
  void save(FileWriter& fileWriter) {
    fileWriter.write(name_);
    fileWriter.write(voiceMode_);

    lfo_[0].save(fileWriter);
    lfo_[1].save(fileWriter);
    midi_.save(fileWriter);
    oscillator_.save(fileWriter);
    filter_.save(fileWriter);
    ampEnvelope_.save(fileWriter);
    modEnvelope_.save(fileWriter);
    modMatrix_.save(fileWriter);
  }

  void load(FileReader& fileReader) {
    fileReader.read(name_);
    fileReader.read(voiceMode_);

    lfo_[0].load(fileReader);
    lfo_[1].load(fileReader);
    midi_.load(fileReader);
    oscillator_.load(fileReader);
    filter_.load(fileReader);
    ampEnvelope_.load(fileReader);
    modEnvelope_.load(fileReader);
    modMatrix_.load(fileReader);
  }

  void paste(Patch* patch) {
    StringUtils::copy(name_, const_cast<char*>(patch->name()), kMaxNameLength);
    voiceMode_ = patch->voiceMode();

    lfo_[0].paste(&patch->lfo(0));
    lfo_[1].paste(&patch->lfo(1));
    midi_.paste(&patch->midi());
    oscillator_.paste(&patch->oscillator());
    filter_.paste(&patch->filter());
    ampEnvelope_.paste(&patch->ampEnvelope());
    modEnvelope_.paste(&patch->modEnvelope());
    modMatrix_.paste(&patch->modMatrix());
  }

 private:
  Midi midi_;
  Oscillator oscillator_;
  Filter filter_;
  Envelope ampEnvelope_;
  Envelope modEnvelope_;
  ModMatrix modMatrix_;
  Lfo lfo_[2];
  char name_[kMaxNameLength];
  VoiceMode voiceMode_;
};

#endif  // Patch_h
