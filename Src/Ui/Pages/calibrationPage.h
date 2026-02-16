#ifndef CalibrationPage_h
#define CalibrationPage_h

#include "topPage.h"

namespace CalibrationPage {

  using TopPage::buttons_;
  using TopPage::canvas_;
  using TopPage::engine_;
  using TopPage::pages_;
  using TopPage::settings_;
  using TopPage::ui_;

  Patch patch_;

  enum FooterOptions {
    SAVE,
    LOAD,
    CLEAR,
    NEXT,
    PREV,
    NEXT_VCO,
    NEXT_VOICE,
    CLOSE,
    NUM_FOOTER_OPTIONS,
  };

  int footerOptionsOffset;
  const char* const footer_text[NUM_FOOTER_OPTIONS] = {"SAVE", "LOAD", "CLEAR", ">", "<", "NEXT VCO", "NEXT VOICE", "CLOSE"};

  int selectedVco_;

  void selectNextVco() {
    ++selectedVco_ %= 2;
    settings_->oscillator().setMuteOsc1(selectedVco_ == 1);
    settings_->oscillator().setMuteOsc2(selectedVco_ == 0);
  }

  void init() {
  }

  void enter() {
    selectedVco_ = 0;
    footerOptionsOffset = 0;

    patch_.paste(&settings_->selectedPatch());
    settings_->oscillator().setType1(Oscillator::TRIANGLE1);
    settings_->oscillator().setType2(Oscillator::TRIANGLE2);
    settings_->oscillator().setShape1(0.f);
    settings_->oscillator().setShape2(0.f);
    settings_->oscillator().setFmEnable(false);
    settings_->filter().setType(Filter::LP2P);
    settings_->filter().setCutoff1(1.f);
    settings_->filter().setCutoff2(1.f);
    settings_->filter().setResonace1(0.f);
    settings_->filter().setResonace2(0.f);
    settings_->filter().setFmEnable1(false);
    settings_->filter().setFmEnable2(false);
    settings_->amp().setAmEnable(false);
    settings_->amp().setDrive(0.f);
    settings_->amp().setPan(0.5f);
    settings_->modMatrix().clear();
    selectNextVco();
    
    settings_->calibration().start();
  }

  void exit() {
    settings_->calibration().stop();
    settings_->selectedPatch().paste(&patch_);
  }

  void on_encoder(int id, int inc) {
    int value = inc * (buttons_->isPressed(Buttons::SHIFT) ? 50 : 1);
    if (id == 0) {
      settings_->calibration().setMin(settings_->calibration().min() + value);
    } else {
      settings_->calibration().setMax(settings_->calibration().max() + value);
    }
  }

  void on_button(int id, int state) {
    if (!state) {
      return;
    }

    int option = buttons_->toFunction(id);
    if (option >= 0) {
      option += footerOptionsOffset;
    }

    switch (option) {
      case SAVE:
        ConfirmationPage::set("OVERWRITE CALIBRATION ?", [](int option) {
          if (option == ConfirmationPage::CONFIRM) {
            if (settings_->saveCalibration()) {
              MessagePainter::show("CALIBRATION SAVED");
            } else {
              MessagePainter::show("FAILED");
            }
          }
        });
        pages_->open(Pages::CONFIRMATION_PAGE);
        break;
      case LOAD:
        ConfirmationPage::set("OVERWRITE CALIBRATION ?", [](int option) {
          if (option == ConfirmationPage::CONFIRM) {
            if (settings_->loadCalibration()) {
              MessagePainter::show("CALIBRATION LOADED");
            } else {
              MessagePainter::show("FAILED");
            }
          }
        });
        pages_->open(Pages::CONFIRMATION_PAGE);
        break;
      case CLEAR:
        ConfirmationPage::set("CLEAR CALIBRATION ?", [](int option) {
          if (option == ConfirmationPage::CONFIRM) {
            settings_->calibration().init();
            MessagePainter::show("CALIBRATION CLEARED");
          }
        });
        break;
      case NEXT:
        footerOptionsOffset = 4;
        break;
      case PREV:
        footerOptionsOffset = 0;
        break;
      case NEXT_VCO:
        selectNextVco();
        break;
      case NEXT_VOICE:
        settings_->calibration().selectNextVoice();
        break;
      case CLOSE:
        pages_->close(Pages::CALIBRATION_PAGE);
        pages_->open(Pages::PATCH_PAGE);
        break;
      default:
        break;
    }
  }

  void refresh_leds() {
  }

  // Bottom to top
  void draw() {
    canvas_->clear();

    const int x = 5;
    const int y = 10;
    const int rowHeight = 8;
    const int collWdith = canvas_->width() / 4;

    canvas_->drawText(x, y, "MIN");
    canvas_->drawText(x + collWdith, y, settings_->calibration().minText());
    canvas_->drawText(x, y + rowHeight, "MAX");
    canvas_->drawText(x + collWdith, y + rowHeight, settings_->calibration().maxText());

    canvas_->drawText(x, y * (rowHeight * 3), "SELECTED VCO");
    canvas_->drawText(x + collWdith, y * (rowHeight * 3), SettingsText::intToText(selectedVco_ + 1));

    canvas_->drawText(x, y * (rowHeight * 4), "SELECTED VOICE");
    canvas_->drawText(x + collWdith, y * (rowHeight * 4), settings_->calibration().selectedVoiceText());

    WindowPainter::draw_footer(footer_text, NUM_FOOTER_OPTIONS, footerOptionsOffset);
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

};  // namespace CalibrationPage

#endif
