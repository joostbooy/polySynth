#ifndef VoiceEngine_h
#define VoiceEngine_h

#include <algorithm>

#include "settings.h"
#include "stack.h"
#include "voice.h"

class VoiceEngine {
 public:
  void init(Settings* settings, ModMatrixEngine* modMatrixEngine, Dac* dac) {
    settings_ = settings;
    activeVoices_.clear();
    availableVoices_.clear();

    for (size_t i = 0; i < Settings::kNumVoices; ++i) {
      voice_[i].init(i, settings, modMatrixEngine, dac);
      availableVoices_.push(i);
    }
  }

  void clear() {
    numRequested_ = 0;
    for (size_t i = 0; i < Settings::kNumVoices; i++) {
      voice_[i].requestStop();
    }
  }

  Voice& voice(uint8_t index) {
    return voice_[index];
  }

  size_t maxNotes() {
    switch (settings_->oscillator().voiceMode()) {
      case Oscillator::MONO:
      case Oscillator::UNISON:  return 1;
      case Oscillator::POLY:    return Settings::kNumVoices;
      default:
        break;
    }
    return 0;
  }

  Voice& mostRecentVoice() {
    return voice(mostRecentVoice_);
  }

  void update() {
    for (size_t i = 0; i < Settings::kNumVoices; ++i) {
      voice_[i].update();
    }

    updateAvailableVoices();
  }

  void requestVoice() {
    if (numRequested_ < maxNotes()) {
      ++numRequested_;
      switch (settings_->oscillator().voiceMode()) {
        case Oscillator::MONO:
        case Oscillator::UNISON:
          if (availableVoices_.size() < Settings::kNumVoices) {
            for (size_t i = 0; i < Settings::kNumVoices; i++) {
              voice_[i].requestStop();
            }
          }
          break;
        case Oscillator::POLY:
          if (availableVoices_.size() < numRequested_) {
            uint8_t v = activeVoices_.pull();
            voice_[v].requestStop();
            activeVoices_.push(v);
          }
          break;
        default:
          break;
      }
    }
  }

  void noteOff(uint8_t port, uint8_t chn, uint8_t note) {
    uint8_t count = activeVoices_.size();

    for (uint8_t i = 0; i < count; ++i) {
      uint8_t v = activeVoices_.read(i);
      if (voice_[v].keyPressed() == true && voice_[v].port() == port && voice_[v].channel() == chn && voice_[v].note() == note) {
        voice_[v].noteOff();
        break;
      }
    }
  }

  bool available() {
    switch (settings_->oscillator().voiceMode()) {
      case Oscillator::MONO:   
      case Oscillator::UNISON:  return availableVoices_.size() == Settings::kNumVoices;
      case Oscillator::POLY:    return availableVoices_.size() >= 1;
      default:
        break;
    }
    return false;
  }

  void assignVoice(MidiEngine::Event e) {
    switch (settings_->oscillator().voiceMode()) {
      case Oscillator::MONO:
        availableVoices_.remove_by_value(0);
        noteOn(0, e);
      case Oscillator::POLY:
        noteOn(availableVoices_.pop(), e);
        break;
      case Oscillator::UNISON:
        for (size_t i = 0; i < Settings::kNumVoices; i++) {
          noteOn(availableVoices_.pop(), e);
        }
        break;
      default:
        break;
    }
  }

 private:
  Settings* settings_;
  Voice voice_[Settings::kNumVoices];
  Stack<uint8_t, Settings::kNumVoices> activeVoices_;
  Stack<uint8_t, Settings::kNumVoices> availableVoices_;
  uint8_t numRequested_ = 0;
  uint8_t mostRecentVoice_ = 0;

  void updateAvailableVoices() {
    uint8_t index = 0;
    while (index < activeVoices_.size()) {
      uint8_t v = activeVoices_.read(index);
      if (voice_[v].isAvailable()) {
        activeVoices_.remove_by_index(index);
        availableVoices_.push(v);
      } else {
        ++index;
      }
    }
  }

  void noteOn(int voiceIndex, MidiEngine::Event& e) {
    voice_[voiceIndex].noteOn(e, activeVoices_.size());
    activeVoices_.push(voiceIndex);
    mostRecentVoice_ = voiceIndex;
    if (numRequested_ > 0) {
      --numRequested_;
    }
  }
};

#endif