#ifndef ModMatrixEngine_h
#define ModMatrixEngine_h

#include "settings.h"

class ModMatrixEngine {
 public:
  struct Frame {
    float data[ModMatrix::NUM_DESTINATIONS];
  };

  void init(Settings* settings) {
    modMatrix_ = &settings->modMatrix();
  }

  void setAmpEnvelope(float value) {
    source_[ModMatrix::AMP_ENVELOPE] = value;
  }

    void setModEnvelope(float value) {
    source_[ModMatrix::MOD_ENVELOPE] = value;
  }

    void setLfo(int index, float value) {
    source_[ModMatrix::LFO_1 + index] = value;
  }

  void set_cv(int channel, float value) {
    source_[ModMatrix::CV_1 + channel] = value;
  }

  void set_midi_velocity(float value) {
    source_[ModMatrix::MIDI_VELOCITY] = value;
  }

  void set_midi_bend(float value) {
    source_[ModMatrix::MIDI_BEND] = value;
  }

  void set_midi_cc(uint8_t number, float value) {
    for (size_t i = 0; i < Settings::kNumUserCc; ++i) {
      if (number == modMatrix_->midi_cc_number(i)) {
        source_[i + ModMatrix::MIDI_CC_A] = value;
      }
    }
  }

  Frame* process() {
    for (int y = 0; y < ModMatrix::NUM_DESTINATIONS; ++y) {
      frame_.data[y] = 1.f;
      for (int x = 0; x < ModMatrix::NUM_SOURCES; ++x) {
        if (modMatrix_->read(x, y)) {
          frame_.data[y] *= source_[x];
        }
      }
    }

    return &frame_;
  }

  //	Frame* process(ModMatrix *matrix) {
  //		std::fill(&frame_.data[0], &frame_.data[ModMatrix::NUM_DESTINATIONS], 1.0f);

  //		for (int x = 0; x < ModMatrix::NUM_SOURCES; ++x) {
  //			for (int y = 0; y < ModMatrix::NUM_DESTINATIONS; ++y) {
  //				if (matrix->read(x, y)) {
  //					frame_.data[y] *= source_[x];
  //				}
  //			}
  //		}

  //		return &frame_;
  //	}

 private:
  Frame frame_;
  ModMatrix* modMatrix_;
  float source_[ModMatrix::NUM_SOURCES];
};

#endif
