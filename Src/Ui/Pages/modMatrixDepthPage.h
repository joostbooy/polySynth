#ifndef ModMatrixDepthPage_h
#define ModMatrixDepthPage_h

#include "topPage.h"

namespace ModMatrixDepthPage {

  using TopPage::buttons_;
  using TopPage::canvas_;
  using TopPage::pages_;
  using TopPage::settings_;

  size_t dest_;

  enum FooterOptions {
    CLOSE,
    NUM_FOOTER_OPTIONS,
  };

  const char* const footerOptionsText[NUM_FOOTER_OPTIONS] = {"CLOSE"};

  void setDestination(size_t dest) {
    dest_ = dest;
  }

  void init() {
  }

  void enter() {
  }

  void exit() {
  }

  void on_button(int id, int state) {
    if (state) {
      switch (buttons_->toFunction(id)) {
        case CLOSE:
          pages_->close(Pages::MOD_MATRIX_DEPTH_PAGE);
          break;
        default:
          break;
      }
    }
  }

  void on_encoder(int id, int state) {
    Patch& p = settings_->selectedPatch();

    bool shift = buttons_->isPressed(Buttons::SHIFT);
    float value = p.modMatrix().destinationDepth(dest_);
    p.modMatrix().setDestinationDepth(dest_, value + SettingsUtils::fInc(state, shift));
  }

  void refresh_leds() {
  }

  void draw() {
    const int w = canvas_->width() / 4;
    const int h = canvas_->height() / 4;
    const int x = (canvas_->width() - w) / 2;
    const int y = (canvas_->height() - h) / 2;

    canvas_->fill(x + 6, y + 6, w, h, Canvas::SUBTRACTED);
    canvas_->fill(x, y, w, h, Canvas::WHITE);
    canvas_->frame(x, y, w, h, Canvas::GRAY, 1);
    canvas_->drawText(x, y + 2, w, h - 2, ModMatrix::destination_text(dest_), Canvas::CENTER, Canvas::TOP);
    canvas_->drawText(x, y, w, h, settings_->modMatrix().destinationDepthText(dest_), Canvas::CENTER, Canvas::CENTER);

    WindowPainter::draw_footer(footerOptionsText, NUM_FOOTER_OPTIONS);
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

};  // namespace ModMatrixDepthPage

#endif
