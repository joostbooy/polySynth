#ifndef ModulationMatrixEngine_h
#define ModulationMatrixEngine_h

#include "settings.h"

class ModulationMatrixEngine {
 public:
  struct Frame {
    float data[ModulationMatrix::NUM_DESTINATIONS];
  };

  void init(Settings* settings) {
    modulationMatrix_ = &settings->modulationMatrix();
  }

  void set_envelope(int index, float value) {
    source_[ModulationMatrix::ENVELOPE_1 + index] = value;
  }

    void setLfo(int index, float value) {
    source_[ModulationMatrix::LFO_1 + index] = value;
  }

  void set_cv(int channel, float value) {
    source_[ModulationMatrix::CV_1 + channel] = value;
  }

  void set_midi_velocity(float value) {
    source_[ModulationMatrix::MIDI_VELOCITY] = value;
  }

  void set_midi_bend(float value) {
    source_[ModulationMatrix::MIDI_BEND] = value;
  }

  void set_midi_cc(uint8_t number, float value) {
    for (size_t i = 0; i < Settings::kNumUserCc; ++i) {
      if (number == modulationMatrix_->midi_cc_number(i)) {
        source_[i + ModulationMatrix::MIDI_CC_A] = value;
      }
    }
  }

  Frame* process() {
    for (int y = 0; y < ModulationMatrix::NUM_DESTINATIONS; ++y) {
      frame_.data[y] = 1.f;
      for (int x = 0; x < ModulationMatrix::NUM_SOURCES; ++x) {
        if (modulationMatrix_->read(x, y)) {
          frame_.data[y] *= source_[x];
        }
      }
    }

    return &frame_;
  }

  //	Frame* process(ModulationMatrix *matrix) {
  //		std::fill(&frame_.data[0], &frame_.data[ModulationMatrix::NUM_DESTINATIONS], 1.0f);

  //		for (int x = 0; x < ModulationMatrix::NUM_SOURCES; ++x) {
  //			for (int y = 0; y < ModulationMatrix::NUM_DESTINATIONS; ++y) {
  //				if (matrix->read(x, y)) {
  //					frame_.data[y] *= source_[x];
  //				}
  //			}
  //		}

  //		return &frame_;
  //	}

 private:
  Frame frame_;
  ModulationMatrix* modulationMatrix_;
  float source_[ModulationMatrix::NUM_SOURCES];
};

#endif
