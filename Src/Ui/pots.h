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

const char* idText(int id) {
  switch (id) {
    case RESONANCE_B:   return "RESONANCE B";
    case RESONANCE_A:   return "RESONANCE A";
    case TUNE_A:        return "TUNE A";
    case TUNE_B:        return "TUNE B";
    case SHAPE_A:       return "SHAPE A";
    case SHAPE_B:       return "SHAPE B";
    case CUTOFF_A:      return "CUTOFF A";
    case CUTOFF_B:      return "CUTOFF B";
    case PAN:           return "PAN";
    case MOD_DEPTH:     return "MOD DEPTH";
    case DRIVE:         return "DRIVE";
    case SLIDE_AMMOUNT: return "SLIDE AMMOUNT";
    case LFO_SKEW_1:    return "LFO SKEW 1";
    case LFO_SPEED_2:   return "LFO SPEED 2";
    case LFO_SKEW_2:    return "LFO SKEW 2";
    case LFO_SPEED_1:   return "LFO SPEED 1";
    case A_TIME_1:      return "A TIME 1";
    case A_SHAPE_1:     return "A SHAPE 1";
    case D_TIME_1:      return "D TIME 1";
    case D_SHAPE_1:     return "D SHAPE 1";
    case S_LEVEL_1:     return "S LEVEL 1";
    case S_HOLD_1:      return "S HOLD 1";
    case R_TIME_1:      return "R_TIME_1";
    case R_SHAPE_1:     return "R SHAPE 1";
    case A_TIME_2:      return "A TIME 2";
    case A_SHAPE_2:     return "A SHAPE 2";
    case D_TIME_2:      return "D TIME 2";
    case D_SHAPE_2:     return "D SHAPE 2";
    case S_LEVEL_2:     return "S LEVEL 2";
    case S_HOLD_2:      return "S HOLD 2";
    case R_TIME_2:      return "R TIME 2";
    case R_SHAPE_2:     return "R SHAPE 2";
    default:
      break;
  }
  return "";
}

   void init() {
    std::fill(&raw_[0], &raw_[NUM_POTS], 0.f);
    std::fill(&filtered_[0], &filtered_[NUM_POTS], 0.f);
  }

   void write(int id, float value) {
    raw_[id] = value;
  }

  float read(int id) {
    return filtered_[id];
  }

  void filterAll() {
    for (size_t i = 0; i < NUM_POTS; ++i) {
      filtered_[i] += 0.01f * (raw_[i] - filtered_[i]);
    }
  }

 private:
    float raw_[NUM_POTS];
    float filtered_[NUM_POTS];
};

#endif  // Pots_h
