#ifndef ModMatrixCcPage_h
#define ModMatrixCcPage_h

#include "topPage.h"

namespace ModMatrixCcPage {

  using TopPage::buttons_;
  using TopPage::canvas_;
  using TopPage::pages_;
  using TopPage::settings_;
  using TopPage::str_;

  enum FooterOptions {
    CLOSE,
    NUM_FOOTER_OPTIONS,
  };

  const char* const footerOptionsText[NUM_FOOTER_OPTIONS] = {"CLOSE"};

  int index_ = 0;

  void init() {
  }

  void enter() {
  }

  void exit() {
  }

  void onButton(int id, int state) {
    if (state) {
      switch (buttons_->toFunction(id)) {
        case CLOSE:
          pages_->close(Pages::MOD_MATRIX_CC_PAGE);
          break;
        default:
          break;
      }
    }
  }

  void onEncoder(int id, int inc) {
    Patch& p = settings_->selectedPatch();

    if (id == 0) {
      index_ = SettingsUtils::clip(0, Settings::kNumUserCc - 1, index_ + inc);
    } else {
      bool shift = buttons_->isPressed(Buttons::SHIFT);
      float ccNumber = p.modMatrix().midiCcNumber(index_);
      p.modMatrix().setMidiCcNumber(index_, ccNumber + (shift ? inc * 10 : inc));
    }
  }

  void refreshLeds() {
  }

  void draw() {
    const int w = canvas_->width() / 4;
    const int h = canvas_->height() / 4;
    const int x = (canvas_->width() - w) / 2;
    const int y = (canvas_->height() - h) / 2;

    canvas_->fill(x + 6, y + 6, w, h, Canvas::SUBTRACTED);
    canvas_->fill(x, y, w, h, Canvas::WHITE);
    canvas_->frame(x, y, w, h, Canvas::GRAY, 1);
    canvas_->drawText(x, y + 2, w, h - 2, str_.write("INDEX ", index_ + 1), Canvas::CENTER, Canvas::TOP);
    canvas_->drawText(x, y, w, h, settings_->modMatrix().midiCcNumberText(index_), Canvas::CENTER, Canvas::CENTER);

    WindowPainter::drawFooter(footerOptionsText, NUM_FOOTER_OPTIONS);
  }

  const size_t targetFps() {
    return 1000 / 16;
  }

  Pages::Page page = {
      &init,
      &enter,
      &exit,
      &draw,
      &refreshLeds,
      &onButton,
      &onEncoder,
      &targetFps,
  };

};  // namespace ModMatrixDepthPage

#endif
