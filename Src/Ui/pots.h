#if !defined(Pots_h)
#define Pots_h

#include "dsp.h"

class Pots {
 public:
  enum Id {
    RESONANCE_B,
    RESONANCE_A,
    TUNE_A,
    TUNE_B,
    SHAPE_A,
    SHAPE_B,
    CUTOFF_A,
    CUTOFF_B,
    PAN,
    MOD_DEPTH,
    DRIVE,
    SLIDE_AMMOUNT,
    LFO_SKEW_1,
    LFO_SPEED_2,
    LFO_SKEW_2,
    LFO_SPEED_1,
    A_TIME_1,
    A_SHAPE_1,
    D_TIME_1,
    D_SHAPE_1,
    S_LEVEL_1,
    S_HOLD_1,
    R_TIME_1,
    R_SHAPE_1,
    A_TIME_2,
    A_SHAPE_2,
    D_TIME_2,
    D_SHAPE_2,
    S_LEVEL_2,
    S_HOLD_2,
    R_TIME_2,
    R_SHAPE_2,

    NUM_POTS
  };

  static void init() {
    std::fill(&raw_[0], &raw_[NUM_POTS], 0.f);
    std::fill(&filtered_[0], &filtered_[NUM_POTS], 0.f);
  }

  static void write(float value, int id) {
    raw_[id] = value;
    for (size_t i = 0; i < NUM_POTS; ++i) {
      filtered_[i] += 0.01f * (raw_[i] - filtered_[i]);
    }
  }

 static float read(int id) {
    return filtered_[id];
  }

 private:
  static inline float raw_[NUM_POTS];
  static inline float filtered_[NUM_POTS];
};

#endif  // Pots_h
