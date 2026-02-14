#ifndef CalibrationPage_h
#define CalibrationPage_h

#include "calibrationList.h"
#include "topPage.h"

namespace CalibrationPage {

	using TopPage::settings_;
	using TopPage::engine_;
	using TopPage::ui_;
	using TopPage::canvas_;
	using TopPage::buttons_;
	using TopPage::pages_;

CalibrationList calibrationList;

enum FooterOptions { 
	SAVE, 
	LOAD, 
  CLEAR,
	CLOSE, 
	NUM_FOOTER_OPTIONS 
};

const char* const footer_text[NUM_FOOTER_OPTIONS] = { "SAVE", "LOAD", "CLEAR", "CLOSE" };

void init() {

}

void enter() {
  ListPage::setList(&calibrationList);
  ListPage::enter();
  settings_->calibration().start();
}

void exit() {
  ListPage::exit();
  settings_->calibration().stop();
}

void on_encoder(int id, int state) {
  ListPage::on_encoder(id, state);
}

void on_button(int id, int state) {
  ListPage::on_button(id, state);

  if (!state) {
    return;
  }

  switch (buttons_->toFunction(id)) {
    case SAVE:
      ConfirmationPage::set("OVERWRITE SETTINGS ?", [](int option) {
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
      ConfirmationPage::set("OVERWRITE SETTINGS ?", [](int option) {
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
    case CLOSE:
      pages_->close(Pages::CALIBRATION_PAGE);
      break;
    case CLEAR:
      ConfirmationPage::set("CLEAR SETTINGS ?", [](int option) {
        if (option == ConfirmationPage::CONFIRM) {
          settings_->calibration().init();
          MessagePainter::show("CALIBRATION CLEARED");
        }
      });
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
  ListPage::draw();
  WindowPainter::draw_footer(footer_text, NUM_FOOTER_OPTIONS);
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
	&target_fps
};

};  // namespace CalibrationPage

#endif
