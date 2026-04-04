#ifndef Buttons_h
#define Buttons_h

#include <stdint.h>

#include "pages.h"

#define SERIAL_ID(collumn, row) ((collumn * 6) + row)

class Buttons {
 public:
  enum Id {
    TUNE_1_OCT_UP           = SERIAL_ID(0, 0),
    TUNE_1_OCT_DOWN         = SERIAL_ID(0, 1),
    TUNE_2_OCT_UP           = SERIAL_ID(0, 2),
    TUNE_2_OCT_DOWN         = SERIAL_ID(0, 3),
    LFO_1_TYPE              = SERIAL_ID(0, 4),
    LFO_2_TYPE              = SERIAL_ID(0, 5),
    TUNE_1_VCO_MOD          = SERIAL_ID(1, 0),
    TUNE_1_MOD_TYPE         = SERIAL_ID(1, 1),
    VCO_2_SYNC              = SERIAL_ID(1, 2),
    TUNE_2_MOD_TYPE         = SERIAL_ID(1, 3),
    SHAPE_1_MOD_TYPE        = SERIAL_ID(2, 0),
    VCO_1_OSC_TYPE          = SERIAL_ID(2, 1),
    SHAPE_2_MOD_TYPE        = SERIAL_ID(2, 2),
    VCO_2_OSC_TYPE          = SERIAL_ID(2, 3),
    VCO_MOD_SOURCE          = SERIAL_ID(2, 4),
    SLIDE_VCO_SELECT        = SERIAL_ID(2, 5),
    CUTOFF_1_MOD_TYPE       = SERIAL_ID(3, 0),
    VCO_1_MUTE              = SERIAL_ID(3, 1),
    CUTOFF_2_MOD_TYPE       = SERIAL_ID(3, 2),
    VCO_2_MUTE              = SERIAL_ID(3, 3),
    VCO_MOD_DEPTH_MOD_TYPE  = SERIAL_ID(3, 4),
    VOLUME_MOD_TYPE         = SERIAL_ID(3, 5),
    VCF_1_TYPE              = SERIAL_ID(4, 0),
    CUTOFF_1_VCO_MOD        = SERIAL_ID(4, 1),
    VCF_2_ROUTING           = SERIAL_ID(4, 2),
    CUTOFF_2_VCO_MOD        = SERIAL_ID(4, 3),
    VOLUME_VCO_MOD          = SERIAL_ID(4, 5),
    DISPLAY_B               = SERIAL_ID(5, 0),
    DISPLAY_A               = SERIAL_ID(5, 1),
    PATCH_PAGE              = SERIAL_ID(5, 2),
    ENVELOPE_PAGE           = SERIAL_ID(5, 3),
    VCO_PAGE                = SERIAL_ID(5, 4),
    PAN_MOD_TYPE            = SERIAL_ID(5, 5),
    DISPLAY_D               = SERIAL_ID(6, 0),
    DISPLAY_C               = SERIAL_ID(6, 1),
    LFO_PAGE                = SERIAL_ID(6, 2),
    MATRIX_PAGE             = SERIAL_ID(6, 3),
    MIDI_PAGE               = SERIAL_ID(6, 4),
    SHIFT                   = SERIAL_ID(6, 5),
    ENC_X                   = SERIAL_ID(7, 0),
    AMP_PAGE                = SERIAL_ID(7, 2),
    VCF_PAGE                = SERIAL_ID(7, 3),
    ENC_Y                   = SERIAL_ID(7, 5),
  };

  const char* idText(int id) {
    switch (id) {
      case TUNE_1_OCT_UP:           return "TUNE 1 OCT UP";
      case TUNE_1_OCT_DOWN:         return "TUNE 1 OCT DOWN";
      case TUNE_2_OCT_UP:           return "TUNE 2 OCT UP";
      case TUNE_2_OCT_DOWN:         return "TUNE_2_OCT_DOWN";
      case LFO_1_TYPE:              return "LFO 1 TYPE";
      case LFO_2_TYPE:              return "LFO 2 TYPE";
      case TUNE_1_VCO_MOD:          return "TUNE 1 VCO MOD";
      case TUNE_1_MOD_TYPE:         return "TUNE 1 MOD TYPE";
      case VCO_2_SYNC:              return "VCO 2 SYNC";
      case TUNE_2_MOD_TYPE:         return "TUNE 2 MOD TYPE";
      case SHAPE_1_MOD_TYPE:        return "SHAPE 1 MOD TYPE";
      case VCO_1_OSC_TYPE:          return "VCO 1 OSC TYPE";
      case SHAPE_2_MOD_TYPE:        return "SHAPE 2 MOD TYPE";
      case VCO_2_OSC_TYPE:          return "VCO 2 OSC TYPE";
      case VCO_MOD_SOURCE:          return "VCO MOD SOURCE";
      case SLIDE_VCO_SELECT:        return "SLIDE VCO SELECT";
      case CUTOFF_1_MOD_TYPE:       return "CUTOFF 1 MOD TYPE";
      case VCO_1_MUTE:              return "VCO 1 MUTE";
      case CUTOFF_2_MOD_TYPE:       return "CUTOFF 2 MOD TYPE";
      case VCO_2_MUTE:              return "VCO 2 MUTE";
      case VCO_MOD_DEPTH_MOD_TYPE:  return "VCO MOD DEPTH MOD TYPE";
      case VOLUME_MOD_TYPE:         return "VOLUME MOD TYPE";
      case VCF_1_TYPE:              return "VCF 1 TYPE";
      case CUTOFF_1_VCO_MOD:        return "CUTOFF 1 VCO MOD";
      case VCF_2_ROUTING:           return "VCF 2 ROUTING";
      case CUTOFF_2_VCO_MOD:        return "CUTOFF 2 VCO MOD";
      case VOLUME_VCO_MOD:          return "VOLUME VCO MOD";
      case DISPLAY_B:               return "DISPLAY B";
      case DISPLAY_A:               return "DISPLAY A";
      case PATCH_PAGE:              return "PATCH PAGE";
      case ENVELOPE_PAGE:           return "ENVELOPE PAGE";
      case VCO_PAGE:                return "VCO PAGE";
      case PAN_MOD_TYPE:            return "PAN MOD TYPE";
      case DISPLAY_D:               return "DISPLAY D";
      case DISPLAY_C:               return "DISPLAY C";
      case LFO_PAGE:                return "LFO PAGE";
      case MATRIX_PAGE:             return "MATRIX PAGE";
      case MIDI_PAGE:               return "MIDI PAGE";
      case SHIFT:                   return "SHIFT";
      case ENC_X:                   return "ENC X";
      case AMP_PAGE:                return "AMP PAGE";
      case VCF_PAGE:                return "VCF PAGE";
      case ENC_Y:                   return "ENC Y";
      default:
        break;
    }
    return "";
  }

   void init() {
    for (int i = 0; i < 2; ++i) {
      pressed_[i] = 0;
    }
  }

   void write(int id, bool state) {
    uint8_t index = id / 32;
    uint32_t mask = 1 << (id & 31);
    pressed_[index] &= ~(mask);
    if (state) {
      pressed_[index] |= mask;
    }
  }

   bool isPressed(int id) {
    uint8_t index = id / 32;
    uint32_t mask = 1 << (id & 31);
    return pressed_[index] & mask;
  }

   int toFunction(int id, int offset = 0) {
    switch (id) {
      case DISPLAY_A: return 0 + offset;
      case DISPLAY_B: return 1 + offset;
      case DISPLAY_C: return 2 + offset;
      case DISPLAY_D: return 3 + offset;
      default:
        return -1;
        break;
    }
  }

   int toPage(int id) {
    switch (id) {
      case LFO_PAGE:       return Pages::LFO_PAGE;
      case MIDI_PAGE:      return Pages::MIDI_PAGE;
      case AMP_PAGE:       return Pages::AMP_PAGE;
      case VCF_PAGE:       return Pages::FILTER_PAGE;
      case ENVELOPE_PAGE:  return Pages::ENVELOPE_PAGE;
      case VCO_PAGE:       return Pages::OSCILLATOR_PAGE;
      case MATRIX_PAGE:    return Pages::MOD_MATRIX_PAGE;
      case PATCH_PAGE:     return Pages::PATCH_PAGE;
      default:
        return -1;
        break;
    }
  }

  bool isDisplayButton(int id) {
    if (toFunction(id) >= 0) {
      return true;
    }
    if (toPage(id) >= 0) {
      return true;
    }
    return id == SHIFT;
  }

 private:
    uint32_t pressed_[2];
};

#endif
