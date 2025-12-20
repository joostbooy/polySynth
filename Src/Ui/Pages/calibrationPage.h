#ifndef CalibrationPage_h
#define CalibrationPage_h

#include "calibrationUiList.h"
#include "topPage.h"

namespace CalibrationPage {

CalibrationList calibrationList;

enum FooterOptions { 
	SAVE, 
	LOAD, 
	CLOSE, 
	NUM_FOOTER_OPTIONS 
};

const char* const footer_text[NUM_FOOTER_OPTIONS] = { "SAVE", "LOAD", "CLOSE" };

void clear_settings() {
  settings->calibration().init();
}

void init() {

}

void enter() {
  ListPage::setList(&calibrationList);
  ListPage::setClearCallback(&clear);
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

  switch (controller.buttonToFunction(id)) {
    case SAVE:
      ConfirmationPage::set("OVERWRITE SETTINGS ?", [](uint8_t option) {
        if (option == ConfirmationPage::CONFIRM) {
          if (settings_->calibration().save()) {
            MessagePainter::show("CALIBRATION SAVED");
          } else {
            MessagePainter::show("FAILED");
          }
        }
      });
      pages_->open(Pages::CONFIRMATION_PAGE);
      break;
    case LOAD:
      ConfirmationPage::set("OVERWRITE SETTINGS ?", [](uint8_t option) {
        if (option == ConfirmationPage::CONFIRM) {
          if (settings_->calibration().load()) {
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
    default:
      break;
  }
}

void refresh_leds() {
  ListPage::refresh_leds();
  LedPainter::footer_buttons(NUM_FOOTER_OPTIONS);
}

void msTick(uint16_t ticks) {
  ListPage::msTick(ticks);
}

// Bottom to top
void draw() {
  canvas_->buffer.clear();
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
