#ifndef VoiceEngine_h
#define VoiceEngine_h

#include <algorithm>

#include "settings.h"
#include "stack.h"
#include "voice.h"

class VoiceEngine {
 public:
  void init(Settings* settings, ModMatrixEngine* modMatrixEngine) {
    active_voices_.clear();
    available_voices_.clear();

    for (size_t i = 0; i < Settings::kNumVoices; ++i) {
      voice_[i].init(settings, modMatrixEngine);
      available_voices_.push(i);
    }
  }

  Voice& voice(uint8_t index) {
    return voice_[index];
  }

  inline bool is_idle() {
    return active_voices_.size() == 0;
  }

  int num_active() {
    return active_voices_.size();
  }

  Voice& most_recent_voice() {
    return voice(most_recent_voice_);
  }

  void update(Dac* dac) {
    for (size_t i = 0; i < Settings::kNumVoices; ++i) {
      if (voice_[i].state() != Voice::IDLE) {
        voice_[i].update(dac);
      }
    }
    update_available_voices();
  }

  void request_voice() {
    if (available_voices_.size() == 0) {
      uint8_t v = active_voices_.pull();
      voice_[v].request_stop();
      active_voices_.push(v);
    }
  }

  void note_off(uint8_t port, uint8_t chn, uint8_t note) {
    uint8_t count = active_voices_.size();

    for (uint8_t i = 0; i < count; ++i) {
      uint8_t v = active_voices_.read(i);
      if (voice_[v].key_pressed() == true && voice_[v].port() == port && voice_[v].channel() == chn && voice_[v].note() == note) {
        voice_[v].note_off();
        break;
      }
    }
  }

  bool available() {
    return available_voices_.size() > 0;
  }

  void assign_voice(MidiEngine::Event e) {
    uint8_t v = available_voices_.pop();
    voice_[v].note_on(e);
    active_voices_.push(v);
    most_recent_voice_ = v;
  }

  void kill_midi_channel(uint8_t port, uint8_t channel) {
    uint8_t count = active_voices_.size();

    for (uint8_t i = 0; i < count; ++i) {
      uint8_t v = active_voices_.read(i);
      if (voice_[v].port() == port && voice_[v].channel() == channel) {
        voice_[v].request_stop();
      }
    }
  }

 private:
  Voice voice_[Settings::kNumVoices];
  Stack<uint8_t, Settings::kNumVoices> active_voices_;
  Stack<uint8_t, Settings::kNumVoices> available_voices_;
  uint8_t most_recent_voice_ = 0;

  void update_available_voices() {
    uint8_t index = 0;
    while (index < active_voices_.size()) {
      uint8_t v = active_voices_.read(index);
      if (voice_[v].is_available()) {
        active_voices_.remove_by_index(index);
        available_voices_.push(v);
      } else {
        ++index;
      }
    }
  }
};

#endif
