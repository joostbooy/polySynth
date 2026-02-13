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
    case Buttons::TUNE_1_OCT_UP:
      return "TUNE_1_OCT_UP";
    case Buttons::TUNE_1_OCT_DOWN:
      return "TUNE_1_OCT_DOWN";
    case Buttons::TUNE_2_OCT_UP:
      return "TUNE_2_OCT_UP";
    case Buttons::TUNE_2_OCT_DOWN:
      return "TUNE_2_OCT_DOWN";
    case Buttons::LFO_1_TYPE:
      return "LFO_1_TYPE";
    case Buttons::LFO_2_TYPE:
      return "LFO_2_TYPE";
    case Buttons::TUNE_1_VCO_MOD:
      return "TUNE_1_VCO_MOD";
    case Buttons::TUNE_1_MOD_TYPE:
      return "TUNE_1_MOD_TYPE";
    case Buttons::VCO_2_SYNC:
      return "VCO_2_SYNC";
    case Buttons::TUNE_2_MOD_TYPE:
      return "TUNE_2_MOD_TYPE";
    case Buttons::SHAPE_1_MOD_TYPE:
      return "SHAPE_1_MOD_TYPE";
    case Buttons::VCO_1_OSC_TYPE:
      return "VCO_1_OSC_TYPE";
    case Buttons::SHAPE_2_MOD_TYPE:
      return "SHAPE_2_MOD_TYPE";
    case Buttons::VCO_2_OSC_TYPE:
      return "VCO_2_OSC_TYPE";
    case Buttons::VCO_MOD_SOURCE:
      return "VCO_MOD_SOURCE";
    case Buttons::SLIDE_VCO_SELECT:
      return "SLIDE_VCO_SELECT";
    case Buttons::CUTOFF_1_MOD_TYPE:
      return "CUTOFF_1_MOD_TYPE";
    case Buttons::VCO_1_MUTE:
      return "VCO_1_MUTE";
    case Buttons::CUTOFF_2_MOD_TYPE:
      return "CUTOFF_2_MOD_TYPE";
    case Buttons::VCO_2_MUTE:
      return "VCO_2_MUTE";
    case Buttons::VCO_MOD_DEPTH_MOD_TYPE:
      return "VCO_MOD_DEPTH_MOD_TYPE";
    case Buttons::VOLUME_MOD_TYPE:
      return "VOLUME_MOD_TYPE";
    case Buttons::VCF_1_TYPE:
      return "VCF_1_TYPE";
    case Buttons::CUTOFF_1_VCO_MOD:
      return "CUTOFF_1_VCO_MOD";
    case Buttons::VCF_2_ROUTING:
      return "VCF_2_ROUTING";
    case Buttons::CUTOFF_2_VCO_MOD:
      return "CUTOFF_2_VCO_MOD";
    case Buttons::VOLUME_VCO_MOD:
      return "VOLUME_VCO_MOD";
    case Buttons::DISPLAY_B:
      return "DISPLAY_B";
    case Buttons::DISPLAY_A:
      return "DISPLAY_A";
    case Buttons::PATCH_PAGE:
      return "PATCH_PAGE";
    case Buttons::ENVELOPE_PAGE:
      return "ENVELOPE_PAGE";
    case Buttons::VCO_PAGE:
      return "VCO_PAGE";
    case Buttons::PAN_MOD_TYPE:
      return "PAN_MOD_TYPE";
    case Buttons::DISPLAY_D:
      return "DISPLAY_D";
    case Buttons::DISPLAY_C:
      return "DISPLAY_C";
    case Buttons::LFO_PAGE:
      return "LFO_PAGE";
    case Buttons::MATRIX_PAGE:
      return "MATRIX_PAGE";
    case Buttons::MIDI_PAGE:
      return "MIDI_PAGE";
    case Buttons::SHIFT:
      return "SHIFT";
    case Buttons::ENC_X:
      return "ENC_X";
    case Buttons::AMP_PAGE:
      return "AMP_PAGE";
    case Buttons::VCF_PAGE:
      return "VCF_PAGE";
    case Buttons::ENC_Y:
      return "ENC_Y";
    default:
      return "";
      break;
  }
  return "";
}

const char* potIdText(int id) {
  switch (id) {
    case Pots::RESONANCE_B:
      return "RESONANCE_B";
    case Pots::RESONANCE_A:
      return "RESONANCE_A";
    case Pots::TUNE_A:
      return "TUNE_A";
    case Pots::TUNE_B:
      return "TUNE_B";
    case Pots::SHAPE_A:
      return "SHAPE_A";
    case Pots::SHAPE_B:
      return "SHAPE_B";
    case Pots::CUTOFF_A:
      return "CUTOFF_A";
    case Pots::CUTOFF_B:
      return "CUTOFF_B";
    case Pots::PAN:
      return "PAN";
    case Pots::MOD_DEPTH:
      return "MOD_DEPTH";
    case Pots::DRIVE:
      return "DRIVE";
    case Pots::SLIDE_AMMOUNT:
      return "SLIDE_AMMOUNT";
    case Pots::LFO_SKEW_1:
      return "LFO_SKEW_1";
    case Pots::LFO_SPEED_2:
      return "LFO_SPEED_2";
    case Pots::LFO_SKEW_2:
      return "LFO_SKEW_2";
    case Pots::LFO_SPEED_1:
      return "LFO_SPEED_1";
    case Pots::A_TIME_1:
      return "A_TIME_1";
    case Pots::A_SHAPE_1:
      return "A_SHAPE_1";
    case Pots::D_TIME_1:
      return "D_TIME_1";
    case Pots::D_SHAPE_1:
      return "D_SHAPE_1";
    case Pots::S_LEVEL_1:
      return "S_LEVEL_1";
    case Pots::S_HOLD_1:
      return "S_HOLD_1";
    case Pots::R_TIME_1:
      return "R_TIME_1";
    case Pots::R_SHAPE_1:
      return "R_SHAPE_1";
    case Pots::A_TIME_2:
      return "A_TIME_2";
    case Pots::A_SHAPE_2:
      return "A_SHAPE_2";
    case Pots::D_TIME_2:
      return "D_TIME_2";
    case Pots::D_SHAPE_2:
      return "D_SHAPE_2";
    case Pots::S_LEVEL_2:
      return "S_LEVEL_2";
    case Pots::S_HOLD_2:
      return "S_HOLD_2";
    case Pots::R_TIME_2:
      return "R_TIME_2";
    case Pots::R_SHAPE_2:
      return "R_SHAPE_2";
    default:
      break;
  }
  return "";
}

enum FooterOptions { TOGGLE_LEDS, TEST_SD_CARD, ENABLE_POTS, NUM_OPTIONS };

const char* const footer_option_text[NUM_OPTIONS] = {"TOGGLE LEDS", "TEST SD CARD", "ENABLE POTS"};

bool led_toggle_state_;

void testPots() {
  for (size_t i = 0; i < Pots::NUM_POTS; i++) {
    uint8_t value = pots_->read(i) * 255;
    if (value != lastPotValue[i]) {
      lastPotValue[i] = value;
      TextBufferPainter::write(str_.write(potIdText(i), " ", value));
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

Pages::Page page = {&init, &enter, &exit, &draw, &refresh_leds, &on_button, &on_encoder, &target_fps};

};  // namespace HardwareTestPage

#endif
