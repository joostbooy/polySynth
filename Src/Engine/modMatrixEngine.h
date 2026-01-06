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

  void setMidiVelocity(float value) {
    source_[ModMatrix::MIDI_VELOCITY] = value;
  }

  void setMidiBend(float value) {
    source_[ModMatrix::MIDI_BEND] = value;
  }

  void setMidiCc(uint8_t number, float value) {
    for (size_t i = 0; i < Settings::kNumUserCc; ++i) {
      if (number == modMatrix_->midiCcNumber(i)) {
        source_[i + ModMatrix::MIDI_CC_A] = value;
      }
    }
  }

  float* process() {
    for (int dest = 0; dest < ModMatrix::NUM_DESTINATIONS; ++dest) {
      destination_[dest] = 1.f;
      for (int src = 0; src < ModMatrix::NUM_SOURCES; ++src) {
        if (modMatrix_->read(src, dest)) {
          destination_[dest] *= source_[src];
        }
      }
    }

    return &destination_[0];
  }

 private:
  ModMatrix* modMatrix_;
  float destination_[ModMatrix::NUM_DESTINATIONS];
  float source_[ModMatrix::NUM_SOURCES];
};

#endif
