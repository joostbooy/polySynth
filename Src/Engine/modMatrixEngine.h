#ifndef ModMatrixEngine_h
#define ModMatrixEngine_h

#include "settings.h"

class ModMatrixEngine {
 public:

  void init(Settings* settings) {
    settings_ = settings;
  }

  void setEnvelope(int index, float value) {
    source_[ModMatrix::ENVELOPE_1 + index] = value;
  }

  void setLfo(int index, float value) {
    source_[ModMatrix::LFO_1 + index] = value;
  }

  void setCv(int channel, float value) {
    source_[ModMatrix::CV_1 + channel] = value;
  }

  void setMidiVelocity(uint8_t value) {
    source_[ModMatrix::MIDI_VELOCITY] = (1.f / 127.f) * value;
  }

  void setMidiBend(uint16_t value) {
    source_[ModMatrix::MIDI_BEND] = (1.f / 16383.f) * value;
  }

  void setMidiNote(uint8_t note) {
    source_[ModMatrix::MIDI_BEND] = (1.f / 127.f) * note;
  }

  void setMidiCc(uint8_t number, uint8_t value) {
    Patch& p = settings_->selectedPatch();

    for (size_t i = 0; i < Settings::kNumUserCc; ++i) {
      if (number == p.modMatrix().midiCcNumber(i)) {
        source_[i + ModMatrix::MIDI_CC_A] = (1.f / 127.f) * value;
      }
    }
  }

  float* process() {
    Patch& p = settings_->selectedPatch();

    for (int dest = 0; dest < ModMatrix::NUM_DESTINATIONS; ++dest) {
      destination_[dest] = 1.f;
      for (int src = 0; src < ModMatrix::NUM_SOURCES; ++src) {
        if (p.modMatrix().read(src, dest)) {
          destination_[dest] *= source_[src];
        }
      }
      destination_[dest] = Dsp::cross_fade(originalValue(dest), destination_[dest], p.modMatrix().destinationDepth(dest));
    }
    return &destination_[0];
  }

 private:
  Settings* settings_;
  float source_[ModMatrix::NUM_SOURCES];
  float destination_[ModMatrix::NUM_DESTINATIONS];

  float originalValue(size_t dest) {
    Patch& p = settings_->selectedPatch();
    switch (dest) {
      case ModMatrix::CUTOFF_1:       return p.filter().cutoff1();
      case ModMatrix::CUTOFF_2:       return p.filter().cutoff2();
      case ModMatrix::RESONANCE_1:    return p.filter().resonance1();
      case ModMatrix::RESONANCE_2:    return p.filter().resonance2();
      case ModMatrix::SHAPE_1:        return p.oscillator().shape1();
      case ModMatrix::SHAPE_2:        return p.oscillator().shape2();
      case ModMatrix::TUNE_1:         return p.oscillator().tune1();
      case ModMatrix::TUNE_2:         return p.oscillator().tune2();
      case ModMatrix::VCO_MOD_DEPTH:  return p.oscillator().modDepth();
      case ModMatrix::GAIN:           return 1.f;
      case ModMatrix::PAN:            return p.amp().pan();
      case ModMatrix::DRIVE:          return p.amp().drive();
      default:
        break;
    }
    return 0.f;
  }
};

#endif
