#ifndef ModMatrixEngine_h
#define ModMatrixEngine_h

#include "settings.h"

class ModMatrixEngine {
 public:

  void init(Settings* settings) {
    modMatrix_ = &settings->modMatrix();
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
    for (size_t i = 0; i < Settings::kNumUserCc; ++i) {
      if (number == modMatrix_->midiCcNumber(i)) {
        source_[i + ModMatrix::MIDI_CC_A] = (1.f / 127.f) * value;
      }
    }
  }

  float* process() {
    for (int dest = 0; dest < ModMatrix::NUM_DESTINATIONS; ++dest) {
      destination_[dest] = 1.f;
      for (int src = 0; src < ModMatrix::NUM_SOURCES; ++src) {
        if (modMatrix_->read(src, dest)) {
          float value = source_[src];
          //float depth = modMatrix_->destinationDepth(dest);
          bool invert = modMatrix_->invert(src, dest);
          destination_[dest] *= (invert ? 1.f - value : value); //* depth;
        }
      }
    }
    return &destination_[0];
  }

 private:
  ModMatrix* modMatrix_;
  float source_[ModMatrix::NUM_SOURCES];
  float destination_[ModMatrix::NUM_DESTINATIONS];
};

#endif
