#ifndef HardwareTestPage_h
#define HardwareTestPage_h

#include "buttons.h"
#include "pots.h"
#include "textBufferPainter.h"

namespace HardwareTestPage {

using TopPage::canvas_;
using TopPage::leds_;
using TopPage::buttons_;
using TopPage::pages_;
using TopPage::settings_;
using TopPage::str_;
using TopPage::pots_;

bool potsEnabled;
uint8_t lastPotValue[Pots::NUM_POTS];

const char* buttonIdText(int id) {
  switch (id) {
    case Buttons::TUNE_1_OCT_UP:          return "TUNE_1_OCT_UP";
    case Buttons::TUNE_1_OCT_DOWN:        return "TUNE_1_OCT_DOWN";
    case Buttons::TUNE_2_OCT_UP:          return "TUNE_2_OCT_UP";
    case Buttons::TUNE_2_OCT_DOWN:        return "TUNE_2_OCT_DOWN";
    case Buttons::LFO_1_TYPE:             return "LFO_1_TYPE";
    case Buttons::LFO_2_TYPE:             return "LFO_2_TYPE";
    case Buttons::TUNE_1_VCO_MOD:         return "TUNE_1_VCO_MOD";
    case Buttons::TUNE_1_MOD_TYPE:        return "TUNE_1_MOD_TYPE";
    case Buttons::VCO_2_SYNC:             return "VCO_2_SYNC";
    case Buttons::TUNE_2_MOD_TYPE:        return "TUNE_2_MOD_TYPE";
    case Buttons::SHAPE_1_MOD_TYPE:       return "SHAPE_1_MOD_TYPE";
    case Buttons::VCO_1_OSC_TYPE:         return "VCO_1_OSC_TYPE";
    case Buttons::SHAPE_2_MOD_TYPE:       return "SHAPE_2_MOD_TYPE";
    case Buttons::VCO_2_OSC_TYPE:         return "VCO_2_OSC_TYPE";
    case Buttons::VCO_MOD_SOURCE:         return "VCO_MOD_SOURCE";
    case Buttons::SLIDE_VCO_SELECT:       return "SLIDE_VCO_SELECT";
    case Buttons::CUTOFF_1_MOD_TYPE:      return "CUTOFF_1_MOD_TYPE";
    case Buttons::VCO_1_MUTE:             return "VCO_1_MUTE";
    case Buttons::CUTOFF_2_MOD_TYPE:      return "CUTOFF_2_MOD_TYPE";
    case Buttons::VCO_2_MUTE:             return "VCO_2_MUTE";
    case Buttons::VCO_MOD_DEPTH_MOD_TYPE: return "VCO_MOD_DEPTH_MOD_TYPE";
    case Buttons::VOLUME_MOD_TYPE:        return "VOLUME_MOD_TYPE";
    case Buttons::VCF_1_TYPE:             return "VCF_1_TYPE";
    case Buttons::CUTOFF_1_VCO_MOD:       return "CUTOFF_1_VCO_MOD";
    case Buttons::VCF_2_ROUTING:          return "VCF_2_ROUTING";
    case Buttons::CUTOFF_2_VCO_MOD:       return "CUTOFF_2_VCO_MOD";
    case Buttons::VOLUME_VCO_MOD:         return "VOLUME_VCO_MOD";
    case Buttons::DISPLAY_B:              return "DISPLAY_B";
    case Buttons::DISPLAY_A:              return "DISPLAY_A";
    case Buttons::PATCH_PAGE:             return "PATCH_PAGE";
    case Buttons::ENVELOPE_PAGE:          return "ENVELOPE_PAGE";
    case Buttons::VCO_PAGE:               return "VCO_PAGE";
    case Buttons::PAN_MOD_TYPE:           return "PAN_MOD_TYPE";
    case Buttons::DISPLAY_D:              return "DISPLAY_D";
    case Buttons::DISPLAY_C:              return "DISPLAY_C";
    case Buttons::LFO_PAGE:               return "LFO_PAGE";
    case Buttons::MATRIX_PAGE:            return "MATRIX_PAGE";
    case Buttons::MIDI_PAGE:              return "MIDI_PAGE";
    case Buttons::SHIFT:                  return "SHIFT";
    case Buttons::ENC_X:                  return "ENC_X";
    case Buttons::AMP_PAGE:               return "AMP_PAGE";
    case Buttons::VCF_PAGE:               return "VCF_PAGE";
    case Buttons::ENC_Y:                  return "ENC_Y";
    default:                              
      break;
  }
  return "";
}

enum FooterOptions {
  TOGGLE_LEDS,
  TEST_SD_CARD,
  ENABLE_POTS,
  CLOSE,
  NUM_OPTIONS,
};

const char* const footer_option_text[NUM_OPTIONS] = {"TOGGLE LEDS", "TEST SD CARD", "ENABLE POTS", "CLOSE"};

bool led_toggle_state_;

void testPots() {
  for (size_t i = 0; i < Pots::NUM_POTS; i++) {
    uint8_t value = pots_->read(i) * 255;
    if (value != lastPotValue[i]) {
      lastPotValue[i] = value;
      TextBufferPainter::write(str_.write(pots_->idText(i), " ", value));
    }
  }
}

void init() {
}

void enter() {
  potsEnabled = false;
}

void exit() {
}

void on_button(int id, int state) {
  TextBufferPainter::write(str_.write(buttonIdText(id), " ", state));

  if (buttons_->isPressed(Buttons::SHIFT)) {
    switch (buttons_->toFunction(id)) {
      case TOGGLE_LEDS:
        if (state) {
          led_toggle_state_ ^= 1;
        }
        break;
      case TEST_SD_CARD:
        if (state) {
          if (settings_->disk()->mount()) {
            MessagePainter::show("SD MOUNTED");
          } else {
            MessagePainter::show("ERROR");
          }
        }
        break;
      case ENABLE_POTS:
        if (state) {
          potsEnabled ^= 1;
          if (potsEnabled) {
            for (size_t i = 0; i < Pots::NUM_POTS; i++) {
              lastPotValue[i] = pots_->read(i) * 255;
            }
          }
        }
        break;
        case CLOSE:
        pages_->close(Pages::HARDWARE_TEST_PAGE);
        pages_->open(Pages::PATCH_PAGE);
        break;
      default:
        break;
    }
  }
}

void on_encoder(int id, int state) {
  TextBufferPainter::write(str_.write(id == 0 ? "X ENC " : "Y ENC ", state));
}

void refresh_leds() {
  if (led_toggle_state_) {
    leds_->setAll(Leds::BLACK);
  } else {
    leds_->setAll(Leds::RED);
  }
}

void draw() {
  if (potsEnabled) {
    testPots();
  }
  TextBufferPainter::draw();
  WindowPainter::draw_footer(footer_option_text, NUM_OPTIONS);
}

const size_t target_fps() {
  return 1000 / 16;
}

Pages::Page page = {
    &init,
    &enter,
    &exit,
    &draw,
    &refresh_leds,
    &on_button,
    &on_encoder,
    &target_fps,
};

};  // namespace HardwareTestPage

#endif
