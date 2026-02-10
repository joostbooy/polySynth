#if !defined(Patch_h)
#define Patch_h

#include "envelope.h"
#include "filter.h"
#include "lfo.h"
#include "midi.h"
#include "modMatrix.h"
#include "oscillator.h"
#include "amp.h"

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
    envelope(0).init();
    envelope(1).init();
    modMatrix().init();
    amp().init();
    setName("EMPTY PATCH");
    setVoiceMode(POLY);
  }

  // Name
  const char* name() {
    return name_;
  }

  void setName(const char* text) {
    StringUtils::copy(name_, const_cast<char*>(text), kMaxNameLength);
  }

  // Voice mode
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

  Envelope& envelope(int index) {
    return envelope_[index];
  }

  ModMatrix& modMatrix() {
    return modMatrix_;
  }

  Lfo& lfo(int index) {
    return lfo_[index];
  }

  Amp& amp() {
    return amp_;
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
    envelope_[0].save(fileWriter);
    envelope_[1].save(fileWriter);
    modMatrix_.save(fileWriter);
    amp_.save(fileWriter);
  }

  void load(FileReader& fileReader) {
    fileReader.read(name_);
    fileReader.read(voiceMode_);

    lfo_[0].load(fileReader);
    lfo_[1].load(fileReader);
    midi_.load(fileReader);
    oscillator_.load(fileReader);
    filter_.load(fileReader);
    envelope_[0].load(fileReader);
    envelope_[1].load(fileReader);
    modMatrix_.load(fileReader);
    amp_.load(fileReader);
  }

  void paste(Patch* patch) {
    StringUtils::copy(name_, const_cast<char*>(patch->name()), kMaxNameLength);
    voiceMode_ = patch->voiceMode();
    
    lfo_[0].paste(&patch->lfo(0));
    lfo_[1].paste(&patch->lfo(1));
    midi_.paste(&patch->midi());
    oscillator_.paste(&patch->oscillator());
    filter_.paste(&patch->filter());
    envelope_[0].paste(&patch->envelope(0));
    envelope_[1].paste(&patch->envelope(1));
    modMatrix_.paste(&patch->modMatrix());
    amp_.paste(&patch->amp());
  }

 private:
  Midi midi_;
  Oscillator oscillator_;
  Filter filter_;
  ModMatrix modMatrix_;
  Amp amp_;
  Lfo lfo_[2];
  Envelope envelope_[2];
  char name_[kMaxNameLength];
  VoiceMode voiceMode_;
};

#endif  // Patch_h
